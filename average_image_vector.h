#ifndef AVERAGE_IMAGE_VECTOR
#define AVERAGE_IMAGE_VECTOR
#include "matrix_operation.h"

struct pricom;

/*计算平均脸*/
/*matrixs是参与运算的矩阵的指针数组,count是数组长度,result是平均脸(计算结果)*/
/*成功返回0,失败返回-1*/
extern int average_image_vector(struct matrix* matrixs[],int count,struct matrix** result);
/*构造矩阵X*/
/*matrixs是参与运算的矩阵的指针数组,count是数组长度,result是 计算结果*/
/*成功返回0,失败返回-1*/
extern int matrix_construct(struct matrix* matrixs[],int count,struct matrix** result);
/*构造c*c阶矩阵R*/
/*matrix是matrix_contruct返回的结果,result是计算结果*/
/*成功返回0,失败返回-1*/
extern int matrix_construct_r(struct matrix* matrix,struct matrix** result);
/*********************************
 * 构造归一化的本征向量
 * vector是要归一化的本征向量
 * x是matrix_construct的计算结果
 * res是记录计算结果的指针的指针
 * 函数成功返回0,失败返回-1
 * *******************************/
extern int eigenvectors_construct_u(struct matrix* vector,struct matrix* x,struct matrix** res);
/*********************************
 * 构造投影变换矩阵A
 * pricoms是归一化后的本征向量的集合(数组)
 * count是集合大小(数组长度)
 * res是记录计算结果的指针的指针
 * 函数成功返回0,失败返回-1
 * ********************************/
extern int matrix_construct_a(struct pricom *pricoms,int count,struct matrix** res);

#endif
