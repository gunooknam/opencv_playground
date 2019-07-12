#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap 함수 사용을 위해 필요
#include <time.h>

using namespace cv;
using namespace std;

// H: Hue 색상        -> 0 ~ 180 값을 지닌다.
//    -> 색의 질이다. 빨, 노, 파
// S: Saturation 채도 -> 0 ~ 255 값을 지닌다.
//    -> 색의 선명도이다. 
// V: Value 명도      -> 0~ 255 값을 지닌다. 
//    -> 색의 밝기가 높을 수록 백, 낮을 수록 흑

// 1. Intensity Images is Decoupled 
//    -> 이미지 intensity 변형
// 2. Color Slicing 
//    -> Hue channel에서 원하는 색깔 찾아서 해당색을 0으로 주면 그 색이 지워진다. => masking
// 3. Color Conversion
//    -> Hue Channel에 접근해서 우리가 원하는 칼라 지역의 색 바꾼다.

int main() {
	Mat img = imread("china.jpg");
	Mat HSV, intensity_change, mask_out, change_color;
	vector<Mat> ic(3);
	vector<Mat> mo(3);
	vector<Mat> cc(3);

	int rows = img.rows;
	int cols = img.cols;

	uchar*h;
	uchar*s;
	uchar*v;

	cvtColor(img, HSV, CV_BGR2HSV); // 여기 
	split(HSV, ic);
	split(HSV, mo);
	split(HSV, cc);

	// imshow("H", ic[0]); // 좀 어둡고 날카로움
	// imshow("S", ic[1]); // 좀 날카롭고 밝음
	// imshow("V", ic[2]); // 되게 잘보임
	// 1. mat의 histogram을 equzlize 시킴 => intensity changing
	equalizeHist(ic[2], ic[2]);

	// 2. color masking out except orange
	for (int i = 0; i < rows; i++) {
		h = mo[0].ptr<uchar>(i);
		s = mo[1].ptr<uchar>(i);
		// 주황색을 h로 찾아서 -> s로 값의 크기를 주는데 거기서 마스킹 
 		for (int j = 0; j < cols; j++) {
			if (h[j] > 9 && h[j] < 23) s[j] = s[j]; // 주황색만 검출
			else s[j] = 0; // color masking
		}
	}

	// 3. color conversion => changing all colors 
	for (int i = 0; i < rows; i++) {
		h = cc[0].ptr<uchar>(i);
		s = cc[1].ptr<uchar>(i);
		for (int j = 0; j < cols; j++) {
			if (h[j] + 50 > 255)
				h[j] = h[j] + 50 - 255;
			else
				h[j] += 50;
		}
	}

	merge(ic, intensity_change);
	merge(mo, mask_out);
	merge(cc, change_color);
	cvtColor(intensity_change, intensity_change, CV_HSV2BGR);
	cvtColor(mask_out, mask_out, CV_HSV2BGR); 
	cvtColor(change_color, change_color, CV_HSV2BGR);

	imshow("image", img);
	imshow("intensity change", intensity_change);
	imshow("mask out", mask_out); 
	imshow("change color", change_color);
	waitKey(0);
	return 0;

}