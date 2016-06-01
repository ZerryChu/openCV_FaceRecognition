#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include "detect_recog.h"
#include <opencv2\contrib\contrib.hpp>  
#include <opencv2\core\core.hpp>  
#include <opencv2\highgui\highgui.hpp> 
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <io.h>  
#include <direct.h> 
#include <sys/types.h>
#include <conio.h>
#include "map_and_path.h"

#include "handleResult.h"
#include "helper.hpp"
#include "eigenfaces.hpp"
using namespace std;
using namespace cv;

int recognition(Mat test) {
	int num_components = 80;
	// compute the eigenfaces
	Eigenfaces eigenfaces(images, labels, num_components, dConfidence);
	// get a prediction
	int result = 0;
	int predicted = eigenfaces.predict(test);
	if (predicted != -1) {
		result = handleResult(predicted);
		// return predicted;
	}
	if (1 == result) {
		return predicted;
	}
	else if (0 == result) {
		return 0;
	}
	else
		return -1;
}

static int hasParsed = 0;
// 验证照片
int recog_and_draw2(string sourcePic) {
	//Mat img = imread(sourcePic, CV_LOAD_IMAGE_GRAYSCALE);
	IplImage* img = cvLoadImage(sourcePic.c_str());
	//int num = recognition(FILEPATH, img);

	if (!hasParsed) {
		hasParsed = 1;
		parseXML();
	}

	static CvScalar colors[] =
	{
		{ { 0, 0, 255 } },
		{ { 0, 128, 255 } },
		{ { 0, 255, 255 } },
		{ { 0, 255, 0 } },
		{ { 255, 128, 0 } },
		{ { 255, 255, 0 } },
		{ { 255, 0, 0 } },
		{ { 255, 0, 255 } }
	};
	IplImage *gray, *small_img;
	int i, j;
	int number = -1;
	gray = cvCreateImage(cvSize(img->width, img->height), 8, 1);
	small_img = cvCreateImage(cvSize(cvRound(img->width / scale),
		cvRound(img->height / scale)), 8, 1);
	cvCvtColor(img, gray, CV_BGR2GRAY); // 彩色RGB图像转为灰度图像 
	cvResize(gray, small_img, CV_INTER_LINEAR);
	cvEqualizeHist(small_img, small_img); // 直方图均衡化 
	cvClearMemStorage(storage);
	if (cascade)
	{
		double t = (double)cvGetTickCount();
		CvSeq* faces = cvHaarDetectObjects(small_img, cascade, storage,
			1.1, 2, 0
			|CV_HAAR_FIND_BIGGEST_OBJECT
			//|CV_HAAR_DO_ROUGH_SEARCH
			//| CV_HAAR_DO_CANNY_PRUNING
			//|CV_HAAR_SCALE_IMAGE
			,
			cvSize(30, 30));
		t = (double)cvGetTickCount() - t; // 统计检测使用时间 
		//printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );
		for (i = 0; i < (faces ? faces->total : 0); i++)
		{
			CvRect* r = (CvRect*)cvGetSeqElem(faces, i); // 将faces数据从CvSeq转为CvRect 
			CvMat small_img_roi;
			//			CvSeq* nested_objects;
			CvPoint center, recPt1, recPt2;
			CvScalar color = colors[i % 8]; // 使用不同颜色绘制各个face，共八种色 
			int radius;
			center.x = cvRound((r->x + r->width*0.5)*scale); // 找出faces中心 
			center.y = cvRound((r->y + r->height*0.5)*scale);
			recPt1.x = cvRound((r->x)*scale);
			recPt1.y = cvRound((r->y)*scale);
			recPt2.x = cvRound((r->x + r->width)*scale);
			recPt2.y = cvRound((r->y + r->height)*scale);
			radius = cvRound((r->width + r->height)*0.25*scale);

			cvGetSubRect(small_img, &small_img_roi, *r);

			IplImage *result;
			CvRect roi;
			roi = *r;
			result = cvCreateImage(cvSize(r->width, r->height), img->depth, img->nChannels);
			cvSetImageROI(img, roi);
			// 创建子图像
			cvCopy(img, result);
			cvResetImageROI(img);

			//IplImage *resizeRes;
			CvSize dst_cvsize;
			dst_cvsize.width = (int)(100);
			dst_cvsize.height = (int)(100);
			resizeRes = cvCreateImage(dst_cvsize, result->depth, result->nChannels);
			cvResize(result, resizeRes, CV_INTER_NN);

			faceGray = cvCreateImage(cvGetSize(resizeRes), IPL_DEPTH_8U, 1);//创建目标图像	
			cvCvtColor(resizeRes, faceGray, CV_BGR2GRAY);//cvCvtColor(src,des,CV_BGR2GRAY)
			cvShowImage("result", resizeRes);
			cvRectangle(img, recPt1, recPt2, color, 3, 8, 0);
			//cvCircle( img, center, radius, color, 3, 8, 0 ); // 从中心位置画圆，圈出脸部区域

			Mat test = faceGray;
			//images[images.size() - 1] = test;


			number = recognition(test);
			if (-1 != number) {
				cvText(img, mp[number].name.c_str(), r->x, r->y);
				cvText(img, "Result: YES", 0, 30);
			}
			else {
				cvText(img, "not found.", r->x, r->y);
				cvText(img, "Result: ", 0, 30);
			}
		}
	}
	//cvShowImage( "result", img );
	cvReleaseImage(&img);
	cvReleaseImage(&gray);
	cvReleaseImage(&small_img);
	return number;
}

