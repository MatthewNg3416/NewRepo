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
	// Ȯ�� �ʿ� : id �� ���� �ʿ�.

	m_pDispWnd->TransferEvent(m_hCreateViewEvent);
	// Ȯ�� �ʿ� : �Ʒ� �ڵ� �κ� return �������� �ٸ� Thread �۾��� ���� ���� �� ���� ����Ǿ�߸� �Ѵ�.
	// ��ư Create �ϰ� ���� �ٷ� �ٸ� Thread �� �ڵ�� ����Ǵ��� InitInstance �� �Ϸ����� ���ϴ� ���� �߻�.
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
