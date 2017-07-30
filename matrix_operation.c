#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <errno.h>
#include <sys/sysctl.h>
#include "matrix_operation.h"

/*�������,����Ĺ�������*/
/*����row*col�ľ���,�ɹ��򷵻ؾ���ĵ�ַ,ʧ���򷵻�NULL*/
struct matrix* matrix_alloc_and_init(int row,int col)
/*struct matrix* matrix_alloc(int row,int col)*/
{
    struct matrix* ptr=(struct matrix*)malloc(sizeof(int)*2+sizeof(unsigned int)*(row*col));
    if(ptr)
    {
        ptr->row=row;
        ptr->col=col;
        for (int i = 0;i<row*col;i++)
            ptr->values[i] = 0;
    }
    return ptr;
}

void matrix_release(struct matrix* matrix)
{
    free(matrix);
}


static struct matrix* matrixMultiplyInternal(struct matrix* left,struct matrix* right);

/*����˷�*/
/*�ɹ��򷵻�0,ʧ���򷵻�-1*/
/*matrixs��Ҫ��˵ľ�����ɵ�����,result����˵Ľ����ָ���ָ��*/
int matrix_multiply(struct matrix* matrixs[],struct matrix** result)
{
    int res;
    if(matrixs[0]->col!=matrixs[1]->row)
        res=-1;
    else
    {
        /*
        struct matrix* matrix=matrix_alloc_and_init(matrixs[0]->row,matrixs[1]->col);
        if(!matrix)
            res=-1;
        else
        {
            int i,j;
            int count=matrixs[0]->col;
            int index;
            int row=matrixs[0]->row;
            int col=matrixs[1]->col;
            int value;
            for(i=0;i<matrix->row;i++)
                for(j=0;j<matrix->col;j++)
                {
                    value=0;
                    for(index=0;index<count;index++)
                      value+=matrixs[0]->values[i*count+index]*matrixs[1]->values[index*col+j];
                    matrix->values[i*col+j]=value;
                }
            
            res=0;
            *result=matrix;
        }
        */
        *result = matrixMultiplyInternal(matrixs[0],matrixs[1]);
        res = 0;
    }
    return res;
}

typedef struct matrixCalcParam
{
    struct matrix* left;
    struct matrix* right;
    struct matrix* matrix;
    int32_t i;
    int32_t j;
}MatrixCalcParam;

struct ThreadParam
{
    MatrixCalcParam** params;
    int32_t index;
    pthread_mutex_t mutex;
} g_threadParam = {
    NULL,
    -1,
    PTHREAD_MUTEX_INITIALIZER
};


static void matrixCalcRoutine(MatrixCalcParam* param);

static void* threadRoutine(void* param) 
{
    do {
        int err = pthread_mutex_trylock(&(g_threadParam.mutex));
        if (EBUSY==err) {
            sched_yield();
            continue;
        } else if (EINVAL==err) {
            assert(0);
        }
        MatrixCalcParam* param = g_threadParam.params[g_threadParam.index];
        if(NULL==param) {
            pthread_mutex_unlock(&(g_threadParam.mutex));
            break;
        }
        g_threadParam.index+=1;
        pthread_mutex_unlock(&(g_threadParam.mutex));
        matrixCalcRoutine(param);
    } while(1);

    return NULL;
}

static void matrixCalcRoutine(MatrixCalcParam* param)
{
    MatrixCalcParam* p = param;
    int32_t value = 0;
    int count=p->left->col;
    int row=p->left->row;
    int col=p->right->col;
    for(int index=0;index<count;index++) {
         value+=p->left->values[p->i*count+index]*p->right->values[index*col+p->j];
    }
    p->matrix->values[p->i*col+p->j]=value;
}

int32_t countOfCPU(void)
{
    int32_t ncpu = 0;
    size_t len = sizeof(ncpu);
    sysctlbyname("hw.ncpu",&ncpu,&len,NULL,0);
    return ncpu;
}


