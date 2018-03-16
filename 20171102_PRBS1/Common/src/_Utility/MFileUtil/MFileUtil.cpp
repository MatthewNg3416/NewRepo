/*
 * T7 모듈 장비 SW 표준화
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
     * MFileUtil.cpp : File 처리를 위한 인터페이스 Class를 구현한 프로그램이다.
     *                      
     * @author		정종철
     * @version		1.0  
     * @see		
     */

// MFileUtil.cpp: implementation of the MFileUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MFileUtil.h"
#include "MyProgressWnd.h"	// 자체 제작한 프로그래스 Class이다.
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
  * MFileUtil의 생성자
  */
MFileUtil::MFileUtil()
{

}

/**
  * MFileUtil의 소멸자
  */
MFileUtil::~MFileUtil()
{

}

/**
  * 지정한 Path가 디렉토리를 포함하지 않은 최하위 디렉토리인지 확인한다.
  *
  * @Param		pDir : 확인할 디렉토리 Path
  * @return	    TRUE : 다른 디렉토리 포함 하지 않음, FALSE : 다른 디렉토리 포함	
  */
BOOL MFileUtil::IsLeafDir(LPCTSTR pDir)
{
	CFileFind	fn;
	CString		strFile;
	int			nextfile;

	strFile.Format(_T("%s\\*.*"),pDir);		// *.*로 검색한다.

	// Open A File Serach
	if (!fn.FindFile(strFile,0)) return FALSE;

	// 디렉토리에서 해당 파일들을 찾기 시작한다.

	nextfile = fn.FindNextFile();	// 다음 파일을 찾는다.
	while (TRUE)
	{
		// '.'나 '..'이 아닌 디렉토리를 포함하면 FALSE를 리턴한다. Leaf디렉토리가 아님
		if (fn.IsDirectory())
			if (!fn.IsDots()) return FALSE;
		if (!nextfile) break;	// 마지막에 도달했드면 루프를 빠진다.
		nextfile = fn.FindNextFile();	// 다음을 검색한다.
	}
	return TRUE;
}

/**
  * 디렉토리내 파일들을 삭제한다.
  * 디렉토리내 하위디렉토리는 삭제하지 않는다.
  * Progress Bar를 뛰어서 진행 경과를 표시한다.
  *
  * @Param		pDir : 삭제할 디렉토리 Path
  * @return		TRUE/FALSE		
  */
