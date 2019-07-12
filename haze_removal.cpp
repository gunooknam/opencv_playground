#include "opencv2\opencv.hpp"

// Code reference 아래의 주소를 참고 
// https://github.com/Jack-CV/Single-Image-Haze-Removal-Using-Dark-Channel-Prior
// Paper : Dark channel prior, Guide Filter

using namespace std;
using namespace cv;

static void makeDepth32f(Mat &source, Mat &output)
{
	if (source.depth() != CV_32F)
		source.convertTo(output, CV_32F);
	else
		output = source;
}

static void mynorm(Mat &source, Mat &output)
{
	for (int i = 0; i < source.rows; i++)
	{
		float *indata = source.ptr<float>(i);
		float *outdata = output.ptr<float>(i);
		for (int j = 0; j < source.cols * source.channels(); j++)
		{
			outdata[j] = indata[j] / 255.0;
		}
	}
}

void GuideFilter(Mat &source, Mat &guided_image, Mat &output, int radius, double epsilon)
{
	CV_Assert(radius >= 2 && epsilon > 0);
	CV_Assert(source.data != NULL && source.channels() == 1);
	CV_Assert(guided_image.channels() == 1);
	CV_Assert(source.rows == guided_image.rows && source.cols == guided_image.cols);

	Mat guided;
	if (guided_image.data == source.data)
	{
		//make a copy
		guided_image.copyTo(guided);
	}
	else
	{
		guided = guided_image;
	}

	Mat source_32f, guided_32f;
	makeDepth32f(source, source_32f);
	mynorm(source_32f, source_32f);
	makeDepth32f(guided, guided_32f);
	mynorm(guided_32f, guided_32f);

	//
	Mat mat_Ip, mat_I2;
	multiply(guided_32f, source_32f, mat_Ip);
	multiply(guided_32f, guided_32f, mat_I2);

	//
	Mat mean_p, mean_I, mean_Ip, mean_I2;
	Size win_size(2 * radius + 1, 2 * radius + 1);
	boxFilter(source_32f, mean_p, CV_32F, win_size);
	boxFilter(guided_32f, mean_I, CV_32F, win_size);
	boxFilter(mat_Ip, mean_Ip, CV_32F, win_size);
	boxFilter(mat_I2, mean_I2, CV_32F, win_size);

	//
	Mat cov_Ip = mean_Ip - mean_I.mul(mean_p);
	Mat var_I = mean_I2 - mean_I.mul(mean_I);
	var_I += epsilon;

	//
	Mat a, b;
	divide(cov_Ip, var_I, a);
	b = mean_p - a.mul(mean_I);

	//
	Mat mean_a, mean_b;
	boxFilter(a, mean_a, CV_32F, win_size);
	boxFilter(b, mean_b, CV_32F, win_size);

	// (depth == CV_32F)
	Mat tempoutput = mean_a.mul(guided_32f) + mean_b;

	output.create(source.rows, source.cols, CV_8U);

	for (int i = 0; i < source.rows; i++)
	{
		float *data = tempoutput.ptr<float>(i);
		uchar *outdata = output.ptr<uchar>(i);
		for (int j = 0; j < source.cols; j++)
		{
			outdata[j] = saturate_cast<uchar>(data[j] * 255);
		}
	}
}


// 패치 단위로 10x10 짤라서 거기서 min값 추려서 그거로 채운다. 
static void MinFilter(Mat & source, Mat & output, int r) {
	Mat input;
	source.copyTo(input);
	output.create(source.rows, source.cols, CV_8U);

	for (int i = 0; i <= (input.rows - 1) / r; i++) {
		for (int j = 0; j <= (input.cols - 1) / r; j++) {
			int w = r;
			int h = r;
			if (i * r + h > input.rows) {
				h = input.rows - i * r; //남는 부분
			}

			if (j *r + w > input.cols) {
				w = input.cols - j * r; //남는 부분
			}
			Mat ROI = input(Rect(j*r, i*r, w, h)); // r이라는 패치만큼 짤라난다. 

			double mmin;
			//cout << ROI << endl;
			minMaxLoc(ROI, &mmin, 0); // mmni값을 포인터로 저장

			Mat desROI = output(Rect(j*r, i*r, w, h)); // 이건 그냥 포인터를 참조한 것이다.
			desROI.setTo(uchar(mmin)); // 최소값을 지정
		}
	}

}


void DarkChannel(Mat & src, Mat &output, int r) {

	Mat input;
	input.create(src.rows, src.cols, CV_8U);

	for (int i = 0; i < src.rows; i++) {
		uchar* srcdata = src.ptr<uchar>(i); //값에 접근하는 방법중에 빠른 방법이다. 
		uchar* indata = input.ptr<uchar>(i);
		// B, G, R 순으로 3배 메모리 할당되어 있다. 
		for (int j = 0; j < src.cols * src.channels(); j += 3) {
		
			uchar mmin;
			mmin = min(srcdata[j], srcdata[j + 1]);
			mmin = min(mmin, srcdata[j + 2]);

			indata[j / 3] = mmin;
		}

	}

	// 3개의 채널중에 가장 작은거 뽑는다. => // 여기서 r은 10이다. 
	MinFilter(input, output, r);

}

