#include "stdafx.h"
#include "MDataDouble.h"
#include "common.h"


void MDataDouble::InputTool()
{
	WCHAR m_cBuf[60], m_cTmpBuf[60];
	swprintf(m_cBuf, _T("%.4f"), m_Data);
	if (!GetNumberBox(m_cTmpBuf, 10, m_cBuf))
		return;

	SetData(wcstod(m_cTmpBuf, NULL));
}

void MDataInt::InputTool()
{
	WCHAR m_cBuf[60], m_cTmpBuf[60];
	swprintf(m_cBuf, _T("%d"), m_Data);
	if (!GetNumberBox(m_cTmpBuf, 10, m_cBuf))
		return;

	SetData(_wtoi(m_cTmpBuf));
}

void MDataBool::InputTool()
{
	if (m_Data == true)
		SetData(false);
	else
		SetData(true);
}

void MDataCString::InputTool()
{
	WCHAR buf[100];
	wsprintf(buf, _T(""));

	WCHAR szTemp[100];
	memset(szTemp, 0, sizeof(szTemp));
	wsprintf(szTemp, _T("%s"), m_Data);
	
	GetValueBox(szTemp, 30, buf, _T("Input the Modify Model name."));

	CString str;
	str.Format(_T("%s"), buf);

	SetData(str);
}