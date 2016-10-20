#ifndef _PRINCIPAL_COMPONENT
#define _PRINCIPAL_COMPONENT

#include "matrix_operation.h"

struct pricom //���ɷ�
{
    double eigenvalue;//����ֵ
    struct matrix* eigenvectors;//��������
};

extern struct pricom* pricom_alloc();

extern struct pricom* pricom_init(struct pricom* pricom);

extern int pricom_cmp(const void* pa,const void* pb);
extern void pricom_sort(struct pricom* pricoms,int count);
extern void pricom_release(struct pricom* ptr);
extern int principal_component_num(struct pricom* pricoms,int count);

#endif
