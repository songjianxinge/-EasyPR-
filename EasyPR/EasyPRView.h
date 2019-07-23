
// EasyPRView.h : CEasyPRView ��Ľӿ�
//

#pragma once

#include <string>
#include "EasyPR.h"
#include "config.h"
#include "EasyPRDoc.h"
#include "character.hpp"
#include "chars_identify.h"
#include "chars_segment.h"
#include "plate_locate.h"
#include "plate_judge.h"
#include <set>
#include "util.h"
#define NM_A (WM_USER + 100)
using namespace easypr;
class CEasyPRView : public CView
{
protected: // �������л�����
	CEasyPRView();
	DECLARE_DYNCREATE(CEasyPRView)

// ����
public:
	CEasyPRDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CEasyPRView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

inline std::string getPlateColor(Color in) const {
		std::string color = "δ֪";
		if (BLUE == in) color = "����";
		if (YELLOW == in) color = "����";
		if (WHITE == in) color = "����";
#ifdef OS_WINDOWS
		color = Utils::utf8_to_gbk(color.c_str());
#endif
		return color;
	}
protected:
int CEasyPRView::charsRecognise(Mat plate, std::string& plateLicense);
int CEasyPRView::charsRecognise(CPlate& plate, std::string& plateLicense);
protected:
	CEasyPRDoc* pDoc ;
	std::string filePath;
	cv::Mat m_MatImg;
	CImage src_img;
	easypr::CPlateLocate plate;
	easypr::CCharsSegment cs;
	int fileflag ;
	set<string> m_set;//���ϴ�ų���
	std::string license;//��ų����ַ�
	vector<cv::Mat> resultVec;
	vector<cv::Mat> resultVec1;
	float maxVal;
	int nHeight;
	int nWidth;
	

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnPlatela();
};

#ifndef _DEBUG  // EasyPRView.cpp �еĵ��԰汾
inline CEasyPRDoc* CEasyPRView::GetDocument() const
   { return reinterpret_cast<CEasyPRDoc*>(m_pDocument); }
#endif

