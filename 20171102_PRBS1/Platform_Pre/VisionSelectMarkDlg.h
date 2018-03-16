//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_VISIONSELECTMARKDLG_H__B4D6AE11_9598_45C9_A059_BB5BBABD1F26__INCLUDED_)
#define AFX_VISIONSELECTMARKDLG_H__B4D6AE11_9598_45C9_A059_BB5BBABD1F26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectMarkDlg.h : header file
//

#include "DefVisionExt.h"

/////////////////////////////////////////////////////////////////////////////
// CVisionSelectMarkDlg dialog
#define	DEF_DLG_SELECT_MARK_MAX_BUTTON	28
enum { DEF_SHOW_MARK, DEF_MAKE_MARK, DEF_DELETE_MARK };

typedef struct sMarkInfo
{
	int m_iCamNo;
	EMarkType m_eType;
	int* m_piNo;
	char m_szName[64];
} SMarkInfo;

class IVision;

class CVisionSelectMarkDlg : public CDialog
{
private:
	int m_iOpType;
	char** m_pMarkName;
	SMarkInfo* m_psMarkInfo;
	IVision* m_plnkVision;

	CBtnEnh m_rgCtrlBtn[DEF_DLG_SELECT_MARK_MAX_BUTTON];

	CString m_rgstrMarkName[DEF_DLG_SELECT_MARK_MAX_BUTTON];

// Construction
public:
	CVisionSelectMarkDlg(IVision* plnkVision, int iOpType, SMarkInfo* pMarkInfo, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVisionSelectMarkDlg)
	enum { IDD = IDD_VISION_SELECT_MARK };
	//}}AFX_DATA	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisionSelectMarkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	BOOL CVisionSelectMarkDlg::isUsedMark(int iCamNo, int iType, int iMarkNo);

protected:
	CBrush m_brBckColor;

	// Generated message map functions
	//{{AFX_MSG(CVisionSelectMarkDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnExit();
	afx_msg void OnSelect0();
	afx_msg void OnSelect1();
	afx_msg void OnSelect2();
	afx_msg void OnSelect3();
	afx_msg void OnSelect4();
	afx_msg void OnSelect5();
	afx_msg void OnClickSelect6();
	afx_msg void OnClickSelect7();
	afx_msg void OnClickSelect8();
	afx_msg void OnClickSelect9();
	afx_msg void OnClickSelect10();
	afx_msg void OnClickSelect11();
	afx_msg void OnClickSelect12();
	afx_msg void OnClickSelect13();
	afx_msg void OnClickSelect14();
	afx_msg void OnClickSelect15();
	afx_msg void OnClickSelect16();
	afx_msg void OnClickSelect17();
	afx_msg void OnClickSelect18();
	afx_msg void OnClickSelect19();
	afx_msg void OnClickSelect20();
	afx_msg void OnClickSelect21();
	afx_msg void OnClickSelect22();
	afx_msg void OnClickSelect23();
	afx_msg void OnClickSelect24();
	afx_msg void OnClickSelect25();
	afx_msg void OnClickSelect26();
	afx_msg void OnClickSelect27();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISIONSELECTMARKDLG_H__B4D6AE11_9598_45C9_A059_BB5BBABD1F26__INCLUDED_)
