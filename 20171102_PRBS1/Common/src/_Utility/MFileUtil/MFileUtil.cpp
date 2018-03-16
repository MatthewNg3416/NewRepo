/*
 * T7 ��� ��� SW ǥ��ȭ
 *
 * Copyright 2003 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung. */

/** 
     * MFileUtil.cpp : File ó���� ���� �������̽� Class�� ������ ���α׷��̴�.
     *                      
     * @author		����ö
     * @version		1.0  
     * @see		
     */

// MFileUtil.cpp: implementation of the MFileUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MFileUtil.h"
#include "MyProgressWnd.h"	// ��ü ������ ���α׷��� Class�̴�.
#include "common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
  * MFileUtil�� ������
  */
MFileUtil::MFileUtil()
{

}

/**
  * MFileUtil�� �Ҹ���
  */
MFileUtil::~MFileUtil()
{

}

/**
  * ������ Path�� ���丮�� �������� ���� ������ ���丮���� Ȯ���Ѵ�.
  *
  * @Param		pDir : Ȯ���� ���丮 Path
  * @return	    TRUE : �ٸ� ���丮 ���� ���� ����, FALSE : �ٸ� ���丮 ����	
  */
BOOL MFileUtil::IsLeafDir(LPCTSTR pDir)
{
	CFileFind	fn;
	CString		strFile;
	int			nextfile;

	strFile.Format(_T("%s\\*.*"),pDir);		// *.*�� �˻��Ѵ�.

	// Open A File Serach
	if (!fn.FindFile(strFile,0)) return FALSE;

	// ���丮���� �ش� ���ϵ��� ã�� �����Ѵ�.

	nextfile = fn.FindNextFile();	// ���� ������ ã�´�.
	while (TRUE)
	{
		// '.'�� '..'�� �ƴ� ���丮�� �����ϸ� FALSE�� �����Ѵ�. Leaf���丮�� �ƴ�
		if (fn.IsDirectory())
			if (!fn.IsDots()) return FALSE;
		if (!nextfile) break;	// �������� �����ߵ�� ������ ������.
		nextfile = fn.FindNextFile();	// ������ �˻��Ѵ�.
	}
	return TRUE;
}

/**
  * ���丮�� ���ϵ��� �����Ѵ�.
  * ���丮�� �������丮�� �������� �ʴ´�.
  * Progress Bar�� �پ ���� ����� ǥ���Ѵ�.
  *
  * @Param		pDir : ������ ���丮 Path
  * @return		TRUE/FALSE		
  */
int MFileUtil::DeleteFilesInDir(LPCTSTR pDir)
{

	CFileFind	fn;
	CString		strFile;
	int			filecnt=0;

	// Directory�� ���� ������ ���Ѵ�.
	if ((filecnt = GetFileCountInDir(pDir)) <= 0) return TRUE;	

	// Show Progress Bar
	CMyProgressWnd ProgWnd(NULL, _T("Delete the file in the directory....."));
	ProgWnd.SetRange(0,filecnt);
	//File�� ����� �ֽ��ϴ�...\n��ø� ��ٷ� �ּ���...
	CString szTextTemp1;
	CString szTextTemp2;
	ReadDisplayMessage(SUB_DLG, _T("MFileUtil_1"), &szTextTemp1);
	ReadDisplayMessage(SUB_DLG, _T("MFileUtil_wait"), &szTextTemp2);
	szTextTemp1 = szTextTemp1 + _T("\r\n") + szTextTemp2;
	ProgWnd.SetText(szTextTemp1);
	ProgWnd.PeekAndPump();

	// Path�� ��� ������ �˻��ϱ� ���� ���ϸ��� �����.
	strFile.Format(_T("%s\\*.*"),pDir);
	if (!fn.FindFile(strFile,0)) return FALSE;
	filecnt = fn.FindNextFile();
	while (TRUE)
	{
		// ���丮�� �ƴ� ���ϵ��� �����Ѵ�.
		if (!fn.IsDirectory())
		{
			strFile = fn.GetFilePath();
			CFile::Remove(strFile);
			ProgWnd.StepIt();
		}
		//Chang_
		else
		{
			strFile = fn.GetFilePath();
			RemoveDirectory(strFile);
		}

		if (!filecnt) break;
		filecnt = fn.FindNextFile();
	}

	return TRUE;
}

/**
  * ���丮�� ���� ���丮�� ������ ���Ѵ�.
  *
  * @Param		pDir : ������ ���丮 Path
  * @return		���� ���丮�� ����		
  */
