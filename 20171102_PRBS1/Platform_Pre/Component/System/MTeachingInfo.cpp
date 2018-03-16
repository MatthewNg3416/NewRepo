/* 
 * System Info 
 *
 * Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Smasung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * MTeachingInfo.h : This file defines object type of System.
 *            
 *
 */

#include "stdafx.h"

#include "MTeachingInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

MTeachingInfo::MTeachingInfo(CString strFileName)
{
	WCHAR	chPath[256];
	CString strPath;
	CString strFile;
	GetCurrentDirectory(256, chPath);
	
	strPath.Format(_T("%s"), chPath);
	m_strFile = strPath + "\\Info\\" + strFileName;
}

MTeachingInfo::~MTeachingInfo()
{
}

int MTeachingInfo::LoadTeachingInfo()
{
	CString strLogMsg;
	FILE	*filePtr;
	char	strBuffer[_MAX_PATH];
	int		ch;
	CString	strFormattedMessage;
	CString	strType;

	int	iTemp1, iTemp2, iTemp, iUnitNo, iPosNo;
	CString strTemp;

    /* Open for read (will fail if file "data" does not exist) */
	if ((filePtr  = _wfopen(m_strFile.GetBuffer(m_strFile.GetLength()), _T("r"))) == NULL)	return -1;

	// reset number of units and number of teaching pos

	while (!feof(filePtr))
	{
		memset(strBuffer, 0x00, sizeof(strBuffer));

		ch = fgetc(filePtr);
		for (int i=0; (ch != '\n') & (feof(filePtr) == 0); i++)
		{
			strBuffer[i] = (char)ch;
			ch = fgetc(filePtr);
		}

		strFormattedMessage = strBuffer;
		strFormattedMessage.TrimLeft();
		strFormattedMessage.TrimRight();

		int n = strFormattedMessage.Find(';',0);
		if (n != -1) 
			continue;

		int iType = strFormattedMessage.Find(',',0);
		strType = strFormattedMessage.Left(iType);
		strType.TrimLeft();
		strType.TrimRight();
		if (!wcscmp(strType.GetBuffer(strType.GetLength()), _T("UNIT")))
		{
			// Unit No
			iTemp1 = strFormattedMessage.Find(',', iType+1);
			strTemp = strFormattedMessage.Mid(iType+1, iTemp1 - iType - 1);
			strTemp.TrimLeft();
			strTemp.TrimRight();	
			iUnitNo = _wtoi(strTemp.GetBuffer(strTemp.GetLength()));

			// Section Name
			iTemp2 = strFormattedMessage.Find(',', iTemp1+1);
			strTemp = strFormattedMessage.Mid(iTemp1+1, iTemp2 - iTemp1 - 1);
			strTemp.TrimLeft();
			strTemp.TrimRight();
			m_sTeachingInfo[iUnitNo].m_strSectionName = strTemp;

			// Num Axis
			iTemp1 = strFormattedMessage.Find(',', iTemp2+1);
			strTemp = strFormattedMessage.Mid(iTemp2+1, iTemp1 - iTemp2 - 1);
			strTemp.TrimLeft();
			strTemp.TrimRight();	
			iTemp2 = _wtoi(strTemp.GetBuffer(strTemp.GetLength()));
			m_sTeachingInfo[iUnitNo].m_iNumAxis = iTemp2;
			
			// Post Fix
			for (int i = 0 ; i < m_sTeachingInfo[iUnitNo].m_iNumAxis ; i ++)
			{
				iTemp2 = strFormattedMessage.Find(',', iTemp1+1);
				strTemp = strFormattedMessage.Mid(iTemp1+1, iTemp2 - iTemp1 - 1);
				strTemp.TrimLeft();
				strTemp.TrimRight();	
				m_sTeachingInfo[iUnitNo].m_strSectionPostfix[i] = strTemp;
				iTemp1 = iTemp2;
			}

			// Num Teaching Pos
			iTemp2 = strFormattedMessage.Find(',', iTemp1+1);
			strTemp = strFormattedMessage.Mid(iTemp1+1, iTemp2 - iTemp1 - 1);
			strTemp.TrimLeft();
			strTemp.TrimRight();	
			iTemp1 = _wtoi(strTemp.GetBuffer(strTemp.GetLength()));
			m_sTeachingInfo[iUnitNo].m_iNumTeachingPos = iTemp1;

			// Gui Name
			for (int i = 0 ; i < 3 ; i ++)
			{
				iTemp1 = strFormattedMessage.Find(',', iTemp2+1);
				strTemp = strFormattedMessage.Mid(iTemp2+1, iTemp1 - iTemp2 - 1);
				strTemp.TrimLeft();
				strTemp.TrimRight();	
				m_sTeachingInfo[iUnitNo].m_strUnitName[i] = strTemp;
				iTemp2 = iTemp1;
			}
		}
		else if (!wcscmp(strType.GetBuffer(strType.GetLength()), _T("POS")))
		{
			// Unit No
			iTemp1 = strFormattedMessage.Find(',', iType+1);
			strTemp = strFormattedMessage.Mid(iType+1, iTemp1 - iType - 1);
			strTemp.TrimLeft();
			strTemp.TrimRight();	
			iUnitNo = _wtoi(strTemp.GetBuffer(strTemp.GetLength()));

			// Position No
			iTemp2 = strFormattedMessage.Find(',', iTemp1+1);
			strTemp = strFormattedMessage.Mid(iTemp1+1, iTemp2 - iTemp1 - 1);
			strTemp.TrimLeft();
			strTemp.TrimRight();
			iPosNo = _wtoi(strTemp.GetBuffer(strTemp.GetLength()));
			
			// Use Flag - GUI Display or Not
			iTemp = strFormattedMessage.Find(',', iTemp2+1);
			strTemp = strFormattedMessage.Mid(iTemp2+1, iTemp - iTemp2 - 1);
			strTemp.TrimLeft();
			strTemp.TrimRight();
			if (strTemp.Compare(_T("YES")) == 0)
			{
				m_sTeachingInfo[iUnitNo].m_sPosition[iPosNo].m_bUse = TRUE;
			}
			else
			{
				m_sTeachingInfo[iUnitNo].m_sPosition[iPosNo].m_bUse = FALSE;
			}
			
			iTemp2 = iTemp;

			// Enable/Disable Flag
			iTemp = strFormattedMessage.Find(',', iTemp2+1);
			strTemp = strFormattedMessage.Mid(iTemp2+1, iTemp - iTemp2 - 1);
			strTemp.TrimLeft();
			strTemp.TrimRight();
			if (strTemp.Compare(_T("YES")) == 0)
			{
				m_sTeachingInfo[iUnitNo].m_sPosition[iPosNo].m_bEnable = TRUE;
			}
			else
			{
				m_sTeachingInfo[iUnitNo].m_sPosition[iPosNo].m_bEnable = FALSE;
			}
			
			iTemp2 = iTemp;

			// Key Name
			iTemp1 = strFormattedMessage.Find(',', iTemp2+1);
			strTemp = strFormattedMessage.Mid(iTemp2+1, iTemp1 - iTemp2 - 1);
			strTemp.TrimLeft();
			strTemp.TrimRight();	
			m_sTeachingInfo[iUnitNo].m_sPosition[iPosNo].m_strName.Format(_T("%s"), strTemp);
			
			// Gui Name
			for (int i = 0 ; i < 3 ; i ++)
			{
				iTemp2 = strFormattedMessage.Find(',', iTemp1+1);
				strTemp = strFormattedMessage.Mid(iTemp1+1, iTemp2 - iTemp1 - 1);
				strTemp.TrimLeft();
				strTemp.TrimRight();	
				m_sTeachingInfo[iUnitNo].m_sPosition[iPosNo].m_strNewLineName[i].Format(_T("%s"), strTemp);
				iTemp1 = iTemp2;
			}
		}
	}
	
   /* Close stream */
   if (fclose(filePtr))	return -1;

   return 0;
}

STeachingInfo	MTeachingInfo::GetTeachingInfo(int iIndex)
{
	ASSERT((iIndex >= 0) && (iIndex < DEF_PREBONDER_UNIT_MAX_TEACHING_UNIT));

	return m_sTeachingInfo[iIndex];
}

