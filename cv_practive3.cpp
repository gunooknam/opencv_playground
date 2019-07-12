#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp> // rectangle�� putText �Լ� ����� ����
#include <iostream>

// ���� ���� �� ���� ����
using namespace std;
using namespace cv;

int main() {
	Mat image;
	image = imread("hello.jpg"); // CV COLOR  //IMREAD_COLOR �� ����Ʈ��.
	// imshow("bonobono", image);

	//Rect�� x, y , w, h �̴�.
	rectangle(image,
		Rect(40, 40, 100, 100), // Ȥ�� Point(), Point()�� �־�����.
		Scalar(255, 0, 0), // ���� 
		10, // �̰� �β�
		1, // �̰� ���� Ÿ��
		10);
	// @param shift Number of fractional bits in the coordinates of the center and in the radius value.

	putText(image, "i like bonobono!",
		Point(30, 340),
		FONT_HERSHEY_PLAIN, // ��Ʈ ����
		2.0,
		Scalar(0,0,255),
		2);

	imshow("new", image);
	waitKey(0);

	return 0;
}