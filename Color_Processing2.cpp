#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap �Լ� ����� ���� �ʿ�
#include <time.h>

using namespace cv;
using namespace std;

// 2. Pseudo Coloring 
// gray�� ���� �ٸ� ��¥������ ���� -> heatMap ���� ������ ���� ����
// applyColorMap �� ���ؼ� 0~255 ��� ����
// Į�� ������ �����ϴ�. COLORMAP_JET�� ���� ������~
//    -> �� ~ ������ visualizing �����ϴ�.


// 3. Color Balancing
// �츮�� ������ ������ �A ������ ����� �ʹٸ�?
// ���� ��� ȭ������ ���� �����̶��... ���������ѵ� -> �̰� ���������Ѱ� ������
// Color Balancing�� �Ѵ�. 
// >>  Global adjustment of the intensities of the colors
// >>  Simple way of color balancing

//   ���� RGB ���������� ����� 255, 255, 255 �̴�.
//   �ٵ� �� ������ ����������� 255, 255, 255 �� �ƴѰŴ�.
//   Rw', Gw', Bw' >> �� �츮�� ������� �˰� �־����� �˰��� ���󿡼��� �ƴϴ��� 
//   ������� �˰� �ִ� ������ �Կ� ��� ����  Rw', Gw', Bw'�̴�.
//   ����󿡼� �׷��� ������ Rw' Gw' Bw' �� 255�� �ٲ��ִ� ������ �ؾ��Ѵ�. 

// [R     [  255/Rw'    0      0        [  R'
//  G    =     0     255/Gw'   0           G'
//  B ]        0        0    255/Bw'  ]    B' ] (���� Į���� �Է�)

// �׷��� ���·� �˸��� �Է� Į���� ã�Ƽ� ���¸� ��������� �Ѵ�.


int main() {
	Mat gray = imread("china.jpg", IMREAD_GRAYSCALE);
	Mat color;

	// 1. Color Mapping 
	applyColorMap(gray, color, COLORMAP_JET);
	imshow("gray", gray);
	imshow("image", color);
	waitKey(0);

	// 2. Color Balancing 
	Mat image = imread("china.jpg", IMREAD_COLOR);
	Mat result;
	vector<Mat> ch(3);
	int b_sum = 0, g_sum = 0, r_sum = 0;
	int b_avg, g_avg, r_avg;
	int b_tmp, g_tmp, r_tmp;
	if (image.empty()) {
		cerr << "read fail" << endl;
		exit(-1);
	}

	int rows = image.rows;
	int cols = image.cols;
	int pixno = rows * cols;

	// split -> B G R
	split(image, ch);
	uchar * b;
	uchar * g;
	uchar * r;
	
	for (int i = 0; i < rows; i++) {
		b = ch[0].ptr<uchar>(i);
		g = ch[1].ptr<uchar>(i);
		r = ch[2].ptr<uchar>(i);
		for (int j = 0; j < cols; j++) {
			b_sum += b[j];
			g_sum += g[j];
			r_sum += r[j];
		}
	}

	b_avg = b_sum / pixno;
	g_avg = g_sum / pixno;
	r_avg = r_sum / pixno;

	// pointer�� ������ �� ���� �ְ� �ϴ� ���
	// gray world assumption ǳ�� �����̳� �Ϲ����� ���������� �� ������ �ȼ��� ��� ����
	// ȸ���� ������ 128. 128. 128 ��, �ٵ� �츮�� �Կ��� ���󿡼� ��� �´��� 128, 128, 128, �ƴ϶��
	//  �׷��� ������ְ� �ϱ� ���� ��Ʈ������ �����ϸ� �ȴ�. 
	for (int i = 0; i < rows; i++) {
		b = ch[0].ptr<uchar>(i);
		g = ch[1].ptr<uchar>(i);
		r = ch[2].ptr<uchar>(i);
		for (int j = 0; j < cols; j++) {
			b_tmp = (128 * b[j]) / b_avg;
			b[j] = b_tmp > 255 ? 255 : b_tmp;
			g_tmp = (128 * g[j]) / g_avg;
			g[j] = g_tmp > 255 ? 255 : g_tmp;
			r_tmp = (128 * r[j]) / r_avg;
			r[j] = r_tmp > 255 ? 255 : r_tmp;
		}
	}

	// merge 3 channels image
	merge(ch, result);

	imshow("image", image);
	imshow("result", result);
	waitKey(0);

	return 0;
}