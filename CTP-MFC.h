
// CTP-MFC.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCTPMFCApp: 
// �йش����ʵ�֣������ CTP-MFC.cpp
//

class CCTPMFCApp : public CWinApp
{
public:
	CCTPMFCApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

private:
	ULONG_PTR m_nGdiplusToken;
};

extern CCTPMFCApp theApp;