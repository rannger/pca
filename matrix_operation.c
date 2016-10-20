#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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
        memset(ptr->values,0,sizeof(ptr->values));
    }
    return ptr;
}

void matrix_release(struct matrix* matrix)
{
    free(matrix);
}



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
    }
    return res;
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

