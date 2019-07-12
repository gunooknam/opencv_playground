#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap �Լ� ����� ���� �ʿ�
#include <time.h>
#include <opencv2/video/background_segm.hpp>

using namespace cv;
using namespace std;


// estimate -> p(A|B)�� �ϴµ�  
// Conditional Probabilities
// >>> P(A��B) = p(A|B)p(B) = p(B|A)p(A)

// Bayes Rule 
// >>> p(B|A)=  P(A��B)/p(A) = ( P(A|B)*P(B) )/p(A) 

// p(A | B) -> �̰��� p(B | A) �� �ٲ㼭 ����
// Mog�� Gaussian Mixture ���� ����Ͽ� Object�� ����� �и��Ѵ�.
// �̰��� p(B|A) �� ���Ѵ�. A�� ���� �� ����� Ȯ���� ������.
int main() {
	Ptr<BackgroundSubtractor> bg_model = createBackgroundSubtractorMOG2();
	Mat image, foregroundMask, backgroundImg, foregroundImg;
	VideoCapture cap("vtest.avi");

	while (true) {
		cap >> image;
		resize(image, image, Size(640, 480));
		if (foregroundMask.empty())
			foregroundMask.create(image.size(), image.type());
		// create �Լ��� ���� ������ ����

		bg_model->apply(image,			 // �̹��� ������
						foregroundMask); // foregroundMask �����.

		GaussianBlur(foregroundMask,
				     foregroundMask,
			         Size(11, 11),
			         3.5, 3.5);
		// ����ũ�� �� ���Ѵ�.

		threshold(foregroundMask,
			      foregroundMask,
			      10,
			      255,
			      THRESH_BINARY);

		foregroundImg = Scalar::all(0);
		image.copyTo(foregroundImg, foregroundMask);
		// �̹����� foregroundImg�� ī���ϴµ� ����ũ �ִ� �͸� ����.
		
		bg_model->getBackgroundImage(backgroundImg); // ����� �̴´�.
		imshow("foreground mask",  foregroundMask);   
		imshow("foreground image", foregroundImg);
		if (!backgroundImg.empty()) {
			imshow("mean background image", backgroundImg);
		}
		waitKey(33);
	}
}