#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap �Լ� ����� ���� �ʿ�
#include <time.h>

using namespace cv;
using namespace std;

/*
  ax + by = �� �� �𵨸� �ϸ� a�� ���Ѵ�� ǥ���Ǵ� ��찡 �־ 
  �׷��� ��,��� �� �𵨸�
  xcos�� + ysin�� = ��
*/

int main() {
	Mat image, edge, result;
	float rho, theta, a, b, x0, y0;
	Point p1, p2;
	vector<Vec4f> lines;
	image = imread("chessboard.png");
	resize(image, image, Size(), 0.20, 0.20);

	result = image.clone();
	cvtColor(image, image, CV_BGR2GRAY);
	Canny(image, 
		  edge,
		  50,
		  200,
		  3);

	// �Ʒ��� ���Ⱑ �����ϴ�.
	//HoughLines(edge, lines, 1, CV_PI / 180, 150);
	// P ���̸� �ٷ� ��ǥ ����
	// HoughLinesP�� ������ �������ϴ�. 
	HoughLinesP(edge,
		        lines, 
				1,           // ���� sub division�� ����
		        CV_PI / 180, //���� sub division�� ����
		        150,  
				10,
				300); // �ĸ����� �ΰ� �߰�

	for (int i = 0; i < lines.size(); i++) {
		Vec4i l = lines[i];

		line(result,
			 Point(l[0], l[1]),
			 Point(l[2], l[3]),
			 Scalar(0, 0, 255),
			 3, 8);
	}
	imshow("input image", image);
	imshow("edge", edge);
	imshow("Hough Transform", result);
	waitKey(0);
}