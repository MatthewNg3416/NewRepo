//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_TABDATADLG_H__2F23583D_1556_4B76_A26E_95114A3E8ED2__INCLUDED_)
#define AFX_TABDATADLG_H__2F23583D_1556_4B76_A26E_95114A3E8ED2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabDataDlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CDataTabICDlg dialog
//@const int DEF_MAX_TAB_DATA = 50;
const int DEF_MAX_TAB_DATA = 100;

const int DEF_XU_DATA_INDEX = 0;
//@const int DEF_XD_DATA_INDEX = 50;
//@const int DEF_YL_DATA_INDEX = 100;
//@const int DEF_YR_DATA_INDEX = 150;
const int DEF_XD_DATA_INDEX = 100;
const int DEF_YL_DATA_INDEX = 200;
const int DEF_YR_DATA_INDEX = 300;

//const int DEF_TAB_DATA_INDEX_START_OFFSET = 11;
//const int DEF_TAB_DATA_INDEX_START_OFFSET = 19;
const int DEF_TAB_DATA_INDEX_START_OFFSET = 20; //SJ_YYK 150213 Modify..
//@const int DEF_TABIC_DLG_MAX_TAB_NUM = 24;
const int DEF_TABIC_DLG_MAX_TAB_NUM = 36;
const int DEF_MAX_SET_DATA_NUM = DEF_TABIC_DLG_MAX_TAB_NUM + DEF_TAB_DATA_INDEX_START_OFFSET;

class MPanelData;
class MPreBonderData;
class MSystemData;
class MTabData;

class CDataTabICDlg : public CDialog
{
private:
	MPanelData* m_plnkPanelData;
	MPreBonderData* m_plnkPreBonderData;
	MSystemData* m_plnkSystemData;
	MTabData* m_plnkTabData;
	//101004.KKY_____
	BOOL		m_bEditable;
	//_______________
// Construction
public:
	CBrush m_brBckColor;
	CBtnEnh	m_btnTabOn[DEF_TABIC_DLG_MAX_TAB_NUM];
	CBtnEnh	m_sData[DEF_MAX_SET_DATA_NUM];
	CBtnEnh	m_btnWorkTabNum;

	double	m_dData[400];
	BOOL	m_iFuncOnOff;
	BOOL	m_bUse[400];
	int		m_iWorkSide;
	int		m_iBadTabLimitCount;
	int		m_iTabPage;

	//20120828 sj_ysb
	double m_dSRLineMarkDistance;
	double m_dTabEndMarkDistance;
	BOOL m_bMarkDistanceFunc;
	BOOL m_bSRLineAlignFunc;

	WCHAR m_cTmpBuf[60];
	WCHAR m_cBuf[60];

	//101004.KKY_____
//	CDataTabICDlg(int iSel, CWnd* pParent = NULL);   // standard constructor
	CDataTabICDlg(BOOL bCurrentModel, BOOL bEditable, int iSel, CWnd* pParent = NULL);   // standard constructor
	//_______________
	void UpdateDisplay(int iWorkSideType);
	void OffsetDisplay();
	void ChangeData(int iSel);
	void ChangeUse(int iSel);

	/*/
	//MERGE_LKH_090704
	double m_uiPermissionXLeftValue13;
	double m_uiPermissionXRightValue13;
	double m_uiPermissionXLeftValue24;
	double m_uiPermissionXRightValue24;
	/*/

