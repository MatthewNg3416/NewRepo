#include "stdafx.h"
#include "MDAConverterData.h"
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
MDAConverterData::MDAConverterData(CString strSection, CString strFileName) : MFileManager(strFileName)
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
MDAConverterData::MDAConverterData(CString strSection, CString strFileName, CString strPath) : MFileManager(strFileName, strPath)
{
	m_strSection = strSection;	// Section�� ����
	Initialize();				// ��� ���� �ʱ�ȭ
}

/**
 * @stereotype destructor 
 */
MDAConverterData::~MDAConverterData()
{
}

/** 
 * DAConverter Data�� �ʱ�ȭ�Ѵ�.
 *
 * @return	void
 */
void MDAConverterData::Initialize()
{
	m_dOffset = 0.0;
	m_dSlant = 0.0;
}

/** 
 * DAConverter Data�� File���� �о�´�.
 *
 * @return	BOOL (TRUE = Success, FALSE = Fail)
 */
BOOL MDAConverterData::ReadData()
{
	BOOL ret = TRUE;

	if (!GetValue(m_strSection, "Offset", &m_dOffset))
		DISPLAY_READ_ERROR(m_strSection, "Offset", ret);

	if (!GetValue(m_strSection, "Slant", &m_dSlant))
		DISPLAY_READ_ERROR(m_strSection, "Slant", ret);

	return ret;
}

/** 
 * DAConverter Data�� File�� �����Ѵ�.
 *
 * @return	BOOL (TRUE = Success, FALSE = Fail)
 */
BOOL MDAConverterData::WriteData()
{
	if (!MakeBackUpFile())
	{
		
		//CString strMsg = ML_MSG("Back Up File�� ����µ� ���� �߽��ϴ�. ��� ���� �ϰڽ��ϱ�?",
		//	"It was failed to make backup file. Go ahead?");
		//if (MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), "File Write Error", M_ICONERROR|M_YESNO) == IDNO) 
		if (MyMessageBox(MY_MESSAGE_BOX, 62000, _T("File Write Error"), M_ICONERROR|M_YESNO) == IDNO) 

			return FALSE;
	}

	BOOL ret = TRUE;

	if (!SetValue(m_strSection, "Offset", m_dOffset))
		DISPLAY_WRITE_ERROR(m_strSection, "Offset", ret);

	if (!SetValue(m_strSection, "Slant", m_dSlant))
		DISPLAY_WRITE_ERROR(m_strSection, "Slant", ret);

	return ret;
}
/* End Of Code */