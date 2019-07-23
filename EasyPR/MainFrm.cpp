
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "EasyPR.h"

#include "MainFrm.h"

#include <opencv2/imgproc/imgproc_c.h>
#include <iostream>
#include <vector>
#include <opencv.hpp>
#include "plate_judge.h"
#include "chars_segment.h"
#include "chars_identify.h"
#include "plate_locate.h"
#include "LifeView.h"
#include "EasyPRView.h"
#include "RightView.h"
using namespace std;
using namespace easypr;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(NM_A, OnMyChange)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����




CMainFrame::CMainFrame()
{
	//// TODO: �ڴ���ӳ�Ա��ʼ������

	/*cout << "test_plate_locate" << endl;
	const string file = "resources/image/plate_lop.bmp";
	cv::Mat src = imread(file);
	vector<cv::Mat> resultVec;
	vector<cv::Mat> resultVec1;
	float maxVal;
	CPlateLocate plate;
	plate.setDebug(1);
	plate.setLifemode(true);
	CCharsSegment cs;
	int result = plate.plateLocate(src, resultVec);
	std::string license;
	if (result != 0) return;
	for (size_t j = 0; j < resultVec.size(); j++)
	{
		imshow("t" + j, resultVec[j]);
	}
	for (size_t j = 0; j < resultVec.size(); j++) {
		cv::Mat resultMat = resultVec[j];
		int score = CPlateJudge::instance()->plateJudge(resultVec[j]);
		if (score == 0) {
				int result_seg = cs.charsSegment(resultMat,resultVec1);
				if (result_seg == 0)
				{
					for (size_t block_i = 0; block_i < resultVec1.size(); block_i++)
					{
						auto block = resultVec1[block_i];
						if (block_i == 0)
						{
							auto character = CharsIdentify::instance()->identify(block, true);
							license.append(character.second);
						}
						else
						{
							auto character = CharsIdentify::instance()->identify(block, false);
							license.append(character.second);
						}
					}
				}
			license.c_str();
			CString str(license.c_str());
			AfxMessageBox(str);
			}
			waitKey(0);
		}
	destroyWindow("plate_locate");
*/
	
	//return result;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: �������Ҫ��ͣ������������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������




BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���
	//���һ������
	m_spliter.CreateStatic(this, 1, 2);
	//�����Ҳ�������ʾ����
	m_spliter.CreateView(0, 0, RUNTIME_CLASS(CEasyPRView), CSize(600, 500), pContext);
	m_spliter.CreateView(0, 1, RUNTIME_CLASS(CRightView), CSize(200, 500), pContext);

	return TRUE;
}
LRESULT CMainFrame::OnMyChange(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NM_A)
	{
		Invalidate();
		//AfxMessageBox(_T("t"));
	}

	return 0;
}