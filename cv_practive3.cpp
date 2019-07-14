#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp> // rectangle과 putText 함수 사용을 위해
#include <iostream>
// 상자 쓰기 및 글자 쓰기
using namespace std;
using namespace cv;
// 도형 그리기

int main() {
	Mat image;
	image = imread("hello.jpg"); // CV COLOR  //IMREAD_COLOR 가 디폴트다.
	// imshow("bonobono", image);

	//Rect는 x, y , w, h 이다.
	rectangle(image,
		Rect(100, 100, 100, 100), // 혹은 Point(), Point()도 넣어진다.
		Scalar(255, 0, 0), // 색깔 
		10, // 이건 두께
		2,  // 이건 라인 타입
		0); // @param shift Number of fractional bits in the coordinates of the center and in the radius value.
	

	circle(image,
		Point(50, 50),     // 원의 중심
		32,		           // 원의 반지름
		Scalar(0, 0, 255), //  색깔
		5,                 // 원의 두께
		8);                 //라인의 타입

	ellipse(image,
		Point(200, 200), //  타원의 중심위치
		Size(100, 140), //  x 축으로의 길이, y축으로의 길이
		45,             //  회전될 각도
		0, 360,          // 회전이 시작되는 각도, 끝나는 각도
		Scalar(255, 0, 0), 
		3, 8);           // 타원의 굵기, 선의 타입


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