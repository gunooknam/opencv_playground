#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp> // resize �Լ� ����� ����
#include <iostream>

using namespace std;
using namespace cv;

// Memory Property ! 
// Mat data ������ header�� data�� �����ȴ�.
// Shallow Copy������ ������ ���� �ּҰ� ī�ǵȴ�.
// => CopyTo 
// Deep copy �ٸ� �޸� ��ġ�� ī�Ǹ� ��Ų��. 
int main() {
	Mat m1 = (Mat_ <double>(3, 3) << 1, 2, 3, 4, 5, 6, 7, 8, 9);
	
	Mat m_shallow = m1; // shallow ������� ����
	Mat m_deep = m1.clone();

	cout << "m1=\n" << m1 << endl << endl;
	cout << "m_shallow\n" << m_shallow << endl << endl;
	cout << "m_deep\n" << m_deep << endl << endl;

	cout << "\n<<< Change >>>\n" << endl;

	// ���⼭ update�� �Ѵ�. 
	m1.at<double>(0, 0) = 100;
	cout << "m1=\n" << m1 << endl << endl; // ���⵵ �ٲ��
	cout << "m_shallow\n" << m_shallow << endl << endl; //���Ⱑ �ٲ�
	cout << "m_deep\n" << m_deep << endl << endl;

}