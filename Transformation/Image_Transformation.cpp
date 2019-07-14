#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap �Լ� ����� ���� �ʿ�

using namespace std;
using namespace cv;

struct MouseParams {
	Mat img; 
	vector<Point2f> in, out;
};

static void onMouse(int event, int x, int y, int, void* param) {
	MouseParams * mp = (MouseParams*) param;
	Mat img = mp->img;
	if (event == EVENT_LBUTTONDOWN) {
		Mat result;
		mp->in.push_back(Point2f(x, y));
		if (mp->in.size() == 4) {
			// �� (x,  y )�� �ش��ϴ� 4���� ��ǥ
			// �� (x', y')�� �ش��ϴ� 4���� ��ǥ�� �����ϴ�
			// Transform Matrix�� ����
			// ���� ���� in ��ǥ���� out�� 4������ ��ȯ�ϴ� matrix ����
			Mat homo_mat = getPerspectiveTransform(mp->in, mp->out);								
			warpPerspective(img, result, homo_mat, Size(img.cols, img.rows));
			// img�� homo_mat�Լ��� ���� 
			imshow("output", result);
			mp->in.clear();
		}
		else {
			result = img.clone();
			for (size_t i = 0; i < mp->in.size(); i++) {
				circle(result, mp->in[i], 3, Scalar(0, 0, 255), 5);
			}
			imshow("input", result);
		}
	}
	if (event == EVENT_RBUTTONDOWN) {
		mp->in.clear();
		imshow("input", img);
	}
}

int main() {
	Mat input = imread("hello.jpg");
	imshow("input", input);
	MouseParams mp;
	mp.out.push_back(Point2f(0, 0));
	mp.out.push_back(Point2f(500, 0));
	mp.out.push_back(Point2f(500, 500));
	mp.out.push_back(Point2f(0, 500));
	mp.img = input;
	setMouseCallback("input", onMouse, (void*)&mp);
	waitKey(0);
	return 0;
}