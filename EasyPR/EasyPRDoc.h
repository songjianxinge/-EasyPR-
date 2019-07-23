
// EasyPRDoc.h : CEasyPRDoc ��Ľӿ�
//


#pragma once
#include <opencv.hpp>
class CEasyPRDoc : public CDocument
{
protected: // �������л�����
	CEasyPRDoc();

	DECLARE_DYNCREATE(CEasyPRDoc)

// ����
public:
	void MatToCImage(cv::Mat &mat, CImage &cImage);
// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CEasyPRDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	cv::Mat locatePicture;
	cv::Mat charsegment;
	std::string strR;
protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
