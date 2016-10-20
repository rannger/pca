#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "pgm.h"


/********************************************/
/*  HISTOGRAM EQUALIZATION of A GRAY IMAGE  */
/********************************************/
static void hist_Eq(unsigned char *pImagedata, long width, long height);

/*从图片文件中读出数据并填充pgm_header结构*/
/*成功返回0,失败返回-1*/
int fill_pgm_header(char* path,struct pgm_header** header)
{
    int result;
    char buffer[128];
    char buf[64];
    int index,i;
    FILE *file;
    struct pgm_header* pgm_header;
    
    result=0;
    file=fopen(path,"r");
    if(!file)
        result=-1;
    else
    {
        //pgm_header=(struct pgm_header*)malloc(sizeof(struct pgm_header));
        pgm_header=pgm_header_alloc_and_init();
        if(!pgm_header)
            result=-1;
        else
        {
            fread(buffer,sizeof(char),128,file);
            index=0;
            if(buffer[0]!='P'&&buffer[1]!='5')
                result=-1;
            else
            {
                index=2;
                i=0;
                pgm_header->magic_num[0]='P';
                pgm_header->magic_num[1]='5';
                index++;
                while(!isspace(buffer[index])&&index<128)
                    buf[i++]=buffer[index++];
                buf[i]='\0';
                pgm_header->width=atoi(buf);
                i=0;
                index++;
                while(!isspace(buffer[index])&&index<128)
                    buf[i++]=buffer[index++];
                buf[i]='\0';
                pgm_header->height=atoi(buf);
                i=0;
                index++;
                while(!isspace(buffer[index])&&index<128)
                    buf[i++]=buffer[index++];
                buf[i]='\0';
                pgm_header->max_gray_value=atoi(buf);
                
                pgm_header->buffer=(unsigned char*)malloc(sizeof(unsigned char)*(pgm_header->height*pgm_header->width));
                pgm_header->size=128-index;
                memcpy(pgm_header->buffer,&buffer[index],128-index);
                int bytes;
                do{
                    bytes=fread(buffer,sizeof(char),128,file);
                    if(!bytes)
                        break;
                    memcpy(pgm_header->buffer+pgm_header->size,buffer,bytes);
                    pgm_header->size+=bytes;
                }while(1);
            }
            fclose(file);
        }
    }
    if(!result)
    {
        int i;
//        for(i=0;i<pgm_header->width*pgm_header->height;i++)
//            pgm_header->buffer[i]/=pgm_header->max_gray_value;
        *header=pgm_header;
        hist_Eq(pgm_header->buffer,pgm_header->width,pgm_header->height);
    }
    
    
    return result;
}
/*销毁pgm_header结构*/
void release_pgm_header(struct pgm_header* header)
{
    if(header)
    {
        free(header->buffer);
        free(header);
    }
}

/*pgm_header结构的工厂方法*/
/*成功返回对象指针,失败返回NULL*/
struct pgm_header* pgm_header_alloc_and_init()
{
    struct pgm_header* result;

    result=(struct pgm_header*)malloc(sizeof(struct pgm_header));
    if(result)
    {
        result->magic_num[0]='\0';
        result->magic_num[1]='\0';
        result->width=0;
        result->height=0;
        result->max_gray_value=0;
        result->buffer=NULL;
        result->size=0;
    }
    return result;
}
/*****************************************************************/
/*  对输入数据四舍五入，取整。                                   */
/*****************************************************************/
int rounding( double x)
{
    int  q;
    double temp;
    temp = (int)x;
    temp = x - temp;
    if(temp>0.5)
        q=(int)x+1;
    else if(temp<-0.5)
        q=(int)x-1;
    else
        q=(int)x;
    return q;
}
/********************************************/
/*  HISTOGRAM EQUALIZATION of A GRAY IMAGE  */
/********************************************/
static void hist_Eq(unsigned char *pImagedata, long width, long height)
{

    static double s_hist_eq[256]={0.0};
    static double sum_of_hist[256]={0.0};

   long   i, j, k, n, state_hst[256]={0};

   unsigned char a;


   n = width * height;

   //1.遍历图像获取图像直方图
   for(i=0;i<height;i++)
   {
	   for(j=0;j<width;j++)
	   {

		a=pImagedata[i*width+j];
		state_hst[a]=state_hst[a]+1;

	   }
   }

   for(i=0;i<256;i++)
	   s_hist_eq[i]=(double)state_hst[i]/(double)n;


	//2.获取输入图像的累积直方图
   sum_of_hist[0] = s_hist_eq[0];

   for (i=1;i<256;i++)        
   {
      sum_of_hist[i] = sum_of_hist[i-1] + s_hist_eq[i];
   }

   //3.由累积直方图确定均衡化后图像的像素值

   for(i=0;i<height;i++)
   {
      for(j=0;j<width;j++)
      {
		  a=pImagedata[i*width+j];
         pImagedata[i*width+j]= (unsigned char)rounding( sum_of_hist[a] * 255.0 );
      }
   }
}
