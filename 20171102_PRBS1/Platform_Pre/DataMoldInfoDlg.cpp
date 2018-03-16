// MoldInformDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "DataMoldInfoDlg.h"
#include "MPlatformOlbSystem.h"
#include "MyMsgWnd.h"
#include "MRFID.h"

#include "atlconv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CDataMoldInfoDlg dialog

CDataMoldInfoDlg::CDataMoldInfoDlg(int iFeederSel, CWnd* pParent /*=NULL*/)
	: CDialog(CDataMoldInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataMoldInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);
	m_iFeederSel = iFeederSel;

	m_pRFID = MOlbSystemPre.GetRFIDComponent(m_iFeederSel);
	m_iZigInfoIndex = -1;
}


void CDataMoldInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataMoldInfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//DDX_Control(pDX, IDC_MSFLEXGRID1, m_FlexGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataMoldInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CDataMoldInfoDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataMoldInfoDlg message handlers

BEGIN_EVENTSINK_MAP(CDataMoldInfoDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDataMoldInfoDlg)
	ON_EVENT(CDataMoldInfoDlg, IDB_SELECT_ZIG_INFO, -600 /* Click */, OnClickSelectZigInfo, VTS_NONE)
	ON_EVENT(CDataMoldInfoDlg, IDB_WRITE_ZIG_INFO, -600 /* Click */, OnClickWriteZigInfo, VTS_NONE)
	ON_EVENT(CDataMoldInfoDlg, IDB_REFRESH_INFO, -600 /* Click */, OnClickRefreshInfo, VTS_NONE)
	ON_EVENT(CDataMoldInfoDlg, IDB_CANCEL, -600 /* Click */, OnClickCancel, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CDataMoldInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_iFeederSel == DEF_TABFEEDER1)
		{
			CString szTextTemp;
//			Feeder #1 ���� ����"
			ReadDisplayMessage(MAIN_DATA, _T("DataMoldInfoDlg_1"), &szTextTemp);
			SetWindowText(szTextTemp);
		}
	else 
		{
			CString szTextTemp;
//			Feeder #2 ���� ����
			ReadDisplayMessage(MAIN_DATA, _T("DataMoldInfoDlg_2"), &szTextTemp);
			SetWindowText(szTextTemp);
		}
	CString strTemp;
//	for (int i =1 ; i< (DEF_MAX_RFID_ITEM + 1) ; i++)
//	{
//		strTemp.Format(_T("%d"),i);
//		m_FlexGrid.SetTextMatrix(i, 0, strTemp);
//	}
//
//	m_FlexGrid.SetRow(0);
//	m_FlexGrid.SetCol(0); 
//	m_FlexGrid.SetText(_T("No."));
//	m_FlexGrid.SetCellFontBold(TRUE);
//
//	m_FlexGrid.SetCol(1); 
//	m_FlexGrid.SetText(_T("Item"));	
//	m_FlexGrid.SetCellFontBold(TRUE);
//
//	m_FlexGrid.SetCol(2); 
//	m_FlexGrid.SetText(_T("Value"));	
//	m_FlexGrid.SetCellFontBold(TRUE);
//
//	// ���� ������ --------------------------------------------------
//
	CString szTextTemp;
