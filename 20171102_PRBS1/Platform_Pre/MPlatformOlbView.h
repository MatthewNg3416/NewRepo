// MPlatformOlbView.h : interface of the CMPlatformOlbView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPlatformOlbVIEW_H__586A7A1B_C7EF_47A5_9135_05FE59F987D8__INCLUDED_)
#define AFX_MPlatformOlbVIEW_H__586A7A1B_C7EF_47A5_9135_05FE59F987D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMPlatformOlbView : public CView
{
protected: // create from serialization only
	CMPlatformOlbView();
	DECLARE_DYNCREATE(CMPlatformOlbView)

// Attributes
public:
	CMPlatformOlbDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMPlatformOlbView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMPlatformOlbView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMPlatformOlbView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MPlatformOlbView.cpp
inline CMPlatformOlbDoc* CMPlatformOlbView::GetDocument()
   { return (CMPlatformOlbDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MPlatformOlbVIEW_H__586A7A1B_C7EF_47A5_9135_05FE59F987D8__INCLUDED_)
