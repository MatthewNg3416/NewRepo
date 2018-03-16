#include "stdafx.h"
#include "common.h"
#include "MAutoAlignData.h"
#include "MyProgressWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

MAutoAlignData::MAutoAlignData(CString strFileName) : MFileManager(strFileName)
{
	initialize();
}

MAutoAlignData::MAutoAlignData(CString strFileName, CString strPath) : MFileManager(strFileName, strPath)
{
	initialize();
}

void MAutoAlignData::initialize()
{
	m_bAutoAlignSystem = FALSE;

	for(int i=0; i<50; i++)
	{
		XuTabOffset_X[i] = 0.0;
		XuTabOffset_Y[i] = 0.0;
		XuTabOffset_T[i] = 0.0;

		XdTabOffset_X[i] = 0.0;
		XdTabOffset_Y[i] = 0.0;
		XdTabOffset_T[i] = 0.0;

		YlTabOffset_X[i] = 0.0;
		YlTabOffset_Y[i] = 0.0;
		YlTabOffset_T[i] = 0.0;

		YrTabOffset_X[i] = 0.0;
		YrTabOffset_Y[i] = 0.0;
		YrTabOffset_T[i] = 0.0;
	}

}

MAutoAlignData::~MAutoAlignData()
{
}

int MAutoAlignData::ReadSystem()
{
	BOOL	bReturn = TRUE;
	CString	strSection;


	/*
	**********************************************************************************************
	** [SYSTEM_SECTION]
	**********************************************************************************************
	*/
	strSection.Format(_T("%s"), SYSTEM_SECTION);

	if (!GetValue(strSection, _T("AutoSystem"), &m_bAutoAlignSystem))
		DISPLAY_READ_ERROR(strSection, _T("AutoSystem"), bReturn);

	return bReturn;
}

int MAutoAlignData::ReadSourceAlignData()
{
	BOOL	bReturn = TRUE;
	CString	strSection;
	CString strItem;
	/*
	**********************************************************************************************
	** [ALIGN_SECTION]
	**********************************************************************************************
	*/
	strSection.Format(_T("%s"), ALIGN_SECTION);


	for(int i=0; i<50; i++)
	{
		strItem.Format(_T("XuTabOffset_X_[%d]"), i + 1);
		
		if (!GetValue(strSection,strItem, &XuTabOffset_X[i]))
			DISPLAY_READ_ERROR(strSection,strItem, bReturn);

		strItem.Format(_T("XuTabOffset_Y_[%d]"), i + 1);
		
		if (!GetValue(strSection,strItem, &XuTabOffset_Y[i]))
			DISPLAY_READ_ERROR(strSection,strItem, bReturn);

		strItem.Format(_T("XuTabOffset_T_[%d]"), i + 1);
		
		if (!GetValue(strSection,strItem, &XuTabOffset_T[i]))
			DISPLAY_READ_ERROR(strSection,strItem, bReturn);


		strItem.Format(_T("XdTabOffset_X_[%d]"), i + 1);
		
		if (!GetValue(strSection,strItem, &XdTabOffset_X[i]))
			DISPLAY_READ_ERROR(strSection,strItem, bReturn);

		strItem.Format(_T("XdTabOffset_Y_[%d]"), i + 1);
		
		if (!GetValue(strSection,strItem, &XdTabOffset_Y[i]))
			DISPLAY_READ_ERROR(strSection,strItem, bReturn);

		strItem.Format(_T("XdTabOffset_T_[%d]"), i + 1);
		
		if (!GetValue(strSection,strItem, &XdTabOffset_T[i]))
			DISPLAY_READ_ERROR(strSection,strItem, bReturn);
	}

	return bReturn;

}


int MAutoAlignData::ReadGateAlignData()
{
	BOOL	bReturn = TRUE;
	CString	strSection;
	CString strItem;
	/*
	**********************************************************************************************
	** [ALIGN_SECTION]
	**********************************************************************************************
	*/
	strSection.Format(_T("%s"), ALIGN_SECTION);


	for(int i=0; i<50; i++)
	{
		strItem.Format(_T("YlTabOffset_X_[%d]"), i + 1);
		
		if (!GetValue(strSection,strItem, &YlTabOffset_X[i]))
			DISPLAY_READ_ERROR(strSection,strItem, bReturn);

		strItem.Format(_T("YlTabOffset_Y_[%d]"), i + 1);
		
		if (!GetValue(strSection,strItem, &YlTabOffset_Y[i]))
			DISPLAY_READ_ERROR(strSection,strItem, bReturn);

		strItem.Format(_T("YlTabOffset_T_[%d]"), i + 1);
		
		if (!GetValue(strSection,strItem, &YlTabOffset_T[i]))
			DISPLAY_READ_ERROR(strSection,strItem, bReturn);


		strItem.Format(_T("YrTabOffset_X_[%d]"), i + 1);
		
		if (!GetValue(strSection,strItem, &YrTabOffset_X[i]))
			DISPLAY_READ_ERROR(strSection,strItem, bReturn);

		strItem.Format(_T("YrTabOffset_Y_[%d]"), i + 1);
		
		if (!GetValue(strSection,strItem, &YrTabOffset_Y[i]))
			DISPLAY_READ_ERROR(strSection,strItem, bReturn);

		strItem.Format(_T("YrTabOffset_T_[%d]"), i + 1);
		
		if (!GetValue(strSection,strItem, &YrTabOffset_T[i]))
			DISPLAY_READ_ERROR(strSection,strItem, bReturn);
	}

	return bReturn;

}
