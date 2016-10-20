
  int main(int argc,char* argv[])
  {
  struct matrix* matrixs[3];
  struct matrix* result;
    
  int a[3]={6,0,2,
  2,0,-2
  };

  int b[6]={2,0,2,
  8,0,-4
  };

  int c[6]={3,-7,9,
  4,8,9
  };

  int i,j;
  int index;
  for(i=0;i<3;i++)
  {
  matrixs[i]=matrix_alloc_and_init(2,3);
  }
  index=0;
  for(i=0;i<2;i++)
  {
  for(j=0;j<3;j++)
  {
  matrixs[0]->values[i*matrixs[0]->col+j]=a[index];
  index++;
  }
        
  }
    
  puts("");
    
  index=0;
  for(i=0;i<2;i++)
  {
  for(j=0;j<3;j++)
  {
  matrixs[1]->values[i*3+j]=b[index++];
  }
  }

  puts("");
    
  index=0;
  for(i=0;i<2;i++)
  {
        
  for(j=0;j<3;j++)
  {
  matrixs[2]->values[i*3+j]=c[index++];
  }
  }
    
  averave_image_vector(matrixs,3,&result);
  for(i=0;i<2;i++)
  {
  for(j=0;j<3;j++)
  printf("%d ",result->values[i*3+j]);
  puts("");
  }
        
    
  for(i=0;i<3;i++)
  {
  matrix_release(matrixs[i]);
  }
    
  return 0;
  }

/*
  int main(int argc,char* argv[])
  {
  struct matrix* matrixs[3];
  struct matrix* result;
    
  int a[3]={6,0,2,
//              2,0,-2
};

int b[3]={2,0,2,
//              8,0,-4
};

int c[3]={3,-7,9,
//              4,8,9
};

int i,j;
int index;
for(i=0;i<3;i++)
{
matrixs[i]=matrix_alloc_and_init(3,1);
}
index=0;
for(i=0;i<matrixs[0]->row;i++)
{
    for(j=0;j<matrixs[0]->col;j++)
    {
        matrixs[0]->values[i*matrixs[0]->col+j]=a[index];
        index++;
    }
        
}
    
puts("");
    
index=0;
for(i=0;i<matrixs[1]->row;i++)
{
    for(j=0;j<matrixs[1]->col;j++)
    {
        matrixs[1]->values[i*matrixs[1]->col+j]=b[index++];
    }
}

puts("");
    
index=0;
for(i=0;i<matrixs[2]->row;i++)
{
    for(j=0;j<matrixs[2]->col;j++)
    {
        matrixs[2]->values[i*matrixs[2]->col+j]=c[index++];
    }
}
    
matrix_construct(matrixs,3,&result);
for(i=0;i<result->row;i++)
{
    for(j=0;j<result->col;j++)
        printf("%d ",result->values[i*3+j]);
    puts("");
}
        
    
for(i=0;i<3;i++)
{
    matrix_release(matrixs[i]);
}
matrix_release(result);
return 0;
}
*/
/*
int main(int argc,char* argv[])
{
    char *path[]={"/home/administrator/需求/orl_database/s0101.pgm",
                  "/home/administrator/需求/orl_database/s0201.pgm"
    };
    
    struct pgm_header* header[2]={NULL,NULL
    };
    
    int index=0;
    for(index=0;index<2;index++)
    {
        header[index]=pgm_header_alloc_and_init();
    }
    for(index=0;index<2;index++)
        if(fill_pgm_header(path[index],&header[index])<0)
        {
            release_pgm_header(header[0]);
            release_pgm_header(header[1]);
            exit(-1);
        }

    struct matrix* picture[2]={NULL,NULL
    };
    
    for(index=0;index<2;index++)
    {
        picture[index]=matrix_alloc_and_init(header[index]->width*header[index]->height,1);
        if(!picture[index])
        {
            release_pgm_header(header[0]);
            release_pgm_header(header[1]);
            matrix_release(picture[0]);
            matrix_release(picture[1]);
            exit(-1);
        }
        memcpy(picture[index]->values,header[index]->buffer,picture[index]->row*picture[index]->col);
    }
    
    release_pgm_header(header[0]);
    release_pgm_header(header[1]);
    
    struct matrix* x;
    if(matrix_construct(picture,2,&x)<0)
    {
        matrix_release(picture[0]);
        matrix_release(picture[1]);
        exit(-1);
    }
    struct matrix* r;
    if(matrix_construct_r(x,&r)<0)
        goto out;
    
    int i,j;
 	double eps=0.000001;
	double *v=(double*)malloc(sizeof(double)*(r->row*r->row));
	double *result;
	if(0<jcbi((char*)r->values,r->row,(double*)v,eps,100,&result))
	{
        double *temp=(double*)malloc(sizeof(double)*r->row);
        for(i=0;i<r->row;i++)
            temp[i]=result[i*r->row+i];
        qsort(temp,r->row,sizeof(double),double_cmp);
        for(i=0;i<r->row;i++)
            printf("%e ",temp[i]);
        free(temp);
        puts("");
	}

    for(i=0;i<r->row;i++)
    {
        for(j=0;j<r->col;j++)
            printf("%d ",r->values[i*x->col+j]);
        puts("");
    }
  out:
    matrix_release(picture[0]);
    matrix_release(picture[1]);
    matrix_release(x);
    matrix_release(r);
    free(v);
    free(result);
    return 0;
}
*/
