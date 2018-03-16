/*
 * OLB Inspection System
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
 * MFileUtil.h : File ó���� ���� Class�� ��� �����̴�.
 *                      
 * @author		����ö
 * @version		1.0  
 * @see		
 */

// MFileUtil.h: interface for the MFileUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFileUtil_H__12DDBE4E_C262_461F_A889_55C3A51319C7__INCLUDED_)
#define AFX_MFileUtil_H__12DDBE4E_C262_461F_A889_55C3A51319C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Error Code Define
#define E_FU_FILE_NOT_FOUND	-1
#define E_FU_FILE_COPY_FAIL	-2
#define E_FU_NOFILE_IN_DIR	-3

/**
 * File ����,����, ���� ���ϱ� ���� Uitlity���� �����ϴ� Class�̴�. 
 */
class MFileUtil  
{
public:
	/**
	 * MFileUtil�� ������
	 */
	MFileUtil();			// ������

	/**
	 * MFileUtil�� �Ҹ���
	 */
	virtual ~MFileUtil();	// �Ҹ���

	/**
	 * ������ Path�� ���丮�� �������� ���� ������ ���丮���� Ȯ���Ѵ�.
	 *
	 * @Param		pDir : Ȯ���� ���丮 Path
	 * @return	    TRUE : �ٸ� ���丮 ���� ���� ����, FALSE : �ٸ� ���丮 ����	
	 */
	BOOL IsLeafDir(LPCTSTR pDir);

	/**
	 * ���丮�� ���ϵ��� �����Ѵ�.
	 * ���丮�� �������丮�� �������� �ʴ´�.
	 * Progress Bar�� �پ ���� ����� ǥ���Ѵ�.
	 *
	 * @Param		pDir : ������ ���丮 Path
	 * @return		TRUE/FALSE		
	 */
	int	DeleteFilesInDir(LPCTSTR pDir);

	/**
	 * ���丮�� �����ϴ� ������ ������ ���Ѵ�.
	 *
	 * @Param		pDir      : ���丮��
	 * @return		������ ����		
	 */
	int	GetFileCountInDir(LPCTSTR pDir);

	/**
	 * ���丮�� ���� ���丮�� ������ ���Ѵ�.
	 *
	 * @Param		pDir : ������ ���丮 Path
	 * @return		���� ���丮�� ����		
	 */
	int	GetDirCountInDir(LPCTSTR pDir);

	/**
	 * ���丮�� ���ϵ��� �����Ѵ�.
	 *
	 * @Param		pDest      : ������ ���丮
	 * @Param		pSrc       : ���� ���丮
	 * @Param		bOverWrite : OverWrite ���� (TRUE/FALSSE) 
	 * @return		���� ���丮�� ����		
	 */
	int	CopyFilesInDir(LPCTSTR pDest,LPCTSTR pSrc,BOOL bOverWrite);

	/**
	 * ���丮�� ������ ���� �ϴ��� �˻��Ѵ�.
	 *
	 * @Param		pFile      : �˻��ϰ��� �ϴ� ���ϸ� (Path ����)
	 * @return		TRUE / FALSE		
	 */
	BOOL IsFileExist(LPCTSTR pFile);

	/**
	 * ���丮�� ���丮�� ����Ʈ�� �����.
	 *
	 * @Param		pDir      : ���丮��
	 * @Param		strDirname[] : string�� �迭
	 * @Param		iCount		: ����		
	 * @return		SUCCESS / ERROR CODE
	 */
	int	GetDirListInDir(LPCTSTR pDir,CString strDirName[], int iCount);

	/**
	 * ���丮 �� ���丮�� ����Ʈ�� ã�´�.
	 *
	 * @Param		pDir      : ���丮��
	 * @Param		strDirname[] : string�� �迭
	 * @Param		iCount		: ����		
	 * @return		SUCCESS / ERROR CODE
	 */
	int	GetDirListInDir(LPCTSTR pDir,CString strDirName[], int *iCount);	
	
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
	BOOL MakeDir(LPCTSTR pDir,BOOL bIsIncludeFile = TRUE);
};

#endif // !defined(AFX_MFileUtil_H__12DDBE4E_C262_461F_A889_55C3A51319C7__INCLUDED_)