//	ReadDisplayMessage(MAIN_DATA, _T("DataMoldInfoDlg_3"), &szTextTemp);
////	���� ID
//	m_FlexGrid.SetTextMatrix(1, 1, szTextTemp);
//	ReadDisplayMessage(MAIN_DATA, _T("DataMoldInfoDlg_4"), &szTextTemp);
////	Regrinding Ƚ��
//	m_FlexGrid.SetTextMatrix(2, 1, szTextTemp);
//	ReadDisplayMessage(MAIN_DATA, _T("DataMoldInfoDlg_5"), &szTextTemp);
////	Regrinding ��¥
//	m_FlexGrid.SetTextMatrix(3, 1, szTextTemp);
//	ReadDisplayMessage(MAIN_DATA, _T("DataMoldInfoDlg_6"), &szTextTemp);
////	MATCODE
//	m_FlexGrid.SetTextMatrix(4, 1, szTextTemp);
//	ReadDisplayMessage(MAIN_DATA, _T("DataMoldInfoDlg_7"), &szTextTemp);
////	�� ���� Ƚ��
//	m_FlexGrid.SetTextMatrix(5, 1, szTextTemp);
//	ReadDisplayMessage(MAIN_DATA, _T("DataMoldInfoDlg_8"), &szTextTemp);
////	�� ��� Ƚ��
//	m_FlexGrid.SetTextMatrix(6, 1, szTextTemp);
//	ReadDisplayMessage(MAIN_DATA, _T("DataMoldInfoDlg_9"), &szTextTemp);
////	Regrinding �� ��� ���� Ƚ��
//	m_FlexGrid.SetTextMatrix(7, 1, szTextTemp);
//	ReadDisplayMessage(MAIN_DATA, _T("DataMoldInfoDlg_10"), &szTextTemp);
////	Regrinding �� ��� Ƚ��
//	m_FlexGrid.SetTextMatrix(8, 1, szTextTemp);
//

	//������������������������������������������������������������������������������������������������������
	// Multi Language Button Title Display                                                                ��
	//������������������������������������������������������������������������������������������������������
	ReadDisplayMessage(MAIN_DATA, _T("DataMoldInfoDlg_12"), &szTextTemp);
//	���� ����\r\n��ȣ ����
	GetDlgItem(IDB_SELECT_ZIG_INFO)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataMoldInfoDlg_13"), &szTextTemp);
//	���� ����\r\n���� ����
	GetDlgItem(IDB_WRITE_ZIG_INFO)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataMoldInfoDlg_14"), &szTextTemp);
//	���� ����\r\n�ٽ� �б�
	GetDlgItem(IDB_REFRESH_INFO)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataMoldInfoDlg_15"), &szTextTemp);
