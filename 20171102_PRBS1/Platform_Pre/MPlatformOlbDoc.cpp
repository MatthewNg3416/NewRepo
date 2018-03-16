// MPlatformOlbDoc.cpp : implementation of the CMPlatformOlbDoc class
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "common.h"
#include "MPlatformOlbDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMPlatformOlbDoc

IMPLEMENT_DYNCREATE(CMPlatformOlbDoc, CDocument)

BEGIN_MESSAGE_MAP(CMPlatformOlbDoc, CDocument)
	//{{AFX_MSG_MAP(CMPlatformOlbDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMPlatformOlbDoc construction/destruction

CMPlatformOlbDoc::CMPlatformOlbDoc()
{
	// TODO: add one-time construction code here

}

CMPlatformOlbDoc::~CMPlatformOlbDoc()
{
}

BOOL CMPlatformOlbDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMPlatformOlbDoc serialization

void CMPlatformOlbDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMPlatformOlbDoc diagnostics

#ifdef _DEBUG
void CMPlatformOlbDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMPlatformOlbDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMPlatformOlbDoc commands
