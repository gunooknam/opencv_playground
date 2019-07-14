#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap 함수 사용을 위해 필요
#include <time.h>

using namespace cv;
using namespace std;

// Video/ Segmentation
// Chroma-keying
// Surveillance camera 에 쓰인다.

// 1 Background subtraction
//  비디오가 주어지면 비디오에서 foreground 물체 확인

// Code => 배경이 제거된 영상이 나옴
// 근데 잘 안됨
int main() {
	VideoCapture capture("vtest.avi");
	Mat image, sum, avg;
	int cnt = 2;

	capture >> avg; // img1 꺼냄

	// avg=img1 
	
	// loop 1 // image는 img2부터 꺼냄
	// img2/2 + img1/2 

	// loop2
	// img3/3 + (img2/2 + img1/2)*2/3

	// loop3
	// img4/4 + (img3/3 + img2/3 + img1/3)*3/4
	// => 결론은 avg는 이미지들의 평균이다.

	while (true) {

		if (!capture.read(image)) break;
		add(image/cnt,
			avg*(cnt - 1)/cnt,
			avg);

		imshow("avg", avg); 
		// opencv 에서는 같은 이름의 창이 두번 불려진다면 창이 두번 켜지는게 아니라 
		// 내용물만 update 된다. 
		cnt++;
		waitKey(0);
	}

}
