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
	//입력받은 wchar_t 변수의 길이를 구함
	int strSize = (iLength == NULL ? WideCharToMultiByte(CP_ACP, 0,wstr,-1, NULL, 0,NULL, NULL): iLength);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, strSize, 0,0); 	//형 변환 
	return str;
}

wchar_t* MString::ConvertCharToWideChar(char* str, wchar_t* wstr, int iLength)
{
	//멀티 바이트 크기 계산 길이 반환
	int strSize = (iLength == NULL ? MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL): iLength);
	MultiByteToWideChar(CP_ACP, 0, str, strlen(str)+1, wstr, strSize);	//형 변환
	return wstr;
}


// 이 함수는 재활용 불가 !! wchar_t 에서 char 로의 형변환 함수
const char* MString::ConvertWideCharToChar(wchar_t* wstr)
{
	static char strChar[10000] = {0, };
	return ConvertWideCharToChar(wstr,strChar);
}
// 이 함수는 재활용 불가 !! char 에서 wchar_t 로의 형변환 함수
const wchar_t* MString::ConvertCharToWideChar(char* str)
{
	static wchar_t strWChar[10000] = {0, };
	return ConvertCharToWideChar(str, strWChar);
}
//___________________________________