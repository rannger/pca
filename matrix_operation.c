#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "matrix_operation.h"

/*申请矩阵,矩阵的工厂方法*/
/*申请row*col的矩阵,成功则返回矩阵的地址,失败则返回NULL*/
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



/*矩阵乘法*/
/*成功则返回0,失败则返回-1*/
/*matrixs是要相乘的矩阵组成的数组,result是相乘的结果的指针的指针*/
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

/*矩阵加分和减法的共有算法*/
/*成功则返回0,失败则返回-1*/
/*matrixs是要相减的矩阵组成的数组,result是相减的结果的指针的指针,flag是运算选择标志,0是减法,1是加法*/
/*该函数是本地函数*/
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

/*矩阵转置*/
/*成功则返回结果,失败则返回NULL*/
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

/*矩阵加法*/
/*成功则返回0,失败则返回-1*/
/*matrixs是要相加的矩阵组成的数组,result是相加的结果的指针的指针*/
int matrix_add(struct matrix* matrixs[],struct matrix** result)
{
    return matrix_add_sub_common(matrixs,result,1);    
}

/*矩阵减法*/
/*成功则返回0,失败则返回-1*/
/*matrixs是要相减的矩阵组成的数组,result是相减的结果的指针的指针*/
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

