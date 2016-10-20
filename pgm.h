#ifndef _PGM_H
#define _PGM_H


struct pgm_header
{
    char magic_num[2];//魔数
    int width;//图片宽
    int height;//图片高
    int max_gray_value;//灰度值
    unsigned char* buffer;//图片数据
    size_t size;//数据长度
};
/*从图片文件中读出数据并填充pgm_header结构*/
/*成功返回0,失败返回-1*/
extern int fill_pgm_header(char* path,struct pgm_header** header);
/*销毁pgm_header结构*/
extern void release_pgm_header(struct pgm_header* header);
/*pgm_header结构的工厂方法*/
/*成功返回对象指针,失败返回NULL*/
extern struct pgm_header* pgm_header_alloc_and_init();
/*****************************************************************/
/*  对输入数据四舍五入，取整。                                   */
/*****************************************************************/
int rounding( double x);

#endif
