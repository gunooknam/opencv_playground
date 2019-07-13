#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp> // resize 함수 사용을 위해
#include <iostream>

using namespace std;
using namespace cv;

// 이미지 플립 및 블러오기
// waitKey란...? 키를 기다리는 함수이다.
// 리턴값으로 해당 키에 대해 눌린 값을 리턴받는다.
// SPCAE 32,
// ESC 27 등의 숫자를 리턴받는다.

int main() {
	Mat image = imread("hello.jpg");
	if (image.empty()) {
		cout << "error when reading image!" << endl;
		return 0;
	}
	cout << "Original image is ( " << image.rows << " ," << image.cols << ")" << endl;	
	// 이 아래 두개 이름이 다르면 창 두개가 뜨고 같으면 1개뜬다. 
	namedWindow("Original image"); // 원본 이미지를 전시할 창 만들기 
	imshow("Original image", image);

	Mat resized_image;
	cout << Size() << endl; // 이것은 [0 x 0]
	cv::resize(image, resized_image, Size(), 0.5, 0.5, CV_INTER_AREA);

	cout << "Resized image is " << resized_image.rows << "x" << resized_image.cols << endl;
	namedWindow("Resized image");
	imshow("Resized image", resized_image);

	Mat result;
	flip(resized_image, result, 1); // flip이라는 함수가 있다. src, dst, 어떻게 뒤집을 것인가? 

	namedWindow("Filed image");
	imshow("Filed image", result);
	imwrite("Filped image.jpg", result);
	waitKey(0);
	destroyAllWindows();
	return 0;
}
