#if !defined(AFX_MDISPLAYTHREAD_H__DDF8B781_E414_4F8A_BBFC_6D44B4FED7AA__INCLUDED_)
#define AFX_MDISPLAYTHREAD_H__DDF8B781_E414_4F8A_BBFC_6D44B4FED7AA__INCLUDED_

#include "DefVision.h"

class MdisplayWnd;

/////////////////////////////////////////////////////////////////////////////
// MdisplayThread thread

class MdisplayThread : public CWinThread
{
public:
	DECLARE_DYNCREATE(MdisplayThread)
protected:
	MdisplayThread();           // protected constructor used by dynamic creation

// Attributes
private:
	int m_iID;
	RECT m_rectWndPos;
	CWnd* m_pParentWnd;
	MdisplayWnd* m_pDispWnd;

public:
	HANDLE m_hCreateViewEvent;

// Operations
public:
	void SetAttrib(SDisplayAttributeData* pData);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MdisplayThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~MdisplayThread();

	// Generated message map functions
	//{{AFX_MSG(MdisplayThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDISPLAYTHREAD_H__DDF8B781_E414_4F8A_BBFC_6D44B4FED7AA__INCLUDED_)
