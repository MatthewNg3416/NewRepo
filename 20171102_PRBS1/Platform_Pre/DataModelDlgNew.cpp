// DataModelDlgNewNew.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "DataModelDlgNew.h"
#include "CommonGetValueDlg.h"
#include "MPlatformOlbSystem.h"
#include "MLCNet.h"
#include "MTrsAutoManager.h"
#include "DataPanelDlg.h"
#include "DataTabICDlg.h"
//#include "DataACFLengthDlg.h"
//110114.kms__________
#include "CommonGetValueDlg.h"
#include "DataModelIDList.h"
#include "DataSearchMajorSpec.h"

extern MPlatformOlbSystem	MOlbSystemPre;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataModelDlgNew dialog


CDataModelDlgNew::CDataModelDlgNew(CWnd* pParent /*=NULL*/)
	: CDialog(CDataModelDlgNew::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataModelDlgNew)
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_bReferanceSelect = FALSE;
}


void CDataModelDlgNew::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataModelDlgNew)
	DDX_Control(pDX, IDC_MODEL_LIST, m_lboxModelList);
	DDX_Control(pDX, IDC_MODEL_LIST2, m_lboxModelList2);
	DDX_Control(pDX, IDS_CURRENT_MODEL, m_lblCurrentModel);
	DDX_Control(pDX, IDS_SELECT_MODEL, m_lblSelectModel);
	DDX_Control(pDX, IDB_SELECT_DEVICE_MODEL_CREATE, m_btnReferenceOfSelectCreateModel);
	DDX_Control(pDX, IDB_SELECT_DEVICE_MODEL_DELETE, m_btnReferenceOfSelectDeleteModel);
	DDX_Control(pDX, IDC_SELECT_REFERENCE_MODEL_LIST, m_lboxSelectModelReferenceList);
	DDX_Control(pDX, IDB_MODEL_MODIFY, m_btnModelModify);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataModelDlgNew, CDialog)
	//{{AFX_MSG_MAP(CDataModelDlgNew)
	ON_WM_PAINT()
	ON_LBN_SELCHANGE(IDC_MODEL_LIST, OnSelchangeModelList)
	ON_LBN_SELCHANGE(IDC_MODEL_LIST2, OnSelchangeModelList2)
	ON_LBN_SELCHANGE(IDC_SELECT_REFERENCE_MODEL_LIST, OnSelchangeSelectReferenceModelList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataModelDlgNew message handlers

void CDataModelDlgNew::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(),&m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CDataModelDlgNew::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitializeModelListBox();

	CString szTextTemp;
	//�� ����Ʈ
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlgNew_1"), &szTextTemp);
	GetDlgItem(IDC_LIST)->SetWindowText(szTextTemp);
	//���� ���� ���� ��
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlgNew_2"), &szTextTemp);
	GetDlgItem(IDC_CURRENT_MODEL)->SetWindowText(szTextTemp);
	//���õ� ��
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlgNew_3"), &szTextTemp);
	GetDlgItem(IDC_SELECT_MODEL)->SetWindowText(szTextTemp);
	//��
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlgNew_4"), &szTextTemp);
	GetDlgItem(IDB_UP)->SetWindowText(szTextTemp);
	//�Ʒ�
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlgNew_5"), &szTextTemp);
	GetDlgItem(IDB_DOWN)->SetWindowText(szTextTemp);
	//������\r\n����
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlgNew_6"), &szTextTemp);
	GetDlgItem(IDB_PAGE_UP)->SetWindowText(szTextTemp);
	//������\r\n�Ʒ���
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlgNew_7"), &szTextTemp);
	GetDlgItem(IDB_PAGE_DOWN)->SetWindowText(szTextTemp);
	//�� ����
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlgNew_8"), &szTextTemp);
	GetDlgItem(IDB_MODEL_CREATE)->SetWindowText(szTextTemp);
	//�� ����
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlgNew_9"), &szTextTemp);
	GetDlgItem(IDB_MODEL_DELETE)->SetWindowText(szTextTemp);
	//�� ����
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlgNew_10"), &szTextTemp);
	GetDlgItem(IDB_MODEL_LOAD)->SetWindowText(szTextTemp);
	//������
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlgNew_11"), &szTextTemp);
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);
	//���� �������� �� 
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlgNew_13"), &szTextTemp);
	GetDlgItem(IDC_CURRENT_MODEL2)->SetWindowText(szTextTemp);
	//�� ����
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlgNew_14"), &szTextTemp);
	GetDlgItem(IDB_MODEL_MODIFY)->SetWindowText(szTextTemp);
	//Data ��ȸ
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlgNew_15"), &szTextTemp);
	GetDlgItem(IDB_SEARCH_DATA)->SetWindowText(szTextTemp);
	//���� Device ID List
	ReadDisplayMessage(MAIN_DATA, _T("DataModelDlgNew_16"), &szTextTemp);
	GetDlgItem(IDC_CURRENT_DEVICE_ID)->SetWindowText(szTextTemp);	
	
	
	//101004.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() == AUTORUN_MODE)
	{
//		GetDlgItem(IDB_MODEL_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDB_MODEL_CREATE)->EnableWindow(FALSE);
		GetDlgItem(IDB_MODEL_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDB_MODEL_LOAD)->EnableWindow(FALSE);
		GetDlgItem(IDB_MODEL_MODIFY)->EnableWindow(FALSE);

		GetDlgItem(IDB_SELECT_DEVICE_MODEL_CREATE)->EnableWindow(FALSE);
		GetDlgItem(IDB_SELECT_DEVICE_MODEL_DELETE)->EnableWindow(FALSE);
	}
	else
	{
//2011.04.11 KWC		GetDlgItem(IDB_PANELDATA)->ShowWindow(SW_HIDE);
//2011.04.11 KWC		GetDlgItem(IDB_ACFDATA)->ShowWindow(SW_HIDE);

		if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
			OnDisplayOperater(); 
		else
			OnDisplayEngMaker(); 
	}
	//_______________

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CDataModelDlgNew, CDialog)
    //{{AFX_EVENTSINK_MAP(CDataModelDlgNew)
	ON_EVENT(CDataModelDlgNew, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CDataModelDlgNew, IDB_MODEL_CREATE, -600 /* Click */, OnModelCreate, VTS_NONE)
	ON_EVENT(CDataModelDlgNew, IDB_MODEL_DELETE, -600 /* Click */, OnModelDelete, VTS_NONE)
	ON_EVENT(CDataModelDlgNew, IDB_MODEL_LOAD, -600 /* Click */, OnModelLoad, VTS_NONE)
	ON_EVENT(CDataModelDlgNew, IDB_UP, -600 /* Click */, OnUp, VTS_NONE)
	ON_EVENT(CDataModelDlgNew, IDB_DOWN, -600 /* Click */, OnDown, VTS_NONE)
	ON_EVENT(CDataModelDlgNew, IDB_PAGE_UP, -600 /* Click */, OnPageUp, VTS_NONE)
	ON_EVENT(CDataModelDlgNew, IDB_PAGE_DOWN, -600 /* Click */, OnPageDown, VTS_NONE)
	ON_EVENT(CDataModelDlgNew, IDB_PANELDATA, -600 /* Click */, OnClickPaneldata, VTS_NONE)
	ON_EVENT(CDataModelDlgNew, IDB_SELECT_DEVICE_MODEL_CREATE, -600 /* Click */, OnClickSelectDeviceModelCreate, VTS_NONE)
	ON_EVENT(CDataModelDlgNew, IDB_SELECT_DEVICE_MODEL_DELETE, -600 /* Click */, OnClickSelectDeviceModelDelete, VTS_NONE)
	ON_EVENT(CDataModelDlgNew, IDB_MODEL_MODIFY, -600 /* Click */, OnClickModelModify, VTS_NONE)
	ON_EVENT(CDataModelDlgNew, IDB_SELECT_DEVICE_MODEL_SELECT, -600 /* Click */, OnClickSelectDeviceModelSelect, VTS_NONE)
	ON_EVENT(CDataModelDlgNew, IDB_SEARCH_DATA, -600 /* Click */, OnClickSearchData, VTS_NONE)
	ON_EVENT(CDataModelDlgNew, IDB_TABDATA, -600 /* Click */, OnClickTabdata, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDataModelDlgNew::OnExit() 
{
	// TODO: Add your control notification handler code here
	/*
	if(m_bReferanceSelect == FALSE && MOlbSystemPre.GetTrsAutoManagerComponent()->m_strEquipModelName == "")
	{
		MyMessageBox(MY_MESSAGE_BOX, 44006, _T("Model create Error"), M_ICONERROR);

		return;
	}
	*/
	CDialog::OnCancel();
}

int CDataModelDlgNew::InitializeModelListBox()
{
	int		i;
	CString	strMsg;

	m_iCurrentSel = -1;

	m_iCurrentSelForDeviceModelList = 0;

	m_strCurrentModelName = MOlbSystemPre.GetCurrentModelName();

	CString tmpCurrentModelDevice;
	//tmpCurrentModelDevice = m_strCurrentModelName+_T("\n(")+MOlbSystemPre.GetTrsAutoManagerComponent()->m_strEquipModelName+_T(")");
	tmpCurrentModelDevice = m_strCurrentModelName+_T("\n(")+MOlbSystemPre.GetSystemData()->m_strCurrentDeviceID+_T(")");
	m_lblCurrentModel.SetCaption(tmpCurrentModelDevice);

	MOlbSystemPre.GetModelNameList(m_strModelList, &m_iModelCount);

	CString strModelDataDir;
	int iTempDevice;
	m_iDeviceIDCount = 0;
	for (i=0; i<m_iModelCount; i++)
	{
		strModelDataDir.Format(_T(".\\Data\\ModelData\\%s\\"), m_strModelList[i]);
		MOlbSystemPre.GetDeviceIDList(strModelDataDir, m_strDeviceIdList, &iTempDevice);	//��ο��� ���� Search
		for (int j=0; j<iTempDevice; j++)
		{
			m_sDeviceIDModel[m_iDeviceIDCount].strDeviceID = m_strDeviceIdList[j];
			m_sDeviceIDModel[m_iDeviceIDCount].strModel = m_strModelList[i];
			m_iDeviceIDCount++;
		}
	}

	//����
	SDeviceIDModel tmpDV;
	for (int u = 0; u < m_iDeviceIDCount; u++)
	{
		for (int v = 0; v < m_iDeviceIDCount; v++)
		{
			if (m_sDeviceIDModel[u].strDeviceID.Compare(m_sDeviceIDModel[v].strDeviceID)<0)
			{
				tmpDV = m_sDeviceIDModel[u];
				m_sDeviceIDModel[u] = m_sDeviceIDModel[v];
				m_sDeviceIDModel[v] = tmpDV;
			}
		}
	}

	if (m_iModelCount <= 0)
	{
		//��ϵ� ���� �ϳ��� �����ϴ�.
		MyMessageBox(MY_MESSAGE_BOX, 44000, _T("Manage Model"), M_ICONERROR);

		m_lblSelectModel.SetCaption(_T(""));
		m_lblCurrentModel.SetCaption(_T(""));
		return -1;
	}

	for (i=0;i<m_iDeviceIDCount;i++)
	{
		if (!_tcscmp(MOlbSystemPre.GetTrsAutoManagerComponent()->m_strEquipModelName,m_sDeviceIDModel[i].strDeviceID))
		{
			m_iCurrentSel = i;
		}
	}

	if (m_iCurrentSel < 0)
	{
		CString szTextTemp1;
		CString szTextTemp2;
		CString szTextTemp;
		//���� ��[%s] ���丮�� �������� �ʽ��ϴ�.
//		ReadDisplayMessage(SUB_DLG, _T("DataModelDlgNew_12a"), &szTextTemp1);
//		ReadDisplayMessage(SUB_DLG, _T("DataModelDlgNew_12b"), &szTextTemp2);
//		szTextTemp.Format(_T("%s [%s] %s"),szTextTemp1,m_strCurrentModelName,szTextTemp2);
//		MyMessageBox(szTextTemp, _T("Manage Model"), M_ICONERROR);

		//������� ������ DeviceID�� �����Ѵ�.
		for (int j=0; j<m_iDeviceIDCount; j++)
		{
			if (!_tcscmp(m_strCurrentModelName,m_sDeviceIDModel[j].strModel))
			{
				m_iCurrentSel = j;
				break;
			}
		}
		if (m_iCurrentSel < 0)
			m_iCurrentSel = 0;
//		return	-1;
	}

	m_lboxModelList.ResetContent();
	m_lboxModelList2.ResetContent();

	//110114.kms_______________
	//Reference Model �����Ͽ� Model Load â�� Display �ϴ� �κ�
	/*
	for (i = 0 ; i < m_iModelCount ;i++ )
	{
		m_strModelList[i].MakeUpper();
		m_lboxModelList.InsertString(-1,(LPCTSTR)m_strModelList[i]);	
	}
	*/
	CString strPanelDataDir;
	CString strReferDataName;

    WCHAR chrData[256];
    memset(chrData, NULL, sizeof(chrData));


	for (i = 0 ; i < m_iDeviceIDCount ;i++)
	{
		strPanelDataDir.Format(_T(".\\Data\\ModelData\\%s\\Panel.dat"), (LPCTSTR)m_strModelList[i]);
	
		GetPrivateProfileString(_T("Panel Data"), _T("Reference_Data"), _T("ERROR"), chrData, 256, strPanelDataDir);

//		strReferDataName.Format(_T("%s <==> %s"), (LPCTSTR)m_strModelList[i], chrData);
		strReferDataName.Format(_T("%s"), (LPCTSTR)m_sDeviceIDModel[i].strDeviceID);
		strReferDataName.MakeUpper();
		m_lboxModelList.InsertString(-1, strReferDataName);	
	}
	//_______________________________

	CString tmpModelName;
	for (i=0; i< m_iModelCount; i++)
	{
		tmpModelName.Format(_T("%s"), (LPCTSTR)m_strModelList[i]);
		tmpModelName.MakeUpper();
		m_lboxModelList2.InsertString(-1, tmpModelName);	
		if (!lstrcmp(m_strCurrentModelName,tmpModelName))
		{
			m_iCurrentSel2 = i;
		}
	}
	
	updateModelListBox();
	//110304.kms________
	updateSelectModelReferenceListBox();
	//__________________
	updateModelListBox2();

	return 0;
}

void CDataModelDlgNew::updateModelListBox()
{
	m_lboxModelList.SetCurSel(m_iCurrentSel);
	//110114.kms_________
	//m_lboxModelList.GetText(m_iCurrentSel,m_strSelectedModelName);

	if (m_iCurrentSel != -1)
		m_strSelectedModelName = m_sDeviceIDModel[m_iCurrentSel].strModel;
	else
		m_strSelectedModelName = m_strModelList[m_iCurrentSel2];

	if ( !m_iModelCount ) 
		m_lblSelectModel.SetCaption(_T(""));
	else 
		m_lblSelectModel.SetCaption(m_strSelectedModelName);
}

void CDataModelDlgNew::updateModelListBox2()
{
	m_lboxModelList2.SetCurSel(m_iCurrentSel2);

	m_strSelectedModelName2 = m_strModelList[m_iCurrentSel2];
}

void CDataModelDlgNew::OnModelCreate() 
{
	// TODO: Add your control notification handler code here
	WCHAR buf[100];
	CString strTemp;
	int iResult;

	swprintf(buf, _T(""));

	//110114.kms_____
	WCHAR szTemp[100];
	memset(szTemp, 0, sizeof(szTemp));
	//_______________

///	CCommonGetValueDlg	valueDialog(20, buf, _T("Input the Model name."));
///	valueDialog.DoModal();
	if(!GetValueBox(szTemp, 30, buf, _T("Input the Model name.")))
	{
		return;
	}

///	strTemp = valueDialog.GetStringValue();

	CString tmpModel;
	tmpModel = szTemp;
	for (int i=0; i<m_iDeviceIDCount; i++)
	{
		if (m_sDeviceIDModel[i].strModel.Compare(tmpModel) == 0)
		{
			//���� �̸��� Model�� ���� �մϴ�. �ٸ� �̸����� �����ϼ���?
			MyMessageBox(MY_MESSAGE_BOX, 44001, _T("Model create Error"), M_ICONERROR);
			
			return;		
		}
	}
/*
	if ( m_lboxModelList.FindStringExact(-1, m_szTmp) != LB_ERR )
	{
		//���� �̸��� Model�� ���� �մϴ�. �ٸ� �̸����� �����ϼ���?
		MyMessageBox(MY_MESSAGE_BOX, 44001, _T("Model create Error"), M_ICONERROR);

		return;
	}
*/
	//101004.KKY_____
	m_strSelectedModelName.TrimLeft();
	m_strSelectedModelName.TrimRight();
	if (m_strSelectedModelName == _T(""))
	{
		MyMessageBox(MY_MESSAGE_BOX, 44006, _T("Model Creation Error"), M_ICONERROR);
		return;
	}
	//_______________

	//101004.KKY_____
//	iResult = MOlbSystemPre.CreateModel(strTemp, m_strCurrentModelName);	
///	iResult = MOlbSystemPre.CreateModel(strTemp, m_strSelectedModelName);	
	iResult = MOlbSystemPre.CreateModel(szTemp, m_strSelectedModelName);
	//_______________
	
// KWC �ӽ� 	if ( iResult != ERR_SYSTEM_SUCCESS)
 	if ( iResult != 0)
	{
		//Model Directory�� �����ϴµ� ���� �Ͽ���.
		MyMessageBox(MY_MESSAGE_BOX, 44002,_T("Model create Error"),M_ICONERROR);

		return;
	}

	InitializeModelListBox();	// ���� �߰� �Ǿ����ϱ� List�� �ʱ�ȭ �ϰ�
}

void CDataModelDlgNew::OnModelDelete() 
{
	// TODO: Add your control notification handler code here
	//���õ� Model �� �����Ͻðڽ��ϱ�?
	int iReturn = MyMessageBox(MY_MESSAGE_BOX, 44003, _T("Model Delete"), M_ICONQUESTION|M_YESNO);

	if (iReturn == IDNO)
		return;

	iReturn = MyMessageBox("If you delete the model, the reference information will be deleted. Do you want to proceed?\r\n�� ������ �����ϸ� Reference�������� �����˴ϴ�. �����Ͻðڽ��ϱ�?", _T("Model Delete"), M_ICONQUESTION|M_YESNO);

	if (iReturn == IDNO)
		return;

	if ( m_strSelectedModelName.Compare(m_strCurrentModelName) == 0 )
	{
		//���� �������� Model�� ���� �� �� �����ϴ�.
		MyMessageBox(MY_MESSAGE_BOX, 44004, _T("Model Delete Error"), M_ICONERROR);

		return;
	}

	MOlbSystemPre.DeleteModel(m_strSelectedModelName);

	InitializeModelListBox();	// ����� ���丮�� ListBox�� �ݿ��Ѵ�.
}

void CDataModelDlgNew::OnModelLoad() 
{
	// TODO: Add your control notification handler code here
	//���ο� Model�� �ҷ����ðڽ��ϱ�?
	if (m_strCurrentModelName.Compare(m_strSelectedModelName)==0)
	{
		MOlbSystemPre.GetTrsAutoManagerComponent()->m_strEquipModelName = m_sDeviceIDModel[m_iCurrentSel].strDeviceID;
		//111205.kms___________
		MOlbSystemPre.GetSystemData()->m_strCurrentDeviceID = m_sDeviceIDModel[m_iCurrentSel].strDeviceID;
		MOlbSystemPre.GetSystemData()->WriteData();
		Sleep(100);
		// AutoView DeviceID Name Display
		::SendMessage(MOlbSystemPre.GetTrsAutoManagerComponent()->GetAutoViewHandle(), WM_DISP_MODEL_DEVICEID, NULL, NULL);
		//_____________________

		//110304.kms________
		updateSelectModelReferenceListBox();
		//__________________
		MyMessageBox("Since it is the same model, only Referernce is changed\r\n���� ���̹Ƿ� Referernce�� �����մϴ�..", "Info", M_ICONERROR);
		CString tmpCurrentModelDevice;
		tmpCurrentModelDevice = m_strCurrentModelName+_T("\n(")+MOlbSystemPre.GetTrsAutoManagerComponent()->m_strEquipModelName+_T(")");
		m_lblCurrentModel.SetCaption(tmpCurrentModelDevice);
		return;
	}

	if ( MyMessageBox(MY_MESSAGE_BOX, 44005, _T("OK"), M_ICONQUESTION|M_YESNO) == IDNO) return;

	// NSMC
	CMyProgressWnd ProgWnd(NULL, _T("Model Load..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("Model Loading...\nWait..."));
	ProgWnd.PeekAndPump();
	ProgWnd.StepIt();

/*
//110114.KMS________________
// MIMS ���� �Ͽ� �۾��Ҷ�  <==> �� �����Ͽ� �Ĵܱ��� ���� ���� ������ MODEL CHANGE �� ���� �ʴ� ��. 
//����
	m_iCurrentSel = m_lboxModelList.GetCurSel();
	m_lboxModelList.GetText(m_iCurrentSel, m_strSelectedModelName);
	m_lblSelectModel.SetCaption(m_strSelectedModelName);
*/
	CString strOldModelName	= m_strCurrentModelName;		// ���� ���� �� ����
	m_strCurrentModelName	= m_strSelectedModelName;		// ���� ���� �� ������Ʈ

	//111205.kms___________
	MOlbSystemPre.GetSystemData()->m_strCurrentDeviceID = m_sDeviceIDModel[m_iCurrentSel].strDeviceID;
	MOlbSystemPre.GetSystemData()->WriteData();
	//_____________________
	

	int iReturn = MOlbSystemPre.ChangeModel(m_strCurrentModelName);	// System�� ���� �ٲٶ� ��� �ϰ�
	if (iReturn)
	{
		// NSMC
		ProgWnd.DestroyWindow();

		CString strError = MOlbSystemPre.GetErrorMessage(iReturn);
		MyMessageBox(strError, _T("Model Change Error"), M_ICONERROR, _T("Confirm"),_T("Cancel"));

		MOlbSystemPre.ChangeModel(strOldModelName);
		return;
		// Model Change ���� �� Error �߻����� ���Ͽ� Model Change �� ���������� �Ϸ���� �ʴ���,
		// ���� Model �� ���� ���õ� Model �� �����ؾ���.
	}

	// NSMC
	ProgWnd.DestroyWindow();

	updateModelListBox();									// Dialog�� ListBox�� �ʱ�ȭ �Ѵ�.

	MOlbSystemPre.GetLCNetComponent()->ModelDataReport(m_strCurrentModelName);

	// NSMC
	CTime t = CTime::GetCurrentTime();
	WCHAR dest[256];
	swprintf(dest, _T("C:\\DataBackup\\%02d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	MFileManager::CreateBackupFolder(dest, _T(".\\Data"), TRUE);

	MOlbSystemPre.GetTrsAutoManagerComponent()->m_strEquipModelName = m_sDeviceIDModel[m_iCurrentSel].strDeviceID;
	//110304.kms________
	updateSelectModelReferenceListBox();
	//__________________

	CString tmpCurrentModelDevice;
	tmpCurrentModelDevice = m_strCurrentModelName+_T("\n(")+MOlbSystemPre.GetTrsAutoManagerComponent()->m_strEquipModelName+_T(")");
	m_lblCurrentModel.SetCaption(tmpCurrentModelDevice);

//	MyMessageBox(_T("Set Tab EndLine / SR Line use and reference value of Tab Data.\r\nTab Data�� Tab EndLine / SR Line ��� ������ ���ذ� Setting �� �ּ���."), _T("Information"), M_ICONINFORMATION); //170926 JSh Del...
}

void CDataModelDlgNew::OnUp() 
{
	// TODO: Add your control notification handler code here
	if (m_iModelCount <= 0 ) return;

	if ( m_iCurrentSel <= 0 ) return;
	else m_iCurrentSel--;

	m_lboxModelList.SetCurSel(m_iCurrentSel);
	m_lboxModelList.GetText(m_iCurrentSel,m_strSelectedModelName);
	m_lblSelectModel.SetCaption(m_strSelectedModelName);

	updateModelListBox();
	//110304.kms________
	updateSelectModelReferenceListBox();
	//__________________
	updateModelListBox2();
}

void CDataModelDlgNew::OnDown() 
{
	// TODO: Add your control notification handler code here
	if (m_iModelCount <= 0 ) return;

	if ( m_iCurrentSel >= (m_iDeviceIDCount-1) ) return;
	else m_iCurrentSel++;
	
	m_lboxModelList.SetCurSel(m_iCurrentSel);
	m_lboxModelList.GetText(m_iCurrentSel,m_strSelectedModelName);
	m_lblSelectModel.SetCaption(m_strSelectedModelName);

	updateModelListBox();
	//110304.kms________
	updateSelectModelReferenceListBox();
	//__________________
	updateModelListBox2();
}

void CDataModelDlgNew::OnPageUp() 
{
	// TODO: Add your control notification handler code here
	if (m_iModelCount <= 0 ) return;

	if ( m_iCurrentSel - 10 <= 0 ) m_iCurrentSel = 0;
	else m_iCurrentSel -= 10;
	
	m_lboxModelList.SetCurSel(m_iCurrentSel);
	m_lboxModelList.GetText(m_iCurrentSel,m_strSelectedModelName);
	m_lblSelectModel.SetCaption(m_strSelectedModelName);

	updateModelListBox();
	//110304.kms________
	updateSelectModelReferenceListBox();
	//__________________	
	updateModelListBox2();
}

void CDataModelDlgNew::OnPageDown() 
{
	// TODO: Add your control notification handler code here
	if (m_iModelCount <= 0 ) return;

	if ( m_iCurrentSel + 10 >= (m_iDeviceIDCount-1) ) m_iCurrentSel = m_iDeviceIDCount-1;
	else m_iCurrentSel += 10;
	
	m_lboxModelList.SetCurSel(m_iCurrentSel);
	m_lboxModelList.GetText(m_iCurrentSel,m_strSelectedModelName);
	m_lblSelectModel.SetCaption(m_strSelectedModelName);	

	updateModelListBox();
	//110304.kms________
	updateSelectModelReferenceListBox();
	//__________________
	updateModelListBox2();
}

void CDataModelDlgNew::OnSelchangeModelList() 
{
	// TODO: Add your control notification handler code here
	m_iCurrentSel = m_lboxModelList.GetCurSel();
	m_lboxModelList.SetCurSel(m_iCurrentSel);
	//110114.kms________
//	m_lboxModelList.GetText(m_iCurrentSel, m_strSelectedModelName);
	m_strSelectedModelName = m_sDeviceIDModel[m_iCurrentSel].strModel;
	//__________________
	m_lblSelectModel.SetCaption(m_strSelectedModelName);	

	for (int i=0; i<m_iModelCount; i++)
	{
		if (m_strSelectedModelName.Compare(m_strModelList[i]) == 0)
		{
			m_iCurrentSel2 = i;
			break;
		}
	}

	updateModelListBox();
	//110304.kms________
	updateSelectModelReferenceListBox();
	updateModelListBox2();
}

void CDataModelDlgNew::OnSelchangeModelList2() 
{
	m_iCurrentSel2 = m_lboxModelList2.GetCurSel();

	m_lboxModelList2.SetCurSel(m_iCurrentSel2);
	//110114.kms________
//	m_lboxModelList.GetText(m_iCurrentSel, m_strSelectedModelName);
	//Chang_
	m_strSelectedModelName = m_strModelList[m_iCurrentSel2];
	//__________________
	m_lblSelectModel.SetCaption(m_strSelectedModelName);

	//Chang_
	m_iCurrentSel = -1;
	for (int i=0; i<m_iDeviceIDCount; i++)
	{
		if (m_strSelectedModelName.Compare(m_sDeviceIDModel[i].strModel) == 0)
		{
			m_iCurrentSel = i;
			break;
		}
	}

	updateModelListBox();
	updateSelectModelReferenceListBox();
	updateModelListBox2();
}

int CDataModelDlgNew::ModelLoad(CString strModelName) 
{
	// TODO: Add your control notification handler code here
	int iResult;
	CString strOldModelName	= MOlbSystemPre.GetCurrentModelName();		// ���� ���� �� ����
	m_strCurrentModelName	= strModelName;		// ���� ���� �� ������Ʈ

	iResult = MOlbSystemPre.ChangeModel(m_strCurrentModelName);		// System�� ���� �ٲٶ� ��� �ϰ�
	if ( iResult != SUCCESS )
	{
		MOlbSystemPre.ChangeModel(strOldModelName);
		return -1;
	}
	return SUCCESS;
}

//101004.KKY_______
void CDataModelDlgNew::OnClickPaneldata() 
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

//_________________

//Chang_
void CDataModelDlgNew::OnSelchangeSelectReferenceModelList() 
{
	// TODO: Add your control notification handler code here
	m_iCurrentSelForDeviceModelList = m_lboxSelectModelReferenceList.GetCurSel();
	CString tmpModel;
	tmpModel = m_strDeviceIdList[m_iCurrentSelForDeviceModelList];

	for (int i=0; i<m_iDeviceIDCount; i++)
	{
		if (m_sDeviceIDModel[i].strDeviceID.Compare(tmpModel) == 0)
		{
			m_iCurrentSel = i;
			break;
		}
	}

	updateModelListBox();
	updateSelectModelReferenceListBox();
}

//Chang_
void CDataModelDlgNew::OnClickSelectDeviceModelCreate() 
{
	// TODO: Add your control notification handler code here
	CString strFullPath = MOlbSystemPre.GetSystemData()->m_strModelFileDir;// + "\\" + m_plnkSystemData->m_strModelFileName;

	if (_T("NONE") == m_strSelectedModelName)
	{
		MyMessageBox("Model not selected.", "Error", M_ICONERROR);
		return;
	}

	WCHAR buf[100];
	swprintf(buf, _T(""));

	WCHAR szTemp[100];
	memset(szTemp, 0, sizeof(szTemp));

	if(!GetValueBox(szTemp, 30, buf, _T("Device ID���� �Է��ϼ���.(Input the Device ID Name.)")))
		return;

	if (m_lboxModelList.FindStringExact(-1, szTemp) != LB_ERR)
	{
		//MyMessageBox(MY_MESSAGE_BOX, 40602, _T("Device ID Creation Error"), M_ICONERROR);	//�޼��� �߰� �ʿ�
		MyMessageBox(_T("Device ID with the same name exists. Please save as a different name.\r\n���� �̸��� Device ID�� ���� �մϴ�. �ٸ� �̸����� �����ϼ���."), _T("Device ID Creation Error"), M_ICONERROR);
		return;
	}

	m_lboxSelectModelReferenceList.ResetContent();
	
	CString strCreateDeviceID;
	strCreateDeviceID = szTemp;

	CString strTemp;
	strTemp.Format(_T("[Create] %s"), strCreateDeviceID);
	AfxMessageBox(strTemp);

	CString strCreateDeviceIDPath;
	strCreateDeviceIDPath.Format(_T(".\\Data\\ModelData\\%s\\%s"), m_strSelectedModelName, strCreateDeviceID);

	CString szTextTemp1;
	CString szTextTemp2;

	CFileFind finder;
	if (!finder.FindFile(strCreateDeviceIDPath))
	{
		if (!CreateDirectory(strCreateDeviceIDPath, NULL))
		{
			//ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_64506"), &szTextTemp1);	//�޼��� �߰� �ʿ�
			//szTextTemp2.Format(_T("[%s] %s"), strCreateDeviceIDPath, szTextTemp1);
			szTextTemp2.Format(_T("[%s] Device ID ���丮�� ������ �� �����ϴ�.\nIt can't create [%s] Device ID Directory"), strCreateDeviceIDPath, strCreateDeviceIDPath);
			MyMessageBox(szTextTemp2, _T("Manage Device ID"), M_ICONERROR);			
		}
	}

	InitializeModelListBox();
	return ;
}

//Chang_
void CDataModelDlgNew::OnClickSelectDeviceModelDelete() 
{
	// TODO: Add your control notification handler code here
	
	CString strDeleteDeviceID;
	strDeleteDeviceID = m_sDeviceIDModel[m_iCurrentSel].strDeviceID;

	CString strTemp;
	strTemp.Format(_T("Are you sure you want to delete [Device ID] %s?\r\n[Device ID] %s�� �����Ͻðڽ��ϱ�?"), strDeleteDeviceID,strDeleteDeviceID);

	if (MyMessageBox(strTemp, "Delete Device ID", M_ICONQUESTION|M_YESNO) == IDNO) 
		return;

	CString strDeleteDeviceIDPath;
	strDeleteDeviceIDPath.Format(_T(".\\Data\\ModelData\\%s\\%s"), m_strSelectedModelName, strDeleteDeviceID);
	MOlbSystemPre.DeleteDeviceID(strDeleteDeviceIDPath);

	m_lboxSelectModelReferenceList.ResetContent();

	updateSelectModelReferenceListBox();

	InitializeModelListBox();

	return ;
}

void CDataModelDlgNew::updateSelectModelReferenceListBox()
{
	//Chang_
	m_lboxSelectModelReferenceList.ResetContent();
	CString strModelDataDir;
	int iTempDevice;
	strModelDataDir.Format(_T(".\\Data\\ModelData\\%s\\"), m_strSelectedModelName);
	MOlbSystemPre.GetDeviceIDList(strModelDataDir, m_strDeviceIdList, &iTempDevice);	//��ο��� ���� Search
	m_lboxSelectModelReferenceList.ResetContent();

	for (int iIndex = 0; iIndex < iTempDevice; iIndex++)
	{
		m_strDeviceIdList[iIndex].MakeUpper();
		m_lboxSelectModelReferenceList.InsertString(-1, m_strDeviceIdList[iIndex]);
	}

	CString strDeviceID;
	strDeviceID = m_sDeviceIDModel[m_iCurrentSel].strDeviceID;
	int iIndex;
	for (iIndex = 0; iIndex < iTempDevice; iIndex++)
	{
		if (strDeviceID.Compare(m_strDeviceIdList[iIndex]) == 0)
		{
			m_lboxSelectModelReferenceList.SetCurSel(iIndex);
			break;
		}
	}
	if (iIndex == iTempDevice)
		m_lboxSelectModelReferenceList.SetCurSel(-1);
}

//________________________________________________________

void CDataModelDlgNew::OnClickModelModify() 
{
	// TODO: Add your control notification handler code here
	
	if (MyMessageBox("Do you want to change the model name of the selected model?\r\n���õ� ���� �𵨸��� �����Ͻðڽ��ϱ�?", "Rename Model", M_ICONQUESTION|M_YESNO) == IDNO) return;

	if (m_strSelectedModelName.Compare(m_strCurrentModelName) == 0)
	{
		MyMessageBox("Model names of current models can not be changed.\r\n���� �������� Model�� �𵨸��� ���� �� �� �����ϴ�.", "Model Delete Error", M_ICONERROR);
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

	if(!GetValueBox(szTemp, 30, buf, _T("Input the Modify Model name.")))
		return;

	for(iIndex=0; iIndex<DEF_MAX_MODEL; iIndex++)
	{
		if(m_strModelList[iIndex] == szTemp)
		{
			MyMessageBox(_T("The current model name exists.\r\n���� ������ �𵨸��� ���� �մϴ�."), _T("Model Modify Error"), M_ICONERROR);
			return;
		}
	}

	strOldFolderName.Format(_T(".\\Data\\ModelData\\%s"), m_strSelectedModelName);
	strNewFolderName.Format(_T(".\\Data\\ModelData\\%s"), szTemp);

	iResult = _wrename(strOldFolderName, strNewFolderName);
	if(iResult)
	{
		MyMessageBox(_T("Failed to change model name.\r\n�𵨸��� �����ϴµ� �����Ͽ����ϴ�."), _T("Model Modify Error"), M_ICONERROR);
		return;
	}

	InitializeModelListBox();	
}

void CDataModelDlgNew::OnClickSelectDeviceModelSelect() 
{
	// TODO: Add your control notification handler code here
	//Chang_
	CString strModelDeviceID;
	strModelDeviceID = m_sDeviceIDModel[m_iCurrentSel].strDeviceID;
	int i;
	for (i=0; i<m_iDeviceIDCount; i++)
	{
		if (m_sDeviceIDModel[i].strModel.Compare(m_strCurrentModelName) == 0
			&& m_sDeviceIDModel[i].strDeviceID.Compare(strModelDeviceID) == 0)
			break;
	}
	if (i==m_iDeviceIDCount)
	{
		MyMessageBox(_T("The selected reference does not match the current model.\r\n������ Reference�� ���� ���� Matching���� �ʽ��ϴ�.."), _T("Model Modify Error"), M_ICONERROR);
		return;
	}

	MOlbSystemPre.GetTrsAutoManagerComponent()->m_strEquipModelName = strModelDeviceID;

	CString strTemp;
	strTemp.Format(_T("[Select] %s"), strModelDeviceID);
	AfxMessageBox(strTemp);

	CString tmpCurrentModelDevice;
	tmpCurrentModelDevice = m_strCurrentModelName+_T("\n(")+MOlbSystemPre.GetTrsAutoManagerComponent()->m_strEquipModelName+_T(")");
	m_lblCurrentModel.SetCaption(tmpCurrentModelDevice);

	m_bReferanceSelect = TRUE;
	return;
}

void CDataModelDlgNew::OnClickSearchData() 
{
	// TODO: Add your control notification handler code here
	CDataSearchMajorSpec dlg(MOlbSystemPre.GetTrsAutoManagerComponent()->m_strEquipModelName, m_sDeviceIDModel[m_iCurrentSel].strDeviceID);
	dlg.DoModal();
}

void CDataModelDlgNew::OnClickTabdata() 
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


void CDataModelDlgNew::OnDisplayOperater() 
{
	GetDlgItem(IDB_MODEL_CREATE)->EnableWindow(FALSE);
	GetDlgItem(IDB_MODEL_DELETE)->EnableWindow(FALSE);
	GetDlgItem(IDB_MODEL_LOAD)->EnableWindow(TRUE);
	GetDlgItem(IDB_MODEL_MODIFY)->EnableWindow(FALSE);

	GetDlgItem(IDB_SELECT_DEVICE_MODEL_CREATE)->EnableWindow(FALSE);
	GetDlgItem(IDB_SELECT_DEVICE_MODEL_DELETE)->EnableWindow(FALSE);
}
void CDataModelDlgNew::OnDisplayEngMaker() 
{
	GetDlgItem(IDB_MODEL_CREATE)->EnableWindow(TRUE);
	GetDlgItem(IDB_MODEL_DELETE)->EnableWindow(TRUE);
	GetDlgItem(IDB_MODEL_LOAD)->EnableWindow(TRUE);
	GetDlgItem(IDB_MODEL_MODIFY)->EnableWindow(TRUE);

	GetDlgItem(IDB_SELECT_DEVICE_MODEL_CREATE)->EnableWindow(TRUE);
	GetDlgItem(IDB_SELECT_DEVICE_MODEL_DELETE)->EnableWindow(TRUE);
}
