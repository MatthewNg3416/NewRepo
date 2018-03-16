#include "stdafx.h"
#include "stdarg.h"
#pragma once



class MString
{
public :

	static CString MFormat(LPCTSTR lpszFormat, ...);
	// wchar_t ���� char ���� ����ȯ �Լ�
	static char* ConvertWideCharToChar(wchar_t* wstr, char *str, int iLength = NULL);
	// char ���� wchar_t ���� ����ȯ �Լ�
	static wchar_t* ConvertCharToWideChar(char* str, wchar_t* wstr, int iLength = NULL);

	//�����ͷ� �޾ư��� ����!!! wchar_t ���� char ���� ����ȯ �Լ�
	static const char* ConvertWideCharToChar(wchar_t* wstr);
	//�����ͷ� �޾Ʊ��� ����!!! char ���� wchar_t ���� ����ȯ �Լ�
	static const wchar_t* ConvertCharToWideChar(char* str);


};
