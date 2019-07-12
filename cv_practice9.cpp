#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap 함수 사용을 위해 필요
using namespace cv;
void wave(const cv::Mat &image, cv::Mat &result); // 물결 효과 함수
void flip(const cv::Mat &image, cv::Mat &result); // 좌우반전 함수

int main() {
	Mat before = imread("hello.jpg");
	imshow("before", before);
	cv::Mat after1;
	after1.create(before.rows, before.cols, before.type());
	wave(before, after1); // 물결 효과
	cv::imshow("after1", after1);
	cv::Mat after2;
	after2.create(before.rows, before.cols, before.type());
	flip(before, after2); // 좌우반전
	cv::imshow("after2", after2);
	cv::waitKey(0);
	return 0;
}

// remap 함수 
// X 맵을 정의한다. matrix 형태 (img.rows, img.cols) => 값 하나가 들어있다.
// Y 맵을 정의한다. 값 하나가 들어있다. 
// remap을 하면 X맵에 정의한 값이 Y맵에 들어간 값이 한쌍이 된다. 
// 그 쌍의 값이 현재 위치로 맵핑이 된다. 
void wave(const Mat& img, Mat & result) {
	Mat srcX(img.rows, img.cols, CV_32F);
	Mat srcY(img.rows, img.cols, CV_32F);

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			srcX.at<float>(i, j) = j;
			srcY.at<float>(i, j) = i + 10 * sin(j / 10);
		}
	}
	remap(img,    // 재매핑처리를 위한 x맵과 y맵을 먼저 정의해야 한다. 
		  result, //dst
		  srcX, 
		  srcY,
		  INTER_LINEAR); // interpolation flag
}
void flip(const cv::Mat &img, cv::Mat &result) // 좌우 반전 함수
{
	// 맵 역할
	cv::Mat srcX(img.rows, img.cols, CV_32F);
	cv::Mat srcY(img.rows, img.cols, CV_32F);
	// 매핑 생성
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			// 화소 (i, j)의 새로운 위치
			srcX.at<float>(i, j) = img.cols - j - 1; // 열은 좌우가 바뀜 ex) 1번째 열은 마지막 열로, 2번째 열은 (마지막-1)열로,..., 마지막 열은 1번째 열로.
			srcY.at<float>(i, j) = i; // 행은 그대로 유지
		}
	}
	// 매핑 적용
	cv::remap(img,    // src
		      result, // dst
		      srcX,   
		      srcY,   
		      INTER_LINEAR);

}

