// LifeView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EasyPR.h"
#include "LifeView.h"


// CLifeView

IMPLEMENT_DYNCREATE(CLeftView, CFormView)

CLeftView::CLeftView()
	: CFormView(IDD_LIFEVIEW)
{

}

CLeftView::~CLeftView()
{
}

void CLeftView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLeftView, CFormView)
END_MESSAGE_MAP()


// CLifeView ���

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLeftView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLifeView ��Ϣ�������


void CLeftView::OnDraw(CDC* /*pDC*/)
{
	// TODO: �ڴ����ר�ô����/����û���
}
