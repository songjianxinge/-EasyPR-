
// EasyPR.h : EasyPR Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CEasyPRApp:
// �йش����ʵ�֣������ EasyPR.cpp
//

class CEasyPRApp : public CWinAppEx
{
public:
	CEasyPRApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CEasyPRApp theApp;
