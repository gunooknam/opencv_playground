#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>
#include <iostream>
#include <direct.h>
#include "opencv2/video/tracking.hpp" // �̰� �����
using namespace std;
using namespace cv;

struct feature {
	Point2f pt;
	int val;
};

bool initialization = false;
void DrawTrackingPoints(vector<Point2f> & points, Mat &image);

int main() {
	VideoCapture cap(0);

	if (!cap.isOpened()) {
		cout << "Cannot open cap" << endl;
		return 0;
	}

	double fps = cap.get(CV_CAP_PROP_FPS);
	Mat currImage, prevImage;
	Mat frame, dstImage;

	double qualityLevel = 0.01;
	double minDistance = 10;
	int blockSize = 3;
	bool useHarrisDetector = false;
	double k = 0.04;
	int maxCorners = 500;

	TermCriteria criteria = TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 10, 0.01);
	Size winSize(11, 11);

	vector<Point2f> prevPoints;
	vector<Point2f> currPoints;
	vector<Point2f> boundPoints;

	int delay = 1000 / fps;
	int nframe = 0;

	while (1) {
		cap >> frame;
		if (frame.empty()) break;
		frame.copyTo(dstImage);
		// Copy the source image
		cvtColor(dstImage, currImage, CV_BGR2GRAY);			 // Gray 
		GaussianBlur(currImage, currImage, Size(5, 5), 0.5); // blur
		// feature detection
		if (initialization) {
			// image�� feature ����
			goodFeaturesToTrack(prevImage, prevPoints,
								maxCorners,
				                qualityLevel,
				                minDistance, Mat(), blockSize, useHarrisDetector, k);

			// ������ ���� ��ǥ�� �����ϰ� �Ѵ�.
			cornerSubPix(prevImage, 
						 prevPoints,
				         winSize,
				         Size(-1, -1),
				         criteria);
						 
			DrawTrackingPoints(prevPoints, dstImage);
			initialization = false;
		}

		if (prevPoints.size()) {

			vector <Mat> prevPyr, currPyr;
			Mat status, err;
			// Pyramid �������� 3���� ���� ����
			buildOpticalFlowPyramid(prevImage, prevPyr, winSize, 3, true);
			buildOpticalFlowPyramid(currImage, currPyr, winSize, 3, true);
		
			calcOpticalFlowPyrLK(prevPyr, currPyr, prevPoints, currPoints, status, err, winSize);
			// invalid corresponding points�� �����.
			
			for (int i = 0; i < prevPoints.size(); i++) {
				if (!status.at<uchar>(i)) { //
					prevPoints.erase(prevPoints.begin() + i);
					currPoints.erase(currPoints.begin() + i);
				}
				else {
					// ����� ���� �̹����� ����Ʈ�� ���� �̹����� ����Ʈ�� �������� ����
					Point p0 = Point(cvRound(prevPoints[i].x), cvRound(prevPoints[i].y));
					Point p1 = Point(cvRound(currPoints[i].x), cvRound(currPoints[i].y));
					arrowedLine(dstImage, p0, p1, CV_RGB(255, 0, 0), 2, 8);
				}
			}
			DrawTrackingPoints(currPoints, dstImage);
			prevPoints = currPoints;

		}

		imshow("dstImage", dstImage);
		currImage.copyTo(prevImage);

		int ch = waitKey(33);
		if (ch == 27) break;
		if (ch == 32) initialization = true; // space ������ 
	}
	return 0;
}



void DrawTrackingPoints(vector <Point2f> &points, Mat &image) {
	// Draw corners detected 
	for (int i = 0; i < points.size(); i++) {
		int x = cvRound(points[i].x);
		int y = cvRound(points[i].y);
		circle(image, Point(x, y), 3, Scalar(255, 0, 0), 0);
	}
}