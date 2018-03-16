// DataMatchingTableDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlbSystem.h"
#include "DataMatchingTableDlg.h"
#include "MFileUtil.h"
#include "Common.h"
#include "DataModelIDList.h"
#include "Shlwapi.h"
#pragma comment(lib, "Shlwapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataMatchingTableDlg dialog
extern MPlatformOlbSystem	MOlbSystemPre;


CDataMatchingTableDlg::CDataMatchingTableDlg(CString strModelFileDir, CWnd* pParent /*=NULL*/)
	: CDialog(CDataMatchingTableDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataMatchingTableDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_strModelFileDir = strModelFileDir;
//NSMC_LKH	m_iOldSelectedRow = -1;
	m_iOldSelectedRow = 1;
	m_iSelectedRow = 1;

	m_iTotalDeviceIDNum = 0;
	m_iSelectedPage = 0;  //NSMC_LKH
}


void CDataMatchingTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataMatchingTableDlg)
	//DDX_Control(pDX, IDC_MSFLEXGRID_MATCHING_TABLE, m_FlexGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataMatchingTableDlg, CDialog)
	//{{AFX_MSG_MAP(CDataMatchingTableDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataMatchingTableDlg message handlers

BEGIN_EVENTSINK_MAP(CDataMatchingTableDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDataMatchingTableDlg)
	ON_EVENT(CDataMatchingTableDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	ON_EVENT(CDataMatchingTableDlg, IDB_CONFIG_MATCHING_DATA, -600 /* Click */, OnClickConfigMatchingData, VTS_NONE)
	ON_EVENT(CDataMatchingTableDlg, IDC_MSFLEXGRID_MATCHING_TABLE, -600 /* Click */, OnClickMsflexgridMatchingTable, VTS_NONE)
	ON_EVENT(CDataMatchingTableDlg, IDB_UP, -600 /* Click */, OnUp, VTS_NONE)
	ON_EVENT(CDataMatchingTableDlg, IDB_DOWN, -600 /* Click */, OnDown, VTS_NONE)
	ON_EVENT(CDataMatchingTableDlg, IDB_PAGE_UP, -600 /* Click */, OnPageUp, VTS_NONE)
	ON_EVENT(CDataMatchingTableDlg, IDB_PAGE_DOWN, -600 /* Click */, OnPageDown, VTS_NONE)
	ON_EVENT(CDataMatchingTableDlg, IDB_ADD_MATCHING_DATA, -600 /* Click */, OnAddMatchingData, VTS_NONE)
	ON_EVENT(CDataMatchingTableDlg, IDB_DELETE_MATCHING_DATA, -600 /* Click */, OnDeleteMatchingData, VTS_NONE)
	ON_EVENT(CDataMatchingTableDlg, IDB_UPDATE_MATERIAL, -600 /* Click */, OnClickUpdateMaterial, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDataMatchingTableDlg::OnClickExit() 
{
	OnOK();	
}

BOOL CDataMatchingTableDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strTemp;
	
	//Matching 정보 추가
	ReadDisplayMessage(MAIN_AUTO, _T("DataMatchingTableDlg_1"), &strTemp);
	GetDlgItem(IDB_ADD_MATCHING_DATA)->SetWindowText(strTemp);
	 	
	//Matching 정보 수정
	ReadDisplayMessage(MAIN_AUTO, _T("DataMatchingTableDlg_2"), &strTemp);
	GetDlgItem(IDB_CONFIG_MATCHING_DATA)->SetWindowText(strTemp);
	 	
	//Matching 정보 삭제
	ReadDisplayMessage(MAIN_AUTO, _T("DataMatchingTableDlg_3"), &strTemp);
	GetDlgItem(IDB_DELETE_MATCHING_DATA)->SetWindowText(strTemp);
	
	//나가기 	
	ReadDisplayMessage(MAIN_AUTO, _T("DataMatchingTableDlg_4"), &strTemp);
	GetDlgItem(IDB_EXIT)->SetWindowText(strTemp);
		
	//위
	ReadDisplayMessage(MAIN_AUTO, _T("DataMatchingTableDlg_5"), &strTemp);
	GetDlgItem(IDB_UP)->SetWindowText(strTemp);
	
	//아래
	ReadDisplayMessage(MAIN_AUTO, _T("DataMatchingTableDlg_6"), &strTemp);
	GetDlgItem(IDB_DOWN)->SetWindowText(strTemp);
	
	// TODO: Add extra initialization here
	calculateTable(); //NSMC_LKH
	displayTable();
	redrawRowColor();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataMatchingTableDlg::OnClickConfigMatchingData() 
{
//	if (MyMessageBox("DeviceID 데이터를 수정하시겠습니까?","확인",M_YESNO) == IDNO)
//		return;

//NSMC_LKH	if (m_iSelectedRow <= 0 || m_iSelectedRow > m_iTotalDeviceIDNum)
	if (m_iSelectedRow <1 || m_iSelectedRow > DEF_MAX_COL)
		return;

//NSMC_LKH	CString strDeviceID = m_strArray_DeviceID.GetAt(m_iSelectedRow - 1);
//NSMC_LKH	CString strOldModelID = m_strArray_ModelID.GetAt(m_iSelectedRow - 1);
	CString strDeviceID = m_strArray_DeviceID.GetAt(DEF_MAX_COL*m_iSelectedPage + m_iSelectedRow - 1);
	CString strOldModelID = m_strArray_ModelID.GetAt(DEF_MAX_COL*m_iSelectedPage + m_iSelectedRow - 1);
	CString strNewModelID = "NONE";

	CDataModelIDList dlg(m_strModelFileDir, &strNewModelID);
	dlg.DoModal();

	if ("NONE" == strNewModelID)
	{
		MyMessageBox("Model not selected.\r\n Model 선택 안됨.", "Error", M_ICONERROR);
		return;
	}

	CString strOldFullPath = m_strModelFileDir + "\\" + strOldModelID + "\\" + strDeviceID;
	if (FALSE == PathIsDirectory(strOldFullPath.GetBuffer(strOldFullPath.GetLength())))
		return;

	if (0 == RemoveDirectory(strOldFullPath.GetBuffer(strOldFullPath.GetLength())))
	{
		MyMessageBox("Failed to delete Device ID\r\n Device ID 삭제 실패", "Error", M_ICONERROR);
		return;
	}

	// 해당 Model Folder 에 Device ID 생성.
	CString strNewFullPath = m_strModelFileDir + "\\" + strNewModelID + "\\" + strDeviceID;

	if (0 == CreateDirectory(strNewFullPath.GetBuffer(strNewFullPath.GetLength()), NULL))
	{
		MyMessageBox("Failed to write Device ID", "Error", M_ICONERROR);
		return;
	}

	calculateTable(); //NSMC_LKH
	displayTable();
	redrawRowColor();
}

void CDataMatchingTableDlg::OnClickMsflexgridMatchingTable() 
{
//	long lRow = m_FlexGrid.GetRow();
//
//	if ((DEF_MAX_COL*m_iSelectedPage + lRow-1) >= m_iTotalDeviceIDNum)
//		return;
//
////NSMC_LKH	if (lRow > 0 && lRow <= m_iTotalDeviceIDNum)
//	if (lRow > 0 && lRow <= DEF_MAX_COL)
//	{
//		if (m_iSelectedRow != lRow)
//			m_iOldSelectedRow = m_iSelectedRow;
//
//		m_iSelectedRow = lRow;
//
//		redrawRowColor();
//	}
}

void CDataMatchingTableDlg::redrawRowColor()
{
  /* NSMC_LKH : 정렬 순서 변경
	if (m_iOldSelectedRow > 0 && m_iOldSelectedRow <= m_iTotalDeviceIDNum
		&& m_iOldSelectedRow != m_iSelectedRow)
	{
		m_FlexGrid.SetRow(m_iOldSelectedRow);
		for (int i = 0; i < 3; i++)
		{
			m_FlexGrid.SetCol(i);
			m_FlexGrid.SetCellBackColor(WHITE);
			m_FlexGrid.SetCellFontBold(FALSE);
		}
	}

	if (m_iSelectedRow > 0 && m_iSelectedRow <= m_iTotalDeviceIDNum)
	{
		m_FlexGrid.SetRow(m_iSelectedRow);
		for (int i = 0; i < 3; i++)
		{
			m_FlexGrid.SetCol(i);
			m_FlexGrid.SetCellBackColor(YELLOW);
			m_FlexGrid.SetCellFontBold(TRUE);
		}
	}
	*/
	//if (m_iOldSelectedRow >= 0)
	//{
	//	m_FlexGrid.SetRow(m_iOldSelectedRow);

	//	for (int i = 0; i < 3; i++)
	//	{
	//		m_FlexGrid.SetCol(i);
	//		m_FlexGrid.SetCellBackColor(WHITE);
	//	}
	//}

	//m_FlexGrid.SetRow(m_iSelectedRow);
	//for (int i = 0; i < 3; i++)
	//{
	//	m_FlexGrid.SetCol(i);
	//	m_FlexGrid.SetCellBackColor(YELLOW);
	//}
}

void CDataMatchingTableDlg::OnUp() 
{
	if (1 == m_iSelectedRow)
	{
		redrawRowColor();
		return;
	}

	m_iOldSelectedRow = m_iSelectedRow--;
	redrawRowColor();
}

void CDataMatchingTableDlg::OnDown() 
{
  //NSMC_LKH
	if (DEF_MAX_COL*m_iSelectedPage + m_iSelectedRow >= m_iTotalDeviceIDNum)
		return;

//NSMC_LKH	if (m_iTotalDeviceIDNum == m_iSelectedRow)
	if ( DEF_MAX_COL == m_iSelectedRow)
		return;

	m_iOldSelectedRow = m_iSelectedRow++;
	redrawRowColor();
}

void CDataMatchingTableDlg::OnPageUp() 
{
//NSMC_LKH	if (1 == m_iSelectedRow)
	if (0 == m_iSelectedPage)
		return;

	m_iSelectedPage--;  //NSMC_LKH
	displayTable(); //NSMC_LKH
	m_iOldSelectedRow = m_iSelectedRow;
	m_iSelectedRow = 1;
	redrawRowColor();
}

void CDataMatchingTableDlg::OnPageDown() 
{
//NSMC_LKH	if (m_iTotalDeviceIDNum == m_iSelectedRow)
	if ((m_iTotalDeviceIDNum/DEF_MAX_COL) == m_iSelectedPage)
		return;

	m_iSelectedPage++;  //NSMC_LKH
	displayTable(); //NSMC_LKH
	m_iOldSelectedRow = m_iSelectedRow;
//NSMC_LKH	m_iSelectedRow = m_iTotalDeviceIDNum;
	m_iSelectedRow = 1;
	redrawRowColor();
}

void CDataMatchingTableDlg::OnAddMatchingData() 
{
	WCHAR szTemp[20];
	WCHAR szBuf[20];

	// Device ID 입력
	memset(szTemp, 0, sizeof(szTemp));
	swprintf(szBuf, _T(""));
	GetValueBox(szTemp, 20, szBuf, _T("Device ID를 입력하세요"));

	CString strDeviceID;
	strDeviceID.Format(_T("%s"), szTemp);

	// Model ID 선택 (Folder 에서 고른다.)
	CString strModelID = "NONE";
	CDataModelIDList dlg(m_strModelFileDir, &strModelID);
	dlg.DoModal();

	if ("NONE" == strModelID)
	{
		MyMessageBox("Model not selected.\r\n Model 선택 안됨.", "Error", M_ICONERROR);
		return;
	}

	// 해당 Model Folder 에 Device ID 생성.
	CString strFullPath = m_strModelFileDir + "\\" + strModelID + "\\" + strDeviceID;

	if (0 == CreateDirectory(strFullPath.GetBuffer(strFullPath.GetLength()), NULL))
	{
		MyMessageBox("Failed to write Device ID", "Error", M_ICONERROR);
		return;
	}
	calculateTable(); //syr..SESL 090609
	displayTable();
	redrawRowColor();
}

void CDataMatchingTableDlg::OnDeleteMatchingData() 
{
//NSMC_LKH	if (m_iSelectedRow < 1 && m_iSelectedRow > m_iTotalDeviceIDNum)
	if (m_iSelectedRow < 1 || m_iSelectedRow > DEF_MAX_COL)
		return;

//NSMC_LKH	CString strDeviceID = m_strArray_DeviceID.GetAt(m_iSelectedRow - 1);
//NSMC_LKH	CString strModelID = m_strArray_ModelID.GetAt(m_iSelectedRow - 1);
	CString strDeviceID = m_strArray_DeviceID.GetAt(DEF_MAX_COL*m_iSelectedPage + m_iSelectedRow - 1);
	CString strModelID = m_strArray_ModelID.GetAt(DEF_MAX_COL*m_iSelectedPage + m_iSelectedRow - 1);

	CString strFullPath = m_strModelFileDir + "\\" + strModelID + "\\" + strDeviceID;
	if (FALSE == PathIsDirectory(strFullPath.GetBuffer(strFullPath.GetLength())))
		return;

	if (0 == RemoveDirectory(strFullPath.GetBuffer(strFullPath.GetLength())))
	{
		MyMessageBox("Failed to delete Device ID\r\nDevice ID 삭제 실패", "Error", M_ICONERROR);
		return;
	}

	calculateTable(); //NSMC_LKH
	displayTable();
	redrawRowColor();
	if ((DEF_MAX_COL*m_iSelectedPage + m_iSelectedRow-1) >= m_iTotalDeviceIDNum)
		OnUp();
}

void CDataMatchingTableDlg::displayTable()
{
//	m_FlexGrid.SetCols(3);
////NSMC_LKH	m_FlexGrid.SetRows(m_iTotalDeviceIDNum + 1);
//	m_FlexGrid.SetRows(DEF_MAX_COL + 1);
//	m_FlexGrid.Clear();
//
//	//NSMC_LKH : 정렬방법 변경
//	m_FlexGrid.SetColAlignment( 0, 4 );
////NSMC_LKH	m_FlexGrid.SetColAlignment( 1, 4 );
////NSMC_LKH	m_FlexGrid.SetColAlignment( 2, 4 );
//	m_FlexGrid.SetColAlignment( 1, 1 );
//	m_FlexGrid.SetColAlignment( 2, 1 );
//		
//	m_FlexGrid.SetTextMatrix(0, 0, _T("No"));
//	m_FlexGrid.SetTextMatrix(0, 1, _T("Device ID"));
//	m_FlexGrid.SetTextMatrix(0, 2, _T("Model Folder Name"));
//		
//	m_FlexGrid.SetColWidth(0, 630);
//	m_FlexGrid.SetColWidth(1, 3700);
//
//	m_FlexGrid.SetColWidth(2, 3540);
//
//	m_FlexGrid.SetFixedCols(0);
//	
//	WCHAR szText[256];
//
////NSMC_LKH	for (int i = 0; i < m_iTotalDeviceIDNum; i++)
//	for (int i = 0; i < DEF_MAX_COL; i++)
//	{
//		if (DEF_MAX_COL*m_iSelectedPage + i >= m_iTotalDeviceIDNum)
//			continue;
//
//		memset(szText, NULL, sizeof(szText));
////NSMC_LKH		swprintf(szText, _T("%d"), i + 1);
//		swprintf(szText, _T("%d"), DEF_MAX_COL*m_iSelectedPage + i + 1);
//		m_FlexGrid.SetTextMatrix(i + 1, 0, szText);
//
//		memset(szText, NULL, sizeof(szText));
////NSMC_LKH		swprintf(szText, _T("%s"), m_strArray_DeviceID.GetAt(i));
//		swprintf(szText, _T("%s"), (m_strArray_DeviceID.GetAt(DEF_MAX_COL*m_iSelectedPage + i).Mid(0,14)));
//		m_FlexGrid.SetTextMatrix(i + 1, 1, szText);
//
//		memset(szText, NULL, sizeof(szText));
////NSMC_LKH		swprintf(szText, _T("%s"), m_strArray_ModelID.GetAt(i));
//		swprintf(szText, _T("%s"), m_strArray_ModelID.GetAt(DEF_MAX_COL*m_iSelectedPage + i));
//		m_FlexGrid.SetTextMatrix(i + 1, 2, szText);
//	}
}

void CDataMatchingTableDlg::calculateTable()
{
	MFileUtil util;

	int iModelFolderCount = util.GetDirCountInDir(m_strModelFileDir);
	if (iModelFolderCount > DEF_MAX_MODEL)
	{
		// hongju 임시 막음
//		CString strMsg;
//		strMsg.Format("모델 갯수(%s)가 시스템 허용치(%s)를 초과 합니다.", iModelFolderCount, DEF_MAX_MODEL);
//		MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), "Manage Model", M_ICONERROR);
		return;
	}

	CString* pstrModelFolderList = new CString[iModelFolderCount];

	util.GetDirListInDir(m_strModelFileDir, pstrModelFolderList, iModelFolderCount);

	int i = 0;
	m_iTotalDeviceIDNum = 0;
	m_strArray_DeviceID.RemoveAll();
	m_strArray_ModelID.RemoveAll();

	int* pSubCount = new int[iModelFolderCount];

	for (int i = 0; i < iModelFolderCount; i++)
	{
		pSubCount[i] = 0;
		pSubCount[i] = util.GetDirCountInDir(m_strModelFileDir + "\\" + pstrModelFolderList[i]);
		m_iTotalDeviceIDNum += pSubCount[i];
	}

	CString* pstrSubDeviceIDList;
	for (int i = 0; i < iModelFolderCount; i++)
	{
		// 080818
		if (pSubCount[i] < 1)
			continue;

		pstrSubDeviceIDList = new CString[pSubCount[i]];
		util.GetDirListInDir(m_strModelFileDir + "\\" + pstrModelFolderList[i], pstrSubDeviceIDList, pSubCount[i]);

		for (int j = 0; j < pSubCount[i]; j++)
		{
			m_strArray_DeviceID.Add(pstrSubDeviceIDList[j]);
			m_strArray_ModelID.Add(pstrModelFolderList[i]);
		}

		delete[] pstrSubDeviceIDList;
	}

	//NSMC KJS DeviceID 정렬
	int j;
	CString strPart[2];
	CString strTemp[2];

	for (int i=0; i<m_iTotalDeviceIDNum-1; i++)
	{
		for (j=i; j<m_iTotalDeviceIDNum; j++)
		{
			if (wcscmp(m_strArray_DeviceID.GetAt(i), m_strArray_DeviceID.GetAt(j)) > 0)
			{
				strTemp[0] = m_strArray_DeviceID.GetAt(i);
				strTemp[1] = m_strArray_ModelID.GetAt(i);
				m_strArray_DeviceID.SetAt(i, m_strArray_DeviceID.GetAt(j));
				m_strArray_ModelID.SetAt(i, m_strArray_ModelID.GetAt(j));
				m_strArray_DeviceID.SetAt(j, strTemp[0]);
				m_strArray_ModelID.SetAt(j, strTemp[1]);
			}
		}
	}

	delete[] pstrModelFolderList;
	delete[] pSubCount;
}

void CDataMatchingTableDlg::OnClickUpdateMaterial() 
{
	// TODO: Add your control notification handler code here
	//NSMC KJS Material Check
	if (IDNO == MyMessageBox("Do you want to update Material Data?", "Inform",M_ICONQUESTION))
		return;

// 	MMaterial* plnkMaterial;
// 	plnkMaterial = &(m_plnkCtrlTabFeeder[DEF_TABFEEDER1]->m_matTabICInfo[DEF_USING_REEL]);
// 	m_plnkLCNet->MaterialReport(eMatInfoRequest, plnkMaterial);
// 	plnkMaterial = &(m_plnkCtrlTabFeeder[DEF_TABFEEDER1]->m_matTabICInfo[DEF_SPARE_REEL]);
// 	m_plnkLCNet->MaterialReport(eMatInfoRequest, plnkMaterial);
// 	plnkMaterial = &(m_plnkCtrlTabFeeder[DEF_TABFEEDER2]->m_matTabICInfo[DEF_USING_REEL]);
// 	m_plnkLCNet->MaterialReport(eMatInfoRequest, plnkMaterial);
// 	plnkMaterial = &(m_plnkCtrlTabFeeder[DEF_TABFEEDER2]->m_matTabICInfo[DEF_SPARE_REEL]);
// 	m_plnkLCNet->MaterialReport(eMatInfoRequest, plnkMaterial);
}
