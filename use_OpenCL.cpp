#include "opencv2/opencv.hpp"
#include "opencv2/core/ocl.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {

	// OpenCL�� ����� �� �ִ��� �׽�Ʈ
	if (!ocl::haveOpenCL()) {
		cout << "���� : OpenCL�� ����� �� ���� �ý����Դϴ�." << endl;
		return -1;
	}

	// ���ؽ�Ʈ ����
	ocl::Context context;
	if (!context.create(ocl::Device::TYPE_GPU)) {
		cout << " ���� : ���ؽ�Ʈ�� ������ �� �����ϴ�." << endl;
		return -1;

	}

	// GPU ��ġ ����
	cout << context.ndevices() << " GPU device (s) detected " << endl;
	for (size_t i = 0; i < context.ndevices(); i++) {
		ocl::Device device = context.device(i);
		cout << " - Device " << i << " --- " << endl;
		cout << " Name : " << device.name() << endl;
		cout << " Availability : " << device.available() << endl;
		cout << " Image Support : " << device.imageSupport() << endl;
		cout << " OpenCL C version : " << device.OpenCL_C_Version() << endl << endl;
	}

	// ��ġ 0 �� ��� 
	ocl::Device(context.device(0));

	// �Է� �̹���
	Mat mSrc = imread("hello.jpg", IMREAD_GRAYSCALE);
	UMat src = mSrc.getUMat(ACCESS_READ);
	// OPENCL Device���� UMat ������ �̷������.
	// Mat => CPU����
	// UMat => GPU����


	// ���� �ð� ���� 
	static int64 start, end;
	static float time;

	// OpenCL Enable ���¿��� Sobel�� 10000�� �ݺ�
	ocl::setUseOpenCL(true);
	cout << " Sobel Test : OpenCL Enabled ";

	start = getTickCount();
	for (int i = 0; i < 10000; i++) {
		UMat dst;
		Sobel(src, dst, -1, 1, 0);
	}
	end = getTickCount();

	// getTickCount() :     ��ü tick�� ��
	// getTickFrequency() : �ʴ� tick ��

	time = (end - start) / getTickFrequency(); 
	cout << " --- Done. Processing Time : " << time << " sec. " << endl;


	// OpenCL Disable ���¿��� Sobel�� 10000�� �ݺ�
	ocl::setUseOpenCL(false);
	cout << " Sobel Test : OpenCL Disabled ";

	start = getTickCount();
	for (int i = 0; i < 10000; i++) {
		UMat dst;
		Sobel(src, dst, -1, 1, 0);
	}

	end = getTickCount();
	time = (end - start) / getTickFrequency();
	cout << " --- Done. Processing Time : " << time << " sec. " << endl;

	return  0;
}