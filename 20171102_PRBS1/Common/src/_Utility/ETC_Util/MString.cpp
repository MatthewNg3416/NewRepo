#include "stdafx.h"
#include "MString.h"
#include <string.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CString MString::MFormat(LPCTSTR lpszFormat, ...)
{
	ASSERT(AfxIsValidString(lpszFormat));
	
	CString strTemp;

	va_list argList;
	va_start(argList, lpszFormat);
	strTemp.FormatV(lpszFormat, argList);
	va_end(argList);
	
	return strTemp;
}

char* MString::ConvertWideCharToChar(wchar_t* wstr, char *str, int iLength)
{
	//�Է¹��� wchar_t ������ ���̸� ����
	int strSize = (iLength == NULL ? WideCharToMultiByte(CP_ACP, 0,wstr,-1, NULL, 0,NULL, NULL): iLength);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, strSize, 0,0); 	//�� ��ȯ 
	return str;
}

wchar_t* MString::ConvertCharToWideChar(char* str, wchar_t* wstr, int iLength)
{
	//��Ƽ ����Ʈ ũ�� ��� ���� ��ȯ
	int strSize = (iLength == NULL ? MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL): iLength);
	MultiByteToWideChar(CP_ACP, 0, str, strlen(str)+1, wstr, strSize);	//�� ��ȯ
	return wstr;
}


// �� �Լ��� ��Ȱ�� �Ұ� !! wchar_t ���� char ���� ����ȯ �Լ�
const char* MString::ConvertWideCharToChar(wchar_t* wstr)
{
	static char strChar[10000] = {0, };
	return ConvertWideCharToChar(wstr,strChar);
}
// �� �Լ��� ��Ȱ�� �Ұ� !! char ���� wchar_t ���� ����ȯ �Լ�
const wchar_t* MString::ConvertCharToWideChar(char* str)
{
	static wchar_t strWChar[10000] = {0, };
	return ConvertCharToWideChar(str, strWChar);
}
//___________________________________