#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp> // rectangle과 putText 함수 사용을 위해
#include <iostream>

// 상자 쓰기 및 글자 쓰기
using namespace std;
using namespace cv;

int main() {
	Mat image;
	image = imread("hello.jpg"); // CV COLOR  //IMREAD_COLOR 가 디폴트다.
	// imshow("bonobono", image);

	//Rect는 x, y , w, h 이다.
	rectangle(image,
		Rect(40, 40, 100, 100), // 혹은 Point(), Point()도 넣어진다.
		Scalar(255, 0, 0), // 색깔 
		10, // 이건 두께
		1, // 이건 라인 타입
		10);
	// @param shift Number of fractional bits in the coordinates of the center and in the radius value.

	putText(image, "i like bonobono!",
		Point(30, 340),
		FONT_HERSHEY_PLAIN, // 폰트 종류
		2.0,
		Scalar(0,0,255),
		2);

	imshow("new", image);
	waitKey(0);

	return 0;
}