#if !defined(AFX_VISION_DISPLAY_H__B0EF039A_9EAE_4543_9567_DE38CB0D5CB3__INCLUDED_)
#define AFX_VISION_DISPLAY_H__B0EF039A_9EAE_4543_9567_DE38CB0D5CB3__INCLUDED_

#include "stdafx.h"
#include "DefVision.h"

// Class Declaration
class MdisplayWnd : public CButton
{
	// Member Variables
private:
	CBitmap m_Bitmap;

	int m_iViewSizeX;	// Display View Window Size
	int m_iViewSizeY;

	HANDLE m_hCreateEvent;
	
	// Member Functions
private:
	// Draw Functions
	void draw(SCameraData* const pData);

public:
	MdisplayWnd();
	virtual ~MdisplayWnd();

	void TransferEvent(HANDLE hEvent);
	
	// Overrides
	//{{AFX_VIRTUAL(MdisplayWnd)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
};

#endif // !defined(AFX_VISION_DISPLAY_H__B0EF039A_9EAE_4543_9567_DE38CB0D5CB3__INCLUDED_)
