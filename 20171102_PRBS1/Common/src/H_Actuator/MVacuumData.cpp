#include "stdafx.h"
#include "MVacuumData.h"
#include "common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/**
 * @stereotype constructor	
 *
 * @param	CString strSection (Section Name)
 * @param	CString strFileName (File Name)
 */
MVacuumData::MVacuumData(CString strSection, CString strFileName) : MFileManager(strFileName)
{
	m_strSection = strSection;	// Section�� ����
	Initialize();				// ��� ���� �ʱ�ȭ
}

/**
 * @stereotype constructor	
 *
 * @param	CString strSection (Section Name)
 * @param	CString strFileName (File Name)
 * @param	CString strPath (File Path)
 */
MVacuumData::MVacuumData(CString strSection, 
						 CString strFileName, 
						 CString strPath) : MFileManager(strFileName, strPath)
{
	m_strSection = strSection;	// Section�� ����
	Initialize();				// ��� ���� �ʱ�ȭ
}

/**
 * @stereotype destructor 
 */
MVacuumData::~MVacuumData()
{
}

/** 
 * Vacuum Data�� �ʱ�ȭ�Ѵ�.
 *
 * @return	void
 */
void MVacuumData::Initialize()
{
	m_dTurningTime		= 0.0;
    m_dOnSettlingTime	= 0.0;
	m_dOffSettlingTime	= 0.0;
	m_dSimulationTime   = 0.0;
}

/** 
 * Vacuum Data�� File���� �о�´�.
 *
 * @return	BOOL (TRUE = Success, FALSE = Fail)
 */
BOOL MVacuumData::ReadData()
{
	BOOL ret = TRUE;

	if (!GetValue(m_strSection, _T("TurningTime"), &m_dTurningTime))
		DISPLAY_READ_ERROR(m_strSection, "TurningTime", ret);

	if (!GetValue(m_strSection, _T("OnSettlingTime"), &m_dOnSettlingTime))
		DISPLAY_READ_ERROR(m_strSection, "OnSettlingTime", ret);

	if (!GetValue(m_strSection, _T("OffSettlingTime"), &m_dOffSettlingTime))
		DISPLAY_READ_ERROR(m_strSection, "OffSettlingTime", ret);

#ifdef SIMULATION
	if (!GetValue(m_strSection, _T("SimulationTime"), &m_dSimulationTime))
//@		DISPLAY_READ_ERROR(m_strSection, "SimulationTime", ret);
		m_dSimulationTime = 0.5;
#endif

	return ret;
}

/** 
 * Vacuum Data�� File�� �����Ѵ�.
 *
 * @return	BOOL (TRUE = Success, FALSE = Fail)
 */
BOOL MVacuumData::WriteData()
{
	if (!MakeBackUpFile())
	{
		//CString strMsg = ML_MSG("Back Up File�� ����µ� ���� �߽��ϴ�. ��� ���� �ϰڽ��ϱ�?",
		//	"It was failed to make backup file. Go ahead?");
		//if (MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), _T("File Write Error"), M_ICONERROR|M_YESNO) == IDNO) 
		// MDAConverterData.cpp (62000)�� ���� �����̶� �ߺ� ���
		if (MyMessageBox(MY_MESSAGE_BOX, 62000, _T("File Write Error"), M_ICONERROR|M_YESNO) == IDNO)
			return FALSE;
	}

	CMyProgressWnd ProgWnd(NULL, _T("Vacuum Data File Writing..."));
	ProgWnd.SetRange(0, 3);
	ProgWnd.SetText(_T("It is Writing the Vacuum Data.\n")
					    _T("Please wait a moment..."));
	ProgWnd.PeekAndPump();

	BOOL ret = TRUE;

	if (!SetValue(m_strSection, _T("TurningTime"), m_dTurningTime))
		DISPLAY_WRITE_ERROR(m_strSection, "TurningTime", ret);

	ProgWnd.StepIt();

	if (!SetValue(m_strSection, _T("OnSettlingTime"), m_dOnSettlingTime))
		DISPLAY_WRITE_ERROR(m_strSection, "OnSettlingTime", ret);

	ProgWnd.StepIt();

	if (!SetValue(m_strSection, _T("OffSettlingTime"), m_dOffSettlingTime))
		DISPLAY_WRITE_ERROR(m_strSection, "OffSettlingTime", ret);

	ProgWnd.StepIt();

#ifdef SIMULATION
	if (!SetValue(m_strSection, _T("SimulationTime"), m_dSimulationTime))
		DISPLAY_WRITE_ERROR(m_strSection, "SimulationTime", ret);
#endif

	return ret;
}

/* End Of Code */