int MFileUtil::DeleteFilesInDir(LPCTSTR pDir)
{

	CFileFind	fn;
	CString		strFile;
	int			filecnt=0;

	// Directory내 파일 갯수를 구한다.
	if ((filecnt = GetFileCountInDir(pDir)) <= 0) return TRUE;	

	// Show Progress Bar
	CMyProgressWnd ProgWnd(NULL, _T("Delete the file in the directory....."));
	ProgWnd.SetRange(0,filecnt);
	//File을 지우고 있습니다...\n잠시만 기다려 주세요...
	CString szTextTemp1;
	CString szTextTemp2;
	ReadDisplayMessage(SUB_DLG, _T("MFileUtil_1"), &szTextTemp1);
	ReadDisplayMessage(SUB_DLG, _T("MFileUtil_wait"), &szTextTemp2);
	szTextTemp1 = szTextTemp1 + _T("\r\n") + szTextTemp2;
	ProgWnd.SetText(szTextTemp1);
	ProgWnd.PeekAndPump();

	// Path내 모들 파일을 검색하기 위한 파일명을 만든다.
	strFile.Format(_T("%s\\*.*"),pDir);
	if (!fn.FindFile(strFile,0)) return FALSE;
	filecnt = fn.FindNextFile();
	while (TRUE)
	{
		// 디렉토리가 아닌 파일들을 삭제한다.
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
  * 디렉토리내 하위 디렉토리의 갯수를 구한다.
  *
  * @Param		pDir : 삭제할 디렉토리 Path
  * @return		하위 디렉토리의 갯수		
  */
int MFileUtil::GetFileCountInDir(LPCTSTR pDir)
{
	CFileFind	ff;
	int			cnt=0, nt;
	CString		strFile;

	strFile.Format(_T("%s\\*.*"),pDir);			// 디렉토리명을 설정한다.
	if (!ff.FindFile(strFile,0)) return 0; // FileFind Open
	nt = ff.FindNextFile();					// 파일을 검색하기 시작한다.
	while (TRUE)
	{
		if (!ff.IsDirectory()) cnt++;		// 디렉토리가 아니면 파일 갯수 증가
		if (!nt) break;					// 마지막일 경우
		nt = ff.FindNextFile();				// 다음 파일을 찾는다
	}

	return cnt;
}

/**
  * 디렉토리에 존재하는 파일의 갯수를 구한다.
  *
  * @Param		pDir      : 디렉토리명
  * @return		파일의 갯수		
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
  * 디렉토리내 파일들을 다른 디렉토리로 복사한다.
  *
  * @author		정종철
  * @Param		pFile : 검색할 파일명
  * @return		
  * @exception	
  * @see
  * @version
  */
int MFileUtil::CopyFilesInDir(LPCTSTR pDestDir, LPCTSTR pSrcDir, BOOL bOverWrite)
{
	int			cnt=0;
	CFileFind	ff;
	CString		strSearchFile;	// File 검색을 위한 String
	CString		strSrcFile;		// 소스 파일 (Path 포함)
	CString		strDestFile;	// 목적지 파일 (Path 포함)

	cnt = GetFileCountInDir(pSrcDir);
	if (!cnt)	return (E_FU_NOFILE_IN_DIR);

	if (bOverWrite) DeleteFilesInDir(pDestDir);

	CMyProgressWnd ProgWnd(NULL, _T("File Copy....."));
	ProgWnd.SetRange(0,cnt);
	//File을 Copy하고 있습니다..\n잠시만 기다려 주세요...
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
  * 파일을 검색한다.
  *
  * @author		정종철
  * @Param		pFile : 검색할 파일명
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
  * 디렉토리내 디렉토리의 리스트를 리턴한다.
  *
  * @author		정종철
  * @Param		pFile : 검색할 파일명
  * @return		
  * @exception	
  * @see
  * @version
  */
int MFileUtil::GetDirListInDir(LPCTSTR pDir,CString strDirName[], int count)
{
	CFileFind	ff;

	int			iCount=0;	// 리턴할 갯수,  Arguement Count 이하의 값
	int			nt;			// 남은 파일의 갯수
	CString		strFind;	// 파일 검색을 위한 String

	// 파일을 검색하여 strDirName에 하나씩 집어 넣는다
	strFind.Format(_T("%s\\*.*"),pDir);
	if (!ff.FindFile(strFind,0)) return 0;
	nt = ff.FindNextFile();
	while (TRUE)
	{
		if (ff.IsDirectory())
		{
			if (!ff.IsDots())
			{
				// Argument로 입력된 한계를 넘는지 체크한다.
				if (iCount > (count-1)) break;

				strDirName[iCount] = ff.GetFileName();
				iCount++;
			}
		}
		if (!nt) break;
		nt = ff.FindNextFile();
	}
	return iCount;	// 디렉토리 갯수를 리턴한다.
}


/**
  * 디렉토리내 디렉토리의 리스트를 리턴한다.
  *
  * @author		정종철
  * @Param		pFile : 검색할 파일명
  * @return		
  * @exception	
  * @see
  * @version
  */
int MFileUtil::GetDirListInDir(LPCTSTR pDir, CString strDirName[], int *count)
{
	CFileFind	ff;
	int			iMaxCount=256;	// 리턴할 갯수,  Arguement Count 이하의 값
	int			nt;			// 남은 파일의 갯수
	CString		strFind;	// 파일 검색을 위한 String
	int			iCount = 0;

	// 파일을 검색하여 strDirName에 하나씩 집어 넣는다
	strFind.Format(_T("%s\\*.*"), pDir);
	if (!ff.FindFile(strFind, 0)) return 0;
	nt = ff.FindNextFile();
	while (TRUE)
	{
		if (ff.IsDirectory())
		{
			if (!ff.IsDots())
			{
				// Argument로 입력된 한계를 넘는지 체크한다.
				if (iCount > (iMaxCount - 1)) break;

				strDirName[iCount] = ff.GetFileName();
				iCount++;
			}
		}
		if (!nt) break;
		nt = ff.FindNextFile();
	}

	*count = iCount;
	return 0;	// 디렉토리 갯수를 리턴한다.
}


/**
  * 존재하지 않는 디렉토리의 경우 새로 생성한다.
  *
  * @author		정종철
  * @param		pDir : Path (파일 포함 또는 미포함)
  * @param		bIsIncludeFile : pDir에 파일을 포함 하는가 ?, Default TRUE
  * @return		TRUE : 성공,  FALSE : 실패
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
