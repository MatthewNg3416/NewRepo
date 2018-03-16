#ifndef	DEF_TAB_LOSS_H
#define DEF_TAB_LOSS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//
#include "AdoControl.h"
#include "Afxmt.h"

/////////////////////////////////////////////////////////////////////////////
const	int	DEF_MAX_COLUMN_TABERR = 14;
//______________________________________________________________________

/*/
0:Time
1:TP(Tab Press)
2:CA1(Carrier1)
3:CA2
4:CA3
5:CA4
6:CA1D(Carrier1 Down)
7:CA2D
8:CA3D
9:CA4D
10:TM1(Tab Mount Front)
11:TM2
12:TM3
13:TM4
/*/

enum E_TABVAC_ERR_FIELD
{
	eTime				= 0,
	eTP					= 1,
	eCA1				= 2,
	eCA2				= 3,
	eCA3				= 4,
	eCA4				= 5,
	eCA1D				= 6,
	eCA2D				= 7,
	eCA3D				= 8,
	eCA4D				= 9,
	eTM1				= 10,
	eTM2				= 11,
	eTM3				= 12,
	eTM4				= 13,
};

class CDBTabLoss : public CAdoControl
{
private:
	CCriticalSection	m_csDBAccess;

	long		m_lCol_Field[DEF_MAX_COLUMN_TABERR];

	CString		m_strDBFilePath;
	CString		m_strOrgDBName;
	CString		m_strTableName;
	CString		m_strFieldName[DEF_MAX_COLUMN_TABERR];

	CString		m_strTime;
	int			m_iCurDay;
	int			m_iCurTime;

	int			m_iLogKeepingDays;
	int			m_iFieldCount[DEF_MAX_COLUMN_TABERR];
public:
	CDBTabLoss(CString strDBFilePath, CString strOrgDBName, CString strTableName, int iKeepingDays);
	virtual ~CDBTabLoss();

public:
	BOOL	IsFileExist(CString strFileName);
	BOOL	Initialize();

	BOOL	LoadData();
	BOOL	AppendFieldCount(E_TABVAC_ERR_FIELD eField);

	CString	GetPath();
	CString GetTableName();
	CString	GetFieldName(int iIndex);

	CString	GetFormatDateTime(CTime& referTime);
	CString GetFormatDate(CTime& referTime);
	CString GetFormatDate(COleDateTime& referTime);
	CString GetFormatTime(CTime& refetTime);

	void deleteOldFilesInFolder(int iKeepingDays);
	
};
//______________________________________________________________________

#endif // DEF_TAB_LOSS_H
