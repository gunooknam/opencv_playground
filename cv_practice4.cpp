#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp> // resize 함수 사용을 위해
#include <iostream>
using namespace std;
using namespace cv;

// 이미지에 로고를 넣는 예제
int main() {
	Mat bonobono = imread("hello.jpg");
	Mat logo = imread("Mark.png");
	Mat logo_gray = imread("Mark.png", CV_LOAD_IMAGE_GRAYSCALE);
	cout << "image size:" << bonobono.size() << endl;
	cout << "logo size:" << logo.size() << endl;


	// Rect는 x, y , w, h 이다. 
	Mat imageROI(bonobono, // 해당 src 이미지에 
				 Rect(bonobono.cols - logo.cols, // 다음 좌표에 해당하는 patch를 뗴온다.
					  bonobono.rows - logo.rows,
					  logo.cols,
					  logo.rows ));
	

	Mat mask(120 - logo_gray); 
	// logo gray가 CV_8U이기 때문에 이걸 120 에 값을 빼도 CV_8U라서
	// clip이 된다. 음수가 안나옴
	cout << mask.size() << imageROI.size() << endl;
	// 120 - logo_gray을 해주면 흰 배경 부분은 다 0이 된다. => 푸
	// logo를 카피한다 
	logo.copyTo(imageROI, // dst
				mask);    // 마스크 값이 0이 아닌 위치에서만 로고가 삽입된다.
						  // 마스크를 두번째 인자로 넣으면
						  // 마스킹 되버려서 0이 아닌 곳만 써진다.
	
	imshow("image", bonobono); // 로고가 넣어진 보노보노
	// 값이 ImageROI는 값이 새롭게 복사가 된것이 아니다. 단지 참조가 된 것이다.
	imageROI = 0;
	imshow("image_ref_zero", bonobono); // 참조인지 확인
	
	waitKey(0);

	return 0;
}