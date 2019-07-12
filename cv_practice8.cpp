#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor 함수를 사용하기 위해 필요. 
using namespace std;
using namespace cv;


int main() {

	Mat img1 = imread("hello.jpg");
	imshow("img1",img1);
	Mat img2 = imread("Mark.png");
	resize(img2, img2, img1.size(), CV_INTER_AREA); // img1 형태로 resize를 함
	imshow("img2", img2);
	Mat result;
	// 아래 방식으로 가중합 방식 1
	addWeighted(img1, 0.7,
				img2, 0.9,
		        0.0, // 감마값 
		        result); // dst 
	// img * 0.7 + img2 * 0.9
	imshow("Result1",result);
	waitKey(0);
	// 방식 2 
	cv::Mat result1;
	result1 = 0.7*img1+0.9*img2; // broadcast 같이 된다. opearator 정의된 \듯 
	cv::imshow("Result2", result1);
	waitKey(0);

	/* 초록 채널에만 2개의 이미지 가중합 */
	cv::Mat img2_gray;
	cv::Mat result2;
	// img2를 img2_gray로 format 을 converting 할 수 있다. 
	cv::cvtColor(img2, img2_gray, CV_BGR2GRAY); // 컬러 영상이던 img2를 그레이 영상으로 변환. 
	// 1. vector인데 Mat형태를 가진 것을 만든다.
	// 2. Mat 형태 3채널을 -> vector로 split 
	// 3. 한 채널에 한 채널을 더함. 타입은 기본으로 uint8이니까 그대로 유지될 듯
	std::vector<cv::Mat> planes; // 3개 영상의 벡터 생성
	cv::split(img1,  //Mat 형태의 multi channel을 -> Mat을 가지는 vector 형태로 담는다.
			  planes); // 3채널인 단일 영상을 3개의 1채널 영상으로 분리
	planes[1] += img2_gray; // 초록 채널에 더하기
	cv::merge(planes, result2); // 3개의 1채널 영상을 3채널인 단일 영상으로 병합
	cv::imshow("Result2", result2);
	cv::waitKey(0);
	return 0;

}