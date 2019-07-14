#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap 함수 사용을 위해 필요
#include <time.h>

using namespace cv;
using namespace std;

// 다양한 필터링의 예제

Mat saltPepperNoise(Mat img, int n) {
	int i, j;
	int salt_or_pepper;
	Mat result=img.clone();
	// result.create(img.rows, img.cols, img.type());
	srand((int)time(NULL));
	for (int k = 0; k < n; k++) {
		i = rand() % img.cols; // x 좌표 0 ~ img.cols-1 까지 랜덤하게 생성
		j = rand() % img.rows; // y 좌표 0 ~ img.rows-1 까지 랜덤하게 생성
		salt_or_pepper = (rand() % 2) * 255; // 0,1 랜덤하게 생성 
		switch (img.type())
		{
		case CV_8UC1:
			result.at<uchar>(j, i) = salt_or_pepper;
			break;
		case CV_8UC3:
			// opencv는 BGR 순으로 들어가 있다.
			// img.at<cv::Vec3b>(j,i) 한덩이로 꺼내면 0,1,2로 접근할 수 있는 배열이다. 
			result.at<cv::Vec3b>(j, i)[0] = salt_or_pepper; // B 채널
			result.at<cv::Vec3b>(j, i)[1] = salt_or_pepper; // G 채널
			result.at<cv::Vec3b>(j, i)[2] = salt_or_pepper; // R 채널
			break;
		default:
			break;
		}
	}
	return result;
}

int main() {
	Mat image,
		avgImg,
		gaussinImg,
		laplacian,
		abs_laplacian,
		sharperning;

	image = imread("hello.jpg", IMREAD_GRAYSCALE);

	// Blurs an image using the normalized box filter
	// average filter이다. 
	blur(image,      //src
		avgImg,      //dst
		Size(5, 5)); // kernel size 5,5


	// 가우시안 필터
	GaussianBlur(image,      //src
				 gaussinImg, //dst
		         Size(5, 5), // kernel size 
		         1.5   //sigmaX 이다. => Gaussian kernel standard deviation in X direction
				);     // sigmaY=0 

	Laplacian(image, laplacian,
		       CV_16S, //dst의 원하는 bit_depth
		       5,      // kernel size이다. 보통 3, 5, 7 이런 필터 사이즈 사용
		       1,      // scale 값-> 게산된 미분값의 배율
		       0);     // delta값 -> 계산된 미분값의 추가

	convertScaleAbs(laplacian, abs_laplacian);
	sharperning = abs_laplacian + image;

	Mat peppers = saltPepperNoise(image, 15000);
	Mat median_filter1, median_filter2, threshImage,cannyEdge, invCannyEdge;
	medianBlur(peppers, median_filter1, 3);
	medianBlur(peppers, median_filter2, 9);
	
	threshold(
		image,
		threshImage,
		120, // 이러한 임계값 보다 높으면
		255,  // 255로 셋팅
		THRESH_BINARY // binary threshold로 작용
	);
	
	Canny(image, 
		  cannyEdge,
		  100,  // low threshold
		  180); // high threshold

	threshold(cannyEdge,
		      invCannyEdge,
		      125,
		      255,
		      THRESH_BINARY_INV); // INV니까 125보다 높으면 0으로 셋팅


	Mat blur, grad_x, grad_y, abs_grad_x, abs_grad_y, result;
	GaussianBlur(image,
				 blur,
		         Size(5, 5),
		         5,
		         5,
		         BORDER_DEFAULT);

	Sobel(blur, grad_x, CV_16S, 1, 0, 3);
	convertScaleAbs(grad_x, abs_grad_x);

	Sobel(blur, grad_y, CV_16S, 1, 0, 3);
	convertScaleAbs(grad_y, abs_grad_y);

	addWeighted(
		abs_grad_x, 0.5,
		abs_grad_y, 0.5,
		0, result);

	imshow("input Image", image);
	imshow("Average image", avgImg);
	imshow("Gaussian blurred image", gaussinImg);
	imshow("laplacian", laplacian);
	imshow("abs_laplacian", abs_laplacian);
	imshow("sharpening", sharperning);
	imshow("peppers", peppers);
	imshow("filter size 3", median_filter1);
	imshow("filter size 9", median_filter2);
	imshow("threshold", threshImage);
	imshow("cannyEdge", cannyEdge);
	imshow("invCannyEdge", invCannyEdge);
	imshow("SobelEdge", result);

	waitKey(0);
	return 0;

}