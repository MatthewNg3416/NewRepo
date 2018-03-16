//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_VISIONJOGKEYDLG_H__1B72F2F5_81F2_4F58_884C_F4C1ABF55BD6__INCLUDED_)
#define AFX_VISIONJOGKEYDLG_H__1B72F2F5_81F2_4F58_884C_F4C1ABF55BD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VisionJogKeyDlg.h : header file
//

/************************************************************************/
/*                             Value Define                             */
/************************************************************************/

/** 사용하는 Object Type 설정 */
const int	DEF_RECTANGLE		= 0;
const int	DEF_POINT			= 1;
//110214.Add_____
const int	DEF_POINT_MEASURE	= 2;
const int	DEF_LINE_VER		= 3;
const int	DEF_LINE_HOR		= 4;
//_______________

/** Keyboard I/F  */
#define	VP_LEFT					0x01
#define	VP_RIGHT				0x02
#define	VP_UP					0x04
#define	VP_DOWN					0x08

#define	VP_FAST_LEFT			0x61
#define	VP_FAST_RIGHT			0x73
#define	VP_FAST_UP				0x77
#define	VP_FAST_DOWN			0x7a

#define	VP_LBUTTON				0x10
#define	VP_CBUTTON				0x20
#define	VP_RBUTTON				0x40

#define JOGKEY_NONE				0
#define JOGKEY_UP				1
#define JOGKEY_DOWN				2
#define JOGKEY_LEFT				3
#define	JOGKEY_RIGHT			4

#define JOGKEY_FAST_UP			11
#define JOGKEY_FAST_DOWN		12
#define JOGKEY_FAST_LEFT		13
#define	JOGKEY_FAST_RIGHT		14

/////////////////////////////////////////////////////////////////////////////
// CVisionJogKeyDlg dialog

class IVision;
class IIO;
class MManageOpPanel;

class CVisionJogKeyDlg : public CDialog
{
private:
	IIO* m_plnkIO;
	IVision* m_plnkIVision;
	MManageOpPanel*	m_pC_MngOpPanel;

	int m_iOldJogID;

	CString m_strMsg;

	int m_iObjectType;
	CRect* m_plnkRect;
	CRect m_rectLimit;
	CPoint* m_plnkPoint;

	int m_iCurrentCam;

	UINT m_KeyTimerID;

	BOOL m_bTimerInitFlag;
	UINT m_vp_key;
	BOOL m_bOpMode;		// Move(FALSE) or Resize(TRUE)

	unsigned m_nJogKeyValue;
	unsigned m_nOldKeyValue;

	unsigned checkKey();
	unsigned getJogKey();
	int	traceBox();
	int tracePoint();
	//110214.Add_____
	int traceLineVer();
	int traceLineHor();
	//_______________

// Construction
public:
	CVisionJogKeyDlg(CWnd* pParent = NULL);   // standard constructor
	CVisionJogKeyDlg(IVision* plnkVision, int iCamNo, CString strMsg, CRect rectLimit, CRect* pRect, CWnd* pParent = NULL);
	CVisionJogKeyDlg(IVision* plnkVision, int iCamNo, CString strMsg, CRect rectLimit, CPoint* pPoint, int iObjectType, CWnd* pParent = NULL);//110214.Add
	~CVisionJogKeyDlg();

	void Trace();

// Dialog Data
	//{{AFX_DATA(CVisionJogKeyDlg)
	enum { IDD = IDD_VISION_JOG_KEY };
	CBtnEnh	m_ctrlJogMsg;
	CBtnEnh	m_ctrlBtnLeft;
	CBtnEnh	m_ctrlBtnRight;
	CBtnEnh	m_ctrlBtnCenter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisionJogKeyDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

protected:
	CBrush		m_brBckColor;

	// Generated message map functions
	//{{AFX_MSG(CVisionJogKeyDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickJogUp();
	afx_msg void OnClickJogCenterBtn();
	afx_msg void OnClickJogLeft();
	afx_msg void OnClickJogRight();
	afx_msg void OnClickJogDown();
	afx_msg void OnClickJogLeftBtn();
	afx_msg void OnClickJogRightBtn();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISIONJOGKEYDLG_H__1B72F2F5_81F2_4F58_884C_F4C1ABF55BD6__INCLUDED_)
