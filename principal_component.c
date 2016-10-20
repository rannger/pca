#include <stdlib.h>
#include "principal_component.h"

int pricom_cmp(const void* pa,const void* pb)
{
    struct pricom* pricom_a=(struct pricom*)pa;
    struct pricom* pricom_b=(struct pricom*)pb;
    return pricom_a->eigenvalue>pricom_b->eigenvalue?0:1;
}

void pricom_sort(struct pricom* pricoms,int count)
{
    qsort(pricoms,count,sizeof(struct pricom),pricom_cmp);
}

void pricom_release(struct pricom* ptr)
{
    matrix_release(ptr->eigenvectors);
    free(ptr);
}

int principal_component_num(struct pricom* pricoms,int count)
{
    int k,i;
    double sum_temp;
    double sum;
    sum=0.0;
    sum_temp=0.0;

    for(i=0;i<count;i++)
    {
        sum+=pricoms[i].eigenvalue;
    }
    
    for(k=0;k<count;k++)
    {
        sum_temp=0.0;
        for(i=0;i<k;i++)
            sum_temp+=pricoms[i].eigenvalue;
        if(sum_temp/sum>0.9)
            break;
    }
    return k;
}

struct pricom* pricom_init(struct pricom* pricom)
{
    if(pricom)
    {
        pricom->eigenvalue=0.0;
        pricom->eigenvectors=NULL;
    }
    return pricom;
}

struct pricom* pricom_alloc()
{
    return (struct pricom*)malloc(sizeof(struct pricom));
}
