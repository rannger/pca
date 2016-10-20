#include <stdlib.h>
#include "matrix_operation.h"
#include "average_image_vector.h"
#include "principal_component.h"
/*����ƽ����*/
/*matrixs�ǲ�������ľ����ָ������,count�����鳤��,result��ƽ����(������)*/
/*�ɹ�����0,ʧ�ܷ���-1*/
int average_image_vector(struct matrix* matrixs[],int count,struct matrix** result)
{
    struct matrix* params[2];
    struct matrix* temp;
    int index;
    int flag;
    int res;
    
    if(count<2)
        return -1;
    params[0]=matrixs[0];
    params[1]=matrixs[1];
    index=2;
    res=0;
    
    while(-1!=(flag=matrix_add(params,&temp))&&index<count)
    {
        if(index!=2)
            matrix_release(params[0]);
        params[0]=temp;
        params[1]=matrixs[index];
        index++;
    }
    if(flag<0)
    {
        matrix_release(params[0]);
        res=-1;
    }
    else
    {
        int i,j;
        int len=temp->col*temp->row;
        
        for(i=0;i<len;i++)
        {
                temp->values[i]/=count;
        }
        *result=temp;
        matrix_release(params[0]);
    }
    return res;
}

/*�������X*/
/*matrixs�ǲ�������ľ����ָ������,count�����鳤��,result�� ������*/
/*�ɹ�����0,ʧ�ܷ���-1*/
int matrix_construct(struct matrix* matrixs[],int count,struct matrix** result)
{
    
    int res;
    struct matrix* matrix;
    struct matrix* avarge;
    res=0;

    if(0>average_image_vector(matrixs,count,&avarge))
        return -1;

    matrix=matrix_alloc_and_init(matrixs[0]->row,count);

    if(!matrix)
    {
        matrix_release(avarge);
        res=-1;
    }
    else
    {
        struct matrix* params[2];
        int i,j;
        
        for(i=0;i<count;i++)
        {
            params[0]=matrixs[i];
            params[1]=avarge;
            struct matrix* temp=NULL;
            if(!matrix_sub(params,&temp))
                for(j=0;j<matrix->row;j++)
                    matrix->values[i*matrix->col+j]=temp->values[j];
            else
            {
                res=-1;
                break;
            }
            if(temp)
                matrix_release(temp);
        }
        matrix_release(avarge);
        if(res!=-1)
            *result=matrix;
        else
            matrix_release(matrix);
    }

    return res;
}

/*����c*c�׾���R*/
/*matrix��matrix_contruct���صĽ��,result�Ǽ�����*/
/*�ɹ�����0,ʧ�ܷ���-1*/
int matrix_construct_r(struct matrix* matrix,struct matrix** result)
{
    int res;
    struct matrix* xt;
    
    res=0;
    xt=matrix_transpose(matrix);
    if(!xt)
        res=-1;
    else
    {
        struct matrix *params[2];
        struct matrix *ptr;
        params[0]=xt;
        params[1]=matrix;
        
        if(0>matrix_multiply(params,&ptr))
            res=-1;
        else
        {
            *result=ptr;
            matrix_release(xt);

        }
    }
    return res;
}

/*********************************
 * ����ͶӰ�任����A
 * pricoms�ǹ�һ����ı��������ļ���(����)
 * count�Ǽ��ϴ�С(���鳤��)
 * res�Ǽ�¼��������ָ���ָ��
 * �����ɹ�����0,ʧ�ܷ���-1
 * ********************************/
int matrix_construct_a(struct pricom *pricoms,int count,struct matrix** res)
{
    int result;
    int i,j;
    struct matrix* matrix;

    result=0;
    matrix=matrix_alloc_and_init(pricoms[0].eigenvectors->row,count);
    if(!matrix)
        result=-1;
    else
    {
        for(i=0;i<count;i++)
        {
            for(j=0;j<pricoms[i].eigenvectors->row;j++)
            {
                matrix->values[j*matrix->col+i]=pricoms[i].eigenvectors->values[j];
            }
            
        }
        struct matrix *temp=matrix_transpose(matrix);
        if(temp)
        {
            *res=temp;
        }
        
        else
            result=-1;
        matrix_release(matrix);
    }
    
    return result;
}

/*********************************
 * �����һ���ı�������
 * vector��Ҫ��һ���ı�������
 * x��matrix_construct�ļ�����
 * res�Ǽ�¼��������ָ���ָ��
 * �����ɹ�����0,ʧ�ܷ���-1
 * *******************************/
int eigenvectors_construct_u(struct matrix* vector,struct matrix* x,struct matrix** res)
{
    int result;
    result=0;

    struct matrix* retval;
    struct matrix* params[2]={x,vector
    };

    if(matrix_multiply(params,&retval)<0)
        result=-1;
    else
        *res=retval;
    
    return result;
}

