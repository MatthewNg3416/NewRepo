//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_VISIONBLOBTHRESHOLDDLG_H__18D74CD7_E8FB_4449_8292_7EC07D3153E5__INCLUDED_)
#define AFX_VISIONBLOBTHRESHOLDDLG_H__18D74CD7_E8FB_4449_8292_7EC07D3153E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VisionBlobThresholdDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVisionBlobThresholdDlg dialog
class IVision;

class CVisionBlobThresholdDlg : public CDialog
{
// Construction
public:
	CVisionBlobThresholdDlg(IVision* plnkVision, int iCameraNo, int iModelNo, int* piReturn, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVisionBlobThresholdDlg)
	enum { IDD = IDD_VISION_BLOB_THRESHOLD };
	CBtnEnh	m_ctrlLblThreshold;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisionBlobThresholdDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void drawHistogram(int iThreshold);

	// Generated message map functions
	//{{AFX_MSG(CVisionBlobThresholdDlg)
	afx_msg void OnClickMinus10();
	afx_msg void OnClickMinus1();
	afx_msg void OnClickPlus1();
	afx_msg void OnClickPlus10();
	afx_msg void OnClickSave();
	afx_msg void OnClickExit();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	IVision* m_plnkVision;
	int m_iCameraNo;
	int m_iModelNo;
	int* m_piReturn;
	unsigned char m_rgucHistogram[256][256];
	int m_iThreshold;
	double m_dMeanValue;
	BOOL m_bTerminateDlg;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISIONBLOBTHRESHOLDDLG_H__18D74CD7_E8FB_4449_8292_7EC07D3153E5__INCLUDED_)
