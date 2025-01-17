#include "stdafx.h"
#include "chars_identify.h"
#include "config.h"
using namespace cv;

namespace easypr
{
	CharsIdentify* CharsIdentify::instance_ = nullptr;

	CharsIdentify* CharsIdentify::instance() {
	if (!instance_) {
		instance_ = new CharsIdentify;
	}
	return instance_;
}
CharsIdentify::CharsIdentify() {
		//LOAD_SVM_MODEL(svm_, kDefaultAnnPath);
		LOAD_ANN_MODEL(ann_, kDefaultAnnPath);
		//    model = ml::ANN_MLP::load(path);
		//ann_ = ml::ANN_MLP::load(kDefaultAnnPath);
		LOAD_ANN_MODEL(annChinese_, kChineseAnnPath);
		//annChinese_ = ml::ANN_MLP::load(kChineseAnnPath);
		LOAD_ANN_MODEL(annGray_, kGrayAnnPath);
		//annGray_ = ml::ANN_MLP::load(kGrayAnnPath);

		kv_ = std::shared_ptr<Kv>(new Kv);
		kv_->load(kChineseMappingPath);

		extractFeature = getGrayPlusProject;
}
void CharsIdentify::LoadModel(std::string path) {
	if (path != std::string(kDefaultAnnPath)) {
		if (!ann_->empty())
			ann_->clear();
		LOAD_ANN_MODEL(ann_, path);
		
	}
}

void CharsIdentify::LoadChineseModel(std::string path) {
	if (path != std::string(kChineseAnnPath)) {
		if (!annChinese_->empty())
			annChinese_->clear();
		LOAD_ANN_MODEL(annChinese_, path);
	}
}

void CharsIdentify::LoadGrayChANN(std::string path) {
	if (path != std::string(kGrayAnnPath)) {
		if (!annGray_->empty())
			annGray_->clear();
		LOAD_ANN_MODEL(annGray_, path);
	}
}

void CharsIdentify::LoadChineseMapping(std::string path) {
	kv_->clear();
	kv_->load(path);
}

void CharsIdentify::classify(cv::Mat featureRows, std::vector<int>& out_maxIndexs,
	std::vector<float>& out_maxVals, std::vector<bool> isChineseVec) {
	int rowNum = featureRows.rows;

	cv::Mat output(rowNum, kCharsTotalNumber, CV_32FC1);
	ann_->predict(featureRows, output);

	for (int output_index = 0; output_index < rowNum; output_index++) {
		Mat output_row = output.row(output_index);
		int result = 0;
		float maxVal = -2.f;
		bool isChinses = isChineseVec[output_index];
		if (!isChinses) {
			result = 0;
			for (int j = 0; j < kCharactersNumber; j++) {
				float val = output_row.at<float>(j);
				// std::cout << "j:" << j << "val:" << val << std::endl;
				if (val > maxVal) {
					maxVal = val;
					result = j;
				}
			}
		}
		else {
			result = kCharactersNumber;
			for (int j = kCharactersNumber; j < kCharsTotalNumber; j++) {
				float val = output_row.at<float>(j);
				//std::cout << "j:" << j << "val:" << val << std::endl;
				if (val > maxVal) {
					maxVal = val;
					result = j;
				}
			}
		}
		out_maxIndexs[output_index] = result;
		out_maxVals[output_index] = maxVal;
	}
}


void CharsIdentify::classify(std::vector<CCharacter>& charVec) {
	size_t charVecSize = charVec.size();

	if (charVecSize == 0)
		return;

	Mat featureRows;
	for (size_t index = 0; index < charVecSize; index++) {
		Mat charInput = charVec[index].getCharacterMat();
		Mat feature = charFeatures(charInput, kPredictSize);
		featureRows.push_back(feature);
	}

	cv::Mat output(charVecSize, kCharsTotalNumber, CV_32FC1);
	ann_->predict(featureRows, output);

	for (size_t output_index = 0; output_index < charVecSize; output_index++) {
		CCharacter& character = charVec[output_index];
		Mat output_row = output.row(output_index);

		int result = 0;
		float maxVal = -2.f;
		std::string label = "";

		bool isChinses = character.getIsChinese();
		if (!isChinses) {
			result = 0;
			for (int j = 0; j < kCharactersNumber; j++) {
				float val = output_row.at<float>(j);
				//std::cout << "j:" << j << "val:" << val << std::endl;
				if (val > maxVal) {
					maxVal = val;
					result = j;
				}
			}
			label = std::make_pair(kChars[result], kChars[result]).second;
		}
		else {
			result = kCharactersNumber;
			for (int j = kCharactersNumber; j < kCharsTotalNumber; j++) {
				float val = output_row.at<float>(j);
				//std::cout << "j:" << j << "val:" << val << std::endl;
				if (val > maxVal) {
					maxVal = val;
					result = j;
				}
			}
			const char* key = kChars[result];
			std::string s = key;
			std::string province = kv_->get(s);
			label = std::make_pair(s, province).second;
		}
		/*std::cout << "result:" << result << std::endl;
		std::cout << "maxVal:" << maxVal << std::endl;*/
		character.setCharacterScore(maxVal);
		character.setCharacterStr(label);
	}
}


void CharsIdentify::classifyChineseGray(std::vector<CCharacter>& charVec) {
	size_t charVecSize = charVec.size();
	if (charVecSize == 0)
		return;

	Mat featureRows;
	for (size_t index = 0; index < charVecSize; index++) {
		Mat charInput = charVec[index].getCharacterMat();
		cv::Mat feature;
		extractFeature(charInput, feature);
		featureRows.push_back(feature);
	}

	cv::Mat output(charVecSize, kChineseNumber, CV_32FC1);
	annGray_->predict(featureRows, output);

	for (size_t output_index = 0; output_index < charVecSize; output_index++) {
		CCharacter& character = charVec[output_index];
		Mat output_row = output.row(output_index);
		bool isChinese = true;

		float maxVal = -2;
		int result = 0;

		for (int j = 0; j < kChineseNumber; j++) {
			float val = output_row.at<float>(j);
			//std::cout << "j:" << j << "val:" << val << std::endl;
			if (val > maxVal) {
				maxVal = val;
				result = j;
			}
		}

		// no match
		if (-1 == result) {
			result = 0;
			maxVal = 0;
			isChinese = false;
		}

		auto index = result + kCharsTotalNumber - kChineseNumber;
		const char* key = kChars[index];
		std::string s = key;
		std::string province = kv_->get(s);

		/*std::cout << "result:" << result << std::endl;
		std::cout << "maxVal:" << maxVal << std::endl;*/

		character.setCharacterScore(maxVal);
		character.setCharacterStr(province);
		character.setIsChinese(isChinese);
	}
}

void CharsIdentify::classifyChinese(std::vector<CCharacter>& charVec) {
	size_t charVecSize = charVec.size();

	if (charVecSize == 0)
		return;

	Mat featureRows;
	for (size_t index = 0; index < charVecSize; index++) {
		Mat charInput = charVec[index].getCharacterMat();
		Mat feature = charFeatures(charInput, kChineseSize);
		featureRows.push_back(feature);
	}

	cv::Mat output(charVecSize, kChineseNumber, CV_32FC1);
	annChinese_->predict(featureRows, output);

	for (size_t output_index = 0; output_index < charVecSize; output_index++) {
		CCharacter& character = charVec[output_index];
		Mat output_row = output.row(output_index);
		bool isChinese = true;

		float maxVal = -2;
		int result = 0;

		for (int j = 0; j < kChineseNumber; j++) {
			float val = output_row.at<float>(j);
			//std::cout << "j:" << j << "val:" << val << std::endl;
			if (val > maxVal) {
				maxVal = val;
				result = j;
			}
		}

		// no match
		if (-1 == result) {
			result = 0;
			maxVal = 0;
			isChinese = false;
		}

		auto index = result + kCharsTotalNumber - kChineseNumber;
		const char* key = kChars[index];
		std::string s = key;
		std::string province = kv_->get(s);

		/*std::cout << "result:" << result << std::endl;
		std::cout << "maxVal:" << maxVal << std::endl;*/

		character.setCharacterScore(maxVal);
		character.setCharacterStr(province);
		character.setIsChinese(isChinese);
	}
}

int CharsIdentify::classify(cv::Mat f, float& maxVal, bool isChinses, bool isAlphabet) {
	int result = 0;
	cv::Mat output(1, kCharactersNumber, CV_32FC1);
	ann_->predict(f, output);

	maxVal = -2.f;
	if (!isChinses) {//如果不是中文字符
		if (!isAlphabet) {//不是字母
			result = 0;
			for (int j = 0; j < kCharactersNumber; j++) {
				float val = output.at<float>(j);
				// std::cout << "j:" << j << "val:" << val << std::endl;
				if (val > maxVal) {
					maxVal = val;
					result = j;
				}
			}
		}
		else {//是字母字符
			result = 0;
			// begin with 11th char, which is 'A'
			for (int j = 10; j < kCharactersNumber; j++) {
				float val = output.at<float>(j);
				// std::cout << "j:" << j << "val:" << val << std::endl;
				if (val > maxVal) {
					maxVal = val;
					result = j;
				}
			}
		}
	}
	else {//是中文字符，从34开始，kCharsTotalNumber为65，
		result = kCharactersNumber;
		for (int j = kCharactersNumber; j < kCharsTotalNumber; j++) {
			float val = output.at<float>(j);
			//std::cout << "j:" << j << "val:" << val << std::endl;
			if (val > maxVal) {
				maxVal = val;
				result = j;
			}
		}
	}
	//std::cout << "maxVal:" << maxVal << std::endl;
	return result;
}

bool CharsIdentify::isCharacter(cv::Mat input, std::string& label, float& maxVal, bool isChinese) {
	cv::Mat feature = charFeatures(input, kPredictSize);
	auto index = static_cast<int>(classify(feature, maxVal, isChinese));

	if (isChinese) {
		//std::cout << "maxVal:" << maxVal << std::endl;
	}

	float chineseMaxThresh = 0.2f;

	if (maxVal >= 0.9 || (isChinese && maxVal >= chineseMaxThresh)) {
		if (index < kCharactersNumber) {
			label = std::make_pair(kChars[index], kChars[index]).second;
		}
		else {
			const char* key = kChars[index];
			std::string s = key;
			std::string province = kv_->get(s);
			label = std::make_pair(s, province).second;
		}
		return true;
	}
	else
		return false;
}

std::pair<std::string, std::string> CharsIdentify::identifyChinese(cv::Mat input, float& out, bool& isChinese) {
	cv::Mat feature = charFeatures(input, kChineseSize);
	float maxVal = -2;
	int result = 0;

	cv::Mat output(1, kChineseNumber, CV_32FC1);
	annChinese_->predict(feature, output);

	for (int j = 0; j < kChineseNumber; j++) {
		float val = output.at<float>(j);
		//std::cout << "j:" << j << "val:" << val << std::endl;
		if (val > maxVal) {
			maxVal = val;
			result = j;
		}
	}

	// no match
	if (-1 == result) {
		result = 0;
		maxVal = 0;
		isChinese = false;
	}
	else if (maxVal > 0.9) {
		isChinese = true;
	}

	auto index = result + kCharsTotalNumber - kChineseNumber;
	const char* key = kChars[index];
	std::string s = key;
	std::string province = kv_->get(s);
	out = maxVal;

	return std::make_pair(s, province);
}

std::pair<std::string, std::string> CharsIdentify::identifyChineseGray(cv::Mat input, float& out, bool& isChinese) {
	cv::Mat feature;
	extractFeature(input, feature);
	float maxVal = -2;
	int result = 0;
	cv::Mat output(1, kChineseNumber, CV_32FC1);
	annGray_->predict(feature, output);

	for (int j = 0; j < kChineseNumber; j++) {
		float val = output.at<float>(j);
		//std::cout << "j:" << j << "val:" << val << std::endl;
		if (val > maxVal) {
			maxVal = val;
			result = j;
		}
	}
	// no match
	if (-1 == result) {
		result = 0;
		maxVal = 0;
		isChinese = false;
	}
	else if (maxVal > 0.9) {
		isChinese = true;
	}
	auto index = result + kCharsTotalNumber - kChineseNumber;
	const char* key = kChars[index];
	std::string s = key;
	std::string province = kv_->get(s);
	out = maxVal;
	return std::make_pair(s, province);
}


std::pair<std::string, std::string> CharsIdentify::identify(cv::Mat input, bool isChinese, bool isAlphabet) {
	cv::Mat feature = charFeatures(input, kPredictSize);
	float maxVal = -2;
	auto index = static_cast<int>(classify(feature, maxVal, isChinese, isAlphabet));
	if (index < kCharactersNumber) {
		return std::make_pair(kChars[index], kChars[index]);
	}
	else {
		const char* key = kChars[index];
		std::string s = key;
		std::string province = kv_->get(s);
		return std::make_pair(s, province);
	}
}

int CharsIdentify::identify(std::vector<cv::Mat> inputs, std::vector<std::pair<std::string, std::string>>& outputs,
	std::vector<bool> isChineseVec) {
	Mat featureRows;
	size_t input_size = inputs.size();
	for (size_t i = 0; i < input_size; i++) {
		Mat input = inputs[i];
		cv::Mat feature = charFeatures(input, kPredictSize);
		featureRows.push_back(feature);
	}

	std::vector<int> maxIndexs;
	std::vector<float> maxVals;
	classify(featureRows, maxIndexs, maxVals, isChineseVec);

	for (size_t row_index = 0; row_index < input_size; row_index++) {
		int index = maxIndexs[row_index];
		if (index < kCharactersNumber) {
			outputs[row_index] = std::make_pair(kChars[index], kChars[index]);
		}
		else {
			const char* key = kChars[index];
			std::string s = key;
			std::string province = kv_->get(s);
			outputs[row_index] = std::make_pair(s, province);
		}
	}
	return 0;
}
}