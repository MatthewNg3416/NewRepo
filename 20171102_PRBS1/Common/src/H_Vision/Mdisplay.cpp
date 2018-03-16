#include "stdafx.h"
#include "Mdisplay.h"
#include "MdisplayThread.h"

/////////////////////////////////////////////////////////////////////////////
// Mdisplay

Mdisplay::Mdisplay() : m_pThread(NULL)
{
	m_sData.m_iID = -1;
	m_sData.m_iPriority = DEF_NONE_PRIORITY;
	m_sData.m_pParentWnd = NULL;
	m_sData.m_rectWndPos.left = 0;
	m_sData.m_rectWndPos.top = 0;
	m_sData.m_rectWndPos.right = 0;
	m_sData.m_rectWndPos.bottom = 0;
}

Mdisplay::~Mdisplay()
{
	::CloseHandle(((MdisplayThread*)m_pThread)->m_hCreateViewEvent);

	// UI Thread 에게 WM_DESTROY message 보냄 : Thread 종료 처리.
	::SendMessage(m_pThread->m_pMainWnd->m_hWnd, WM_DESTROY, NULL, NULL);
}

void Mdisplay::SetDisplayAttrib(SDisplayAttributeData* pData)
{
	m_sData.m_iID = pData->m_iID;
	m_sData.m_iPriority = pData->m_iPriority;
	m_sData.m_rectWndPos.left = pData->m_rectWndPos.left;
	m_sData.m_rectWndPos.top = pData->m_rectWndPos.top;
	m_sData.m_rectWndPos.right = pData->m_rectWndPos.right;
	m_sData.m_rectWndPos.bottom = pData->m_rectWndPos.bottom;
	m_sData.m_pParentWnd = pData->m_pParentWnd;
}

const SDisplayAttributeData& Mdisplay::GetDisplayAttrib()
{
	return m_sData;
}

int Mdisplay::CreateDisplay()
{
	m_pThread = AfxBeginThread(RUNTIME_CLASS(MdisplayThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if (m_pThread)
	{
		((MdisplayThread*)m_pThread)->m_hCreateViewEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
		((MdisplayThread*)m_pThread)->SetAttrib(&m_sData);
		m_pThread->ResumeThread();
	}

	::WaitForSingleObject(((MdisplayThread*)m_pThread)->m_hCreateViewEvent, INFINITE);

	return ERR_VISION_SUCCESS;
}

int Mdisplay::GetViewID()
{
	return m_sData.m_iID;
}

int Mdisplay::GetDisplayPriority()
{
	return m_sData.m_iPriority;
}

HWND Mdisplay::GetDispWndHandle()
{
	return m_pThread->m_pMainWnd->m_hWnd;
}
