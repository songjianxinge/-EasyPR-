#pragma once
#include "feature.h"
#include "plate.hpp"

namespace easypr {

	class CPlateJudge
	{
	public:
		static CPlateJudge* instance();
		void LoadModel(std::string path);
		//ʹ��NMSɾ���ص��Ŀ�
		int plateJudgeUsingNMS(const std::vector<CPlate> &c, std::vector<CPlate>&z, int maxPlates = 5);
		//ͨ������SVMģ�ͽ����б� ���ó��Ʒ�����0 is plate, -1 is not.
		int plateSetScore(CPlate & plate);
		//�������ͨMatת��ΪCPlate���󣬵���plateSetScore���������score
		int plateJudge(const Mat& plateMat);
		//����һ��Mat������Mat�����������ṹ��plateJudge,���ÿ��Mat��Ӧ��score
		int plateJudge(const std::vector<Mat> &inVec, std::vector<Mat> &resultVec);
		//ͬ�ϣ�����һ��CPlate����ȡ������Mat��Ա��Ȼ�����������������һ���ṹ��plateJudge
		int plateJudge(const std::vector<CPlate> &inVec, std::vector<CPlate> &resultVec);
	private:
		CPlateJudge();
		
		static CPlateJudge* instance_;
		svmCallback extractFeature;
		cv::Ptr<ml::SVM> svm_;
	};

}