//	������
	GetDlgItem(IDB_CANCEL)->SetWindowText(szTextTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDataMoldInfoDlg::DisplayRFIDInformation()
{
	CString strTemp;
	//char cbuf[50];

	CMyMsgWnd wndMyMsg(this);
	wndMyMsg.SetMessage(_T("Read mold data..Wait a moment..n"));
	wndMyMsg.Show();
	wndMyMsg.PeekAndPump();

	//for (int i = 1; i <= (DEF_MAX_RFID_ITEM + 1); i++)
	//	m_FlexGrid.SetTextMatrix(i, 2, _T("")); 

	//memset(cbuf,0,sizeof(cbuf));
	//strTemp = m_pRFID->ReadData(ZIGID);
	//m_FlexGrid.SetTextMatrix(1, 2, strTemp.GetBuffer(strTemp.GetLength())); // ZIG ID
	//wndMyMsg.PeekAndPump();

	//memset(cbuf,0,sizeof(cbuf));
	//strTemp = m_pRFID->ReadData(REG_QTY);
	//m_FlexGrid.SetTextMatrix(2, 2, strTemp.GetBuffer(strTemp.GetLength()));
	//wndMyMsg.PeekAndPump();
	//
	//memset(cbuf,0,sizeof(cbuf));
	//strTemp = m_pRFID->ReadData(REG_DATE);
	//m_FlexGrid.SetTextMatrix(3, 2, strTemp.GetBuffer(strTemp.GetLength()));
	//wndMyMsg.PeekAndPump();

	//memset(cbuf,0,sizeof(cbuf));
	//strTemp = m_pRFID->ReadData(MATCODE);
	//m_FlexGrid.SetTextMatrix(4, 2, strTemp.GetBuffer(strTemp.GetLength()));
	//wndMyMsg.PeekAndPump();

	//memset(cbuf,0,sizeof(cbuf));
	//strTemp = m_pRFID->ReadData(TOT_QTY_ZIG);
	//m_FlexGrid.SetTextMatrix(5, 2, strTemp.GetBuffer(strTemp.GetLength()));
	//wndMyMsg.PeekAndPump();

	//memset(cbuf,0,sizeof(cbuf));
	//strTemp = m_pRFID->ReadData(ZIG_USED_QTY);
	//m_FlexGrid.SetTextMatrix(6, 2, strTemp.GetBuffer(strTemp.GetLength()));
	//wndMyMsg.PeekAndPump();

	//memset(cbuf,0,sizeof(cbuf));
	//strTemp = m_pRFID->ReadData(TOT_QTY_AFTER_REG);
	//m_FlexGrid.SetTextMatrix(7, 2, strTemp.GetBuffer(strTemp.GetLength()));
	//wndMyMsg.PeekAndPump();

	//memset(cbuf,0,sizeof(cbuf));
	//strTemp = m_pRFID->ReadData(REG_USED_QTY);
	//m_FlexGrid.SetTextMatrix(8,2 , strTemp.GetBuffer(strTemp.GetLength()));
	//wndMyMsg.PeekAndPump();

	if (strTemp == "Data Read Error")
	{
//		Data �б� ����. �۾�â�� �ٽ� ���� ��õ��� �ּ���.
		MyMessageBox(MY_MESSAGE_BOX, 40700, _T("Error"), M_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

void CDataMoldInfoDlg::OnClickSelectZigInfo() 
{
	WCHAR szGetBuf[10];
	memset(szGetBuf,0,sizeof(szGetBuf));
	if (!GetNumberBox(szGetBuf, 10, _T("-"))) 
		return;	

	if (_wtoi(szGetBuf) <= 0 || _wtoi(szGetBuf) > DEF_MAX_RFID_ITEM)
	{

		CString strMaxItemNum, strMsg;
		strMaxItemNum.Format(_T("%d]"), DEF_MAX_RFID_ITEM);
//		���� �߸� �Է��Ͽ����ϴ�.[1 ~ ]
		ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_40701"), &strMsg);
		strMsg += strMaxItemNum;
		MyMessageBox(strMsg, _T("Error"), M_ICONERROR);
		return ; 
	}
	if (6 == _wtoi(szGetBuf) || 8 == _wtoi(szGetBuf))
	{
//		���� Ÿ�� Ƚ���� �����Ͻ� �� �����ϴ�.
		MyMessageBox(MY_MESSAGE_BOX, 40702, _T("Error"), M_ICONERROR);
		return ; 
	}

	int iNewIndex = _wtoi(szGetBuf);
	int iOldIndex = m_iZigInfoIndex;
	//if (iNewIndex != iOldIndex && iOldIndex > 0 && iOldIndex <= DEF_MAX_RFID_ITEM)
	//{
	//	m_FlexGrid.SetRow(iOldIndex);
	//	for (int i = 0; i < 3; i++)
	//	{
	//		m_FlexGrid.SetCol(i);
	//		m_FlexGrid.SetCellBackColor(WHITE);
	//		m_FlexGrid.SetCellFontBold(FALSE);
	//	}
	//}

	if (iNewIndex == iOldIndex)
		return;

	m_iZigInfoIndex = iNewIndex;

	//m_FlexGrid.SetRow(iNewIndex);
	//for (int i = 0; i < 3; i++)
	//{
	//	m_FlexGrid.SetCol(i);
	//	m_FlexGrid.SetCellBackColor(YELLOW);
	//	m_FlexGrid.SetCellFontBold(TRUE);
	//}
}

void CDataMoldInfoDlg::OnClickWriteZigInfo() 
{
	// LKH_20081030 : Unicode ���� WCHAR�� char ȣȯ ������ ����
	//WCHAR strTemp[100];
	WCHAR strWTemp[100];
	char *strTemp;
	char strData[100];
	WCHAR buf[100];
	int iResult;

	memset(strWTemp, 0, sizeof(WCHAR)*100);
	memset(strData, 0, sizeof(char)*100);
	swprintf(buf, _T(""));
	GetValueBox(strWTemp, 20, buf, _T("Input value."));

	USES_CONVERSION;
	strTemp = W2A(strWTemp);
 
	switch (m_iZigInfoIndex)
	{
	case 1:
        sprintf(strData, "            ");
		iResult = m_pRFID->WriteData(ZIGID,strData);
		Sleep(100);
		sprintf(strData, "%s",strTemp);
		iResult = m_pRFID->WriteData(ZIGID,strData);
		break;
	case 2:
		sprintf(strData, "  ");
		iResult = m_pRFID->WriteData(REG_QTY,strData);
		Sleep(100);
		sprintf(strData, "%s",strTemp);
		iResult = m_pRFID->WriteData(REG_QTY,strData);
		break;
	case 3:
		sprintf(strData, "        ");
		iResult = m_pRFID->WriteData(REG_DATE,strData);
		Sleep(100);
		sprintf(strData, "%s",strTemp);
		iResult = m_pRFID->WriteData(REG_DATE,strData);
		break;
	case 4:
		sprintf(strData, "            ");
		iResult = m_pRFID->WriteData(MATCODE,strData);
		Sleep(100);
		sprintf(strData, "%s",strTemp);
		iResult = m_pRFID->WriteData(MATCODE,strData);
		break;
	case 5:
		sprintf(strData, "                ");
		iResult = m_pRFID->WriteData(TOT_QTY_ZIG,strData);
		Sleep(100);
		sprintf(strData, "%s",strTemp);
		iResult = m_pRFID->WriteData(TOT_QTY_ZIG,strData);
		break;
	case 6:
		sprintf(strData, "        ");
		iResult = m_pRFID->WriteData(ZIG_USED_QTY,strData);
		Sleep(100);
		sprintf(strData, "%s",strTemp);
		iResult = m_pRFID->WriteData(ZIG_USED_QTY,strData);
		break;
	case 7:
		sprintf(strData, "        ");
		iResult = m_pRFID->WriteData(TOT_QTY_AFTER_REG,strData);
		Sleep(100);
		sprintf(strData, "%s",strTemp);
		iResult = m_pRFID->WriteData(TOT_QTY_AFTER_REG,strData);
		break;
	case 8:
		sprintf(strData, "        ");
		iResult = m_pRFID->WriteData(REG_USED_QTY,strData);
		Sleep(100);
		sprintf(strData, "%s",strTemp);
		iResult = m_pRFID->WriteData(REG_USED_QTY,strData);
		break;
	default:
		//�߸��� Item ��ȣ��.;
		MyMessageBox(MY_MESSAGE_BOX, 40703, _T("Error"), M_ICONERROR);
		break;
	}

	if (iResult != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
}

void CDataMoldInfoDlg::OnClickRefreshInfo() 
{
	GetDlgItem(IDB_SELECT_ZIG_INFO)->EnableWindow(FALSE);
	GetDlgItem(IDB_WRITE_ZIG_INFO)->EnableWindow(FALSE);
	GetDlgItem(IDB_REFRESH_INFO)->EnableWindow(FALSE);
	GetDlgItem(IDB_CANCEL)->EnableWindow(FALSE);

	DisplayRFIDInformation();

	GetDlgItem(IDB_SELECT_ZIG_INFO)->EnableWindow(TRUE);
	GetDlgItem(IDB_WRITE_ZIG_INFO)->EnableWindow(TRUE);
	GetDlgItem(IDB_REFRESH_INFO)->EnableWindow(TRUE);
	GetDlgItem(IDB_CANCEL)->EnableWindow(TRUE);
}

void CDataMoldInfoDlg::OnClickCancel() 
{
	CDialog::OnCancel();
}

void CDataMoldInfoDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}
