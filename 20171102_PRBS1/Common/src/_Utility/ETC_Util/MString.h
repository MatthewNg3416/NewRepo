#include "stdafx.h"
#include "stdarg.h"
#pragma once



class MString
{
public :

	static CString MFormat(LPCTSTR lpszFormat, ...);
	// wchar_t 에서 char 로의 형변환 함수
	static char* ConvertWideCharToChar(wchar_t* wstr, char *str, int iLength = NULL);
	// char 에서 wchar_t 로의 형변환 함수
	static wchar_t* ConvertCharToWideChar(char* str, wchar_t* wstr, int iLength = NULL);

	//포인터로 받아가지 말것!!! wchar_t 에서 char 로의 형변환 함수
	static const char* ConvertWideCharToChar(wchar_t* wstr);
	//포인터로 받아기지 말것!!! char 에서 wchar_t 로의 형변환 함수
	static const wchar_t* ConvertCharToWideChar(char* str);


};