void detect_and_draw( IplImage* img ) // 只是检测，并圈出人脸
{

    static CvScalar colors[] = 
    {
        {{0,0,255}},
        {{0,128,255}},
        {{0,255,255}},
        {{0,255,0}},
        {{255,128,0}},
        {{255,255,0}},
        {{255,0,0}},
        {{255,0,255}}
    };
    IplImage *gray, *small_img;
    int i, j;
    gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );
    small_img = cvCreateImage( cvSize( cvRound (img->width/scale),
                         cvRound (img->height/scale)), 8, 1 );
    cvCvtColor( img, gray, CV_BGR2GRAY ); // 彩色RGB图像转为灰度图像 
    cvResize( gray, small_img, CV_INTER_LINEAR );
    cvEqualizeHist( small_img, small_img ); // 直方图均衡化 
    cvClearMemStorage( storage );
    if( cascade )
    {
        double t = (double)cvGetTickCount(); 
        CvSeq* faces = cvHaarDetectObjects( small_img, cascade, storage,
                                            1.1, 2, 0
                                            //|CV_HAAR_FIND_BIGGEST_OBJECT
                                           // |CV_HAAR_DO_ROUGH_SEARCH
                                            |CV_HAAR_DO_CANNY_PRUNING
                                            //|CV_HAAR_SCALE_IMAGE
                                            ,
                                            cvSize(30, 30) );
        t = (double)cvGetTickCount() - t; // 统计检测使用时间 
        //printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );
        for( i = 0; i < (faces ? faces->total : 0); i++ )
        {
            CvRect* r = (CvRect*)cvGetSeqElem( faces, i ); // 将faces数据从CvSeq转为CvRect 
            CvMat small_img_roi;
            CvSeq* nested_objects;
            CvPoint center,recPt1,recPt2;
            CvScalar color = colors[i%8]; // 使用不同颜色绘制各个face，共八种色 
            int radius;
            center.x = cvRound((r->x + r->width*0.5)*scale); // 找出faces中心 
            center.y = cvRound((r->y + r->height*0.5)*scale);
			recPt1.x = cvRound((r->x)*scale);
			recPt1.y = cvRound((r->y)*scale);
			recPt2.x = cvRound((r->x + r->width)*scale);
			recPt2.y = cvRound((r->y + r->height)*scale);
            radius = cvRound((r->width + r->height)*0.25*scale); 
				
			cvGetSubRect( small_img, &small_img_roi, *r );
			
			IplImage *result;
			CvRect roi;
			roi = *r;
			result = cvCreateImage( cvSize(r->width, r->height), img->depth, img->nChannels );
			cvSetImageROI(img,roi);
			// 创建子图像
			cvCopy(img,result);
			cvResetImageROI(img);
			
			//IplImage *resizeRes;
			CvSize dst_cvsize;
			dst_cvsize.width=(int)(100);
			dst_cvsize.height=(int)(100);
			resizeRes=cvCreateImage(dst_cvsize,result->depth,result->nChannels);
			cvResize(result,resizeRes,CV_INTER_NN);
			faceGray = cvCreateImage(cvGetSize(resizeRes), IPL_DEPTH_8U, 1);//创建目标图像	
			cvCvtColor(resizeRes,faceGray,CV_BGR2GRAY);//cvCvtColor(src,des,CV_BGR2GRAY)
            cvShowImage( "result", resizeRes );
			cvRectangle(img,recPt1,recPt2,color,1, 8,0);
			//rectangle(img,recPt1,recPt2,color,1,8,0);
			//cvCircle( img, center, radius, color, 3, 8, 0 ); // 从中心位置画圆，圈出脸部区域
           /*  if( !nested_cascade )
                continue;
            nested_objects = cvHaarDetectObjects( &small_img_roi, nested_cascade, storage,
                                        1.1, 2, 0
                                        //|CV_HAAR_FIND_BIGGEST_OBJECT
                                        //|CV_HAAR_DO_ROUGH_SEARCH
                                        //|CV_HAAR_DO_CANNY_PRUNING
                                        //|CV_HAAR_SCALE_IMAGE
                                        ,
                                        cvSize(0, 0) );
            for( j = 0; j < (nested_objects ? nested_objects->total : 0); j++ )
            {
                CvRect* nr = (CvRect*)cvGetSeqElem( nested_objects, j );
                center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
                center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
                radius = cvRound((nr->width + nr->height)*0.25*scale);
                cvCircle( img, center, radius, color, 3, 8, 0 );
            }
			*/
        }
    }
    //cvShowImage( "result", img );
    cvReleaseImage( &gray );
    cvReleaseImage( &small_img );

}

