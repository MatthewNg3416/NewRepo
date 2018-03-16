//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_DATAPANELDLG_H__0F1429CF_BF37_43F9_9AC1_974B6ECD4D36__INCLUDED_)
#define AFX_DATAPANELDLG_H__0F1429CF_BF37_43F9_9AC1_974B6ECD4D36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataPanelDlg.h : header file
//

#include "MPanelData.h"
class MSystemData;

const int DEF_MAX_ROW	 = 9;
const int DEF_MAX_COLUMN = 4;

/////////////////////////////////////////////////////////////////////////////
// CDataPanelDlg dialog

class CDataPanelDlg : public CDialog
{
// Construction
public:
	//101004.KKYT_____
//	CDataPanelDlg(CWnd* pParent = NULL);   // standard constructor
	CDataPanelDlg(BOOL bCurrentModel, BOOL bEditable, CWnd* pParent = NULL);   // standard constructor
	//________________

// Dialog Data
	//{{AFX_DATA(CDataPanelDlg)
	enum { IDD = IDD_DATA_PANEL };
	CBtnEnh	m_lblPanelSize_X;
	CBtnEnh	m_lblPanelSize_Y;
	CBtnEnh	m_lblPanelThickness;
	CBtnEnh	m_lblPanelLowSidePolThickness;
	CBtnEnh	m_lblPanel_InDirection;
	CBtnEnh	m_lblPanel_OutDirection;
	CBtnEnh	m_lblRunMode;
	//110114.KMS_________
	CBtnEnh m_lblReferenceDevice;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataPanelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush m_brBckColor;

	// Generated message map functions
	//{{AFX_MSG(CDataPanelDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnExit();
	afx_msg void OnSave();
	afx_msg void OnPanelMarkX();
	afx_msg void OnPanelMarkY();
	afx_msg void OnPanelMarkDist();
	afx_msg void OnSubMarkUse();
	afx_msg void OnSubMarkX();
	afx_msg void OnSubMarkY();
	afx_msg void OnSubMarkDist();
	afx_msg void OnXUpper();
	afx_msg void OnXLower();
	afx_msg void OnYLeft();
	afx_msg void OnYRight();
	afx_msg void OnPanelSizeX();
	afx_msg void OnPanelSizeY();
	afx_msg void OnLoadDir();
	afx_msg void OnUnloadDir();
	afx_msg void OnPanelThickness();
	afx_msg void OnLowPolThickness();
	afx_msg void OnClickUseSide();
	afx_msg void OnClickRunMode();
	afx_msg void OnClickMarkDistPaneltotab();
	afx_msg void OnClickReferenceInfo();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	MPanelData* m_plnkPanelData;

	MSystemData* m_plnkSystemData;

	CBtnEnh m_lblRowData[DEF_MAX_ROW][DEF_MAX_COLUMN];

	WCHAR	m_szOldValue[100];
	WCHAR	m_szNewValue[100];

	int		m_iSelectedRow;

	double	m_dPanelSize_X;				// X변 PanelSize
    double	m_dPanelSize_Y;				// Y변 PanelSize
 
	UINT	m_ePanel_InDirection;		// Panel Input 방향
	UINT	m_ePanel_OutDirection;		// Panel Output 방향

	double	m_dPanelThickness;			// Panel 두께
	double	m_dPanelLowSidePolThickness;// LowSide Panel 두께

	BOOL	m_rgbUseSideFlag[DEF_MAX_COLUMN];
	BOOL	m_rgbUse2ndMarkFlag[DEF_MAX_COLUMN];
	double  m_rgdPanelData[DEF_MAX_ROW][DEF_MAX_COLUMN];

	//110114.kms________
	CString m_strReferenceDevice;

	void updateDisplay(int iColNo);
	void UpdateDisplay();

	ERunMode		m_eRunMode;
	//101004.KKY_____
	BOOL			m_bEditable;
	//_______________

	void calculateFiducialMarkDist();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAPANELDLG_H__0F1429CF_BF37_43F9_9AC1_974B6ECD4D36__INCLUDED_)
