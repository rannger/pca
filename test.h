#include <vector>
using namespace std;

//#define BYTE unsigned char
typedef unsigned char BYTE;


// 点
struct Point
{

    int x;
    //横坐标
    int y;
    //纵坐标
};


// 区域
struct Area
{

    Point lowLeft;
    //左下角
    Point lowRight;
    //右下角
    Point upLeft;
    //左上角
    Point upRight;
    //右上角
};


// 尺寸
enum Size
{
    S_large,S_medium,S_small
};


// 特征
struct Feature
{

    unsigned int color;
    //颜色
    Size size;
    //尺寸
};


// 人脸特征
struct Face
{

    Area aHair;
    //头发区域
    Area aEyebrows;
    //眼眉区域
    Area aEyes;
    //眼睛区域
    Area aLEar;
    //左耳区域
    Area aREar;
    //右耳区域
    Area aNose;
    //鼻子区域
    Area aMouth;
    //嘴巴区域
    Area aChinBeard;
    //下巴胡须区域
    Area aLipBeard;
    //唇部胡须区域

    Feature fFace;
    //面部整体特征
    Feature fHair;
    //头发特征
    Feature fEyebrows;
    //眼眉特征
    Feature fEyes;
    //眼睛特征
    Feature fEars;
    //耳朵特征
    Feature fNose;
    //鼻子特征
    Feature fMouth;
    //嘴巴特征
    Feature fChinBeard;
    //下巴胡须特征
    Feature fLipBeard;
    //唇部胡须特征
};


// 颜色空间
enum SAColorSpace
{
    SA_RGB24,SA_I420
};


// 人脸检测结果
struct FDResult
{

    bool err;
    //检测过程中出错
    bool bFace;
    //当前图像中是否存在人脸
    vector<Face*> lFace;
    //检测到的人脸列表
};


// 人脸搜索结果
struct FSResult
{

    bool err;
    //搜索过程中出错
    bool bFace;
    //当前人脸库中是否存在此特征的人脸
    vector<char*> lName;
    //搜索到的图片名称列表
};



// 人脸检测-检测出输入图片中所有人脸
FDResult FaceDetection(BYTE *pFrame/*输入图像*/,int width/*图像宽度*/,
                       int height/*图像高度*/,SAColorSpace colorSpace/*颜色空间*/);

