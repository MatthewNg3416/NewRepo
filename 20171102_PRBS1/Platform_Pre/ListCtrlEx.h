#if !defined(AFX_LISTCTRLEX_H__48D7938D_79E0_4BAD_9DCB_AAE83C6EDCDF__INCLUDED_)
#define AFX_LISTCTRLEX_H__48D7938D_79E0_4BAD_9DCB_AAE83C6EDCDF__INCLUDED_

#include "XHeaderCtrl.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListCtrlEx.h : header file
//

struct LISTCTRLEXDATA
{
	// ctor
	LISTCTRLEXDATA()
	{
		bEnabled             = TRUE;
		bBold                = FALSE;
		bModified            = FALSE;
		nImage               = -1;
		strToolTip           = _T("");
		bCombo               = FALSE;
		bSort                = FALSE;
		psa                  = NULL;
		nComboListHeight     = 10;
		nInitialComboSel     = -1;	// use default
		bEdit                = FALSE;
		crText               = ::GetSysColor(COLOR_WINDOWTEXT);
		crBackground         = ::GetSysColor(COLOR_WINDOW);

		bShowProgress        = FALSE;
		nProgressPercent     = 0;
		strProgressMessage   = _T("");
		bShowProgressMessage = TRUE;
		nCheckedState        = -1;
		dwItemData           = 0;
	}

	BOOL			bEnabled;				// TRUE = enabled, FALSE = disabled (gray text)
	BOOL			bBold;					// TRUE = display bold text
	BOOL			bModified;				// TRUE = subitem has been modified
	int				nImage;					// index in image list, else -1
	CString			strToolTip;				// tool tip text for cell

	// for combo
	BOOL			bCombo;					// TRUE = display combobox
	BOOL			bSort;					// TRUE = add CBS_SORT style;  this means
											// that the list will be re-sorted on
											// each AddString()
	CStringArray *	psa;					// pointer to string array for combo listbox
	int				nComboListHeight;		// combo listbox height (in rows)
	int				nInitialComboSel;		// initial combo listbox selection (0 = first)

	//for edit
	BOOL		bEdit;						// true = enable editing

	// for color
	COLORREF	crText;
	COLORREF	crBackground;

	// for progress
	BOOL		bShowProgress;				// true = show progress control
	int			nProgressPercent;			// 0 - 100
	CString		strProgressMessage;			// custom message for progress indicator -
											// MUST INCLUDE %d
	BOOL		bShowProgressMessage;		// TRUE = display % message, or custom message
											// if one is supplied
	// for checkbox
	int			nCheckedState;				// -1 = don't show, 0 = unchecked, 1 = checked

	DWORD		dwItemData;					// pointer to app's data
};


/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx window

class CListCtrlEx : public CListCtrl
{
// Construction
public:
	CListCtrlEx();
	CXHeaderCtrl	m_HeaderCtrl;

// Attributes
public:

// Operations
public:
	int		InsertItem(int nItem, LPCTSTR lpszItem);
	int		InsertItem(int nItem, 
					   LPCTSTR lpszItem, 
					   COLORREF crText, 
					   COLORREF crBackground);
	int		InsertItem(const LVITEM* pItem);

	BOOL	SetItemText(int nItem, int nSubItem, LPCTSTR lpszText); 
	BOOL	SetItemText(int nItem, 
						int nSubItem, 
						LPCTSTR lpszText,
						COLORREF crText, 
						COLORREF crBackground);
	int		SetItem(const LVITEM* pItem);

	void	SetItemColors(int nItem,
						  int nSubItem, 
						  COLORREF crText, 
						  COLORREF crBackground);
	int		GetColumns();
	BOOL	GetSubItemRect(int iItem, int iSubItem, int nArea, CRect& rect);
	void	UpdateSubItem(int nItem, int nSubItem);
	void	SubclassHeaderControl();

	void	GetDrawColors(int nItem,
					   int nSubItem,
					   COLORREF& colorText,
					   COLORREF& colorBkgnd);

	int		DrawImage(int nItem, 
				  int nSubItem, 
				  CDC* pDC, 
				  COLORREF crText,
				  COLORREF crBkgnd,
				  CRect rect,
				  LISTCTRLEXDATA *pXLCD);

	void	DrawText(int nItem, 
				  int nSubItem, 
				  CDC *pDC, 
				  COLORREF crText,
				  COLORREF crBkgnd,
				  CRect& rect, 
				  LISTCTRLEXDATA *pCLD);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlEx)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListCtrlEx();

	// Generated message map functions
protected:
	COLORREF		m_cr3DFace;
	COLORREF		m_cr3DHighLight;
	COLORREF		m_cr3DShadow;
	COLORREF		m_crActiveCaption;
	COLORREF		m_crBtnFace;
	COLORREF		m_crBtnShadow;
	COLORREF		m_crBtnText;
	COLORREF		m_crGrayText;
	COLORREF		m_crHighLight;
	COLORREF		m_crHighLightText;
	COLORREF		m_crInactiveCaption;
	COLORREF		m_crInactiveCaptionText;
	COLORREF		m_crWindow;
	COLORREF		m_crWindowText;

	BOOL			m_bHeaderIsSubclassed;

	//{{AFX_MSG(CListCtrlEx)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLEX_H__48D7938D_79E0_4BAD_9DCB_AAE83C6EDCDF__INCLUDED_)
