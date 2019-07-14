#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>
#include <iostream>
#include <direct.h>
#include "opencv2/video/tracking.hpp" // 이거 써야함
using namespace std;
using namespace cv;

#define WEBCAM



// Mean Shift
// initialize Image -> detection한 이미지 Set
// -> ROI Histogram 만듬
// 1. Window 자체의 무게중심 지정
// 2. 윈도우 내부점들의 무게 중심 설정
// 1과 2의 점이 일치하지 않으면, 윈도우의 무게 중심점을 현재 무게 중심점으로 이동
// 다시 현재 위치에서 무게 중심 계산


// Mean Shift 
// 어떠한 점들이 있을 때 그 밀도가 최대가 되는 지점으로 이동시키는 알고리즘
struct CallbackParam {
	Mat frame;
	Point2f pt1, pt2;
	Rect roi;
	bool drag;
	bool updated;
};

void onMouse(int event, int x, int y, int flags, void* param) {
	// event는 int형인데 상황에 따른 숫자가 들어온다.
	CallbackParam*p = (CallbackParam*) param;

	if (event == EVENT_LBUTTONDOWN) {  // 누르면
		p->pt1.x = x;
		p->pt1.y = y;
		p->pt2 = p->pt1;
		p->drag = true;
	}					               // roi 시작지점

	if (event == EVENT_LBUTTONUP) {    // 마우스를 떼면
		int w = x - p->pt1.x;          // 현재x - 시작.y
		int h = y - p->pt1.y;          // 현재y - 시작.y
		p->roi.x  = p->pt1.x;
		p->roi.y  = p->pt1.y;
		p->roi.width  = w;
		p->roi.height = h;
		// roi는 x, y, w, h로 지정
		p->drag = false;
		if (w >= 10 && h >= 10) { // 일정 크기이상만 적용
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

		cvtColor(frame, hsv, COLOR_BGR2HSV); // frame -> BGR -> HSV 형태로 만들고 
		if (param.updated) {
			Rect rc = param.roi;             // roi영역

			Mat mask = Mat::zeros(rc.height, rc.width, CV_8U);

			// roi 부분을 뗴오고 
			ellipse(mask,    // 그 roi에 타원형 그리고
				    Point(rc.width / 2, rc.height / 2),
				    Size( rc.width / 2, rc.height / 2),
				    0,
				    0, 360, 
				    255);
			
			Mat roi(hsv, rc);                 // hsv에서 해당 rc 뗴어옴
											  // 해당 위치의 Rect 부분으로 전체이미지에서 패치 떼옴
			calcHist(&roi, // 해당 input image에 대해 Hist를 계산하는데
					 1, 
				     channels,   // { 0, 1, 2 };
				     mask,       // 다음 마스킹을 사용
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
		if (tracking) { // roi를 지정했다면 
			// histogram backprojection
			calcBackProject(&hsv,        // hsv
				            1,           // image nums
				            channels,
				            m_model3d,   // input
				            m_backproj,  // output
				            ranges );    // { hrange, vrange, vrange }; 
			meanShift(m_backproj, m_rc, TermCriteria(TermCriteria::EPS|TermCriteria::COUNT, 10, 1)); // 2. 
			// 위에것 meanShift 
			// 아래것 Camshift  => Search Window 사이즈 변할 수 있음
			// TermCriteria는 반복적인 연산을 수행하는 알고리즘의 종료 기준을 설정한다. 
			// EPS, COUNT 기준, max_iter, epsillon
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