int attendence[30];
int has_init = 0;
//检测并识别人脸，并在每帧图片上写入结果
int recog_and_draw( IplImage* img )
{
	if (!hasParsed) {
		hasParsed = 1;
		parseXML();
	}

	if (!has_init) {
		has_init = 1;
		memset(attendence, 0, sizeof(attendence));
	}

    static CvScalar colors[] = 
    {
        {{0,0,255}},
        {{0,128,255}},
        {{0,255,255}},
        {{0,255,0}},
        {{255,128,0}},
        {{255,255,0}},
        {{255,0,0}},
        {{255,0,255}}
    };
    IplImage *gray, *small_img;
    int i, j;
	int number = -1;
    gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );
    small_img = cvCreateImage( cvSize( cvRound (img->width/scale),
                         cvRound (img->height/scale)), 8, 1 );
    cvCvtColor( img, gray, CV_BGR2GRAY ); // 彩色RGB图像转为灰度图像 
    cvResize( gray, small_img, CV_INTER_LINEAR );
    cvEqualizeHist( small_img, small_img ); // 直方图均衡化 
    cvClearMemStorage( storage );
    if( cascade )
    {
        //double t = (double)cvGetTickCount(); 
        CvSeq* faces = cvHaarDetectObjects( small_img, cascade, storage,
                                            1.1, 2, 0
                                            |CV_HAAR_FIND_BIGGEST_OBJECT
                                            //|CV_HAAR_DO_ROUGH_SEARCH
                                            //|CV_HAAR_DO_CANNY_PRUNING
                                            //|CV_HAAR_SCALE_IMAGE
                                            ,
                                            cvSize(30, 30) );
       // t = (double)cvGetTickCount() - t; // 统计检测使用时间 
        //printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );
		for (i = 0; i < (faces ? faces->total : 0); i++)
		{
			CvRect* r = (CvRect*)cvGetSeqElem(faces, i); // 将faces数据从CvSeq转为CvRect 
			CvMat small_img_roi;
//			CvSeq* nested_objects;
			CvPoint center, recPt1, recPt2;
			CvScalar color = colors[i % 8]; // 使用不同颜色绘制各个face，共八种色 
			int radius;
			center.x = cvRound((r->x + r->width*0.5)*scale); // 找出faces中心 
			center.y = cvRound((r->y + r->height*0.5)*scale);
			recPt1.x = cvRound((r->x)*scale);
			recPt1.y = cvRound((r->y)*scale);
			recPt2.x = cvRound((r->x + r->width)*scale);
			recPt2.y = cvRound((r->y + r->height)*scale);
			radius = cvRound((r->width + r->height)*0.25*scale);

			cvGetSubRect(small_img, &small_img_roi, *r);

			IplImage *result;
			CvRect roi;
			roi = *r;
			result = cvCreateImage(cvSize(r->width, r->height), img->depth, img->nChannels);
			cvSetImageROI(img, roi);
			// 创建子图像
			cvCopy(img, result);
			cvResetImageROI(img);

			//IplImage *resizeRes;
			CvSize dst_cvsize;
			dst_cvsize.width = (int)(100);
			dst_cvsize.height = (int)(100);
			resizeRes = cvCreateImage(dst_cvsize, result->depth, result->nChannels);
			cvResize(result, resizeRes, CV_INTER_NN);

			faceGray = cvCreateImage(cvGetSize(resizeRes), IPL_DEPTH_8U, 1);//创建目标图像	
			cvCvtColor(resizeRes, faceGray, CV_BGR2GRAY);//cvCvtColor(src,des,CV_BGR2GRAY)
			cvShowImage("result", resizeRes);
			cvRectangle(img, recPt1, recPt2, color, 3, 8, 0);
			//cvCircle( img, center, radius, color, 3, 8, 0 ); // 从中心位置画圆，圈出脸部区域
			
			//Mat test = resizeRes;
		    Mat test = faceGray;
			//images[images.size() - 1] = test;
	
			number = recognition(test);
			if (0 == number) {
				cvText(img, "checking...please wait.", r->x, r->y);
			}
			else if (-1 == number) {
				cvText(img, "not found. check again.", r->x, r->y);
			}
			// predict succeeded
			else {
				cvText(img, mp[number].name.c_str(), r->x, r->y);
			}		
			
			/*
			if( !nested_cascade )
			continue;

			nested_objects = cvHaarDetectObjects( &small_img_roi, nested_cascade, storage,
			1.1, 2, 0
			//|CV_HAAR_FIND_BIGGEST_OBJECT
			//|CV_HAAR_DO_ROUGH_SEARCH
			//|CV_HAAR_DO_CANNY_PRUNING
			//|CV_HAAR_SCALE_IMAGE
			,
			cvSize(0, 0) );
			for( j = 0; j < (nested_objects ? nested_objects->total : 0); j++ )
			{
			CvRect* nr = (CvRect*)cvGetSeqElem( nested_objects, j );
			center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
			center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
			radius = cvRound((nr->width + nr->height)*0.25*scale);
			cvCircle( img, center, radius, color, 3, 8, 0 );
			}
			*/
			  }
		}
    //cvShowImage( "result", img );
    cvReleaseImage( &gray );
    cvReleaseImage( &small_img );
	return number;
}

