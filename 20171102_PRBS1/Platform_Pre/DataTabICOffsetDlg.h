//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_TABOFFSETDLG_H__29F25133_BDC9_41A7_9933_D08712EEC2E0__INCLUDED_)
#define AFX_TABOFFSETDLG_H__29F25133_BDC9_41A7_9933_D08712EEC2E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabOffsetDlg.h : header file
//
#include "DefSystem.h"
#include "MPos_XYT.h"

/////////////////////////////////////////////////////////////////////////////
// CDataTabICOffsetDlg dialog

class MSystemData;
class MTabData;
class MTabOffsetData;
class MPanelData;

//@const int DEF_TABIC_OFFSET_DLG_MAX_ITEM = 20;
//@const int DEF_TABIC_OFFSET_DLG_MAX_ITEM = 24;
const int DEF_TABIC_OFFSET_DLG_MAX_ITEM = 36;

class CDataTabICOffsetDlg : public CDialog
{
private:
	int				m_iWorkGroup;//0->Front, 1->Rear
	MSystemData*	m_plnkSystemData;
	MTabData*		m_plnkTabData;
	MPanelData*		m_plnkPanelData;
	MTabOffsetData* m_plnkTabOffset;
	MTabOffsetData* m_plnkTabOffset_Rear;

	MPos_XYT		m_xytTabOffset[DEF_MAX_TAB];

private:
	void pitchDisplay(int value);
	void display(int cnt);
	void changeWorkSide();
	void changeButtonState();

// Construction
public:
	CBrush m_brBckColor;
	CBtnEnh m_sData[DEF_TABIC_OFFSET_DLG_MAX_ITEM * 3];
	CBtnEnh m_bBtn[DEF_TABIC_OFFSET_DLG_MAX_ITEM * 3];
	CDataTabICOffsetDlg(CWnd* pParent = NULL);   // standard constructor


	WCHAR m_cBufGet[60];
	WCHAR m_cBuf[60];
	void ChangeData(int sel);
	int m_iWorkSide;	
	int m_iTabPage;

// Dialog Data
	//{{AFX_DATA(CDataTabICOffsetDlg)
	enum { IDD = IDD_DATA_TAB_OFFSET };
	CBtnEnh	m_bBatchY;
	CBtnEnh	m_bBatchX;
	CBtnEnh	m_bNext;
	CBtnEnh	m_bPrevious;
	CBtnEnh	m_bSave;
	CBtnEnh	m_bExit;
	CBtnEnh	m_bTabOffsetTable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataTabICOffsetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataTabICOffsetDlg)
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
	afx_msg void OnUse21();
	afx_msg void OnUse22();
	afx_msg void OnUse23();
	afx_msg void OnUse24();
	afx_msg void OnUse25();
	afx_msg void OnUse26();
	afx_msg void OnUse27();
	afx_msg void OnUse28();
	afx_msg void OnUse29();
	afx_msg void OnUse30();
	afx_msg void OnUse31();
	afx_msg void OnUse32();
	afx_msg void OnUse33();
	afx_msg void OnUse34();
	afx_msg void OnUse35();
	afx_msg void OnUse36();
	afx_msg void OnUse37();
	afx_msg void OnUse38();
	afx_msg void OnUse39();
	afx_msg void OnUse40();
	afx_msg void OnUse41();
	afx_msg void OnUse42();
	afx_msg void OnUse43();
	afx_msg void OnUse44();
	afx_msg void OnUse45();
	afx_msg void OnUse46();
	afx_msg void OnUse47();
	afx_msg void OnUse48();
	afx_msg void OnUse49();
	afx_msg void OnUse50();
	afx_msg void OnUse51();
	afx_msg void OnUse52();
	afx_msg void OnUse53();
	afx_msg void OnUse54();
	afx_msg void OnUse55();
	afx_msg void OnUse56();
	afx_msg void OnUse57();
	afx_msg void OnUse58();
	afx_msg void OnUse59();
	afx_msg void OnUse60();
	afx_msg void OnUse61();
	afx_msg void OnUse62();
	afx_msg void OnUse63();
	afx_msg void OnUse64();
	afx_msg void OnUse65();
	afx_msg void OnUse66();
	afx_msg void OnUse67();
	afx_msg void OnUse68();
	afx_msg void OnUse69();
	afx_msg void OnUse70();
	afx_msg void OnUse71();
	afx_msg void OnUse72();
	afx_msg void OnUse73();
	afx_msg void OnUse74();
	afx_msg void OnUse75();
	afx_msg void OnUse76();
	afx_msg void OnUse77();
	afx_msg void OnUse78();
	afx_msg void OnUse79();
	afx_msg void OnUse80();
	afx_msg void OnUse81();
	afx_msg void OnUse82();
	afx_msg void OnUse83();
	afx_msg void OnUse84();
	afx_msg void OnUse85();
	afx_msg void OnUse86();
	afx_msg void OnUse87();
	afx_msg void OnUse88();
	afx_msg void OnUse89();
	afx_msg void OnUse90();
	afx_msg void OnUse91();
	afx_msg void OnUse92();
	afx_msg void OnUse93();
	afx_msg void OnUse94();
	afx_msg void OnUse95();
	afx_msg void OnUse96();
	afx_msg void OnUse97();
	afx_msg void OnUse98();
	afx_msg void OnUse99();
	afx_msg void OnUse100();
	afx_msg void OnUse101();
	afx_msg void OnUse102();
	afx_msg void OnUse103();
	afx_msg void OnUse104();
	afx_msg void OnUse105();
	afx_msg void OnUse106();
	afx_msg void OnUse107();
	afx_msg void OnUse108();
	afx_msg void OnTabDataSave();
	afx_msg void OnBatchxInput();
	afx_msg void OnBatchyInput();
	afx_msg void OnTabPrevious();
	afx_msg void OnTabNext();
	afx_msg void OnTabDataCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClickBatchtInput();
	afx_msg void OnClickViewOffsetDir();
	afx_msg void OnClickWorkGroup1();
	afx_msg void OnClickWorkGroup2();
	afx_msg void OnClickTabOffsetTable();
	afx_msg void OnClickTabPrevious2();
	afx_msg void OnClickTabNext2();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABOFFSETDLG_H__29F25133_BDC9_41A7_9933_D08712EEC2E0__INCLUDED_)
