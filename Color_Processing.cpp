#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap �Լ� ����� ���� �ʿ�
#include <time.h>

using namespace cv;
using namespace std;

// H: Hue ����        -> 0 ~ 180 ���� ���Ѵ�.
//    -> ���� ���̴�. ��, ��, ��
// S: Saturation ä�� -> 0 ~ 255 ���� ���Ѵ�.
//    -> ���� �����̴�. 
// V: Value ��      -> 0~ 255 ���� ���Ѵ�. 
//    -> ���� ��Ⱑ ���� ���� ��, ���� ���� ��

// 1. Intensity Images is Decoupled 
//    -> �̹��� intensity ����
// 2. Color Slicing 
//    -> Hue channel���� ���ϴ� ���� ã�Ƽ� �ش���� 0���� �ָ� �� ���� ��������. => masking
// 3. Color Conversion
//    -> Hue Channel�� �����ؼ� �츮�� ���ϴ� Į�� ������ �� �ٲ۴�.

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

	cvtColor(img, HSV, CV_BGR2HSV); // ���� 
	split(HSV, ic);
	split(HSV, mo);
	split(HSV, cc);

	// imshow("H", ic[0]); // �� ��Ӱ� ��ī�ο�
	// imshow("S", ic[1]); // �� ��ī�Ӱ� ����
	// imshow("V", ic[2]); // �ǰ� �ߺ���
	// 1. mat�� histogram�� equzlize ��Ŵ => intensity changing
	equalizeHist(ic[2], ic[2]);

	// 2. color masking out except orange
	for (int i = 0; i < rows; i++) {
		h = mo[0].ptr<uchar>(i);
		s = mo[1].ptr<uchar>(i);
		// ��Ȳ���� h�� ã�Ƽ� -> s�� ���� ũ�⸦ �ִµ� �ű⼭ ����ŷ 
 		for (int j = 0; j < cols; j++) {
			if (h[j] > 9 && h[j] < 23) s[j] = s[j]; // ��Ȳ���� ����
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