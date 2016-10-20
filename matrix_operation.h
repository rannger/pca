#ifndef _MATRIX_H
#define _MATRIX_H

struct matrix //矩阵
{
    int row;//行
    int col;//列
    unsigned int values[0];//矩阵值
};

 

/*申请矩阵,矩阵的工厂方法*/
/*申请row*col的矩阵,成功则返回矩阵的地址,失败则返回NULL*/
extern struct matrix* matrix_alloc_and_init(int row,int col);

/*释放矩阵*/
extern void matrix_release(struct matrix* matrix);

/*矩阵乘法*/
/*成功则返回0,失败则返回-1*/
/*matrixs是要相乘的矩阵组成的数组,result是相乘的结果的指针的指针*/
extern int matrix_multiply(struct matrix* matrixs[],struct matrix** result);

/*矩阵加分和减法的共有算法*/
/*成功则返回0,失败则返回-1*/
/*matrixs是要相减的矩阵组成的数组,result是相减的结果的指针的指针,flag是运算选择标志,0是减法,1是加法*/
extern int matrix_add_sub_common(struct matrix* matrixs[],struct matrix** result,int flag);

/*矩阵加法*/
/*成功则返回0,失败则返回-1*/
/*matrixs是要相加的矩阵组成的数组,result是相加的结果的指针的指针*/
extern int matrix_add(struct matrix* matrixs[],struct matrix** result);

/*矩阵减法*/
/*成功则返回0,失败则返回-1*/
/*matrixs是要相减的矩阵组成的数组,result是相减的结果的指针的指针*/
extern int matrix_sub(struct matrix* matrixs[],struct matrix** result);

/*矩阵转置*/
/*成功则返回结果,失败则返回NULL*/
extern struct matrix* matrix_transpose(struct matrix* matrix);

extern void print_matrix(struct matrix* param);

#endif
