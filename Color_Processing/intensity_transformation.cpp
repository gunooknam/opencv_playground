#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap 함수 사용을 위해 필요
using namespace cv;
using namespace std;

// 값을 반전한다. 
void neg_img(const Mat & src, Mat& dst) {
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			dst.at<uchar>(i, j) = 255 - src.at<uchar>(i, j);
		}
	}
}

// log transform  => 어두운 부분이 log scale 형태로 맵핑되서 밝아진다.
// s = c*log(1+r)
void log_trans(const Mat & src, Mat& dst) {
	double c = 1.5f; // scale constant 
	Mat f_img;
	src.convertTo(f_img, CV_32F);
	f_img = abs(f_img) + 1;
	log(f_img, f_img);
	normalize(f_img, f_img, 0, 255, NORM_MINMAX); // normalize image to (0~255) 
	convertScaleAbs(f_img, dst, c); // scaling by c, conversion to an unsigned 8-bit type
}

// s = c*input^r
// r => gamma
void gamma_correction(const Mat & src, Mat& dst) {
	MatIterator_<uchar> it, end; 
	float gamma = 0.5;

	// pix 라는 function 생성
	// index 값에 넣으면 해당되는 y값 나옴

	unsigned char pix[256];
	for (int i = 0; i < 256; i++) {
		pix[i] = saturate_cast<uchar>(pow((float)(i / 255.0), gamma) * 255.0f);
	}
	dst = src.clone();
	for (it = dst.begin<uchar>(), end = dst.end<uchar>(); it != end; it++) {
		*it = pix[(*it)];
	}
}

int main() {
	Mat image = imread("hello.jpg", 0);
	Mat negative_img = image.clone();
	Mat log_img = image.clone();
	Mat gamma_cor = image.clone();
	neg_img(image, negative_img);
	log_trans(image, log_img);
	gamma_correction(image, gamma_cor);
	imshow("Input image", image); 
	waitKey(0);
	imshow("Negative transformation", negative_img);
	waitKey(0);
	imshow("log transformation", log_img);
	waitKey(0);
	imshow("gamma correction", gamma_cor);
	waitKey(0);
	return 0;
}