void cvText(IplImage* img, const char* text, int x, int y)  
{  
    CvFont font;  
    double hscale = 1.0;  
    double vscale = 1.0;  
    int linewidth = 2;  
    cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC,hscale,vscale,0,linewidth);  
    CvScalar textColor =cvScalar(0,255,255);  
    CvPoint textPos =cvPoint(x, y);  
    cvPutText(img, text, textPos, &font,textColor);  
}

Mat norm_0_255(cv::InputArray _src)
{
	Mat src = _src.getMat();
	Mat dst;

	switch(src.channels())
	{
	case 1:
		cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC1);
		break;
	case 3:
		cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC3);
		break;
	default:
		src.copyTo(dst);
		break;
	}
	return dst;
}

void read_trainningFiles() {
	//读取你的CSV文件路径.
	string fn_csv = FILEPATH;
	// 2个容器来存放图像数据和对应的标签
	// 读取数据. 如果文件不合法就会出错
	// 输入的文件名已经有了.
	try {
		read_csv(fn_csv, images, labels);

	}

	catch (cv::Exception& e) {
		cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
		exit(1);
	}

	// 如果没有读取到足够图片，退出.
	if (images.size() <= 1) {
		string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
		//CV_Error(CV_StsError, error_message);
	}
}

void read_csv(const string &filename, vector<Mat> &images, vector<int> &labels, char separator)
{
	std::ifstream file(filename.c_str(), ifstream::in);
	if(!file)
	{
		string error_message = "No valid input file was given.";
		CV_Error(CV_StsBadArg, error_message);
	}

	string line, path, classlabel;
	while(getline(file, line))
	{
		stringstream liness(line);
		getline(liness, path, separator);  //遇到分号就结束
		getline(liness, classlabel);     //继续从分号后面开始，遇到换行结束
		if(!path.empty() && !classlabel.empty())
		{
			images.push_back(imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
}

//实现了从trainningdata 目录下读取jpg文件数目
int read_img_number()
{
	long file;  
	int i = 0;
    struct _finddata_t find; 
	//AllocConsole();
	string path = "..//einfacedata//trainingdata/";
    if((file=_findfirst("..//einfacedata//trainingdata/*.jpg", &find))==-1L) 
        return i;
	i++;
    while(_findnext(file, &find)==0)  
    {  
		i++;
    }  
    _findclose(file);
	return i;
}