void HazeRemoval(Mat & src, 
				 Mat & out,
				int minr = 5, 
				int maxA = 230, 
				double w = 0.98, 
				int guider=30, 
				double guideeps=0.001, 
				int L = 0) {

	Mat input;
	src.copyTo(input);

	Mat dark;
	DarkChannel(input, dark, minr * 2);
	
	int histogram[256];
	memset(histogram, 0, sizeof(histogram));
	for (int i = 0; i < dark.rows; i++) {
		uchar * data = dark.ptr<uchar>(i);
		// i열에서의 1차원 배열을 뽑아온다. 
		for (int j = 0; j < dark.cols; j++) {
			//cout << data[j] << endl;
			// 고거를 이제 히스토그램으로 만듬
			histogram[data[j]]++;
		}
	}

	int num = dark.rows * dark.cols / 1000.0;
	int count = 0;
	uchar thres;
	for (int i = 0; i < 256; i++) {
		count += histogram[255 - i];
		if (count >= num) {
			thres = 255 - i;
			break;
		}
	}
	num = count;
	double b_max = 0, B;
	double g_max = 0, G;
	double r_max = 0, R;

	for (int i = 0; i < dark.rows; i++)
	{
		uchar *data = dark.ptr<uchar>(i);
		uchar *indata = input.ptr<uchar>(i);
		for (int j = 0; j < dark.cols; j++)
		{
			if (data[j] >= thres)
			{
				B = indata[3 * j];
				G = indata[3 * j + 1];
				R = indata[3 * j + 2];
				b_max += B;
				g_max += G;
				r_max += R;
			}
		}
	}
	b_max /= num;
	g_max /= num;
	r_max /= num;
	uchar MMAX = maxA;
	if (b_max > MMAX) b_max = MMAX;
	if (g_max > MMAX) g_max = MMAX;
	if (r_max > MMAX) r_max = MMAX;

	Mat img_t;
	img_t.create(dark.rows, dark.cols, CV_8U);
	Mat temp;
	temp.create(dark.rows, dark.cols, CV_8UC3);
	double b_temp = b_max / 255;
	double g_temp = g_max / 255;
	double r_temp = r_max / 255;
	for (int i = 0; i < dark.rows; i++)
	{
		uchar *data = input.ptr<uchar>(i);
		uchar *tdata = temp.ptr<uchar>(i);
		for (int j = 0; j < dark.cols * 3; j += 3)
		{
			tdata[j] = saturate_cast<uchar>(data[j] / b_temp);
			tdata[j + 1] = saturate_cast<uchar>(data[j] / g_temp);
			tdata[j + 2] = saturate_cast<uchar>(data[j] / r_temp);
		}
	}
	Mat gray;
	cvtColor(temp, gray, CV_BGR2GRAY);
	DarkChannel(temp, temp, minr * 2);
	for (int i = 0; i < dark.rows; i++)
	{
		uchar *darkdata = temp.ptr<uchar>(i);
		uchar *tdata = img_t.ptr<uchar>(i);
		for (int j = 0; j < dark.cols; j++)
		{
			tdata[j] = 255 - w * darkdata[j];
		}
	}

	GuideFilter(img_t, gray, img_t, guider, guideeps);
	imshow("Transmissivity", img_t);

	out.create(input.rows, input.cols, CV_8UC3);
	for (int i = 0; i < input.rows; i++)
	{
		uchar *tdata = img_t.ptr<uchar>(i);
		uchar *indata = input.ptr<uchar>(i);
		uchar *outdata = out.ptr<uchar>(i);
		for (int j = 0; j < input.cols; j++)
		{
			uchar b = indata[3 * j];
			uchar g = indata[3 * j + 1];
			uchar r = indata[3 * j + 2];
			double t = tdata[j];
			t /= 255;
			if (t < 0.1) t = 0.1;

			outdata[3 * j] = saturate_cast<uchar>((b - b_max) / t + b_max + L);
			outdata[3 * j + 1] = saturate_cast<uchar>((g - g_max) / t + g_max + L);
			outdata[3 * j + 2] = saturate_cast<uchar>((r - r_max) / t + r_max + L);
		}
	}
}

int main() {
	Mat src = imread("imgs/" + to_string(1)+ ".bmp");
	Mat res;
	imshow("input", src);
	HazeRemoval(src, res);
	imshow("Dehaze", res);
	waitKey(0);
	return 0;
}