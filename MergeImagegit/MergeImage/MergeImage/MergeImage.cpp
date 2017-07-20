// MergeImage.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

void GenerateMaskofMatch(const Mat&, const Mat&, Mat&);
void DrawMatches(const Mat& , const vector<KeyPoint>& ,
							   const Mat&, const vector<KeyPoint>&,
							   const vector<DMatch>&, Mat& );

void DrawMatches(const Mat&, const vector<KeyPoint>&,
	const Mat&, const vector<KeyPoint>&,
	const vector<vector<DMatch>>&, Mat&);
void RemoveErrorMatch(vector<vector<DMatch>>&);
void 	GenerateMatchPoints(const vector<KeyPoint>&,
	const vector<KeyPoint>&,
	const vector<vector<DMatch>>&,
	vector<Point2f>&,
	vector<Point2f>&);

int main()
{
	Mat matPersonMat = imread("image//Person.bmp", -1);
	Mat matBackground = imread("image//Background.bmp", -1);
	Mat matPersonKeyPointMat, matBackgroundKeyPointMat, matMask, matAffinePersonMat;

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
	vector<vector<DMatch>>vvdmKNNMatch;
	double dMatchTime = static_cast<double>(getTickCount());
	//BFMatcher bfmMatcher;
	Ptr<DescriptorMatcher>pdmDMatcher = DescriptorMatcher::create("BruteForce");
	
	//bfmMatcher.match(matPersonExtractor, matBackgroundExtractor, vdmDMatch);
	pdmDMatcher->knnMatch(matPersonExtractor, matBackgroundExtractor, vvdmKNNMatch, 2);
	dMatchTime = (static_cast<double>(getTickCount()) - dMatchTime) / getTickFrequency();

	RemoveErrorMatch(vvdmKNNMatch);
	vector<Point2f>vpt2fPersonMatchPoint;
	vector<Point2f>vpt2fBackgroudMatchPoint;
	GenerateMatchPoints(vkpPersonKeyPoints, vkpBackgroundKeyPoints, vvdmKNNMatch, vpt2fPersonMatchPoint, vpt2fBackgroudMatchPoint);

	Mat matAffineMat = estimateRigidTransform(vpt2fPersonMatchPoint, vpt2fBackgroudMatchPoint, false);
	warpAffine(matPersonMat, matAffinePersonMat, matAffineMat, Size(matPersonMat.cols, matPersonMat.rows), 1, 0, Scalar(255, 255, 255));
	Mat matMergeMat = 0.5 * matBackground + 0.5 * matAffinePersonMat;
	//double dMatchTime = static_cast<double>(getTickCount());
	//FlannBasedMatcher fbmMatcher;
	//fbmMatcher.match(matPersonExtractor, matBackgroundExtractor, vdmDMatch);
	//dMatchTime = (static_cast<double>(getTickCount()) - dMatchTime) / getTickFrequency() ;
	//vdmDMatch
	//drawMatches(matPersonMat, vkpPersonKeyPoints,
	//					  matBackground, vkpBackgroundKeyPoints,
	//					  vdmDMatch, matBFMatchMat);
	//DrawMatches(matPersonMat, vkpPersonKeyPoints,
	//					   matBackground, vkpBackgroundKeyPoints,
	//					   vdmDMatch, matBFMatchMat);


	DrawMatches(matPersonMat, vkpPersonKeyPoints,
		matBackground, vkpBackgroundKeyPoints,
		vvdmKNNMatch, matBFMatchMat);

	


	dSurfTime = static_cast<double>(getTickCount() - dSurfTime) / getTickFrequency();
	cout << "Surf Process time is:" << dSurfTime << endl;
	cout << "Match Process time is:" << dMatchTime << endl;
	
	Mat matDisplayPersonMat, matDisplayBackground, matDisplayMergeMat;
	resize(matPersonMat, matDisplayPersonMat, Size(matPersonMat.cols / 3, matPersonMat.rows / 3));
	resize(matBackground, matDisplayBackground, Size(matBackground.cols/3, matBackground.rows/3));
	resize(matPersonKeyPointMat, matPersonKeyPointMat, Size(matPersonKeyPointMat.cols / 3, matPersonKeyPointMat.rows / 3));
	resize(matBackgroundKeyPointMat, matBackgroundKeyPointMat, Size(matBackgroundKeyPointMat.cols / 3, matBackgroundKeyPointMat.rows / 3));
	resize(matBFMatchMat, matBFMatchMat, Size(matBFMatchMat.cols / 2, matBFMatchMat.rows / 2));
	resize(matMergeMat, matDisplayMergeMat, Size(matMergeMat.cols / 2, matMergeMat.rows / 2));



	//namedWindow("matPersonMat");
	//namedWindow("matBackground");
	namedWindow("matPersonKeyPointMat");
	namedWindow("matBackgroundKeyPointMat");
	namedWindow("matBFMatchMat");
	namedWindow("matMergeMat");
	//moveWindow("matPersonMat", 0, 0);
	//moveWindow("matBackground", matDisplayPersonMat.cols, 0);
	moveWindow("matPersonKeyPointMat", 0, 0);
	moveWindow("matBackgroundKeyPointMat", matDisplayPersonMat.cols, 0);
	moveWindow("matBFMatchMat", 0, matDisplayPersonMat.rows);
	moveWindow("matMergeMat", matDisplayPersonMat.cols * 2, 0);

	//imshow("matPersonMat", matDisplayPersonMat);
	//imshow("matBackground", matDisplayBackground);
	imshow("matPersonKeyPointMat", matPersonKeyPointMat);
	imshow("matBackgroundKeyPointMat", matBackgroundKeyPointMat);
	imshow("matBFMatchMat", matBFMatchMat);
	imshow("matMergeMat", matDisplayMergeMat);

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

	int n;
	for(n = 0;  n < vdmDMatch.size(); n++)
	{
		DMatch tempmatch = vdmDMatch[n];

		line(matMatchMat, vkpKeyPoint1[tempmatch.queryIdx].pt, vkpKeyPoint2[tempmatch.trainIdx].pt + Point2f(matKeyPointMat1.cols, 0.0f), Scalar(255, 255, 0), 2);
	}

	int i = 0;
}

