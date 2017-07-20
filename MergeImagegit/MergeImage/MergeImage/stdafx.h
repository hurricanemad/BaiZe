// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO:  在此处引用程序需要的其他头文件
#include<iostream>
//#include<opencv2/core.hpp>
//#include<opencv2/highgui.hpp>
//#include<opencv2/imgproc.hpp>
//#include<opencv2/imgcodecs.hpp>
//#include<opencv2/ximgproc.hpp>
#include <opencv2/xfeatures2d.hpp>
//#include <opencv2/features2d.hpp>
//#include <opencv2/flann.hpp>
#include <opencv2/opencv.hpp>


#define VERSION_ID CVAUX_STR(CV_VERSION_MAJOR) CVAUX_STR(CV_VERSION_MINOR) CVAUX_STR(CV_VERSION_REVISION)

#ifdef _DEBUG
	#define cvLib(name) "opencv_" name VERSION_ID "d.lib"
#else
	#define cvLib(name) "opencv_" name VERSION_ID ".lib"
#endif

#pragma comment(lib, cvLib("core"))
#pragma comment(lib, cvLib("highgui"))
#pragma comment(lib, cvLib("imgproc"))
#pragma comment(lib, cvLib("imgcodecs"))
#pragma comment(lib, cvLib("ximgproc"))
#pragma comment(lib, cvLib("xfeatures2d"))
#pragma comment(lib, cvLib("features2d"))
#pragma comment(lib, cvLib("flann"))
#pragma comment(lib, cvLib("objdetect"))
#pragma comment(lib, cvLib("video"))

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

const double dDistRatio = 1.0 / 1.5;