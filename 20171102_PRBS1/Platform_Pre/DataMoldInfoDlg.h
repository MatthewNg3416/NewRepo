#if !defined(AFX_MOLDINFORMDLG_H__97466045_1D4F_42CA_9DE9_77D76CBB375D__INCLUDED_)
#define AFX_MOLDINFORMDLG_H__97466045_1D4F_42CA_9DE9_77D76CBB375D__INCLUDED_
#include "msflexgrid.h"
#include "btnenh.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MoldInformDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataMoldInfoDlg dialog
class MRFID;

class CDataMoldInfoDlg : public CDialog
{
// Construction
public:
	CBrush m_brBckColor;
	int m_iFeederSel;
	CDataMoldInfoDlg(int iFeederSel, CWnd* pParent = NULL);   // standard constructor

private:
	MRFID *m_pRFID;
	BOOL DisplayRFIDInformation();
	int m_iZigInfoIndex;

// Dialog Data
	//{{AFX_DATA(CDataMoldInfoDlg)
	enum { IDD = IDD_DATA_MOLD_INFO };
	//CMSFlexGrid	m_FlexGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataMoldInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataMoldInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickSelectZigInfo();
	afx_msg void OnClickWriteZigInfo();
	afx_msg void OnClickRefreshInfo();
	afx_msg void OnClickCancel();
	afx_msg void OnPaint();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOLDINFORMDLG_H__97466045_1D4F_42CA_9DE9_77D76CBB375D__INCLUDED_)
