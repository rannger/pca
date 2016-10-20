#ifndef _PGM_H
#define _PGM_H


struct pgm_header
{
    char magic_num[2];//ħ��
    int width;//ͼƬ��
    int height;//ͼƬ��
    int max_gray_value;//�Ҷ�ֵ
    unsigned char* buffer;//ͼƬ����
    size_t size;//���ݳ���
};
/*��ͼƬ�ļ��ж������ݲ����pgm_header�ṹ*/
/*�ɹ�����0,ʧ�ܷ���-1*/
extern int fill_pgm_header(char* path,struct pgm_header** header);
/*����pgm_header�ṹ*/
extern void release_pgm_header(struct pgm_header* header);
/*pgm_header�ṹ�Ĺ�������*/
/*�ɹ����ض���ָ��,ʧ�ܷ���NULL*/
extern struct pgm_header* pgm_header_alloc_and_init();
/*****************************************************************/
/*  �����������������룬ȡ����                                   */
/*****************************************************************/
int rounding( double x);

#endif
