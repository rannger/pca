#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "average_image_vector.h"
#include "pgm.h"
#include "svd.h"
#include "principal_component.h"
#include "filelist.h"
#include "average_image_vector.h"

extern  int picture_check(struct matrix *pictures[],int count,struct matrix *matrix_a,struct pgm_header* check_picture,int distance_func_num);
/*************************************************
 * 欧氏距离
 * 参数是要输入的向量组,数量为
 * **********************************************/
extern double euclidean_distance(struct matrix* matrixs[]);
/*************************************************
 * camnerra距离
 * 参数是要输入的向量组,数量为
 * **********************************************/
double camberra_distance(struct matrix* matrix[]);
/*************************************************
 * x^2距离
 * 参数是要输入的向量组,数量为
 * **********************************************/
double x_2_distance(struct matrix* matrix[]);

typedef double (*distance_func)(struct matrix* matrix[]);

static distance_func funcs[3]={euclidean_distance,camberra_distance,x_2_distance};
char dir_path[128];

int face_detection(char *file_name,int pictures_class,int distance_func_num)
{
     int size=0;
     int retval;
     struct matrix* r=NULL;
     struct matrix* x=NULL;
     retval=-1;
     for(size=0;files[size];size++)
         continue;
     const int SIZE=size-1;

     struct pgm_header* header[SIZE];
     
     int index=0;

     for(index=0;index<SIZE;index++)
         header[index]=NULL;
     /*读取文件数据*/
     /*训练集中的数据都会填充到pgm_header结构体中*/
     for(index=0;index<SIZE;index++)
     {
         char paths[128];
         strcpy(paths,dir_path);
         strcat(paths,files[index]);
         if(fill_pgm_header((char*)paths,&header[index])<0)
         {
             for(index=0;index<SIZE;index++)
                 release_pgm_header(header[index]);
             exit(-1);
         }
     }
     
     struct matrix* pictures[SIZE];
     for(index=0;index<SIZE;index++)
         pictures[index]=NULL;
     /*图像数据矩阵化*/
     /*矩阵数据位于pictures集合中*/
     for(index=0;index<SIZE;index++)
     {
         pictures[index]=matrix_alloc_and_init(1,header[index]->width*header[index]->height);
         if(!pictures[index])
         {
             for(index=0;index<SIZE;index++)
                 release_pgm_header(header[index]);
             for(index=0;index<SIZE;index++)
                 matrix_release(pictures[index]);
             exit(-1);
         }
         int i;
         for(i=0;i<pictures[index]->row*pictures[index]->col;i++)
             pictures[index]->values[i]=header[index]->buffer[i];
             
         
         struct matrix* res=matrix_transpose(pictures[index]);
         if(res)
         {
             matrix_release(pictures[index]);
             pictures[index]=res;
         }
         else
         {
             puts("\nwarning:no memory.\n");
             exit(-1);
         }
     }
     

     /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
     /*****************************************************************/
     int mask=10;
     struct pgm_header* check_picture=NULL;
     char check_file_path[128];
     strcpy(check_file_path,dir_path);
     strcat(check_file_path,file_name);
     if(0>fill_pgm_header(check_file_path,&check_picture))
     {
         puts("sorry,no memory!");
         exit(-1);
     }
     /*****************************************************************/
     /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
     

     /*构造矩阵X=[x_1-u,x_2-u,...,x_c-u]*/
     if(!matrix_construct(pictures,SIZE,&x))
     {
         /*构造矩阵R=X*X */
/*------------------------------------------------------------*/
         if(!matrix_construct_r(x,&r))
         {
             int i,j;
             int k;
             k=0;
             double* v=(double*)malloc(sizeof(double)*(r->row*r->row));
             double* u=(double*)malloc(sizeof(double)*(r->col*r->col));
             memset(v,0,sizeof(double)*(r->row*r->row));
             double* result;
             double eps=0.000001;
             struct pricom *pricoms=(struct pricom*)malloc(sizeof(struct pricom)*r->row);
         
             for(i=0;i<r->row;i++)
                 pricom_init(&pricoms[i]);
             /*矩阵R的特征值和特征向量*/
             if(0<dluav((char*)r->values,r->row,r->col,(double*)u,(double*)v,eps,1000,&result))
             {
                 /*开始构造主成份数据pricom集合pricoms*/
                 for(i=0;i<r->row;i++)
                     pricoms[i].eigenvalue=result[i*r->row+i];
                 /*将特征向量填充的pricoms集合中*/
                 for(i=0;i<r->col;i++)
                     pricoms[i].eigenvectors=matrix_alloc_and_init(1,r->col);
                 for(i=0;i<r->col;i++)
                 {
                     for(j=0;j<r->col;j++)
                         pricoms[i].eigenvectors->values[j]=fabs(v[i*r->col+j])>0.125?1:0;
                 
                     struct matrix* mat_tran=matrix_transpose(pricoms[i].eigenvectors);
                     if(mat_tran)
                     {
                         matrix_release(pricoms[i].eigenvectors);
                         pricoms[i].eigenvectors=mat_tran;
                     }
                 }

                 pricom_sort(pricoms,r->row);//主成份数据整理
                 k=principal_component_num(pricoms,r->row);//主成份数据有效数量,前k个数据项是有效的
                 //主成份数据归一化
                 for(i=0;i<k;i++)
                 {
                     struct matrix* mu;
                     if(eigenvectors_construct_u(pricoms[i].eigenvectors,x,&mu)<0)
                         break;
                     matrix_release(pricoms[i].eigenvectors);
                     pricoms[i].eigenvectors=mu;
                 }
                 //构造投影变换矩阵A
                 
                 struct matrix* matrix_a=NULL;
                 if(matrix_construct_a(pricoms,k,&matrix_a)==0)
                 {
                     //至此,检测算法所需的条件齐全
                     int row,col;

                     if(check_picture)
                     {
                         //图像检测开始
                         retval=picture_check(pictures,SIZE,matrix_a,check_picture,distance_func_num);
                         retval++;
                     }
                 }
        
                 free(result);
                 for(i=0;i<r->row;i++)
                     matrix_release(pricoms[i].eigenvectors);
                 free(pricoms);
                 free(u);
                 free(v);
                 matrix_release(matrix_a);
             }

         }
/*------------------------------------------------------------*/
     }
     for(index=0;index<SIZE;index++)
        matrix_release(pictures[index]);
     release_pgm_header(check_picture);
     for(index=0;index<SIZE;index++)
         if(header[index])
         {
             free(header[index]->buffer);
             free(header[index]);
         }
     
    matrix_release(x);
    matrix_release(r);
    return retval;
    
//    FDResult result;
//    return result;
}

