#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp> // resize 함수 사용을 위해
#include <iostream>

using namespace std;
using namespace cv;

// Memory Property ! 
// Mat data 구조는 header와 data로 구성된다.
// Shallow Copy에서는 데이터 값의 주소가 카피된다.
// => CopyTo 
// Deep copy 다른 메모리 위치에 카피를 시킨다. 
int main() {
	Mat m1 = (Mat_ <double>(3, 3) << 1, 2, 3, 4, 5, 6, 7, 8, 9);
	
	Mat m_shallow = m1; // shallow 방식으로 복사
	Mat m_deep = m1.clone();

	cout << "m1=\n" << m1 << endl << endl;
	cout << "m_shallow\n" << m_shallow << endl << endl;
	cout << "m_deep\n" << m_deep << endl << endl;

	cout << "\n<<< Change >>>\n" << endl;

	// 여기서 update를 한다. 
	m1.at<double>(0, 0) = 100;
	cout << "m1=\n" << m1 << endl << endl; // 여기도 바뀌고
	cout << "m_shallow\n" << m_shallow << endl << endl; //여기가 바뀜
	cout << "m_deep\n" << m_deep << endl << endl;

}