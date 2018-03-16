#include "stdafx.h"
#include "MdisplayWnd.h"

extern int g_iDisplayReferenceLine;

MdisplayWnd::MdisplayWnd() : m_iViewSizeX(0), m_iViewSizeY(0)
{
}

MdisplayWnd::~MdisplayWnd()
{
}

void MdisplayWnd::TransferEvent(HANDLE hEvent)
{
	m_hCreateEvent = hEvent;
}

LRESULT MdisplayWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	HDC hDC;
	HDC hMemDC;
	PAINTSTRUCT ps;
	HBITMAP hBitmap;
	HBITMAP hOldBitmap;
	
	switch (message)
	{
	case WM_CREATE:
		{
			RECT rectView;
			this->GetWindowRect(&rectView);
			m_iViewSizeX = rectView.right - rectView.left;
			m_iViewSizeY = rectView.bottom - rectView.top;
			m_Bitmap.CreateCompatibleBitmap(this->GetDC(), m_iViewSizeX, m_iViewSizeY);
		}
		// 확인 필요 : 이게 더 의미 있나? 아래 코드가 더 의미 있나? 어차피 안되긴 마찬가지...
		::SetEvent(m_hCreateEvent);
		return 0;

	case WM_MOVE:
		if (::WaitForSingleObject(m_hCreateEvent, 0) != WAIT_OBJECT_0)
			::SetEvent(m_hCreateEvent);
		break;

	case WM_DISPLAY_SOURCE_IMAGE:
		draw((SCameraData*)wParam);
		return 0;

	case WM_PAINT:
		hDC = ::BeginPaint(m_hWnd, &ps);
		hMemDC = ::CreateCompatibleDC(hDC);
		hBitmap = (HBITMAP)m_Bitmap.GetSafeHandle();
		hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hBitmap);
		::SetStretchBltMode(hMemDC, HALFTONE);
		::BitBlt(hDC, 0, 0, m_iViewSizeX, m_iViewSizeY, hMemDC, 0, 0, SRCCOPY);
		::SelectObject(hMemDC, hOldBitmap);
		::DeleteDC(hMemDC);
		::EndPaint(m_hWnd, &ps);
		return 0;

	case WM_DESTROY:
		m_Bitmap.DeleteObject();
		::PostQuitMessage(0);
		return 0;
	}
	
	return CButton::WindowProc(message, wParam, lParam);
}

void MdisplayWnd::draw(SCameraData* const pData)
{
	HDC hDC = ::GetDC(this->m_hWnd);
	HDC hMemDC = ::CreateCompatibleDC(hDC);

	HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, m_Bitmap.GetSafeHandle());

	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hMemDC, GetStockObject(HOLLOW_BRUSH));

	HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	HPEN hRedPen = ::CreatePen(PS_DOT, 1, RGB(255, 0, 0));//110214.Add
	HPEN hOldPen = (HPEN)::SelectObject(hMemDC, hPen);

	::SetBkMode(hMemDC, TRANSPARENT);
//	::SetTextColor(hMemDC, RGB(0x00, 0xff, 0x00));
	::SetTextColor(hMemDC, RGB(0xff, 0x00, 0x00));
	//SetTextAlign(hMemDC, TA_CENTER);

	::SetStretchBltMode(hMemDC, HALFTONE);

	// Camera 번호에 해당하는 Grab Buffer Size 를 받아야 한다.
	int iWidth = pData->m_lBufferSizeX;
	int iHeight = pData->m_lBufferSizeY;

	EnterCriticalSection(&pData->m_csSource);
	HDC hSrcDC;
	MbufControl(pData->m_lSource, M_WINDOW_DC_ALLOC, M_DEFAULT);
#ifndef DEF_MIL_VER_90
	MbufInquire(pData->m_lSource, M_WINDOW_DC, &hSrcDC);
#else
	hSrcDC = ((HDC)MbufInquire(pData->m_lSource, M_WINDOW_DC, M_NULL));
#endif

	::StretchBlt(hMemDC, 0, 0, m_iViewSizeX, m_iViewSizeY, hSrcDC, 0, 0, iWidth, iHeight, SRCCOPY);
	MbufControl(pData->m_lSource, M_WINDOW_DC_FREE, M_DEFAULT);
	LeaveCriticalSection(&pData->m_csSource);

	// Draw Object Display Function Calls

	// Zoom Factor 계산.
	double dZoomFactorX = static_cast<double>(m_iViewSizeX) / static_cast<double>(iWidth);
	double dZoomFactorY = static_cast<double>(m_iViewSizeY) / static_cast<double>(iHeight);

	// Draw Center Line
	::MoveToEx(hMemDC, 0, m_iViewSizeY / 2, NULL);	::LineTo(hMemDC, m_iViewSizeX, m_iViewSizeY / 2);
	::MoveToEx(hMemDC, m_iViewSizeX / 2, 0, NULL);	::LineTo(hMemDC, m_iViewSizeX / 2, m_iViewSizeY);

	if (TRUE == g_iDisplayReferenceLine)
	{
		::MoveToEx(hMemDC, 0, m_iViewSizeY / 4, NULL);	::LineTo(hMemDC, m_iViewSizeX, m_iViewSizeY / 4);
		::MoveToEx(hMemDC, m_iViewSizeX / 4, 0, NULL);	::LineTo(hMemDC, m_iViewSizeX / 4, m_iViewSizeY);
		::MoveToEx(hMemDC, m_iViewSizeX * 3 / 4, 0, NULL);	::LineTo(hMemDC, m_iViewSizeX * 3 / 4, m_iViewSizeY);
		::MoveToEx(hMemDC, 0, m_iViewSizeY * 3 / 4, NULL);	::LineTo(hMemDC, m_iViewSizeX, m_iViewSizeY * 3 / 4);
		
		::MoveToEx(hMemDC, 0, m_iViewSizeY * 3 / 8, NULL);	::LineTo(hMemDC, m_iViewSizeX, m_iViewSizeY * 3 / 8);
		
		::MoveToEx(hMemDC, m_iViewSizeX * 1 / 8, 0, NULL);	::LineTo(hMemDC, m_iViewSizeX * 1 / 8, m_iViewSizeY);
		::MoveToEx(hMemDC, m_iViewSizeX * 7 / 8, 0, NULL);	::LineTo(hMemDC, m_iViewSizeX * 7 / 8, m_iViewSizeY);
	}

	// Draw Text
	int iPointX = 10;
	int iPointY = 10;
	WCHAR szCamNo[10];
