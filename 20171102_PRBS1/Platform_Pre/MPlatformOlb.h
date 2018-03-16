// MPlatformOlb.h : main header file for the MPlatformOlb application
//

#if !defined(AFX_MPlatformOlb_H__569D6456_F7BC_4B6B_8C47_8DEB3C8BCB1F__INCLUDED_)
#define AFX_MPlatformOlb_H__569D6456_F7BC_4B6B_8C47_8DEB3C8BCB1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

//100807.ECID___________________
#include "simpleAdo.h"

extern simpleAdo MyInputDB;
extern bool IsConnect;
extern BOOL	gIOTestMode;

/////////////////////////////////////////////////////////////////////////////
// CMPlatformOlbApp:
// See MPlatformOlb.cpp for the implementation of this class
//

class CMPlatformOlbApp : public CWinApp
{
public:
	CMPlatformOlbApp();
	~CMPlatformOlbApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMPlatformOlbApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMPlatformOlbApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MPlatformOlb_H__569D6456_F7BC_4B6B_8C47_8DEB3C8BCB1F__INCLUDED_)