int main(int argc,char *argv[])
{
    int func_num;
    char filename[128];
    int picture_class;
    puts("which distance func do you want to set?\n"
         "0.euclidean_distance\n"
         "1.camberra_distance\n"
         "2.x_2_distance");
    printf("your choice:");
    scanf("%d",&func_num);
    // printf("what is your picture directory:");
    // scanf("%s",dir_path);
    // printf("\n%s\n",dir_path);
    // scanf("%d",&picture_class);
    strcpy(dir_path,"orl_faces/");
    printf("file name:");
    scanf("%s",filename);
    printf("picture class:");

    int record=face_detection(filename,10,1);
    printf("\n%d-----%s\n",check_class(record),files[record-1]);
    return 0;
}

int picture_check(struct matrix* pictures[],const int count,struct matrix *matrix_a,struct pgm_header* check_picture,int func_num)
{
    int result;
    int index;
    struct matrix* matrix_y[count];
    struct matrix* check_y;
    struct matrix* matrix_u;
    struct matrix* check_matrix;
    
    result=0;
    memset(matrix_y,0,sizeof(struct matrix*)*count);
    check_y=NULL;
    matrix_u=NULL;

    if(!average_image_vector(pictures,count,&matrix_u))
    {
        for(index=0;index<count;index++)
        {
            struct matrix* temp[2]={NULL,NULL
            };
            struct matrix* params[2]={NULL,NULL
            };
            
            params[0]=pictures[index];
            params[1]=matrix_u;
            if(!matrix_sub(params,&temp[0]))
            {
                params[0]=matrix_a;
                params[1]=temp[0];
                if(!matrix_multiply(params,&temp[1]))
                    matrix_y[index]=temp[1];
                else
                    puts("error1");
            }
            matrix_release(temp[0]);
        }
        check_matrix=matrix_alloc_and_init(1,check_picture->width*check_picture->height);
        if(check_matrix)
        {
            struct matrix* temp;
            struct matrix* params[2]={NULL,NULL};
            int i;
            for(i=0;i<check_matrix->row*check_matrix->col;i++)
                check_matrix->values[i]=check_picture->buffer[i];
            temp=matrix_transpose(check_matrix);
            if(temp)
            {
                matrix_release(check_matrix);
                check_matrix=temp;
                params[0]=check_matrix;
                params[1]=matrix_u;

                if(!matrix_sub(params,&temp))
                {
                    params[0]=matrix_a;
                    params[1]=temp;
                    if(!matrix_multiply(params,&temp))
                    {
                        check_y=temp;
                        double value;
                        value=0.0;
                        int record=0;
                        int i;
                        struct matrix* params[2];
                        params[0]=check_y;
                        params[1]=matrix_y[0];
                        value=funcs[func_num](params);
                        for(index=1;index<count;index++)
                        {
                            double temp=0.0;
                            params[0]=check_y;
                            params[1]=matrix_y[index];
                            temp=funcs[func_num](params);
                            if(value>temp)
                            {
                                value=temp;
                                record=index;
                            }
                        }
                        result=record;
                    }
                    matrix_release(params[1]);
                }
            }
        }
        matrix_release(check_matrix);
        matrix_release(check_y);
        matrix_release(matrix_u);
        for(index=0;index<count;index++)
            matrix_release(matrix_y[index]);
    }
    
    return result;
}

