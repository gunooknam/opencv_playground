#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap �Լ� ����� ���� �ʿ�
#include <time.h>

using namespace cv;
using namespace std;

/*

  xcos�� + ysin�� = ��

*/


int main() {
	Mat image, edge, result;
	float rho, theta, a, b, x0, y0;
	Point p1, p2;
	vector<Vec2f> lines;
	image = imread("chessboard.png");
	result = image.clone();

	cvtColor(image, image, CV_BGR2GRAY);
	
	Canny(image, 
		  edge,
		  50,
		  200,
		  3);

	HoughLines(edge, lines, 1, CV_PI / 180, 150);
	// P ���̸� �ٷ� ��ǥ ����
	// HoughLinesP(edge, lines, 1, CV_PI / 180, 150,  
	// 10,
	// 300); // �ĸ����� �ΰ� �߰�
	for (int i = 0; i < lines.size(); i++) {
		rho = lines[i][0];
		theta = lines[i][1];
		a = cos(theta);
		b = sin(theta);

		x0 = a * rho;
		y0 = b * rho;

		p1 = Point(cvRound(x0 + 1000*(-b)), cvRound(y0 + 1000 * a));
		p2 = Point(cvRound(x0 - 1000*(-b)), cvRound(y0 - 1000 * a));

		line(result,
			 p1,
			 p2,
			 Scalar(0, 0, 255), 3, 8);
	}
	imshow("input image", image);
	imshow("edge", edge);
	imshow("Hough Transform", result);
	waitKey(0);
}