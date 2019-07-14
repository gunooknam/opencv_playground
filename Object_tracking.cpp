#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>
#include <iostream>
#include <direct.h>
#include "opencv2/video/tracking.hpp" // �̰� �����
using namespace std;
using namespace cv;

#define WEBCAM



// Mean Shift
// initialize Image -> detection�� �̹��� Set
// -> ROI Histogram ����
// 1. Window ��ü�� �����߽� ����
// 2. ������ ���������� ���� �߽� ����
// 1�� 2�� ���� ��ġ���� ������, �������� ���� �߽����� ���� ���� �߽������� �̵�
// �ٽ� ���� ��ġ���� ���� �߽� ���


// Mean Shift 
// ��� ������ ���� �� �� �е��� �ִ밡 �Ǵ� �������� �̵���Ű�� �˰���
struct CallbackParam {
	Mat frame;
	Point2f pt1, pt2;
	Rect roi;
	bool drag;
	bool updated;
};

void onMouse(int event, int x, int y, int flags, void* param) {
	// event�� int���ε� ��Ȳ�� ���� ���ڰ� ���´�.
	CallbackParam*p = (CallbackParam*) param;

	if (event == EVENT_LBUTTONDOWN) {  // ������
		p->pt1.x = x;
		p->pt1.y = y;
		p->pt2 = p->pt1;
		p->drag = true;
	}					               // roi ��������

	if (event == EVENT_LBUTTONUP) {    // ���콺�� ����
		int w = x - p->pt1.x;          // ����x - ����.y
		int h = y - p->pt1.y;          // ����y - ����.y
		p->roi.x  = p->pt1.x;
		p->roi.y  = p->pt1.y;
		p->roi.width  = w;
		p->roi.height = h;
		// roi�� x, y, w, h�� ����
		p->drag = false;
		if (w >= 10 && h >= 10) { // ���� ũ���̻� ����
			p->updated = true;
		}
	}

	if (p->drag && event == EVENT_MOUSEMOVE) {
		if (p->pt2.x != x || p->pt2.y != y) {
			Mat img = p->frame.clone();
			p->pt2.x = x;
			p->pt2.y = y;
			rectangle(img, p->pt1, p->pt2, Scalar(0, 255, 0), 1);
			imshow("Tracker", img);
		}
	}

}


int main(int argc, char* argv[]) {
	
	#ifdef WEBCAM 
	VideoCapture cap(0);
	#else
	VideoCapture cap("vtest.avi");
	#endif

	CallbackParam param;
	Mat frame, m_backproj, hsv;
	Mat m_model3d;
	Rect m_rc;
	
	int   channels[] = { 0, 1, 2 };
	int hist_sizes[] = { 16, 16, 16 };
	float hrange[] = { 0, 180 };
	float vrange[] = { 0, 255 };
	const float * ranges[] = { hrange,   // hue 
							   vrange,   // saturation
							   vrange }; // brightness
	// check if we succeeded 
	if (!cap.isOpened()) {
		cout << "can't open video file" << endl;
		return 0;
	}
	// click and drag on image to set ROI
	cap >> frame;
	imshow("Tracker", frame);
	param.frame = frame;
	param.drag = false;
	param.updated = false;
	setMouseCallback("Tracker", onMouse, &param);
	bool tracking = false;

	while (true) {
		// imageg acquisition & target init 
		if (param.drag) {
			if (waitKey(33) == 27) break;
			continue;
		}

		cvtColor(frame, hsv, COLOR_BGR2HSV); // frame -> BGR -> HSV ���·� ����� 
		if (param.updated) {
			Rect rc = param.roi;             // roi����

			Mat mask = Mat::zeros(rc.height, rc.width, CV_8U);

			// roi �κ��� ����� 
			ellipse(mask,    // �� roi�� Ÿ���� �׸���
				    Point(rc.width / 2, rc.height / 2),
				    Size( rc.width / 2, rc.height / 2),
				    0,
				    0, 360, 
				    255);
			
			Mat roi(hsv, rc);                 // hsv���� �ش� rc ����
											  // �ش� ��ġ�� Rect �κ����� ��ü�̹������� ��ġ ����
			calcHist(&roi, // �ش� input image�� ���� Hist�� ����ϴµ�
					 1, 
				     channels,   // { 0, 1, 2 };
				     mask,       // ���� ����ŷ�� ���
				     m_model3d,  // output array
				     3,
				     hist_sizes, // { 16, 16, 16 };
				     ranges);    // { hrange, vrange, vrange };

			m_rc = rc;
			param.updated = false;
			tracking = true;
		}
		cap >> frame;
		if (frame.empty()) break;
		// image processing 
		if (tracking) { // roi�� �����ߴٸ� 
			// histogram backprojection
			calcBackProject(&hsv,        // hsv
				            1,           // image nums
				            channels,
				            m_model3d,   // input
				            m_backproj,  // output
				            ranges );    // { hrange, vrange, vrange }; 
			meanShift(m_backproj, m_rc, TermCriteria(TermCriteria::EPS|TermCriteria::COUNT, 10, 1)); // 2. 
			// ������ meanShift 
			// �Ʒ��� Camshift  => Search Window ������ ���� �� ����
			// TermCriteria�� �ݺ����� ������ �����ϴ� �˰����� ���� ������ �����Ѵ�. 
			// EPS, COUNT ����, max_iter, epsillon
			// CamShift(m_backproj, m_rc, cvTermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 20, 1)); // 2. 
			rectangle(frame, m_rc, Scalar(0, 0, 255), 3);
		}

		// image display
		imshow("Tracker", frame);

		// user input
		char ch = waitKey(33);
		if (ch == 27) break; //ESC 
		else if (ch == 32) { 
			while ((ch = waitKey(33))!= 32 && ch != 27);
			if (ch == 27) break;
		}
	}
	return 0;
}