void DrawMatches(const Mat& matKeyPointMat1, const vector<KeyPoint>& vkpKeyPoint1,
							   const Mat& matKeyPointMat2, const vector<KeyPoint>& vkpKeyPoint2,
							   const vector<vector<DMatch>>& vdmDMatch, Mat& matMatchMat)
{
	if (matKeyPointMat1.empty() || matKeyPointMat2.empty() ||
		vkpKeyPoint1.empty() || vkpKeyPoint2.empty() ||
		vdmDMatch.empty() || (matKeyPointMat1.size() != matKeyPointMat2.size()))
	{
		cerr << "DrawMatches image inputed error!" << endl;
		exit(-1);
	}

	if (matMatchMat.empty())
	{
		matMatchMat.create(matKeyPointMat1.rows, matKeyPointMat1.cols * 2, matKeyPointMat1.type());
	}

	Rect rectMat0(0, 0, matKeyPointMat1.cols, matKeyPointMat1.rows);
	Rect rectMat1(matKeyPointMat1.cols, 0, matKeyPointMat1.cols, matKeyPointMat1.rows);

	matKeyPointMat1.copyTo(matMatchMat(rectMat0));
	matKeyPointMat2.copyTo(matMatchMat(rectMat1));

	int n;
	for (n = 0; n < vdmDMatch.size(); n++)
	{
		DMatch tempmatch = vdmDMatch[n][0];

		line(matMatchMat, vkpKeyPoint1[tempmatch.queryIdx].pt, vkpKeyPoint2[tempmatch.trainIdx].pt + Point2f(matKeyPointMat1.cols, 0.0f), Scalar(255, 255, 0), 2);
	}

	int i = 0;
}

void RemoveErrorMatch(vector<vector<DMatch>>& vvdmDMatch)
{
	int nc, nr;
	DMatch dmMatch1, dmMatch2;
	for (nr = static_cast<int>(vvdmDMatch.size()) - 1; nr >= 0; nr--)
	{
		dmMatch1 = vvdmDMatch[nr][0];
		dmMatch2 = vvdmDMatch[nr][1];

		if (dmMatch1.distance / dmMatch2.distance > dDistRatio)
		{
			vvdmDMatch.erase(vvdmDMatch.begin() + nr);
		}
	}
}

void 	GenerateMatchPoints(const vector<KeyPoint>& vkpPersonKeyPoints, 
											const vector<KeyPoint>&vkpBackgroundKeyPoints,
											const vector<vector<DMatch>>&vvdmKNNMatch, 
											vector<Point2f>&vpt2fPersonMatchPoint, 
											vector<Point2f>&vpt2fBackgroudMatchPoint)
{
	int nDMCounter;
	for (nDMCounter = 0; nDMCounter < vvdmKNNMatch.size(); nDMCounter++)
	{
		vpt2fPersonMatchPoint.push_back(vkpPersonKeyPoints[vvdmKNNMatch[nDMCounter][0].queryIdx].pt);
		vpt2fBackgroudMatchPoint.push_back(vkpBackgroundKeyPoints[vvdmKNNMatch[nDMCounter][0].trainIdx].pt);
	}

}