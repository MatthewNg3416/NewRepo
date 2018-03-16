// MPlatformOlbView.cpp : implementation of the CMPlatformOlbView class
//

#include "stdafx.h"
#include "MPlatformOlb.h"

#include "MPlatformOlbDoc.h"
#include "MPlatformOlbView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMPlatformOlbView

IMPLEMENT_DYNCREATE(CMPlatformOlbView, CView)

BEGIN_MESSAGE_MAP(CMPlatformOlbView, CView)
	//{{AFX_MSG_MAP(CMPlatformOlbView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMPlatformOlbView construction/destruction

CMPlatformOlbView::CMPlatformOlbView()
{
	// TODO: add construction code here

}

CMPlatformOlbView::~CMPlatformOlbView()
{
}

BOOL CMPlatformOlbView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMPlatformOlbView drawing

void CMPlatformOlbView::OnDraw(CDC* pDC)
{
	CMPlatformOlbDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CMPlatformOlbView printing

BOOL CMPlatformOlbView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMPlatformOlbView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMPlatformOlbView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMPlatformOlbView diagnostics

#ifdef _DEBUG
void CMPlatformOlbView::AssertValid() const
{
	CView::AssertValid();
}

void CMPlatformOlbView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMPlatformOlbDoc* CMPlatformOlbView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMPlatformOlbDoc)));
	return (CMPlatformOlbDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMPlatformOlbView message handlers
