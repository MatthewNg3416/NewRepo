//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_DATATOWERLAMPDLG_H__1EB62696_486C_46DE_954D_7BF5A27DBF69__INCLUDED_)
#define AFX_DATATOWERLAMPDLG_H__1EB62696_486C_46DE_954D_7BF5A27DBF69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataTowerLampDlg.h : header file
//

#include "MSystemData.h"
#include "MTowerLampData.h"

const int DEF_TOWER_MAX_ROW	 = 6;
const int DEF_TOWER_MAX_COLUMN = 6;

/////////////////////////////////////////////////////////////////////////////
// CDataTowerLampDlg dialog

class CDataTowerLampDlg : public CDialog
{
// Construction
public:
	CDataTowerLampDlg(CWnd* pParent = NULL);   // standard constructor

	

// Dialog Data
	//{{AFX_DATA(CDataTowerLampDlg)
	enum { IDD = IDD_DATA_TOWER_LAMP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataTowerLampDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CDataTowerLampDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickSave();
	afx_msg void OnClickCancel();
	afx_msg void OnClickRedLampState();
	afx_msg void OnClickYellowLampState();
	afx_msg void OnClickGreenLampState();
	afx_msg void OnClickLampStateRun();
	afx_msg void OnClickLampStateStepStop();
	afx_msg void OnClickLampStateErrorStop();
	afx_msg void OnClickLampStatePanelEmpty();
	afx_msg void OnClickLampStateMaterialChange();
	afx_msg void OnClickBuzzerSelect();
	afx_msg void OnClickLampStateOpCall();
	afx_msg void OnClickStartLampState();
	afx_msg void OnClickStopLampState();
	afx_msg void OnClickDelayTime();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CBtnEnh m_lblRowData[DEF_TOWER_MAX_ROW][DEF_TOWER_MAX_COLUMN];
	CBtnEnh m_lblDelayTime;	//20170920 KDW Add
	WCHAR m_rgOldValue[100];
	WCHAR m_rgNewValue[100];
	int  m_iDelayTime;		//20170920 KDW Add
	int  m_rgdTowerLampData[DEF_TOWER_MAX_ROW][DEF_TOWER_MAX_COLUMN];
	int		m_iSelectedRow,m_iSelectState;
	void SelectLamp(int iColNo);
	void UpdateDisplay();

	MTowerLampData* m_plnkTowerLampData;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATATOWERLAMPDLG_H__1EB62696_486C_46DE_954D_7BF5A27DBF69__INCLUDED_)
