#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap �Լ� ����� ���� �ʿ�
#include <time.h>

using namespace cv;
using namespace std;

// Video/ Segmentation
// Chroma-keying
// Surveillance camera �� ���δ�.

// 1 Background subtraction
//  ������ �־����� �������� foreground ��ü Ȯ��

// Code => ����� ���ŵ� ������ ����
// �ٵ� �� �ȵ�
int main() {
	VideoCapture capture("vtest.avi");
	Mat image, sum, avg;
	int cnt = 2;

	capture >> avg; // img1 ����

	// avg=img1 
	
	// loop 1 // image�� img2���� ����
	// img2/2 + img1/2 

	// loop2
	// img3/3 + (img2/2 + img1/2)*2/3

	// loop3
	// img4/4 + (img3/3 + img2/3 + img1/3)*3/4
	// => ����� avg�� �̹������� ����̴�.

	while (true) {

		if (!capture.read(image)) break;
		add(image/cnt,
			avg*(cnt - 1)/cnt,
			avg);

		imshow("avg", avg); 
		// opencv ������ ���� �̸��� â�� �ι� �ҷ����ٸ� â�� �ι� �����°� �ƴ϶� 
		// ���빰�� update �ȴ�. 
		cnt++;
		waitKey(0);
	}

}
