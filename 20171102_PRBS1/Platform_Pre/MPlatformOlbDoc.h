// MPlatformOlbDoc.h : interface of the CMPlatformOlbDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPlatformOlbDOC_H__79AE359F_5009_4040_8295_7EB3BC61C5D8__INCLUDED_)
#define AFX_MPlatformOlbDOC_H__79AE359F_5009_4040_8295_7EB3BC61C5D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMPlatformOlbDoc : public CDocument
{
protected: // create from serialization only
	CMPlatformOlbDoc();
	DECLARE_DYNCREATE(CMPlatformOlbDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMPlatformOlbDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMPlatformOlbDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMPlatformOlbDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MPlatformOlbDOC_H__79AE359F_5009_4040_8295_7EB3BC61C5D8__INCLUDED_)
