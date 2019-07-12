#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap 함수 사용을 위해 필요
#include <time.h>

using namespace cv;
using namespace std;

// 2. Pseudo Coloring 
// gray의 색을 다른 가짜색으로 맵핑 -> heatMap 같은 것으로 맵핑 가능
// applyColorMap 을 통해서 0~255 밝기 값을
// 칼라 맵핑이 가능하다. COLORMAP_JET를 쓰면 붉은색~
//    -> 파 ~ 붉으로 visualizing 가능하다.


// 3. Color Balancing
// 우리가 조명의 영상을 뺸 영상을 만들고 싶다면?
// 예를 들어 화성에서 찍은 영상이라면... 붉으스름한데 -> 이걸 붉으스름한걸 빼려면
// Color Balancing을 한다. 
// >>  Global adjustment of the intensities of the colors
// >>  Simple way of color balancing

//   원래 RGB 색공간에서 흰색은 255, 255, 255 이다.
//   근데 그 영역이 사진찍었더니 255, 255, 255 가 아닌거다.
//   Rw', Gw', Bw' >> 는 우리가 흰색으로 알고 있었더니 알고보니 영상에서는 아니더라 
//   흰색으로 알고 있던 영상의 촬영 결과 값이  Rw', Gw', Bw'이다.
//   영상상에선 그렇지 않으니 Rw' Gw' Bw' 를 255로 바꿔주는 연산을 해야한다. 

// [R     [  255/Rw'    0      0        [  R'
//  G    =     0     255/Gw'   0           G'
//  B ]        0        0    255/Bw'  ]    B' ] (여기 칼럼은 입력)

// 그러한 형태로 알맞은 입력 칼럼을 찾아서 형태를 변형해줘야 한다.


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

	// pointer를 꺼내고 그 값에 넣고 하는 방식
	// gray world assumption 풍경 사진이나 일반적인 사진찍으면 그 사진의 픽셀의 평균 내면
	// 회색에 가깝다 128. 128. 128 등, 근데 우리가 촬영한 영상에서 평균 냈더니 128, 128, 128, 아니라면
	//  그렇게 만들어주게 하기 위한 메트릭스를 구성하면 된다. 
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