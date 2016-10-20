#ifndef AVERAGE_IMAGE_VECTOR
#define AVERAGE_IMAGE_VECTOR
#include "matrix_operation.h"

struct pricom;

/*����ƽ����*/
/*matrixs�ǲ�������ľ����ָ������,count�����鳤��,result��ƽ����(������)*/
/*�ɹ�����0,ʧ�ܷ���-1*/
extern int average_image_vector(struct matrix* matrixs[],int count,struct matrix** result);
/*�������X*/
/*matrixs�ǲ�������ľ����ָ������,count�����鳤��,result�� ������*/
/*�ɹ�����0,ʧ�ܷ���-1*/
extern int matrix_construct(struct matrix* matrixs[],int count,struct matrix** result);
/*����c*c�׾���R*/
/*matrix��matrix_contruct���صĽ��,result�Ǽ�����*/
/*�ɹ�����0,ʧ�ܷ���-1*/
extern int matrix_construct_r(struct matrix* matrix,struct matrix** result);
/*********************************
 * �����һ���ı�������
 * vector��Ҫ��һ���ı�������
 * x��matrix_construct�ļ�����
 * res�Ǽ�¼��������ָ���ָ��
 * �����ɹ�����0,ʧ�ܷ���-1
 * *******************************/
extern int eigenvectors_construct_u(struct matrix* vector,struct matrix* x,struct matrix** res);
/*********************************
 * ����ͶӰ�任����A
 * pricoms�ǹ�һ����ı��������ļ���(����)
 * count�Ǽ��ϴ�С(���鳤��)
 * res�Ǽ�¼��������ָ���ָ��
 * �����ɹ�����0,ʧ�ܷ���-1
 * ********************************/
extern int matrix_construct_a(struct pricom *pricoms,int count,struct matrix** res);

#endif
