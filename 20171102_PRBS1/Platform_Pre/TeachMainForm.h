//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_TEACHMAINFORM_H__157AEC42_85C6_44B5_9082_B8DB95789962__INCLUDED_)
#define AFX_TEACHMAINFORM_H__157AEC42_85C6_44B5_9082_B8DB95789962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TeachMainForm.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CTeachMainForm form view

const int ERR_TEACH_MAINFORM_SUCCESS				= 0;
const int ERR_TEACH_MAINFORM_FAILURE				= 1;
const int ERR_TEACH_MAINFORM_SKIP_ERROR_DISPLAY		= 2;

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "DefSystem.h"

class MSystemData;
class MProcessData;
class MPanelData;

class MCameraCarrier;
class MTabCarrier;

class MCtrlPanelAligner;
class MPos_XY;

// Doolin kwangilshin 2017. 08. 24.
//
#define	DEF_AXIS_STS_GRD_WIDTH		15


namespace AxisSnsStsEx
{
#define DEF_AXIS_SNS_STS(DEF_AXIS_SNS_STS_IDX)\
	DEF_AXIS_SNS_STS_IDX(S)\
	DEF_AXIS_SNS_STS_IDX(N)\
	DEF_AXIS_SNS_STS_IDX(H)\
	DEF_AXIS_SNS_STS_IDX(P)\
	DEF_AXIS_SNS_STS_IDX(AxisStsEnd)

#define DEF_AXIS_SNS_STS_ENUM(ENUM)		ENUM,
#define DEF_AXIS_SNS_STS_STR(STRING)	_T(#STRING),
	
	//171228 JSH.s
	enum SensorIdx
	{
		DEF_AXIS_SNS_STS(DEF_AXIS_SNS_STS_ENUM)
	};

	enum AxisIdx
	{
		eAxisX		= 0,
		eAxisY		= 1,
		eAxisT		= 2,
		eAxisZ		= 3,
		eAxisEnd	= 4
	};
	//171228 JSH.e
	
	static const CString strName[]		= { DEF_AXIS_SNS_STS(DEF_AXIS_SNS_STS_STR) };
	static const CString strCvtName[]	= { _T("S"), _T("-"), _T("H"), _T("+") };
};
//
// End.



class CTeachMainForm : public CFormView
{
protected:
	CTeachMainForm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTeachMainForm)

// Form Data
public:
	//{{AFX_DATA(CTeachMainForm)
	enum { IDD = IDD_TEACH_MAIN_FORM };
	CBtnEnh	m_lblFixedGroup;
	CBtnEnh	m_lblOffsetGroup;
	CBtnEnh	m_lblOffsetDirectInputGroup;
	//}}AFX_DATA

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeachMainForm)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush	m_brBckColor;
	
	virtual ~CTeachMainForm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CTeachMainForm)
	afx_msg void OnPaint();
	afx_msg void OnPanelAlignerFix();
	afx_msg void OnPanelAlignerModel();
	afx_msg void OnPanelAlignerOffset();
	afx_msg void OnTabCarrierFix();
	afx_msg void OnTabCarrierModel();
	afx_msg void OnTabFeederPressFix();
	afx_msg void OnTabFeederSupplierFix();
	afx_msg void OnTabFeederBadTabModel();
	afx_msg void OnTabMounterFix();
	afx_msg void OnTabMounterModel();
	afx_msg void OnInspectCamModel2();
	afx_msg void OnClickTabMounterFix2();
	afx_msg void OnClickTabMounterModel2();
	afx_msg void OnClickInspectCamFix1();
	afx_msg void OnClickInspectCamFix2();
	afx_msg void OnClickInspectCamModel1();
	afx_msg void OnClickTabCarrierFix2();
	afx_msg void OnClickTabCarrierModel2();
	afx_msg void OnClickTabFeederPressFix2();
	afx_msg void OnClickTabFeederBadtabModel2();
	afx_msg void OnClickPanelCamFix();
	afx_msg void OnClickPanelCamModel();
	afx_msg void OnClickModelChangeFix();
	afx_msg void OnClickModelChangeModel();
	afx_msg void OnTabFeederSupplierFix2();
	afx_msg LRESULT OnDisplayLanguage(UINT wParam, LPARAM lParam);
	afx_msg void OnClickPanelPressGetTOffset();
	afx_msg void OnClickTabCarrierGetLoadOffset();
	afx_msg void OnClickPanelTransferCenterFix();
	afx_msg void OnClickPanelTransferInFix();
	afx_msg void OnClickPanelTransferOutFix();
	afx_msg void OnClickPanelTransferCenterModel();
	afx_msg void OnClickPanelTransferInModel();
	afx_msg void OnClickPanelTransferOutModel();
	afx_msg void OnClickFrontWorkSchedule();
	afx_msg void OnClickRearWorkSchedule();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClickModelchange2Fix();
	afx_msg void OnClickModelchange2Model();
	afx_msg void OnClickFrontWorkSchedule2();
	afx_msg void OnClickRearWorkSchedule2();
	afx_msg void OnClickTabCarrierCamExpand();
	afx_msg void OnClickRtabCarrierCamExpand();
	afx_msg void OnClickTabCidbTurnHandlerFixarrierFix3();
	afx_msg void OnClickTurnHandlerModel();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL			m_bInitialized;

	MSystemData*	m_plnkSystemData;
	MProcessData*	m_plnkProcessData;
	MPanelData*		m_plnkPanelData;

	int getWorkOption(EWorkingSide* peWorkSide, int* piGroupNo, int* piTab);
	int operateTabICInspection(EWorkingSide eWorkSide, int iGroupNo, int iWorkTabNo, int iTargetStatus, MPos_XY* prgAlignOffset = NULL);
	int moveGateInspectionCameraToSafePosition(EWorkingSide eWorkSide, int iGroupNo);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEACHMAINFORM_H__157AEC42_85C6_44B5_9082_B8DB95789962__INCLUDED_)
