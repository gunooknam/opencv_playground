#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap 함수 사용을 위해 필요
#include <time.h>

using namespace cv;
using namespace std;

/*
  ax + by = ρ 를 모델링 하면 a가 무한대로 표현되는 경우가 있어서 
  그래서 Θ,ρ로 축 모델링
  xcosΘ + ysinΘ = ρ
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

	// 아래건 쓰기가 불편하다.
	//HoughLines(edge, lines, 1, CV_PI / 180, 150);
	// P 붙이면 바로 좌표 붙음
	// HoughLinesP는 굉장히 쓰기편하다. 
	HoughLinesP(edge,
		        lines, 
				1,           // ρ의 sub division의 간격
		        CV_PI / 180, //Θ의 sub division의 간격
		        150,  
				10,
				300); // 파리미터 두개 추가

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