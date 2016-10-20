#include <vector>
using namespace std;

//#define BYTE unsigned char
typedef unsigned char BYTE;


// ��
struct Point
{

    int x;
    //������
    int y;
    //������
};


// ����
struct Area
{

    Point lowLeft;
    //���½�
    Point lowRight;
    //���½�
    Point upLeft;
    //���Ͻ�
    Point upRight;
    //���Ͻ�
};


// �ߴ�
enum Size
{
    S_large,S_medium,S_small
};


// ����
struct Feature
{

    unsigned int color;
    //��ɫ
    Size size;
    //�ߴ�
};


// ��������
struct Face
{

    Area aHair;
    //ͷ������
    Area aEyebrows;
    //��ü����
    Area aEyes;
    //�۾�����
    Area aLEar;
    //�������
    Area aREar;
    //�Ҷ�����
    Area aNose;
    //��������
    Area aMouth;
    //�������
    Area aChinBeard;
    //�°ͺ�������
    Area aLipBeard;
    //������������

    Feature fFace;
    //�沿��������
    Feature fHair;
    //ͷ������
    Feature fEyebrows;
    //��ü����
    Feature fEyes;
    //�۾�����
    Feature fEars;
    //��������
    Feature fNose;
    //��������
    Feature fMouth;
    //�������
    Feature fChinBeard;
    //�°ͺ�������
    Feature fLipBeard;
    //������������
};


// ��ɫ�ռ�
enum SAColorSpace
{
    SA_RGB24,SA_I420
};


// ���������
struct FDResult
{

    bool err;
    //�������г���
    bool bFace;
    //��ǰͼ�����Ƿ��������
    vector<Face*> lFace;
    //��⵽�������б�
};


// �����������
struct FSResult
{

    bool err;
    //���������г���
    bool bFace;
    //��ǰ���������Ƿ���ڴ�����������
    vector<char*> lName;
    //��������ͼƬ�����б�
};



// �������-��������ͼƬ����������
FDResult FaceDetection(BYTE *pFrame/*����ͼ��*/,int width/*ͼ����*/,
                       int height/*ͼ��߶�*/,SAColorSpace colorSpace/*��ɫ�ռ�*/);

