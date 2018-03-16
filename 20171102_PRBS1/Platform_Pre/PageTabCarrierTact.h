#if !defined(AFX_PAGETABCARRIERTACT_H__E10F3B69_3ED9_4914_B84A_B71BB6FABA45__INCLUDED_)
#define AFX_PAGETABCARRIERTACT_H__E10F3B69_3ED9_4914_B84A_B71BB6FABA45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageTabCarrierTact.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageTabCarrierTact dialog
class MTrsAutoManager;

class CPageTabCarrierTact : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageTabCarrierTact)
private:
	HWND m_hAutoMainWnd;
	MTrsAutoManager* m_plnkAutoManager;

public:
	void SetAutoMainWnd(HWND hMainView);
	void RenewTabCarrierTactTime();
	void ClearDisplay();
	void UpdateDisplay();
// Construction
public:
	CPageTabCarrierTact();
	~CPageTabCarrierTact();

// Dialog Data
	//{{AFX_DATA(CPageTabCarrierTact)
	enum { IDD = IDD_PAGE_TAB_CARRIER_TACT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageTabCarrierTact)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageTabCarrierTact)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGETABCARRIERTACT_H__E10F3B69_3ED9_4914_B84A_B71BB6FABA45__INCLUDED_)
