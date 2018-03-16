#if !defined(AFX_PAGEETCTACT_H__1DA771F3_C74C_4A77_9857_9B410F8AFF44__INCLUDED_)
#define AFX_PAGEETCTACT_H__1DA771F3_C74C_4A77_9857_9B410F8AFF44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageEtcTact.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageEtcTact dialog
class MTrsAutoManager;

class CPageEtcTact : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageEtcTact)

private:
	HWND m_hAutoMainWnd;
	MTrsAutoManager* m_plnkAutoManager;

public:
	void SetAutoMainWnd(HWND hMainView);
	void RenewEtcTactTime();
	void ClearDisplay();
	void UpdateDisplay();
// Construction
public:
	CPageEtcTact();
	~CPageEtcTact();

// Dialog Data
	//{{AFX_DATA(CPageEtcTact)
	enum { IDD = IDD_PAGE_ETC_TACT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageEtcTact)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageEtcTact)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEETCTACT_H__1DA771F3_C74C_4A77_9857_9B410F8AFF44__INCLUDED_)
