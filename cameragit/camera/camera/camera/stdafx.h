// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO:  在此处引用程序需要的其他头文件
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>


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
#pragma comment(lib, cvLib("videoio"))

using namespace std;
using namespace cv;