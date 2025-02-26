// camera.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
void addpictuer(Mat & src1, double alpha, Mat & src2, double beta, Mat &dst);
void bold(const Mat & src1, Mat & src2);

int main()
{
 	VideoCapture capture(0);

	capture.set(CAP_PROP_FRAME_WIDTH, 1280); // 设置摄像头的像素值，为1280 x 720
	capture.set(CAP_PROP_FRAME_HEIGHT, 720);

	if (!capture.isOpened())                  // 判断是否有摄像头
	{
		cout << "No Camera Input! " << endl;
		exit(-1);
	}
	
	int nKey = '\0';

	while ('\r' != nKey) 
	{
		double dProcessTime = static_cast<double>(getTickCount());
		Mat frame,framegray,framemake, framemakewide,framenew;

		capture >> frame;                                    

		cvtColor(frame, framegray, COLOR_BGR2GRAY);   // 灰度化

		//blur(framegray, framegray, Size(3, 3));       // 滤波
		GaussianBlur(framegray, framegray, Size(5, 5), 0.67);

		Canny(framegray, framemake, 80, 150, 3);      //边缘检测

		Mat element = getStructuringElement(MORPH_RECT, Size(2, 2));  // 膨胀 加粗步骤

		morphologyEx(framemake, framemake, MORPH_DILATE, element);

		//bold(framemake, framemakewide);

		cvtColor(framemake, framemake, CV_GRAY2BGR);            //融合函数
		addWeighted(frame, 1.0, framemake, 1.0, 0.0, framenew);
		//addpictuer(frame, 1, framemakewide, 1.0, framenew);
	
		imshow("photo", frame);
		imshow("photonew", framenew);

		nKey = waitKey(1);
		dProcessTime = (static_cast<double>(getTickCount()) - dProcessTime) / getTickFrequency();
		cout << dProcessTime << endl;

		cout << "Frame Width:" << frame.cols << "\tFrame Height:" << frame.rows <<endl;
	}



    return 0;
}

//--------------------------------融合函数----------------------------------

void addpictuer(Mat & src1, double alpha, Mat & src2, double beta, Mat &dst)
{
	if ((src1.empty()) && (src2.empty()))
	{
		exit(-1);
	}

	dst.create(src1.size(), src1.type());

	for (int j = 0; j < src1.rows; j++) {
		Vec3b* data1 = src1.ptr<Vec3b>(j);
		uchar* data2 = src2.ptr<uchar>(j);
		Vec3b* data3 = dst.ptr<Vec3b>(j);
		for (int i = 0; i < src1.cols; i++) {

			data3[i][0] = saturate_cast<uchar>(alpha*data1[i][0] + beta*data2[i]);
			data3[i][1] = saturate_cast<uchar>(alpha*data1[i][1] + beta*data2[i]);
			data3[i][2] = saturate_cast<uchar>(alpha*data1[i][2] + beta*data2[i]);

		}
	}

}
void bold(const Mat& src1, Mat & src2)
{
	src2.create(src1.size(), src1.type());

	for (int j = 0; j < src1.rows; j++) {
		const uchar* data1 = src1.ptr<uchar>(j);
		uchar* data2 = src2.ptr<uchar>(j);
		for (int i = 0; i < src1.cols; i++) {
			if (data1[i] == 255)
			{
				data2[i - 2] = 255;
				data2[i - 1] = 255;
				data2[i] = 255;
				data2[i + 1] = 255;
				data2[i + 2] = 255;
			}
			else
			{
				data2[i] = data1[i];
			}
		}
	}
}