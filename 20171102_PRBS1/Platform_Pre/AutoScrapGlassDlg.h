//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_SCRAPUNSCRAPDLG_H__1166C2DC_6DE8_4230_A320_31EE2CEC6E6A__INCLUDED_)
#define AFX_SCRAPUNSCRAPDLG_H__1166C2DC_6DE8_4230_A320_31EE2CEC6E6A__INCLUDED_
#include "btnenh.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScrapUnScrapDlg.h : header file
//

const int DEF_SCRAP_GLASS		= 1;
const int DEF_UNSCRAP_GLASS		= 2;

/////////////////////////////////////////////////////////////////////////////
// CAutoScrapGlassDlg dialog
class MTrsAutoManager;
class MNetH;
class CAutoMainForm;

class CAutoScrapGlassDlg : public CDialog
{
// Construction
public:
	CBrush				m_brBckColor;

	int					m_iScrapModeSelect;

	CString				m_strLotID;
	CString				m_strDeviceID;
	CString				m_strBatchID;

	int					m_iUnit;

	MTrsAutoManager*	m_plnkTrsAutoManager;
	MNetH*				m_plnkMelsecIF;

	CAutoMainForm*		m_plnkAutoMainForm;

	CAutoScrapGlassDlg(int iScrapModeSelect, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAutoScrapGlassDlg)
	enum { IDD = IDD_AUTO_SCRAP_UNSCRAP };
	CBtnEnh	m_btnBatchID;
	CBtnEnh	m_btnDeviceID;
	CBtnEnh	m_btnLotID;
	CBtnEnh	m_btnSave;
	CBtnEnh	m_btnUnit;
	CBtnEnh	m_lblOpTitle;
	CBtnEnh	m_lblBatchID;
	CBtnEnh	m_lblDeviceID;
	CBtnEnh	m_lblLotID;
	CBtnEnh	m_lblUnit;
	CBtnEnh	m_btnLoadConveyor;
	CBtnEnh	m_btnPanelTransfer;
	CBtnEnh	m_btnPanelAligner;
	CBtnEnh	m_btnUnloadConveyor;
	CBtnEnh	m_btnProductDeviceID;
	CBtnEnh	m_btnProductBatchID;
	CBtnEnh	m_btnPanelPassWordAssign;
	CBtnEnh	m_btnTurnHandler; //170123 SJ_YSH
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoScrapGlassDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void enablePanelDataButton(BOOL bState);

	// Generated message map functions
	//{{AFX_MSG(CAutoScrapGlassDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnExit();
	afx_msg void OnClickSave();
	afx_msg void OnClickLotId();
	afx_msg void OnClickDeviceId();
	afx_msg void OnClickBatchId();
	afx_msg void OnClickLoadConveyor();
	afx_msg void OnClickPanelTransfer();
	afx_msg void OnClickPanelAligner();
	afx_msg void OnClickUnloadConveyor();
	afx_msg void OnPaint();
	afx_msg void OnClickProductDeviceId();
	afx_msg void OnClickProductBatchId();
	afx_msg void OnClickPanelPassWordAssign();
	afx_msg void OnClickTurnHandler();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRAPUNSCRAPDLG_H__1166C2DC_6DE8_4230_A320_31EE2CEC6E6A__INCLUDED_)
