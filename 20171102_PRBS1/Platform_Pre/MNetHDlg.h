//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_MNETHDLG_H__B1159095_B4FA_493B_9C35_90F1ECC16B1D__INCLUDED_)
#define AFX_MNETHDLG_H__B1159095_B4FA_493B_9C35_90F1ECC16B1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MNetHDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MNetHDlg dialog

#include "MNetHData.h"
#include "BtnST.h"
#include "MLCNetData.h" //SJ_YYK 150821 Add..

class MNetH;

class MNetHDlg : public CDialog
{
// Construction
public:
	MNetHDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(MNetHDlg)
	enum { IDD = IDD_MNETH };
	CStatic	m_lblPrevLocal;
	CStatic	m_lblOwnLocal;
	CStatic	m_lblNextLocal;
	CStatic	m_lblWord_1;
	CStatic	m_lblWord_2;
	CStatic	m_lblWord_3;
	CStatic	m_lblWord_4;
	CStatic	m_lblWord_5;
	CStatic	m_lblWord_6;
	CStatic	m_lblWord_7;
	CStatic	m_lblWord_8;
	CStatic	m_lblWord_9;
	CStatic	m_lblWord_10;
	CStatic	m_lblWord_11;
	CStatic	m_lblWord_12;
	CStatic	m_lblWord_13;
	CStatic	m_lblWord_14;
	CStatic	m_lblWord_15;
	CStatic	m_lblWord_16;
	CStatic	m_lblWord_17;
	CStatic	m_lblWord_18;
	CStatic	m_lblWord_19;
	CStatic	m_lblWord_20;
	CStatic	m_lblWord_21;
	CStatic	m_lblWord_22;
	CStatic	m_lblWord_23;
	CStatic	m_lblWord_24;
	CStatic	m_lblWord_25;
	CStatic	m_lblWord_26;
	CStatic	m_lblWord_27;
	CStatic	m_lblWord_28;
	CStatic	m_lblWord_29;
	CStatic	m_lblWord_30;
	CStatic	m_lblWord_31;
	CStatic	m_lblWord_32;
	CEdit	m_edtWord_1;
	CEdit	m_edtWord_2;
	CEdit	m_edtWord_3;
	CEdit	m_edtWord_4;
	CEdit	m_edtWord_5;
	CEdit	m_edtWord_6;
	CEdit	m_edtWord_7;
	CEdit	m_edtWord_8;
	CEdit	m_edtWord_9;
	CEdit	m_edtWord_10;
	CEdit	m_edtWord_11;
	CEdit	m_edtWord_12;
	CEdit	m_edtWord_13;
	CEdit	m_edtWord_14;
	CEdit	m_edtWord_15;
	CEdit	m_edtWord_16;
	CEdit	m_edtWord_17;
	CEdit	m_edtWord_18;
	CEdit	m_edtWord_19;
	CEdit	m_edtWord_20;
	CEdit	m_edtWord_21;
	CEdit	m_edtWord_22;
	CEdit	m_edtWord_23;
	CEdit	m_edtWord_24;
	CEdit	m_edtWord_25;
	CEdit	m_edtWord_26;
	CEdit	m_edtWord_27;
	CEdit	m_edtWord_28;
	CEdit	m_edtWord_29;
	CEdit	m_edtWord_30;
	CEdit	m_edtWord_31;
	CEdit	m_edtWord_32;
	CTabCtrl	m_tabArea;
	CTabCtrl	m_tabUnit;
	CTabCtrl	m_tabLocal;
	CTabCtrl	m_tabData;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MNetHDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(MNetHDlg)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTabPLC(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTabUnit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTabData(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTabLocal(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTabArea(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnBit1();
	afx_msg void OnBtnBit2();
	afx_msg void OnBtnBit3();
	afx_msg void OnBtnBit4();
	afx_msg void OnBtnBit5();
	afx_msg void OnBtnBit6();
	afx_msg void OnBtnBit7();
	afx_msg void OnBtnBit8();
	afx_msg void OnBtnBit9();
	afx_msg void OnBtnBit10();
	afx_msg void OnBtnBit11();
	afx_msg void OnBtnBit12();
	afx_msg void OnBtnBit13();
	afx_msg void OnBtnBit14();
	afx_msg void OnBtnBit15();
	afx_msg void OnBtnBit16();
	afx_msg void OnBtnBit17();
	afx_msg void OnBtnBit18();
	afx_msg void OnBtnBit19();
	afx_msg void OnBtnBit20();
	afx_msg void OnBtnBit21();
	afx_msg void OnBtnBit22();
	afx_msg void OnBtnBit23();
	afx_msg void OnBtnBit24();
	afx_msg void OnBtnBit25();
	afx_msg void OnBtnBit26();
	afx_msg void OnBtnBit27();
	afx_msg void OnBtnBit28();
	afx_msg void OnBtnBit29();
	afx_msg void OnBtnBit30();
	afx_msg void OnBtnBit31();
	afx_msg void OnBtnBit32();
	afx_msg void OnBtnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CButtonST		m_btnBit_1;
	CButtonST		m_btnBit_2;
	CButtonST		m_btnBit_3;
	CButtonST		m_btnBit_4;
	CButtonST		m_btnBit_5;
	CButtonST		m_btnBit_6;
	CButtonST		m_btnBit_7;
	CButtonST		m_btnBit_8;
	CButtonST		m_btnBit_9;
	CButtonST		m_btnBit_10;
	CButtonST		m_btnBit_11;
	CButtonST		m_btnBit_12;
	CButtonST		m_btnBit_13;
	CButtonST		m_btnBit_14;
	CButtonST		m_btnBit_15;
	CButtonST		m_btnBit_16;
	CButtonST		m_btnBit_17;
	CButtonST		m_btnBit_18;
	CButtonST		m_btnBit_19;
	CButtonST		m_btnBit_20;
	CButtonST		m_btnBit_21;
	CButtonST		m_btnBit_22;
	CButtonST		m_btnBit_23;
	CButtonST		m_btnBit_24;
	CButtonST		m_btnBit_25;
	CButtonST		m_btnBit_26;
	CButtonST		m_btnBit_27;
	CButtonST		m_btnBit_28;
	CButtonST		m_btnBit_29;
	CButtonST		m_btnBit_30;
	CButtonST		m_btnBit_31;
	CButtonST		m_btnBit_32;

	CButtonST		*m_btnBit[32];
	CStatic			*m_lblWord[32];
	CEdit			*m_edtWord[32];

public:
	MNetH			*m_pMNetH;
	BOOL			m_bRun;	
	bool			m_bEditSignalMode;

public:
	void	InitProcess();
	void	InitCtl(); 
	void	InitBitAreaTitle(int nTitleIndex=0); 
	void	InitWordAreaTitle(int nTitleIndex=0); 
	void	InitTab();
	void	InitDataTab(bool bBitArea=true);
	void	InitTitle_UpperLower();
	void	ViewBitCtlVisible(bool bView=true);
	void	ViewWordCtlVisible(bool bView=true);
	void	UpdateSignalBit(int nBitIndex, int nValue);
	void	UpdateWordData(int nItemIndex, CString sValue); 

	void	UpdateBitData(MRobotArmStateBit *RS, bool bEtcFullZero=false);
	void	UpdateBitData(MStageStateBit *SS, bool bEtcFullZero=false);
	void	UpdateBitData(MToUpperEqBit *pUS);
	void	UpdateBitData(MToLowerEqBit *pLS);
	void	UpdateBitData(MModelChangeBitDatas *pBD);

	void	UpdateEqState_Own(MEqStates *pES_Own); 
	void	UpdateEqState_Related(MEqStates *pES_Related); 
	void	UpdateJobSummary(MGlassSummarys *pGS); 
	void	UpdateUpperEqData(MToUpperEqs *pUE); 
	void	UpdateLowerEqData(MToLowerEqs *pLE); 
	void	UpdateModelChangeWord(MModelChangeWordDatas *pWD); 
	void	UpdateMaterialInfo(MMaterial* pMat); //SJ_YYK 150821 Add...
	void	UpdateFDCInfo(MProcessParamInfo* pParam);

	void	MonitoringStart();
	void	MonitoringStop();

	int		GetCurSelAreaTab();
	int		GetCurSelLocalTab();
	int		GetCurSelUnitTab();
	int		GetCurSelDataTab();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MNETHDLG_H__B1159095_B4FA_493B_9C35_90F1ECC16B1D__INCLUDED_)