//	swprintf(szCamNo, _T("Cam%2d"), pData->m_iUserCameraNo + 1);
	swprintf(szCamNo, _T("Cam%2d"), pData->m_iUserCameraNo + 1);
	::TextOut(hMemDC, iPointX, iPointY, szCamNo, 5);

	for (int i = 0; i <= pData->m_iTextIndex; i++)
	{
		::TextOutA(hMemDC, iPointX, iPointY + 20 * (i + 1), pData->m_rgszText[i], lstrlenA(pData->m_rgszText[i]));
	}
	
	// Draw Rectangle
	for (int i = 0; i <= pData->m_iRectIndex; i++)
	{
		int iLeft = static_cast<int>(pData->m_rgdRectLeft[i] * dZoomFactorX);
		int iTop = static_cast<int>(pData->m_rgdRectTop[i] * dZoomFactorX);
		int iRight = static_cast<int>(pData->m_rgdRectRight[i] * dZoomFactorY);
		int iBottom = static_cast<int>(pData->m_rgdRectBottom[i] * dZoomFactorY);

		::MoveToEx(hMemDC, iLeft, iTop, NULL);		::LineTo(hMemDC, iRight, iTop);
		::MoveToEx(hMemDC, iRight, iTop, NULL);		::LineTo(hMemDC, iRight, iBottom);
		::MoveToEx(hMemDC, iLeft, iBottom, NULL);	::LineTo(hMemDC, iRight, iBottom);
		::MoveToEx(hMemDC, iLeft, iTop, NULL);		::LineTo(hMemDC, iLeft, iBottom);
	}

	// Draw Point
	for (int i = 0; i <= pData->m_iPointIndex; i++)
	{
		int iCenterX = static_cast<int>(pData->m_rgdPointX[i] * dZoomFactorX);
		int iCenterY = static_cast<int>(pData->m_rgdPointY[i] * dZoomFactorY);
		
		::MoveToEx(hMemDC, iCenterX - 20, iCenterY, NULL);	::LineTo(hMemDC, iCenterX + 20, iCenterY);
		::MoveToEx(hMemDC, iCenterX, iCenterY - 20, NULL);	::LineTo(hMemDC, iCenterX, iCenterY + 20);
	}

	//110214.Add_____
	::SelectObject(hMemDC, hRedPen);
	for (int i = 0; i <= pData->m_iPointIndex_Measure; i++)
	{
		int iCenterX = static_cast<int>(pData->m_rgdPointX_Measure[i] * dZoomFactorX);
		int iCenterY = static_cast<int>(pData->m_rgdPointY_Measure[i] * dZoomFactorY);
		
		::MoveToEx(hMemDC, iCenterX - 30, iCenterY, NULL);	::LineTo(hMemDC, iCenterX + 30, iCenterY);
		::MoveToEx(hMemDC, iCenterX, iCenterY - 30, NULL);	::LineTo(hMemDC, iCenterX, iCenterY + 30);
	}

	for (int i = 0; i <= pData->m_iLineIndex_Measure; i++)
	{
		int iSX = static_cast<int>(pData->m_rgdLineStartX_Measure[i] * dZoomFactorX);
		int iSY = static_cast<int>(pData->m_rgdLineStartY_Measure[i] * dZoomFactorY);
		int iEX = static_cast<int>(pData->m_rgdLineEndX_Measure[i] * dZoomFactorX);
		int iEY = static_cast<int>(pData->m_rgdLineEndY_Measure[i] * dZoomFactorY);

		::MoveToEx(hMemDC, iSX, iSY, NULL);				::LineTo(hMemDC, iEX, iEY);
	}
	//_______________

	::SelectObject(hMemDC, hOldBitmap);
	::SelectObject(hMemDC, hOldBrush);
	::SelectObject(hMemDC, hOldPen);
	::DeleteObject(hPen);
	::DeleteObject(hRedPen);//110214.Add
	::DeleteDC(hMemDC);
	::ReleaseDC(this->m_hWnd, hDC);

	::InvalidateRgn(this->m_hWnd, NULL, FALSE);
}
