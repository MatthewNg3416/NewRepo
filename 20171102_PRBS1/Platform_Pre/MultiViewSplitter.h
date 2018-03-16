// MultiViewSplitter.h: interface for the CMultiViewSplitter class.
//
// Written by Caroline Englebienne of AniWorld, Inc.
// Copyright (c) 2000 AniWorld, Inc. 
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_CMultiViewSplitter_H__464C08E9_8989_11D4_B4E3_005004D85AA1__INCLUDED_)
#define AFX_CMultiViewSplitter_H__464C08E9_8989_11D4_B4E3_005004D85AA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include <map>
using namespace std;

class CMultiViewSplitter : public CSplitterWnd  
{

public:
	int GetCurrentViewID();
	CMultiViewSplitter();
	virtual ~CMultiViewSplitter();
	int AddView(int nRow, int nCol, CRuntimeClass * pViewClass, 
				CCreateContext* pContext);
	void ShowView(int nViewID);
	CWnd* GetView(int nViewID);
	int		HitTest(CPoint pt) const;

protected:
	map<int, long> m_mapViewPane;
	map<long, int> m_mapCurrentViews;
	map<int, CWnd*> m_mapIDViews;

	int m_nIDCounter;

	CWnd* GetCurrentView(int nRow, int nCol, int * nCurID);
	void SetCurrentView(int nRow, int nCol, int nViewID);
	int HideCurrentView(int nRow, int nCol);
	void GetPaneFromViewID(int nViewID, CPoint * pane);
	void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);
}; 

#endif // !defined(AFX_CMultiViewSplitter_H__464C08E9_8989_11D4_B4E3_005004D85AA1__INCLUDED_)
