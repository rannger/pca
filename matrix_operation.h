#ifndef _MATRIX_H
#define _MATRIX_H
#include <stdint.h>

struct matrix //����
{
    int row;//��
    int col;//��
    uint64_t *values;//����ֵ
};

 

/*�������,����Ĺ�������*/
/*����row*col�ľ���,�ɹ��򷵻ؾ���ĵ�ַ,ʧ���򷵻�NULL*/
extern struct matrix* matrix_alloc_and_init(int row,int col);

/*�ͷž���*/
extern void matrix_release(struct matrix* matrix);

/*����˷�*/
/*�ɹ��򷵻�0,ʧ���򷵻�-1*/
/*matrixs��Ҫ��˵ľ�����ɵ�����,result����˵Ľ����ָ���ָ��*/
extern int matrix_multiply(struct matrix* matrixs[],struct matrix** result);

/*����ӷֺͼ����Ĺ����㷨*/
/*�ɹ��򷵻�0,ʧ���򷵻�-1*/
/*matrixs��Ҫ����ľ�����ɵ�����,result������Ľ����ָ���ָ��,flag������ѡ���־,0�Ǽ���,1�Ǽӷ�*/
extern int matrix_add_sub_common(struct matrix* matrixs[],struct matrix** result,int flag);

/*����ӷ�*/
/*�ɹ��򷵻�0,ʧ���򷵻�-1*/
/*matrixs��Ҫ��ӵľ�����ɵ�����,result����ӵĽ����ָ���ָ��*/
extern int matrix_add(struct matrix* matrixs[],struct matrix** result);

/*�������*/
/*�ɹ��򷵻�0,ʧ���򷵻�-1*/
/*matrixs��Ҫ����ľ�����ɵ�����,result������Ľ����ָ���ָ��*/
extern int matrix_sub(struct matrix* matrixs[],struct matrix** result);

/*����ת��*/
/*�ɹ��򷵻ؽ��,ʧ���򷵻�NULL*/
extern struct matrix* matrix_transpose(struct matrix* matrix);

extern void print_matrix(struct matrix* param);

#endif