	//2009.08.14 CYJ
	double m_dLength_between_ChipCenter_and_TabEnd;
	
// Dialog Data
	//{{AFX_DATA(CDataTabICDlg)
	enum { IDD = IDD_DATA_TAB };
		CBtnEnh	m_bBatch;
		CBtnEnh	m_bNext;
		CBtnEnh	m_bPrevious;
		CBtnEnh	m_bExit;
		CBtnEnh	m_bSave;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataTabICDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataTabICDlg)
	afx_msg void OnTitle1();
	afx_msg void OnTitle2();
	afx_msg void OnTitle3();
	afx_msg void OnTitle4();
	afx_msg void OnTitle5();
	afx_msg void OnTitle6();
	afx_msg void OnTitle7();
	afx_msg void OnTitle8();
	afx_msg void OnTitle9();
	afx_msg void OnTitle10();
	afx_msg void OnTitle11();
	afx_msg void OnOn1();
	afx_msg void OnOn2();
	afx_msg void OnOn3();
	afx_msg void OnOn4();
	afx_msg void OnOn5();
	afx_msg void OnOn6();
	afx_msg void OnOn7();
	afx_msg void OnOn8();
	afx_msg void OnOn9();
	afx_msg void OnOn10();
	afx_msg void OnOn11();
	afx_msg void OnOn12();
	afx_msg void OnOn13();
	afx_msg void OnOn14();
	afx_msg void OnOn15();
	afx_msg void OnOn16();
	afx_msg void OnOn17();
	afx_msg void OnOn18();
	afx_msg void OnOn19();
	afx_msg void OnOn20();
	afx_msg void OnClickOn21();
	afx_msg void OnClickOn22();
	afx_msg void OnClickOn23();
	afx_msg void OnClickOn24();
	afx_msg void OnClickOn25();
	afx_msg void OnClickOn26();
	afx_msg void OnClickOn27();
	afx_msg void OnClickOn28();
	afx_msg void OnClickOn29();
	afx_msg void OnClickOn30();
	afx_msg void OnClickOn31();
	afx_msg void OnClickOn32();
	afx_msg void OnClickOn33();
	afx_msg void OnClickOn34();
	afx_msg void OnClickOn35();
	afx_msg void OnClickOn36();
	afx_msg void OnUse1();
	afx_msg void OnUse2();
	afx_msg void OnUse3();
	afx_msg void OnUse4();
	afx_msg void OnUse5();
	afx_msg void OnUse6();
	afx_msg void OnUse7();
	afx_msg void OnUse8();
	afx_msg void OnUse9();
	afx_msg void OnUse10();
	afx_msg void OnUse11();
	afx_msg void OnUse12();
	afx_msg void OnUse13();
	afx_msg void OnUse14();
	afx_msg void OnUse15();
	afx_msg void OnUse16();
	afx_msg void OnUse17();
	afx_msg void OnUse18();
	afx_msg void OnUse19();
	afx_msg void OnUse20();
	afx_msg void OnClickUse21();
	afx_msg void OnClickUse22();
	afx_msg void OnClickUse23();
	afx_msg void OnClickUse24();
	afx_msg void OnClickUse25();
	afx_msg void OnClickUse26();
	afx_msg void OnClickUse27();
	afx_msg void OnClickUse28();
	afx_msg void OnClickUse29();
	afx_msg void OnClickUse30();
	afx_msg void OnClickUse31();
	afx_msg void OnClickUse32();
	afx_msg void OnClickUse33();
	afx_msg void OnClickUse34();
	afx_msg void OnClickUse35();
	afx_msg void OnClickUse36();
	afx_msg void OnTabDataSave();
	afx_msg void OnBatchInput();
	afx_msg void OnTabPrevious();
	afx_msg void OnTabNext();
	afx_msg void OnTabDataCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnTabPitch();
	afx_msg void OnClickImageView();
	afx_msg void OnPaint();
	afx_msg void OnClickTitle12();
	afx_msg void OnClickTitle13();
	afx_msg void OnClickTitle14();
	afx_msg void OnClickTitle15();
	afx_msg void OnClickTitle16();
	afx_msg void OnClickTitle17();
	afx_msg void OnClickTitle19();
	afx_msg void OnClickTitle18();
	afx_msg void OnClickTabPrevious3();
	afx_msg void OnClickTabNext3();
	afx_msg void OnClickTitle20();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABDATADLG_H__2F23583D_1556_4B76_A26E_95114A3E8ED2__INCLUDED_)
