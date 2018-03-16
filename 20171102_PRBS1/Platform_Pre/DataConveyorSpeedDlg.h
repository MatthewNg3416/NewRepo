//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_DATACONVEYORSPEEDDLG_H__101C852E_76C6_4257_BA6D_BB2EAC76C110__INCLUDED_)
#define AFX_DATACONVEYORSPEEDDLG_H__101C852E_76C6_4257_BA6D_BB2EAC76C110__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataConveyorSpeedDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataConveyorSpeedDlg dialog

#include "DefSystemConfig.h"

class IInverter;
//class MRollerConveyor;
class MPreBonderData;

class CDataConveyorSpeedDlg : public CDialog
{
// Construction
public:
	CDataConveyorSpeedDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataConveyorSpeedDlg)
	enum { IDD = IDD_DATA_CONVEYOR_SPEED };
	CBtnEnh	m_lblLoadConveyorNormalSpeed;
	CBtnEnh	m_lblLoadConveyorReducedSpeed;
	CBtnEnh	m_lblLoadConveyorMotorNormalFreq;
	CBtnEnh	m_lblLoadConveyorMotorReducedFreq;
	CBtnEnh	m_lblUnloadConveyorNormalSpeed;
	CBtnEnh	m_lblUnloadConveyorReducedSpeed;
	CBtnEnh	m_lblUnloadConveyorMotorNormalFreq;
	CBtnEnh	m_lblUnloadConveyorMotorReducedFreq;
	CBtnEnh	m_lblLoadConveyorTransferSpeed;
	CBtnEnh	m_lblLoadConveyorMotorTransferFreq;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataConveyorSpeedDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataConveyorSpeedDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickSave();
	afx_msg void OnClickExit();
	afx_msg void OnClickLoadConveyorNormalSpeed();
	afx_msg void OnClickLoadConveyorReducedSpeed();
	afx_msg void OnClickUnloadConveyorNormalSpeed();
	afx_msg void OnClickUnloadConveyorReducedSpeed();
	afx_msg void OnClickLoadConveyorNormalSpeedMoveForward();
	afx_msg void OnClickLoadConveyorNormalSpeedMoveBackward();
	afx_msg void OnClickLoadConveyorReducedSpeedMoveForward();
	afx_msg void OnClickLoadConveyorReducedSpeedMoveBackward();
	afx_msg void OnClickLoadConveyorStop();
	afx_msg void OnClickUnloadConveyorNormalSpeedMoveForward();
	afx_msg void OnClickUnloadConveyorNormalSpeedMoveBackward();
	afx_msg void OnClickUnloadConveyorReducedSpeedMoveForward();
	afx_msg void OnClickUnloadConveyorReducedSpeedMoveBackward();
	afx_msg void OnClickUnloadConveyorStop();
	afx_msg void OnClickLoadConveyorTransferSpeed();
	afx_msg void OnClickLoadConveyorTransferSpeedMoveForward();
	afx_msg void OnPaint();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CBrush m_brBckColor;

	IInverter*	m_plnkLoadConveyorInverter;
	IInverter*	m_plnkUnloadConveyorInverter;

	MPreBonderData*	m_plnkPreBonderData;

//	MRollerConveyor* m_plnkLoadConveyor;
//	MRollerConveyor* m_plnkUnloadConveyor;

	WCHAR	m_szOldValue[100];
	WCHAR	m_szNewValue[100];

	double	m_dLoadConveyorNormalSpeed;
	double	m_dLoadConveyorReducedSpeed;
	double	m_dUnloadConveyorNormalSpeed;
	double	m_dUnloadConveyorReducedSpeed;

	double	m_dLoadConveyorMotorNormalFreq;
	double	m_dLoadConveyorMotorReducedFreq;
	double	m_dUnloadConveyorMotorNormalFreq;
	double	m_dUnloadConveyorMotorReducedFreq;

	double	m_dLoadConveyorTransferSpeed;
	double	m_dLoadConveyorMotorTransferFreq;

	void	UpdateDisplay();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATACONVEYORSPEEDDLG_H__101C852E_76C6_4257_BA6D_BB2EAC76C110__INCLUDED_)
