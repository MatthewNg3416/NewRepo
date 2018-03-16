#if !defined(AFX_MDISPLAY_H__162F4106_E0FB_49A2_A69B_0D10460BE66A__INCLUDED_)
#define AFX_MDISPLAY_H__162F4106_E0FB_49A2_A69B_0D10460BE66A__INCLUDED_

// Mdisplay.h : header file
//
#include "DefVision.h"

/////////////////////////////////////////////////////////////////////////////
// Mdisplay

class Mdisplay
{
private:
	SDisplayAttributeData m_sData;	// Display Attributes Data
	CWinThread* m_pThread;	// UI Thread Pointer

public:
	Mdisplay();
	virtual ~Mdisplay();

	int CreateDisplay();
	int GetViewID();
	int GetDisplayPriority();
	HWND GetDispWndHandle();
	const SDisplayAttributeData& GetDisplayAttrib();
	void SetDisplayAttrib(SDisplayAttributeData* pData);
};

#endif // !defined(AFX_MDISPLAY_H__162F4106_E0FB_49A2_A69B_0D10460BE66A__INCLUDED_)