static struct matrix* matrixMultiplyInternal(struct matrix* left,struct matrix* right)
{
    struct matrix* matrix=matrix_alloc_and_init(left->row,right->col);
    const int threadCount = countOfCPU()*2;
    pthread_t *pid = NULL;
    pid = (pthread_t *)malloc(sizeof(pthread_t)*threadCount);
    MatrixCalcParam** params = (MatrixCalcParam**)malloc(sizeof(MatrixCalcParam*)*(matrix->row*matrix->col+1));

    for(int i=0;i<matrix->row;i++) {
        for(int j=0;j<matrix->col;j++) {
            int index = i*matrix->col+j;
            MatrixCalcParam* p = (MatrixCalcParam*)malloc(sizeof(MatrixCalcParam));
            p->left = left;
            p->right = right;
            p->matrix = matrix;
            p->i = i;
            p->j = j;
            params[index] = p;
       }
    }
    params[matrix->row*matrix->col] = NULL;
    g_threadParam.params = params;
    g_threadParam.index = 0;

    for(int i=0;i<threadCount;++i) {
        int err = pthread_create(pid+i,NULL,threadRoutine,NULL);
        assert(err==0);
    }

    for(int i=0;i<threadCount;++i) {
        void* retval = NULL;
        pthread_join(pid[i],&retval);
    }

    for(int i = 0;i<matrix->row*matrix->col;++i) 
        free(params[i]);
    free(params);
    free(pid);
    params = NULL;

    g_threadParam.params = NULL;
    g_threadParam.index = -1;
    return matrix;
}



/*����ӷֺͼ����Ĺ����㷨*/
/*�ɹ��򷵻�0,ʧ���򷵻�-1*/
/*matrixs��Ҫ����ľ�����ɵ�����,result������Ľ����ָ���ָ��,flag������ѡ���־,0�Ǽ���,1�Ǽӷ�*/
/*�ú����Ǳ��غ���*/
int matrix_add_sub_common(struct matrix* matrixs[],struct matrix** result,int flag)
{
    int res;
    if(matrixs[0]->row!=matrixs[1]->row&&
       matrixs[0]->col!=matrixs[1]->col)
        res=-1;
    else
    {
        int col=matrixs[0]->col;
        int row=matrixs[0]->row;
        
        struct matrix* ptr=matrix_alloc_and_init(row,col);
        if(!ptr)
            res=-1;
        else
        {
            int i,j;
            for(i=0;i<row;i++)
                for(j=0;j<col;j++)
                {
                    if(flag)
                        ptr->values[i*col+j]=matrixs[0]->values[i*col+j]+matrixs[1]->values[i*col+j];
                    else
                        ptr->values[i*col+j]=matrixs[0]->values[i*col+j]-matrixs[1]->values[i*col+j];
                }
            
            *result=ptr;
            res=0;
        }
    }
    return res;
}

/*����ת��*/
/*�ɹ��򷵻ؽ��,ʧ���򷵻�NULL*/
struct matrix* matrix_transpose(struct matrix* matrix)
{
    struct matrix* result=matrix_alloc_and_init(matrix->col,matrix->row);
    if(result)
    {
        int i,j;
        int row=matrix->col;
        int col=matrix->row;
        for(i=0;i<row;i++)
            for(j=0;j<col;j++)
                result->values[i*col+j]=matrix->values[j*row+i];
    }
    return result;
}

/*����ӷ�*/
/*�ɹ��򷵻�0,ʧ���򷵻�-1*/
/*matrixs��Ҫ��ӵľ�����ɵ�����,result����ӵĽ����ָ���ָ��*/
int matrix_add(struct matrix* matrixs[],struct matrix** result)
{
    return matrix_add_sub_common(matrixs,result,1);    
}

/*�������*/
/*�ɹ��򷵻�0,ʧ���򷵻�-1*/
/*matrixs��Ҫ����ľ�����ɵ�����,result������Ľ����ָ���ָ��*/
int matrix_sub(struct matrix* matrixs[],struct matrix** result)
{
    return matrix_add_sub_common(matrixs,result,0);
}

void print_matrix(struct matrix* param)
{
    int i,j;
    for(i=0;i<param->row;i++)
    {
        for(j=0;j<param->col;j++)
            printf("%d ",param->values[i*param->col+j]);
        puts("");
    }
}