int MFileUtil::GetFileCountInDir(LPCTSTR pDir)
{
	CFileFind	ff;
	int			cnt=0, nt;
	CString		strFile;

	strFile.Format(_T("%s\\*.*"),pDir);			// ���丮���� �����Ѵ�.
	if (!ff.FindFile(strFile,0)) return 0; // FileFind Open
	nt = ff.FindNextFile();					// ������ �˻��ϱ� �����Ѵ�.
	while (TRUE)
	{
		if (!ff.IsDirectory()) cnt++;		// ���丮�� �ƴϸ� ���� ���� ����
		if (!nt) break;					// �������� ���
		nt = ff.FindNextFile();				// ���� ������ ã�´�
	}

	return cnt;
}

/**
  * ���丮�� �����ϴ� ������ ������ ���Ѵ�.
  *
  * @Param		pDir      : ���丮��
  * @return		������ ����		
  */
int MFileUtil::GetDirCountInDir(LPCTSTR pDir)
{
	CFileFind	ff;
	int			cnt=0, nt;
	CString		strFind;

	strFind.Format(_T("%s\\*.*"),pDir);

	if (!ff.FindFile(strFind,0)) return 0;

	nt = ff.FindNextFile();
	while (TRUE)
	{
		if (ff.IsDirectory()) 
		{
			if (!ff.IsDots()) cnt++;
		}
		if (!nt) break;
		nt = ff.FindNextFile();
	}

	return cnt;
}

/**
  * ���丮�� ���ϵ��� �ٸ� ���丮�� �����Ѵ�.
  *
  * @author		����ö
  * @Param		pFile : �˻��� ���ϸ�
  * @return		
  * @exception	
  * @see
  * @version
  */
int MFileUtil::CopyFilesInDir(LPCTSTR pDestDir, LPCTSTR pSrcDir, BOOL bOverWrite)
{
	int			cnt=0;
	CFileFind	ff;
	CString		strSearchFile;	// File �˻��� ���� String
	CString		strSrcFile;		// �ҽ� ���� (Path ����)
	CString		strDestFile;	// ������ ���� (Path ����)

	cnt = GetFileCountInDir(pSrcDir);
	if (!cnt)	return (E_FU_NOFILE_IN_DIR);

	if (bOverWrite) DeleteFilesInDir(pDestDir);

	CMyProgressWnd ProgWnd(NULL, _T("File Copy....."));
	ProgWnd.SetRange(0,cnt);
	//File�� Copy�ϰ� �ֽ��ϴ�..\n��ø� ��ٷ� �ּ���...
	CString szTextTemp1;
	CString szTextTemp2;
	ReadDisplayMessage(SUB_DLG, _T("MFileUtil_2"), &szTextTemp1);
	ReadDisplayMessage(SUB_DLG, _T("MFileUtil_wait"), &szTextTemp2);
	szTextTemp1 = szTextTemp1 + _T("\r\n") + szTextTemp2;
	ProgWnd.SetText(szTextTemp1);
	ProgWnd.PeekAndPump();

	strSearchFile.Format(_T("%s\\*.*"),pSrcDir);
	if (!ff.FindFile(strSearchFile,0)) return (E_FU_FILE_NOT_FOUND);
	cnt = ff.FindNextFile();
	while (TRUE)
	{
		if (!ff.IsDirectory())
		{
			strSrcFile = ff.GetFilePath();
			strDestFile.Format(_T("%s\\%s"),pDestDir,ff.GetFileName());
			if (!CopyFile(strSrcFile, strDestFile, FALSE))
			{
				return (E_FU_FILE_COPY_FAIL);
			}
			ProgWnd.StepIt();
		}
		if (!cnt) break;
		cnt = ff.FindNextFile();
	}

	return 0;
}

/**
  * ������ �˻��Ѵ�.
  *
  * @author		����ö
  * @Param		pFile : �˻��� ���ϸ�
  * @return		
  * @exception	
  * @see
  * @version
  */
BOOL MFileUtil::IsFileExist(LPCTSTR pFile)
{
	CFileFind fn;

	if (!fn.FindFile(pFile,0)) return FALSE;
	else return TRUE;
}

/**
  * ���丮�� ���丮�� ����Ʈ�� �����Ѵ�.
  *
  * @author		����ö
  * @Param		pFile : �˻��� ���ϸ�
  * @return		
  * @exception	
  * @see
  * @version
  */
