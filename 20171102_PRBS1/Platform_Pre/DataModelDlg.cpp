// ModelDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "DataModelDlg.h"
#include "CommonGetValueDlg.h"
#include "MPlatformOlbSystem.h"
#include "MPanelAligner.h"
#include "MTrsAutoManager.h"
#include "DataPanelDlg.h"
#include "DataTabICDlg.h"

//20111205 SJ_HJG
#include "MTabData.h"
#include "MTabOffsetData.h"
#include "MTabMounter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CDataModelDlg dialog

CDataModelDlg::CDataModelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataModelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataModelDlg)
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_bReferanceSelect = FALSE;
}


void CDataModelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataModelDlg)
	DDX_Control(pDX, IDC_MODEL_LIST, m_lboxModelList);
	DDX_Control(pDX, IDS_CURRENT_MODEL, m_lblCurrentModel);
	DDX_Control(pDX, IDS_SELECT_MODEL, m_lblSelectModel);

	DDX_Control(pDX, IDC_SELECT_REFERENCE_MODEL_LIST, m_lboxSelectModelReferenceList);
	DDX_Control(pDX, IDB_SELECT_DEVICE_MODEL_CREATE, m_btnReferenceOfSelectCreateModel);
	DDX_Control(pDX, IDB_SELECT_DEVICE_MODEL_DELETE, m_btnReferenceOfSelectDeleteModel);
	DDX_Control(pDX, IDB_MODEL_MODIFY, m_btnModelModify);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataModelDlg, CDialog)
	//{{AFX_MSG_MAP(CDataModelDlg)
	ON_WM_PAINT()
	ON_LBN_SELCHANGE(IDC_MODEL_LIST, OnSelchangeModelList)
	ON_LBN_SELCHANGE(IDC_SELECT_REFERENCE_MODEL_LIST, OnSelchangeSelectReferenceModelList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataModelDlg message handlers

void CDataModelDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CDataModelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitializeModelListBox();

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

	CString szTextTemp;
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlg_1"), &szTextTemp);
//	모델 리스트
	GetDlgItem(IDC_LIST)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlg_2"), &szTextTemp);
//	현재 생산 중인 모델
	GetDlgItem(IDC_CURRENT_MODEL)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlg_3"), &szTextTemp);
//	선택된 모델
	GetDlgItem(IDC_SELECT_MODEL)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlg_4"), &szTextTemp);
//	위
	GetDlgItem(IDB_UP)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlg_5"), &szTextTemp);
//	아래
	GetDlgItem(IDB_DOWN)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlg_6"), &szTextTemp);
//	페이지\r\n위로
	GetDlgItem(IDB_PAGE_UP)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlg_7"), &szTextTemp);
//	페이지\r\n아래로
	GetDlgItem(IDB_PAGE_DOWN)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlg_8"), &szTextTemp);
//	모델 생성
	GetDlgItem(IDB_MODEL_CREATE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlg_9"), &szTextTemp);
//	모델 삭제
	GetDlgItem(IDB_MODEL_DELETE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlg_10"), &szTextTemp);
//	모델 선택
	GetDlgItem(IDB_MODEL_LOAD)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlg_11"), &szTextTemp);
//	나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);
	
	//101004.KKY_____
/*	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() == AUTORUN_MODE)
	{
//		GetDlgItem(IDB_MODEL_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDB_MODEL_LOAD)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDB_PANELDATA)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_TABDATA)->ShowWindow(SW_HIDE);
	}
	//_______________*/ 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CDataModelDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDataModelDlg)
	ON_EVENT(CDataModelDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CDataModelDlg, IDB_MODEL_CREATE, -600 /* Click */, OnModelCreate, VTS_NONE)
	ON_EVENT(CDataModelDlg, IDB_MODEL_DELETE, -600 /* Click */, OnModelDelete, VTS_NONE)
	ON_EVENT(CDataModelDlg, IDB_MODEL_LOAD, -600 /* Click */, OnModelLoad, VTS_NONE)
	ON_EVENT(CDataModelDlg, IDB_UP, -600 /* Click */, OnUp, VTS_NONE)
	ON_EVENT(CDataModelDlg, IDB_DOWN, -600 /* Click */, OnDown, VTS_NONE)
	ON_EVENT(CDataModelDlg, IDB_PAGE_UP, -600 /* Click */, OnPageUp, VTS_NONE)
	ON_EVENT(CDataModelDlg, IDB_PAGE_DOWN, -600 /* Click */, OnPageDown, VTS_NONE)
	ON_EVENT(CDataModelDlg, IDB_PANELDATA, -600 /* Click */, OnClickPaneldata, VTS_NONE)
	ON_EVENT(CDataModelDlg, IDB_TABDATA, -600 /* Click */, OnClickTabdata, VTS_NONE)
	ON_EVENT(CDataModelDlg, IDB_SELECT_DEVICE_MODEL_CREATE, -600 /* Click */, OnClickSelectDeviceModelCreate, VTS_NONE)
	ON_EVENT(CDataModelDlg, IDB_SELECT_DEVICE_MODEL_DELETE, -600 /* Click */, OnClickSelectDeviceModelDelete, VTS_NONE)
	ON_EVENT(CDataModelDlg, IDB_MODEL_MODIFY, -600 /* Click */, OnModelModify, VTS_NONE)
	ON_EVENT(CDataModelDlg, IDB_SELECT_DEVICE_MODEL_SELECT, -600 /* Click */, OnClickSelectDeviceModelSelect, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDataModelDlg::OnExit() 
{

	if(m_bReferanceSelect == FALSE && MOlbSystemPre.GetTrsAutoManagerComponent()->m_strEquipModelName == "" )
	{
		MyMessageBox(MY_MESSAGE_BOX, 40611, _T("Device ID Error"), M_ICONERROR);
		return;
	}
	

	CDialog::OnCancel();
}

int CDataModelDlg::InitializeModelListBox()
{
	m_iCurrentSel = -1;

	m_iCurrentSelForDeviceModelList = 0;

	m_strCurrentModelName = MOlbSystemPre.GetCurrentModelName();

	m_lblCurrentModel.SetCaption(m_strCurrentModelName);

	MOlbSystemPre.GetModelNameList(m_strModelList, &m_iModelCount);

	if (m_iModelCount <= 0)
	{
//      등록된 모델이 하나도 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 40600, _T("Manage Model"), M_ICONERROR);
		m_lblSelectModel.SetCaption(_T(""));
		m_lblCurrentModel.SetCaption(_T(""));
		return -1;
	}

	for (int i = 0; i < m_iModelCount; i++)
	{
		if (!lstrcmp(m_strCurrentModelName,m_strModelList[i]))
		{
			m_iCurrentSel = i;
		}
	}

	if (m_iCurrentSel < 0)
	{
		CString strMsg;
//		현재 모델 디렉토리가 존재하지 않습니다. []
		ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_40601"), &strMsg);
		strMsg += m_strCurrentModelName + _T("]");
		MyMessageBox(strMsg, _T("Manage Model"), M_ICONERROR);
		return	-1;
	}

	m_lboxModelList.ResetContent();

	//110114.kms_______________
	//Reference Model 관련하여 Model Load 창에 Display 하는 부분
	/*
	for (int i = 0 ; i < m_iModelCount ;i++)
	{
		m_strModelList[i].MakeUpper();
		m_lboxModelList.InsertString(-1,(LPCTSTR)m_strModelList[i]);	
	}*/

	CString strPanelDataDir;
	CString strReferDataName;

    WCHAR chrData[256];
    memset(chrData, NULL, sizeof(chrData));

	for (int i = 0 ; i < m_iModelCount ;i++)
	{
		strPanelDataDir.Format(_T(".\\Data\\ModelData\\%s\\Panel.dat"), (LPCTSTR)m_strModelList[i]);
	
		GetPrivateProfileString(_T("Panel Data"), _T("Reference_Data"), _T("ERROR"), chrData, 256, strPanelDataDir);

		strReferDataName.Format(_T("%s <==> %s"), (LPCTSTR)m_strModelList[i], chrData);
		strReferDataName.MakeUpper();
		m_lboxModelList.InsertString(-1, strReferDataName);	
	}
	//_______________________________

	updateModelListBox();
	//110304.kms________
	updateSelectModelReferenceListBox();
	//__________________

	return 0;
}

void CDataModelDlg::updateModelListBox()
{
	m_lboxModelList.SetCurSel(m_iCurrentSel);
	//110114.kms_________
//	m_lboxModelList.GetText(m_iCurrentSel, m_strSelectedModelName);
	m_strSelectedModelName = m_strModelList[m_iCurrentSel];
	//___________________


	if (!m_iModelCount)
		m_lblSelectModel.SetCaption(_T(""));
	else
		m_lblSelectModel.SetCaption(m_strSelectedModelName);
}

void CDataModelDlg::OnModelCreate() 
{
	WCHAR buf[100];
	swprintf(buf, _T(""));

	//110114.kms_____
	WCHAR szTemp[100];
	memset(szTemp, 0, sizeof(szTemp));
	//_______________

//	Model 이름을 입력하세요
//	CCommonGetValueDlg	valueDialog(20, buf, _T("Input Model Name"));
//	valueDialog.DoModal();

	if(!GetValueBox(szTemp, 20, buf, _T("Input the Model name.")))
		return;

//	CString strTemp = valueDialog.GetStringValue();

	if (m_lboxModelList.FindStringExact(-1, m_szTmp) != LB_ERR)
	{
//		같은 이름의 Model이 존재 합니다. 다른 이름으로 저장하세요?
		MyMessageBox(MY_MESSAGE_BOX, 40602, _T("Model Creation Error"), M_ICONERROR);
		return;
	}	

	//101004.KKY_____
	m_strSelectedModelName.TrimLeft();
	m_strSelectedModelName.TrimRight();
	if (m_strSelectedModelName == _T(""))
	{
		MyMessageBox(MY_MESSAGE_BOX, 40610, _T("Model Creation Error"), M_ICONERROR);
		return;
	}
	//_______________

	//101004.KKY_____
//	int iResult = MOlbSystemPre.CreateModel(strTemp, m_strCurrentModelName);
//	int iResult = MOlbSystemPre.CreateModel(strTemp, m_strSelectedModelName);
	int iResult = MOlbSystemPre.CreateModel(szTemp, m_strSelectedModelName);
	//_______________
	if (iResult)
	{
//		Model Directory를 생성하는데 실패 하였음.
		MyMessageBox(MY_MESSAGE_BOX, 40603, _T("Model Creation Error"), M_ICONERROR);
		return;
	}

	InitializeModelListBox();	// 모델이 추가 되었으니까 List를 초기화 하고

	/* selsk_jdy 불필요 삭제. - Create 후 Load 해야 정상 동작 함. Change Model 시 저장함.
	MFileManager::AutoDeleteFile("C:\\DataBackup\\*.*", 50, TRUE);

	CTime t = CTime::GetCurrentTime();
	char dest[256];
	sprintf(dest, "C:\\DataBackup\\%02d%02d%02d", t.GetYear(), t.GetMonth(), t.GetDay());
	MFileManager::CreateBackupFolder(dest, ".\\Data", TRUE);
	*/

	//20110825 SJ_HJG 생성 이후 현재 모델로 Load 됨
	/*
	//110307.KMS_________
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() == MANUAL_MODE)
	{
		OnModelLoad();
	}
	//___________________
	*/
}

void CDataModelDlg::OnModelDelete() 
{
//	선택된 모델을 삭제하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 40604, _T("Model Delete"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	if (m_strSelectedModelName.Compare(m_strCurrentModelName) == 0)
	{
//		현재 진행 중인 Model은 삭제 할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 40605, _T("Model Delete Error"), M_ICONERROR);
		return;
	}

	MOlbSystemPre.DeleteModel(m_strSelectedModelName);

	InitializeModelListBox();	// 변경된 디렉토리를 ListBox에 반영한다.
}

void CDataModelDlg::OnModelLoad() 
{
//	새로운 Model을 Loading 하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 40606, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CString strLog;
	strLog.Format(_T("ModelChange [Old Model: %s, New Model: %s]"), m_strCurrentModelName, m_strSelectedModelName);
	MOlbSystemPre.GetTrsAutoManagerComponent()->WriteMonitoringLog("Start " + strLog);

//20120126 SJ_HJG
#ifndef DEF_GATE_SYSTEM
	CString strAutoAlignLog;
	strAutoAlignLog.Format(_T("ModelChange [Old Model: %s -> New Model: %s]"), m_strCurrentModelName, m_strSelectedModelName);
	MOlbSystemPre.GetTabOffsetData()->WriteData();
	MOlbSystemPre.GetTabOffsetData_Rear()->WriteData();
	MOlbSystemPre.GetTrsAutoManagerComponent()->WriteAutoAlignLog(strAutoAlignLog);
#endif
	//___________________________________________________

	int iResult = MOlbSystemPre.ChangeModel(m_strSelectedModelName);
	if (iResult != SUCCESS)
	{
		MOlbSystemPre.GetTrsAutoManagerComponent()->WriteMonitoringLog("Fail " + strLog);

		MOlbSystemPre.ChangeModel(m_strCurrentModelName);
		
		if (iResult != ERR_DATA_READ_FAILURE)
		{
// jdy sesl			MyMessageBox(iResult, _T("Error Information"), M_ICONINFORMATION);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		}

//		Model Loading 작업 실패!.\r\nData 설정을 직접 확인하시오.
		MyMessageBox(MY_MESSAGE_BOX, 40607, _T("Error Information"), M_ICONINFORMATION);
		return;
	}

	m_strCurrentModelName = m_strSelectedModelName;

	MOlbSystemPre.GetTrsAutoManagerComponent()->WriteMonitoringLog("Finish " + strLog);

	m_lblCurrentModel.SetCaption(m_strSelectedModelName);
	updateModelListBox();
	//110304.kms________
	updateSelectModelReferenceListBox();
	//__________________
}

void CDataModelDlg::OnUp() 
{
	if (m_iModelCount <= 0)
		return;

	if (m_iCurrentSel <= 0)
		return;
	else
		m_iCurrentSel--;

	m_lboxModelList.SetCurSel(m_iCurrentSel);
	m_lboxModelList.GetText(m_iCurrentSel,m_strSelectedModelName);
	m_lblSelectModel.SetCaption(m_strSelectedModelName);

	updateModelListBox();
	//110304.kms________
	updateSelectModelReferenceListBox();
	//__________________
}

void CDataModelDlg::OnDown() 
{
	if (m_iModelCount <= 0)
		return;

	if (m_iCurrentSel >= (m_iModelCount-1))
		return;
	else
		m_iCurrentSel++;
	
	m_lboxModelList.SetCurSel(m_iCurrentSel);
	m_lboxModelList.GetText(m_iCurrentSel,m_strSelectedModelName);
	m_lblSelectModel.SetCaption(m_strSelectedModelName);

	updateModelListBox();
	//110304.kms________
	updateSelectModelReferenceListBox();
	//__________________
}

void CDataModelDlg::OnPageUp() 
{
	if (m_iModelCount <= 0)
		return;

	if (m_iCurrentSel - 10 <= 0)
		m_iCurrentSel = 0;
	else
		m_iCurrentSel -= 10;
	
	m_lboxModelList.SetCurSel(m_iCurrentSel);
	m_lboxModelList.GetText(m_iCurrentSel,m_strSelectedModelName);
	m_lblSelectModel.SetCaption(m_strSelectedModelName);

	updateModelListBox();	
	//110304.kms________
	updateSelectModelReferenceListBox();
	//__________________
}

void CDataModelDlg::OnPageDown() 
{
	if (m_iModelCount <= 0)
		return;

	if (m_iCurrentSel + 10 >= (m_iModelCount-1))
		m_iCurrentSel = m_iModelCount-1;
	else
		m_iCurrentSel += 10;
	
	m_lboxModelList.SetCurSel(m_iCurrentSel);
	m_lboxModelList.GetText(m_iCurrentSel,m_strSelectedModelName);
	m_lblSelectModel.SetCaption(m_strSelectedModelName);	

	updateModelListBox();	
	//110304.kms________
	updateSelectModelReferenceListBox();
	//__________________
}

void CDataModelDlg::OnSelchangeModelList() 
{
	m_iCurrentSel = m_lboxModelList.GetCurSel();

	m_lboxModelList.SetCurSel(m_iCurrentSel);
	//110114.kms________
//	m_lboxModelList.GetText(m_iCurrentSel, m_strSelectedModelName);
	m_strSelectedModelName = m_strModelList[m_iCurrentSel];
	//__________________
	m_lblSelectModel.SetCaption(m_strSelectedModelName);	
	updateSelectModelReferenceListBox();
}

//101004.KKY_______
void CDataModelDlg::OnClickPaneldata()
{
	// TODO: Add your control notification handler code here
	CString strSelModel = m_strSelectedModelName;
	if (strSelModel.Compare(m_strCurrentModelName) == 0)
	{
		MyMessageBox(MY_MESSAGE_BOX, 40608, _T("Model Delete Error"), M_ICONERROR);
		return;
	}
	MOlbSystemPre.CreateTmpModelData(strSelModel);

	CDataPanelDlg dlg(FALSE, TRUE);
	dlg.DoModal();
}

void CDataModelDlg::OnClickTabdata()
{
	// TODO: Add your control notification handler code here
	CString strSelModel = m_strSelectedModelName;
	if (strSelModel.Compare(m_strCurrentModelName) == 0)
	{
		MyMessageBox(MY_MESSAGE_BOX, 40609, _T("Model Delete Error"), M_ICONERROR);
		return;
	}
	MOlbSystemPre.CreateTmpModelData(strSelModel);
	CDataTabICDlg dlg(FALSE, TRUE, 0);
	dlg.DoModal();
}
//_________________


//110304.kms__________________________
//Reference Model 추가 관련
//
void CDataModelDlg::OnSelchangeSelectReferenceModelList() 
{
	// TODO: Add your control notification handler code here
	m_iCurrentSelForDeviceModelList = m_lboxSelectModelReferenceList.GetCurSel();
}

void CDataModelDlg::OnClickSelectDeviceModelCreate() 
{
	// TODO: Add your control notification handler code here
	WCHAR buf[100];
	swprintf(buf, _T(""));

	//110114.kms_____
	WCHAR szTemp[100];
	memset(szTemp, 0, sizeof(szTemp));
	//_______________

	if(!GetValueBox(szTemp, 20, buf, _T("Input the Model name.")))
		return;


	if (m_lboxModelList.FindStringExact(-1, m_szTmp) != LB_ERR)
	{
//		같은 이름의 Model이 존재 합니다. 다른 이름으로 저장하세요?
		MyMessageBox(MY_MESSAGE_BOX, 40602, _T("Model Creation Error"), M_ICONERROR);
		return;
	}	


	//2011.02.28 : Model별 Reference Model 추가
	int iIndex;
	m_lboxSelectModelReferenceList.ResetContent();
	
	CString strModelDataDir;
	CString strReferDataName[DEF_MAX_REFERENCE_MODEL];
	CString strDataName;

	WCHAR chrData[256];
    memset(chrData, NULL, sizeof(chrData));

	strModelDataDir.Format(_T(".\\Data\\ModelData\\%s\\Panel.dat"), (LPCTSTR)m_strSelectedModelName);
	for(iIndex=0; iIndex<DEF_MAX_REFERENCE_MODEL; iIndex++)
	{
		strDataName.Format(_T("ReferenceModel_%d"), iIndex);

		GetPrivateProfileString(REFERENCE_MODEL_SECTION, strDataName, _T(""), chrData, 256, strModelDataDir);
		strReferDataName[iIndex].Format(_T("%s"), chrData);
		strReferDataName[iIndex].MakeUpper();
	}

	for(iIndex=0; iIndex<DEF_MAX_REFERENCE_MODEL; iIndex++)
	{
		if(strReferDataName[iIndex] == szTemp)
		{
			MyMessageBox(_T("A Reference Model with the same name already exists.\r\n이미 같은 이름의 Reference Model이 존재합니다."), _T("Device Model 생성 Error"), M_ICONERROR);
			updateSelectModelReferenceListBox();
			return;
		}
	}

	for(iIndex=0; iIndex<DEF_MAX_REFERENCE_MODEL; iIndex++)
	{
		if(strReferDataName[iIndex] == "")
		{
			strDataName.Format(_T("ReferenceModel_%d"), iIndex);
			strReferDataName[iIndex].Format(_T("%s"), szTemp);
			break;
		}
	}

	if(iIndex >= DEF_MAX_REFERENCE_MODEL)
	{
		MyMessageBox("The total number of reference models that can be stored is 30.\r\n 저장할 수 있는 Reference Model의 총 갯수는 30개 입니다.", "Reference Model Create Error", M_ICONERROR);
		updateSelectModelReferenceListBox();
		return;
	}

	WritePrivateProfileString(REFERENCE_MODEL_SECTION, strDataName, strReferDataName[iIndex], strModelDataDir);
	
	updateSelectModelReferenceListBox();
	
	return ;
}

void CDataModelDlg::OnClickSelectDeviceModelDelete() 
{
	// TODO: Add your control notification handler code here
	if (MyMessageBox("Are you sure you want to delete the Reference Model?\r\n Reference Model을 삭제 하시 겠습니까?", "Delete Device Model", M_ICONQUESTION|M_YESNO) == IDNO) return;

	//2011.02.28 : Model별 Reference Model 추가
	int iIndex;
	m_lboxSelectModelReferenceList.ResetContent();
	
	CString strModelDataDir;
	CString strReferDataName[DEF_MAX_REFERENCE_MODEL];
	CString strDataName;

	WCHAR chrData[256];
    memset(chrData, NULL, sizeof(chrData));
	
	strModelDataDir.Format(_T(".\\Data\\ModelData\\%s\\Panel.dat"), (LPCTSTR)m_strSelectedModelName);
	for(iIndex=0; iIndex<DEF_MAX_REFERENCE_MODEL; iIndex++)
	{
		strDataName.Format(_T("ReferenceModel_%d"), iIndex);
		GetPrivateProfileString(REFERENCE_MODEL_SECTION, strDataName, _T(""), chrData, 256, strModelDataDir);
		strReferDataName[iIndex].Format(_T("%s"), chrData);
		strReferDataName[iIndex].MakeUpper();
	}

	for(iIndex=m_iCurrentSelForDeviceModelList; iIndex<DEF_MAX_REFERENCE_MODEL; iIndex++)
	{
		if(iIndex >= DEF_MAX_REFERENCE_MODEL-1)
		{
			strReferDataName[iIndex] = "";
			
		}
		else
		{
			strReferDataName[iIndex] = strReferDataName[iIndex+1];
		}
	}

	for(iIndex=0; iIndex<DEF_MAX_REFERENCE_MODEL; iIndex++)
	{
		strDataName.Format(_T("ReferenceModel_%d"), iIndex);
		WritePrivateProfileString(REFERENCE_MODEL_SECTION, strDataName, strReferDataName[iIndex], strModelDataDir);
	}

	updateSelectModelReferenceListBox();
	
	return ;
}

void CDataModelDlg::updateSelectModelReferenceListBox()
{
	int iIndex;

	m_lboxSelectModelReferenceList.ResetContent();

	//2011.02.28 : Model별 Reference Model 추가
	CString strModelDataDir;
	CString strReferDataName[DEF_MAX_REFERENCE_MODEL];
	CString strDataName;

	WCHAR chrData[256];
    memset(chrData, NULL, sizeof(chrData));
	
	strModelDataDir.Format(_T(".\\Data\\ModelData\\%s\\Panel.dat"), m_strSelectedModelName);
	for(iIndex=0; iIndex<DEF_MAX_REFERENCE_MODEL; iIndex++)
	{
		strDataName.Format(_T("ReferenceModel_%d"), iIndex);
		GetPrivateProfileString(REFERENCE_MODEL_SECTION, strDataName, _T(""), chrData, 256, strModelDataDir);
		strReferDataName[iIndex].Format(_T("%s"), chrData);
		strReferDataName[iIndex].MakeUpper();
		m_lboxSelectModelReferenceList.InsertString(-1, strReferDataName[iIndex]);
	}
}


void CDataModelDlg::OnModelModify()
{
	
	if (MyMessageBox("Do you want to change the model name of the selected model?\r\n선택된 모델의 모델명을 변경하시겠습니까?", "Rename Model", M_ICONQUESTION|M_YESNO) == IDNO) return;

	if (m_strSelectedModelName.Compare(m_strCurrentModelName) == 0)
	{
		MyMessageBox("Model names of current models can not be changed.\r\n현재 진행중인 Model의 모델명은 변경 할 수 없습니다.", "Model Delete Error", M_ICONERROR);
		return;
	}

	WCHAR buf[100];
	swprintf(buf, _T(""));

	WCHAR szTemp[100];
	memset(szTemp, 0, sizeof(szTemp));

	CString strOldFolderName;
	CString strNewFolderName;

	int iIndex;
	int iResult;

	if(!GetValueBox(szTemp, 20, buf, _T("Input the Modify Model name.")))
		return;

	for(iIndex=0; iIndex<DEF_MAX_MODEL; iIndex++)
	{
		if(m_strModelList[iIndex] == szTemp)
		{
			MyMessageBox(_T("The current model name exists.\r\n현재 변경한 모델명이 존재 합니다."), _T("Model Modify Error"), M_ICONERROR);
			return;
		}
	}

	strOldFolderName.Format(_T(".\\Data\\ModelData\\%s"), m_strSelectedModelName);
	strNewFolderName.Format(_T(".\\Data\\ModelData\\%s"), szTemp);

	iResult = _wrename(strOldFolderName, strNewFolderName);
	if(iResult)
	{
		MyMessageBox(_T("Failed to change model name.\r\n모델명을 변경하는데 실패하였습니다."), _T("Model Modify Error"), M_ICONERROR);
		return;
	}

	InitializeModelListBox();
}

//________________________________________________________



/* End Of File */

void CDataModelDlg::OnClickSelectDeviceModelSelect() 
{
	// TODO: Add your control notification handler code here
	
	CString strModelDataDir;
	CString strReferDataName[DEF_MAX_REFERENCE_MODEL];
	CString strDataName;
	CString strModelDeviceID;

	WCHAR chrData[256];
    memset(chrData, NULL, sizeof(chrData));
	
	strModelDataDir.Format(_T(".\\Data\\ModelData\\%s\\Panel.dat"), (LPCTSTR)m_strSelectedModelName);
	for(int iIndex=0; iIndex<DEF_MAX_REFERENCE_MODEL; iIndex++)
	{
		strDataName.Format(_T("ReferenceModel_%d"), iIndex);
		GetPrivateProfileString(REFERENCE_MODEL_SECTION, strDataName, _T(""), chrData, 256, strModelDataDir);
		strReferDataName[iIndex].Format(_T("%s"), chrData);
		strReferDataName[iIndex].MakeUpper();
	}

	strModelDeviceID = strReferDataName[m_iCurrentSelForDeviceModelList];
	MOlbSystemPre.GetTrsAutoManagerComponent()->m_strEquipModelName = strModelDeviceID;

	m_bReferanceSelect = TRUE;
	return;
	
}
