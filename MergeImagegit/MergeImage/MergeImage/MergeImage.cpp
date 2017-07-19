// MergeImage.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

void GenerateMaskofMatch(const Mat&, const Mat&, Mat&);
void DrawMatches(const Mat& , const vector<KeyPoint>& ,
							   const Mat&, const vector<KeyPoint>&,
							   const vector<DMatch>&, Mat& );

int main()
{
	Mat matPersonMat = imread("image//Person.bmp", -1);
	Mat matBackground = imread("image//Background.bmp", -1);
	Mat matPersonKeyPointMat, matBackgroundKeyPointMat, matMask;

	if (matPersonMat.empty() || matBackground.empty())
	{
		cerr << "Images inputed are empty!" <<endl;
		exit(-1);
	}
	//GenerateMaskofMatch(matPersonMat, matBackground, matMask);

	int nminHessian = 2000;
	Ptr<SurfFeatureDetector> ptrsfdDetector = SurfFeatureDetector::create(nminHessian);
	Ptr<SurfDescriptorExtractor> ptrsdfeExtractor = SurfDescriptorExtractor::create();

	Mat matPersonExtractor, matBackgroundExtractor, matBFMatchMat;


	vector<KeyPoint> vkpPersonKeyPoints, vkpBackgroundKeyPoints;
	double dSurfTime = static_cast<double>(getTickCount());
	/*Surf key points detect*/
	ptrsfdDetector->detect(matPersonMat, vkpPersonKeyPoints);
	ptrsfdDetector->detect(matBackground, vkpBackgroundKeyPoints);


	/*Surf key points Descriptor*/
	ptrsdfeExtractor->compute(matPersonMat, vkpPersonKeyPoints, matPersonExtractor);
	ptrsdfeExtractor->compute(matBackground, vkpBackgroundKeyPoints, matBackgroundExtractor);


	/*draw key points detect*/
	drawKeypoints(matPersonMat, vkpPersonKeyPoints, matPersonKeyPointMat, Scalar(255, 255, 0), DrawMatchesFlags::DEFAULT);
	drawKeypoints(matBackground, vkpBackgroundKeyPoints, matBackgroundKeyPointMat, Scalar(255, 255, 0), DrawMatchesFlags::DEFAULT);

	/*BruteForceMatch surf key points*/
	vector<DMatch>vdmDMatch;
	BFMatcher bfmMatcher;
	bfmMatcher.match(matPersonExtractor, matBackgroundExtractor, vdmDMatch);

	//drawMatches(matPersonMat, vkpPersonKeyPoints,
	//					  matBackground, vkpBackgroundKeyPoints,
	//					  vdmDMatch, matBFMatchMat);
	DrawMatches(matPersonMat, vkpPersonKeyPoints,
						   matBackground, vkpBackgroundKeyPoints,
						   vdmDMatch, matBFMatchMat);



	dSurfTime = static_cast<double>(getTickCount() - dSurfTime) / getTickFrequency();
	cout << "Surf Process time is:" << dSurfTime << endl;
	
	Mat matDisplayPersonMat, matDisplayBackground;
	resize(matPersonMat, matDisplayPersonMat, Size(matPersonMat.cols / 3, matPersonMat.rows / 3));
	resize(matBackground, matDisplayBackground, Size(matBackground.cols/3, matBackground.rows/3));
	resize(matPersonKeyPointMat, matPersonKeyPointMat, Size(matPersonKeyPointMat.cols / 3, matPersonKeyPointMat.rows / 3));
	resize(matBackgroundKeyPointMat, matBackgroundKeyPointMat, Size(matBackgroundKeyPointMat.cols / 3, matBackgroundKeyPointMat.rows / 3));
	resize(matBFMatchMat, matBFMatchMat, Size(matBFMatchMat.cols / 2, matBFMatchMat.rows / 2));



	namedWindow("matPersonMat");
	namedWindow("matBackground");
	namedWindow("matPersonKeyPointMat");
	namedWindow("matBackgroundKeyPointMat");
	namedWindow("matBFMatchMat");
	moveWindow("matPersonMat", 0, 0);
	moveWindow("matBackground", matDisplayPersonMat.cols, 0);
	moveWindow("matPersonKeyPointMat", 0, matDisplayPersonMat.rows);
	moveWindow("matBackgroundKeyPointMat", matDisplayPersonMat.cols, matDisplayPersonMat.rows);
	moveWindow("matBFMatchMat", 0, 0);

	imshow("matPersonMat", matDisplayPersonMat);
	imshow("matBackground", matDisplayBackground);
	imshow("matPersonKeyPointMat", matPersonKeyPointMat);
	imshow("matBackgroundKeyPointMat", matBackgroundKeyPointMat);
	imshow("matBFMatchMat", matBFMatchMat);

	waitKey(-1);

    return 0;
}

void GenerateMaskofMatch(const Mat& matPersonMat, const Mat& matBackgroundMat, Mat& matMask)
{
	if (matPersonMat.empty() || matBackgroundMat.empty())
	{
		cerr << "GenerateMaskofMatch function images inputed are empty!" << endl;
		exit(-1);
	}

	Mat matGrayPersonMat, matGrayBackgroundMat;
	cvtColor(matPersonMat, matGrayPersonMat, COLOR_BGR2GRAY);
	cvtColor(matBackgroundMat, matGrayBackgroundMat, COLOR_BGR2GRAY);
	matGrayPersonMat.convertTo(matGrayPersonMat, CV_32SC1);
	matGrayBackgroundMat.convertTo(matGrayBackgroundMat, CV_32SC1);

	Mat matDiffMat = matGrayPersonMat - matGrayBackgroundMat;
	matDiffMat = abs(matDiffMat);

	int i = 0;
}

void DrawMatches(const Mat& matKeyPointMat1, const vector<KeyPoint>& vkpKeyPoint1,
							   const Mat& matKeyPointMat2, const vector<KeyPoint>& vkpKeyPoint2,
							   const vector<DMatch>& vdmDMatch, Mat& matMatchMat)
{
	if (matKeyPointMat1.empty() || matKeyPointMat2.empty() || 
		vkpKeyPoint1.empty() || vkpKeyPoint2.empty() || 
		vdmDMatch.empty() || (matKeyPointMat1.size() != matKeyPointMat2.size()))
	{
		cerr << "DrawMatches image inputed error!" <<endl;
		exit(-1);
	}

	if (matMatchMat.empty())
	{
		matMatchMat.create( matKeyPointMat1.rows, matKeyPointMat1.cols * 2, matKeyPointMat1.type());
	}

	Rect rectMat0(0, 0, matKeyPointMat1.cols, matKeyPointMat1.rows);
	Rect rectMat1(matKeyPointMat1.cols, 0, matKeyPointMat1.cols, matKeyPointMat1.rows);

	matKeyPointMat1.copyTo(matMatchMat(rectMat0));
	matKeyPointMat2.copyTo(matMatchMat(rectMat1));

	int i = 0;

}