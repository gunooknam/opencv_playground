#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap 함수 사용을 위해 필요
#include <time.h>

using namespace cv;
using namespace std;

// image/video segmentation 
//   --> Process of partitioning a digital image into multiple regions
// Application 
//   --> Object classification

// input 이미지가 gray-scale 이라면
// -> binary image로 
// Thresholding의 Challaenging은 Noise이다.
// illumination and reflextance 도 Challenging하다.

int main() {
	Mat image, thresh, otsu, local;
	int thresh_T, low_cnt, high_cnt, low_sum, high_sum, th;

	thresh_T = 200;
	th = 10;
	low_cnt = high_cnt = low_sum = high_sum = 0;

	image = imread("hello.jpg", 0);
	cout << " threshold value : " << thresh_T << endl;

	while (1) {
		for (int j = 0; j < image.rows; j++) {
			for (int i = 0; i < image.cols; i++) {
				if (image.at<uchar>(j, i) < thresh_T) {
					low_sum += image.at<uchar>(j, i);
					low_cnt++;
				}
				else {
					high_sum += image.at<uchar>(j, i);
					high_cnt++;
				}
			}
		}

		if (abs(thresh_T - (low_sum / low_cnt + high_sum / high_cnt) / 2.0f) < th) {
			break;
		}
		else {
			thresh_T = (low_sum / low_cnt + high_sum / high_cnt) / 2.0f;
			cout << "threshold value:" << thresh_T << endl;
			low_cnt = high_cnt = low_sum = high_sum = 0;
		}
	}
	threshold(image, thresh, thresh_T, 255, THRESH_BINARY);

	// otsu
	threshold(image,
		      otsu,
		      0,
		      255,
		      THRESH_BINARY | THRESH_OTSU);

	// > ADAPTIVE THRESH_MEAN
	// > ADAPTIVE THRESH_GAUSSIAN 선택

	// local // Threshold는 -> block단위로 Mean 값 x 주변 블락들 - 15(상수)
	adaptiveThreshold(image,
					  local,
					  255,
					  ADAPTIVE_THRESH_MEAN_C,
					  THRESH_BINARY,
					  85,  // local한 커널사이즈
					  15); // 평균에서 15뺌

	imshow("input image", image);
	imshow("thresholding", thresh);
	imshow("otsu algorithm", otsu);
	imshow("local(Adaptive) Thresholding", local);
	waitKey(0);

}