/*************************************************
 * 欧氏距离
 * 参数是要输入的向量组,数量为
 * **********************************************/
double euclidean_distance(struct matrix* matrixs[])
{
    int i;
    int row=matrixs[0]->row;
    int col=matrixs[1]->col;
    double value;
    value=0.0;
    for(i=0;i<row*col;i++)
        value+=(matrixs[0]->values[i]-matrixs[1]->values[i])*(matrixs[0]->values[i]-matrixs[1]->values[i]);
    value=sqrt(value);
    return value;
}
/*************************************************
 * camnerra距离
 * 参数是要输入的向量组,数量为
 * **********************************************/
double camberra_distance(struct matrix* matrixs[])
{
    int i;
    int row=matrixs[0]->row;
    int col=matrixs[1]->col;
    double value;
    value=0.0;
    for(i=0;i<row*col;i++)
    {
        double a=fabs(matrixs[0]->values[i]-matrixs[1]->values[i]);
        double b=fabs(matrixs[0]->values[i]+matrixs[1]->values[i]);
        if(!b)
            b+=0.000001;
        value+=a/b;
    }
    return value;
}
/*************************************************
 * x^2距离
 * 参数是要输入的向量组,数量为
 * **********************************************/
double x_2_distance(struct matrix* matrixs[])
{
    double value;
    int row=matrixs[0]->row;
    int col=matrixs[1]->col;
    int i;
    value=0.0;
    for(i=0;i<row*col;i++)
    {
        double a=(matrixs[0]->values[i]-matrixs[1]->values[i])*(matrixs[0]->values[i]-matrixs[1]->values[i]);
        double b=(matrixs[0]->values[i]+matrixs[1]->values[i]);
        if(!b)
            b+=0.000001;
        value+=a/b;
    }
    return value;
}

int check_class(int param)
{
    int result;
    if(param<0)
        result=-1;
    result=param/10;
    return result;
}
