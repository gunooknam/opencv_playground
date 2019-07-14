#include "opencv2/opencv.hpp"
#include "opencv2/core/ocl.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {

	// OpenCL을 사용할 수 있는지 테스트
	if (!ocl::haveOpenCL()) {
		cout << "에러 : OpenCL을 사용할 수 없는 시스템입니다." << endl;
		return -1;
	}

	// 컨텍스트 생성
	ocl::Context context;
	if (!context.create(ocl::Device::TYPE_GPU)) {
		cout << " 에러 : 컨텍스트를 생성할 수 없습니다." << endl;
		return -1;

	}

	// GPU 장치 정보
	cout << context.ndevices() << " GPU device (s) detected " << endl;
	for (size_t i = 0; i < context.ndevices(); i++) {
		ocl::Device device = context.device(i);
		cout << " - Device " << i << " --- " << endl;
		cout << " Name : " << device.name() << endl;
		cout << " Availability : " << device.available() << endl;
		cout << " Image Support : " << device.imageSupport() << endl;
		cout << " OpenCL C version : " << device.OpenCL_C_Version() << endl << endl;
	}

	// 장치 0 번 사용 
	ocl::Device(context.device(0));

	// 입력 이미지
	Mat mSrc = imread("hello.jpg", IMREAD_GRAYSCALE);
	UMat src = mSrc.getUMat(ACCESS_READ);
	// OPENCL Device에서 UMat 연산이 이루어진다.
	// Mat => CPU영역
	// UMat => GPU영역


	// 실행 시간 측정 
	static int64 start, end;
	static float time;

	// OpenCL Enable 상태에서 Sobel을 10000번 반복
	ocl::setUseOpenCL(true);
	cout << " Sobel Test : OpenCL Enabled ";

	start = getTickCount();
	for (int i = 0; i < 10000; i++) {
		UMat dst;
		Sobel(src, dst, -1, 1, 0);
	}
	end = getTickCount();

	// getTickCount() :     전체 tick의 수
	// getTickFrequency() : 초당 tick 수

	time = (end - start) / getTickFrequency(); 
	cout << " --- Done. Processing Time : " << time << " sec. " << endl;


	// OpenCL Disable 상태에서 Sobel을 10000번 반복
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