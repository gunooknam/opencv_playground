#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap 함수 사용을 위해 필요
#include <time.h>
#include <opencv2/video/background_segm.hpp>

using namespace cv;
using namespace std;


// estimate -> p(A|B)를 하는데  
// Conditional Probabilities
// >>> P(A∩B) = p(A|B)p(B) = p(B|A)p(A)

// Bayes Rule 
// >>> p(B|A)=  P(A∩B)/p(A) = ( P(A|B)*P(B) )/p(A) 

// p(A | B) -> 이것을 p(B | A) 로 바꿔서 생각
// Mog는 Gaussian Mixture 모델을 사용하여 Object와 배경을 분리한다.
// 이것을 p(B|A) 로 구한다. A가 있을 때 배경인 확률을 따진다.
int main() {
	Ptr<BackgroundSubtractor> bg_model = createBackgroundSubtractorMOG2();
	Mat image, foregroundMask, backgroundImg, foregroundImg;
	VideoCapture cap("vtest.avi");

	while (true) {
		cap >> image;
		resize(image, image, Size(640, 480));
		if (foregroundMask.empty())
			foregroundMask.create(image.size(), image.type());
		// create 함수로 동일 사이즈 생성

		bg_model->apply(image,			 // 이미지 들어오고
						foregroundMask); // foregroundMask 만든다.

		GaussianBlur(foregroundMask,
				     foregroundMask,
			         Size(11, 11),
			         3.5, 3.5);
		// 마스크에 블러 가한다.

		threshold(foregroundMask,
			      foregroundMask,
			      10,
			      255,
			      THRESH_BINARY);

		foregroundImg = Scalar::all(0);
		image.copyTo(foregroundImg, foregroundMask);
		// 이미지를 foregroundImg에 카피하는데 마스크 있는 것만 들어간다.
		
		bg_model->getBackgroundImage(backgroundImg); // 배경을 뽑는다.
		imshow("foreground mask",  foregroundMask);   
		imshow("foreground image", foregroundImg);
		if (!backgroundImg.empty()) {
			imshow("mean background image", backgroundImg);
		}
		waitKey(33);
	}
}