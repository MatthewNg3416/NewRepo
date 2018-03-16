#if !defined(AFX_PAGETABMOUNTERTACT_H__51166584_5CF5_4336_9F40_85F306D7116D__INCLUDED_)
#define AFX_PAGETABMOUNTERTACT_H__51166584_5CF5_4336_9F40_85F306D7116D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageTabMounterTact.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageTabMounterTact dialog
class MTrsAutoManager;

class CPageTabMounterTact : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageTabMounterTact)
private:
	HWND m_hAutoMainWnd;
	MTrsAutoManager* m_plnkAutoManager;

public:
	void SetAutoMainWnd(HWND hMainView);
	void RenewTabMounterTactTime();
	void ClearDisplay();
	void UpdateDisplay();

// Construction
public:
	CPageTabMounterTact();
	~CPageTabMounterTact();

// Dialog Data
	//{{AFX_DATA(CPageTabMounterTact)
	enum { IDD = IDD_PAGE_TAB_MOUNTER_TACT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageTabMounterTact)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageTabMounterTact)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGETABMOUNTERTACT_H__51166584_5CF5_4336_9F40_85F306D7116D__INCLUDED_)
