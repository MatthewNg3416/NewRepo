// MLCNetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MLCNetDlg.h"

#include "common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MLCNetDlg dialog


MLCNetDlg::MLCNetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(MLCNetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(MLCNetDlg)
	//}}AFX_DATA_INIT
	//110629.KKY_____
	CTime		curTime = CTime::GetCurrentTime();
	m_iCurTime = curTime.GetDay();
	//_______________
}


void MLCNetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MLCNetDlg)
	DDX_Control(pDX, IDC_EDIT_IP, m_editIP);
	DDX_Control(pDX, IDC_EDIT_PORT, m_editPort);
	DDX_Control(pDX, IDC_LOG_LIST, m_cLog);
	DDX_Control(pDX, IDC_XNET, m_cXNet);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_BTN_START, m_btnStart);
	DDX_Control(pDX, IDC_BTN_STOP, m_btnStop);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MLCNetDlg, CDialog)
	//{{AFX_MSG_MAP(MLCNetDlg)
	ON_MESSAGE(WMU_RCV_SEND_MSG, OnRecvSendMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MLCNetDlg message handlers

BEGIN_EVENTSINK_MAP(MLCNetDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(MLCNetDlg)
	ON_EVENT(MLCNetDlg, IDC_XNET, 3 /* CommClose */, OnCommCloseXnet, VTS_NONE)
	ON_EVENT(MLCNetDlg, IDC_XNET, 2 /* CommEstablish */, OnCommEstablishXnet, VTS_NONE)
	ON_EVENT(MLCNetDlg, IDC_XNET, 1 /* Receive */, OnReceiveXnet, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(MLCNetDlg, IDC_BTN_START, -600 /* Click */, OnClickBtnStart, VTS_NONE)
	ON_EVENT(MLCNetDlg, IDC_BTN_STOP, -600 /* Click */, OnClickBtnStop, VTS_NONE)
	ON_EVENT(MLCNetDlg, IDC_BTN_CLOSE, -600 /* Click */, OnClickBtnClose, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/*========================================================================================
	FUNCTION : MLCNetDlg::OnCommCloseXnet()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/08/10, KJH
	UPDATE	 : 2004/09/23, KJH
========================================================================================*/
void MLCNetDlg::OnCommCloseXnet() 
{
	CString szLog;

	m_pMLCNet->m_bConnected = false;

	szLog.Format(_T("XNet Disconnected"));
	LogTrace(0, szLog);

	m_btnStart.EnableWindow(TRUE);
	m_btnStop.EnableWindow(FALSE);
}

/*========================================================================================
	FUNCTION : MLCNetDlg::OnCommEstablishXnet()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/08/10, KJH
	UPDATE	 : 2004/09/23, KJH
========================================================================================*/
void MLCNetDlg::OnCommEstablishXnet() 
{
	CString szLog;

	m_pMLCNet->m_bConnected = true;

	szLog.Format(_T("XNet Connected"));
	LogTrace(0, szLog);

	m_btnStart.EnableWindow(FALSE);
	m_btnStop.EnableWindow(TRUE);
	
	m_pMLCNet->OnConnectionEvent();
}

/*========================================================================================
	FUNCTION : MLCNetDlg::OnReceiveXnet()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/08/10, KJH
	UPDATE	 : 2004/08/10, KJH
========================================================================================*/
void MLCNetDlg::OnReceiveXnet(long mesgType, long mesgID, long mesgLen) 
{
	CString sLog;			//Log Msg
	CString sMsg;			//Recv Msg Data
	CString sFormatType;	//Recv Msg Type

	//Get Recv Msg
	sMsg = m_cXNet.GetString(mesgID);
	//Log Trace
	sLog.Format(_T("XNet Recv,[%s]=RcvLen-%d,OrgLen-%d"), sMsg, mesgLen, sMsg.GetLength());
	LogTrace(0, sLog);
	//Msg Parsing Rule
	sFormatType = sMsg.Left(1);
	if(_T("@")==sFormatType)      // 자체적으로 정의한 메세지형식
	{
	  m_pMLCNet->AnaysisMessage_SelfFormat(sMsg);
	}
	else if(_T("<")==sFormatType) // XML 메세지 형식
	{
	  m_pMLCNet->AnaysisMessage_XMLFormat(sMsg);
	}
}

/*========================================================================================
	FUNCTION : MLCNetDlg::LogTrace()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/08/10, KJH
	UPDATE	 : 2004/08/10, KJH
========================================================================================*/
void MLCNetDlg::LogTrace(int nType, CString szLog)
{
	CString		sTimeStr="";

	CTime t = CTime::GetCurrentTime();
	sTimeStr.Format(_T("[%02d/%02d %02d:%02d:%02d] "), 
		t.GetMonth(), t.GetDay(), 
		t.GetHour(), t.GetMinute(), t.GetSecond());
	m_cLog.InsertString(0, sTimeStr + szLog);
	if(m_cLog.GetCount() > 200) {
		m_cLog.DeleteString(m_cLog.GetCount() - 1);
	}
	m_cLog.UpdateWindow();
	//110629.KKY_____
	//날짜가 바뀌면 오래된 파일을 지운다
	CString szlogFolder = _T("..\\Log\\Online");
	CTime	curTime = CTime::GetCurrentTime();
	int iCurTime = curTime.GetDay();
	if (m_iCurTime != iCurTime)
	{
		deleteOldFilesInFolder(szlogFolder, 30);
		m_iCurTime = iCurTime;
	}
	//csv형식으로 Log파일에 기록한다
	CString szLine;
	szLine.Format(_T("%s,%s"), sTimeStr, szLog);
	writeLog(szlogFolder, szLine);
	//_______________
}

/*========================================================================================
	FUNCTION : MLCNetDlg::OnBtnClose()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/08/10, KJH
	UPDATE	 : 2004/08/10, KJH
========================================================================================*/
void MLCNetDlg::OnClickBtnClose() 
{
	this->ShowWindow(SW_HIDE);
}

/*========================================================================================
	FUNCTION : MLCNetDlg::OnInitDialog()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/08/10, KJH
	UPDATE	 : 2004/09/23, KJH
========================================================================================*/
BOOL MLCNetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString		sPort="";

	m_btnStart.EnableWindow(FALSE);
	m_btnStop.EnableWindow(FALSE);
	
	m_editIP.SetWindowText(m_pMLCNet->m_szIP);
	sPort.Format(_T("%d"), m_pMLCNet->m_nPort);
	m_editPort.SetWindowText(sPort);

	m_cLog.ShowScrollBar(SB_BOTH, TRUE);
	m_cLog.EnableScrollBarCtrl(SB_HORZ, TRUE);
	m_cLog.EnableScrollBarCtrl(SB_VERT, TRUE);

	m_cLog.SetHorizontalExtent(4000);

	m_editIP.EnableWindow(FALSE);
	m_editPort.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*========================================================================================
	FUNCTION : MLCNet::Start()
	DESCRIPT : Start for xnet
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/23, KJH
	UPDATE	 : 2004/09/23, KJH
========================================================================================*/
void MLCNetDlg::OnClickBtnStart() 
{
	long	lRet;
	int		nPort;
	CString sLog, sIP, sPort;
	
	m_editIP.GetWindowText(sIP);
	m_editPort.GetWindowText(sPort);
	nPort = _wtoi(sPort);
	m_cXNet.SetIp((LPCTSTR)sIP);
	m_cXNet.SetPort(nPort);
//	m_cXNet.SetActive(bActive);
	lRet = m_cXNet.Start();
	if(!lRet){
		m_btnStart.EnableWindow(FALSE);
		m_btnStop.EnableWindow(TRUE);
		sLog.Format(_T("XNet Start=%d"), lRet);
	}
	else
		sLog.Format(_T("XNet Start=%d - %s"), lRet, (LPCTSTR)m_cXNet.GetLastErrorString());
	//110817 SJ_KJS
	m_pMLCNet->m_bConnected = TRUE;
	//______________________________-


	LogTrace(0, sLog);
}

/*========================================================================================
	FUNCTION : MLCNet::Stop()
	DESCRIPT : Stop for xnet
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/23, KJH
	UPDATE	 : 2004/09/23, KJH
========================================================================================*/
void MLCNetDlg::OnClickBtnStop() 
{
	long	lRet;
	CString sLog;

	if (MyMessageBox(_T("You can not send or receive all of the current production related information. Would you like to exit? \r\n현재 생산과 관련된 모든 사항을 상위로 전송할수 없거나 받을수 없습니다. \n 그래도 종료 하시겠습니까?"), _T("질문"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO)
		return;

	lRet = m_cXNet.Stop();
	if(!lRet){
		m_btnStart.EnableWindow(TRUE);
		m_btnStop.EnableWindow(FALSE);
	}

	sLog.Format(_T("XNet Stop=%d - %s"), lRet, (LPCTSTR)m_cXNet.GetLastErrorString());
	//110817 SJ_KJS
	m_pMLCNet->m_bConnected = FALSE;
	//_______________________________
	LogTrace(0, sLog);
}

/*========================================================================================
FUNCTION : MLCNet::OnRecvSendMsg()
DESCRIPT : 
RETURN	 : 
ARGUMENT :
UPDATE	 : 2007/11/07, GrouchyKim
========================================================================================*/
LRESULT MLCNetDlg::OnRecvSendMsg(WPARAM wParam, LPARAM lParam)
{
	CString sLog;
	CString *psMsg=NULL;
	CString szMsg="";
	long	lRet;
	
	//psMsg = (CString *)wParam;
	//szMsg = *psMsg;
	m_mutex.Lock();

	if(m_srgSendMsg.GetSize() > 0){
		szMsg = m_srgSendMsg.GetAt(0);
		m_srgSendMsg.RemoveAt(0);
	}

	m_mutex.Unlock();

	if(m_cXNet.GetEstablished())
	{
		lRet = m_cXNet.SendString((LPCTSTR)szMsg, szMsg.GetLength());
		if(0==lRet)
		{
			sLog.Format(_T("XNet Send,[%d] %s"), lRet, szMsg);
		}
		else
		{
			sLog.Format(_T("XNet Send,[%d] %s"), lRet, (LPCTSTR)m_cXNet.GetLastErrorString());
		}
	}
	else
	{
		lRet = -9999;
		sLog.Format(_T("XNet Not Send,[%d] %s"), lRet, (LPCTSTR)szMsg);
	}
	LogTrace(0, sLog);
	
	//if(psMsg)
	//{
	//	delete psMsg;
	//	psMsg=NULL;
	//}
	return 0;
}

/*========================================================================================
FUNCTION : MLCNetDlg::AddSendMsg()
DESCRIPT : 
RETURN	 : 
ARGUMENT :
UPDATE	 : 2007/11/07, GrouchyKim
========================================================================================*/
void MLCNetDlg::AddSendMsg(CString sMsg)
{
	m_mutex.Lock();

	m_srgSendMsg.Add(sMsg);

	m_mutex.Unlock();
}

//110629.KKY.Add_____
void MLCNetDlg::writeLog(CString strFolder, CString strLog)
{
	CFileFind ff;
	CString strPath;
	CTime t = CTime::GetCurrentTime();
	strPath.Format(_T("%s\\%s.csv"), strFolder, t.Format(_T("%y%m%d")));
	if (FALSE == ff.FindFile(strFolder, 0))
		CreateDirectory(strFolder, NULL);

	FILE* fp = _tfopen(strPath, _T("a+"));
	if (fp == NULL) 
	{
		return;	
	}
	CString strLine;
	strLine.Format(_T("%s\n"), strLog);
	_ftprintf(fp, strLine);
	fclose(fp);
}

void MLCNetDlg::deleteOldFilesInFolder(CString strFolder, int iKeepingDays)
{
	CFileFind	ff;
	int			nt;			// 남은 파일의 갯수
	CString		strFind;
	CTime		curTime = CTime::GetCurrentTime();
	CTime		createTime;
	// 파일을 검색하여 strDirName에 하나씩 집어 넣는다
	strFind.Format(_T("%s\\*.*"),strFolder);
	if( !ff.FindFile(strFind,0) ) return;
	nt = ff.FindNextFile();
	while(TRUE)
	{
		if(!ff.IsDirectory())
		{
			strFind = ff.GetFilePath();
			ff.GetLastWriteTime(createTime);
			if (curTime - createTime > CTimeSpan(iKeepingDays,0,0,0))
			{
				DeleteFile(strFind);
			}
		}
		if( !nt ) break;
		nt = ff.FindNextFile();
	}
}
//___________________
