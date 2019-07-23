#pragma once
#pragma once
#include <opencv.hpp>
using namespace cv;
namespace easypr {
	//! ��ó��Ƶ�������
	cv::Mat getHistogram(cv::Mat in);

	//! EasyPR��getFeatures�ص�����
	//! ���ڴӳ��Ƶ�image����svm��ѵ������features
	typedef void(*svmCallback)(const cv::Mat& image, cv::Mat& features);

	//! EasyPR��getFeatures�ص�����
	//! convert from images to features used by gray char ann
	typedef void(*annCallback)(const cv::Mat& image, cv::Mat& features);

	//! gray and project feature
	void getGrayPlusProject(const cv::Mat& grayChar, cv::Mat& features);

	//!  EasyPR��getFeatures�ص�����
	//! �������ǻ�ȡ��ֱ��ˮƽ��ֱ��ͼͼֵ
	void getHistogramFeatures(const cv::Mat& image, cv::Mat& features);

	//! �������ǻ�ȡSIFT������
	void getSIFTFeatures(const cv::Mat& image, cv::Mat& features);

	//! �������ǻ�ȡHOG������
	void getHOGFeatures(const cv::Mat& image, cv::Mat& features);

	//! �������ǻ�ȡHSV�ռ�������ֱ��ͼ������
	void getHSVHistFeatures(const cv::Mat& image, cv::Mat& features);

	//! LBP feature
	void getLBPFeatures(const cv::Mat& image, cv::Mat& features);

	//! color feature
	void getColorFeatures(const cv::Mat& src, cv::Mat& features);

	//! color feature and histom
	void getHistomPlusColoFeatures(const cv::Mat& image, cv::Mat& features);

	//! get character feature
	cv::Mat charFeatures(cv::Mat in, int sizeData);
	cv::Mat charFeatures2(cv::Mat in, int sizeData);

	//! LBP feature + Histom feature
	void getLBPplusHistFeatures(const cv::Mat& image, cv::Mat& features);

	//! grayChar feauter
	void getGrayCharFeatures(const cv::Mat& grayChar, cv::Mat& features);

	void getGrayPlusLBP(const Mat& grayChar, Mat& features);


}