int MFileUtil::GetDirListInDir(LPCTSTR pDir,CString strDirName[], int count)
{
	CFileFind	ff;

	int			iCount=0;	// ������ ����,  Arguement Count ������ ��
	int			nt;			// ���� ������ ����
	CString		strFind;	// ���� �˻��� ���� String

	// ������ �˻��Ͽ� strDirName�� �ϳ��� ���� �ִ´�
	strFind.Format(_T("%s\\*.*"),pDir);
	if (!ff.FindFile(strFind,0)) return 0;
	nt = ff.FindNextFile();
	while (TRUE)
	{
		if (ff.IsDirectory())
		{
			if (!ff.IsDots())
			{
				// Argument�� �Էµ� �Ѱ踦 �Ѵ��� üũ�Ѵ�.
				if (iCount > (count-1)) break;

				strDirName[iCount] = ff.GetFileName();
				iCount++;
			}
		}
		if (!nt) break;
		nt = ff.FindNextFile();
	}
	return iCount;	// ���丮 ������ �����Ѵ�.
}


/**
  * ���丮�� ���丮�� ����Ʈ�� �����Ѵ�.
  *
  * @author		����ö
  * @Param		pFile : �˻��� ���ϸ�
  * @return		
  * @exception	
  * @see
  * @version
  */
int MFileUtil::GetDirListInDir(LPCTSTR pDir, CString strDirName[], int *count)
{
	CFileFind	ff;
	int			iMaxCount=256;	// ������ ����,  Arguement Count ������ ��
	int			nt;			// ���� ������ ����
	CString		strFind;	// ���� �˻��� ���� String
	int			iCount = 0;

	// ������ �˻��Ͽ� strDirName�� �ϳ��� ���� �ִ´�
	strFind.Format(_T("%s\\*.*"), pDir);
	if (!ff.FindFile(strFind, 0)) return 0;
	nt = ff.FindNextFile();
	while (TRUE)
	{
		if (ff.IsDirectory())
		{
			if (!ff.IsDots())
			{
				// Argument�� �Էµ� �Ѱ踦 �Ѵ��� üũ�Ѵ�.
				if (iCount > (iMaxCount - 1)) break;

				strDirName[iCount] = ff.GetFileName();
				iCount++;
			}
		}
		if (!nt) break;
		nt = ff.FindNextFile();
	}

	*count = iCount;
	return 0;	// ���丮 ������ �����Ѵ�.
}


/**
  * �������� �ʴ� ���丮�� ��� ���� �����Ѵ�.
  *
  * @author		����ö
  * @param		pDir : Path (���� ���� �Ǵ� ������)
  * @param		bIsIncludeFile : pDir�� ������ ���� �ϴ°� ?, Default TRUE
  * @return		TRUE : ����,  FALSE : ����
  * @see
  * @version
  */
BOOL MFileUtil::MakeDir(LPCTSTR pDir, BOOL bIsIncludeFile)
{
	CFileFind	fn;
	CString		Dir;
	CString		SubDir;
	CFile		file;
	CFileException	e;
	int			iIndex, vIndex;
	BOOL		bFirst = TRUE;

	Dir = pDir;
	iIndex = 0;
	vIndex = 0;

	while (TRUE)
	{
		iIndex = Dir.Find(_T("\\"),vIndex + 1);
		if (iIndex == -1) 
		{
			iIndex = Dir.Find(_T("/"), vIndex + 1);
			if (iIndex == -1)	break;
			else vIndex = iIndex;
		}
		else
			vIndex = iIndex;

		SubDir = Dir.Left(iIndex);
		if (bFirst && fn.FindFile(SubDir, 0))
		{
			bFirst = FALSE;

			fn.FindNextFile();
			if (!fn.IsDirectory())
			{
				return FALSE;
			}
		}
		else
		{
//			if (CreateDirectory(SubDir,NULL) == 0) return FALSE;
			CreateDirectory(SubDir, NULL);
		}
	}

#if 0
	if (!bIsIncludeFile)
	{
		if (fn.FindFile(Dir, 0))
		{
			fn.FindNextFile();
			if (fn.IsDirectory()) return TRUE;
			else return FALSE;
		}
		else
		{
			if (CreateDirectory(Dir, NULL) == 0) return FALSE;
			CreateDirectory(SubDir, NULL);
		}
	}
#endif
	return TRUE;
}

/* End Of Code */
