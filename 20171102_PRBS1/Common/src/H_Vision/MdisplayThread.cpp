#include "stdafx.h"
#include "MdisplayThread.h"
#include "MdisplayWnd.h"

/////////////////////////////////////////////////////////////////////////////
// MdisplayThread

IMPLEMENT_DYNCREATE(MdisplayThread, CWinThread)

MdisplayThread::MdisplayThread() : m_pDispWnd(NULL)
{
}

MdisplayThread::~MdisplayThread()
{
}

BOOL MdisplayThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	m_pDispWnd = new MdisplayWnd;
	m_pMainWnd = dynamic_cast<CWnd*>(m_pDispWnd);
	// 확인 필요 : id 랑 정리 필요.

	m_pDispWnd->TransferEvent(m_hCreateViewEvent);
	// 확인 필요 : 아래 코드 부분 return 전까지는 다른 Thread 작업에 영향 없이 한 번에 실행되어야만 한다.
	// 버튼 Create 하고 나서 바로 다른 Thread 의 코드들 실행되느라 InitInstance 를 완료하지 못하는 문제 발생.
	m_pDispWnd->Create(_T("view_frame"), BS_OWNERDRAW|WS_VISIBLE|WS_DISABLED, m_rectWndPos, m_pParentWnd, 1500 + m_iID);

	return TRUE;
}

int MdisplayThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	if (m_pDispWnd)
		delete m_pDispWnd;

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(MdisplayThread, CWinThread)
	//{{AFX_MSG_MAP(MdisplayThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void MdisplayThread::SetAttrib(SDisplayAttributeData* pData)
{
	m_iID = pData->m_iID;
	m_rectWndPos.left = pData->m_rectWndPos.left;
	m_rectWndPos.top = pData->m_rectWndPos.top;
	m_rectWndPos.right = pData->m_rectWndPos.right;
	m_rectWndPos.bottom = pData->m_rectWndPos.bottom;
	m_pParentWnd = pData->m_pParentWnd;
}
