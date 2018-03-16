#include "stdafx.h"
#include "MCylinderData.h"
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
MCylinderData::MCylinderData(CString strSection, CString strFileName) : MFileManager(strFileName)
{
	m_strSection = strSection;	// Section명 설정
	Initialize();				// 멤버 변수 초기화
}

/**
 * @stereotype constructor	
 *
 * @param	CString strSection (Section Name)
 * @param	CString strFileName (File Name)
 * @param	CString strPath (File Path)
 */
MCylinderData::MCylinderData(CString strSection, 
							 CString strFileName,
							 CString strPath) : MFileManager(strFileName, strPath)
{
	m_strSection = strSection;	// Section명 설정
	Initialize();				// 멤버 변수 초기화
}

/**
 * @stereotype destructor 
 */
MCylinderData::~MCylinderData()
{
}

/** 
 * Cylinder Data를 초기화한다.
 *
 * @return	void
 */
void MCylinderData::Initialize()
{
	m_dMovingTime		= 0.0;
    m_dSettlingTime1	= 0.0;
	m_dSettlingTime2	= 0.0;
    m_dNoSenMovingTime	= 0.0;
	m_dSimulationTime	= 0.0;
}

/** 
 * Cylinder Data를 File에서 읽어온다.
 *
 * @return	BOOL (TRUE = Success, FALSE = Fail)
 */
BOOL MCylinderData::ReadData()
{
	BOOL ret = TRUE;

	if (!GetValue(m_strSection, _T("MovingTime"), &m_dMovingTime))
		DISPLAY_READ_ERROR(m_strSection, "MovingTime", ret);

	if (!GetValue(m_strSection, _T("SettlingTime1"), &m_dSettlingTime1))
		DISPLAY_READ_ERROR(m_strSection, "SettlingTime1", ret);

	if (!GetValue(m_strSection, _T("SettlingTime2"), &m_dSettlingTime2))
		DISPLAY_READ_ERROR(m_strSection, "SettlingTime2", ret);

	if (!GetValue(m_strSection, _T("NoSenMovingTime"), &m_dNoSenMovingTime))
		DISPLAY_READ_ERROR(m_strSection, "NoSenMovingTime", ret);

#ifdef SIMULATION
	if (!GetValue(m_strSection, _T("SimulationTime"), &m_dSimulationTime))
	{
//@		DISPLAY_READ_ERROR(m_strSection, "SimulationTime", ret);
		m_dSimulationTime = 0.1;
	}

	if (m_dSimulationTime <= 0.1)
		m_dSimulationTime = 0.1;
#endif

	return ret;
}

/** 
 * Cylinder Data를 File에 저장한다.
 *
 * @return	BOOL (TRUE = Success, FALSE = Fail)
 */
BOOL MCylinderData::WriteData()
{
	if (!MakeBackUpFile())
	{
		//CString strMsg = ML_MSG("Back Up File을 만드는데 실패 했습니다. 계속 진행 하겠습니까?",
		//	"It was failed to make backup file. Go ahead?");
		//if (MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), "File Write Error", M_ICONERROR|M_YESNO) == IDNO) 
		//MDAConverterData.cpp (62000)과 같은 내용이라 중복 사용
		if (MyMessageBox(MY_MESSAGE_BOX, 62000, _T("File Write Error"), M_ICONERROR|M_YESNO) == IDNO)
		
			return FALSE;
	}

	BOOL ret = TRUE;

	if (!SetValue(m_strSection, _T("MovingTime"), m_dMovingTime))
		DISPLAY_WRITE_ERROR(m_strSection, "MovingTime", ret);

	if (!SetValue(m_strSection, _T("SettlingTime1"), m_dSettlingTime1))
		DISPLAY_WRITE_ERROR(m_strSection, "SettlingTime1", ret);

	if (!SetValue(m_strSection, _T("SettlingTime2"), m_dSettlingTime2))
		DISPLAY_WRITE_ERROR(m_strSection, "SettlingTime2", ret);

	if (!SetValue(m_strSection, _T("NoSenMovingTime"), m_dNoSenMovingTime))
		DISPLAY_WRITE_ERROR(m_strSection, "NoSenMovingTime", ret);

#ifdef SIMULATION
	if (!SetValue(m_strSection, _T("SimulationTime"), m_dSimulationTime))
		DISPLAY_WRITE_ERROR(m_strSection, "SimulationTime", ret);
#endif

	return ret;
}

/* End Of Code */