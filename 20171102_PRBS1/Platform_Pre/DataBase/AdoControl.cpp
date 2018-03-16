#include "StdAfx.h"
#include "AdoControl.h"

inline void TESTHR(HRESULT _hr) { if FAILED(_hr) _com_issue_error(_hr); }

CAdoControl::CAdoControl()
{
#ifdef _DEBUG
	TRACE(_T("������\n"));
#endif
	m_pConnection=NULL;
	m_pRecordset=NULL;
	m_pvtData=NULL;
	m_nRowSize=0;
	m_nColSize=0;
	m_bConnected=FALSE;
	m_bLock = FALSE;
	InitializeCriticalSection(&m_csLock);
}

CAdoControl::~CAdoControl()
{
	DeleteCriticalSection(&m_csLock);
	if(m_pvtData!=NULL)
		delete [] m_pvtData;
#ifdef _DEBUG
	TRACE(_T("�Ҹ���\n"));
#endif
}

// ==================================================================
// FUNCTION :  CAdoControl::Connect()
// * Description :DB�� �����ϰ� Provider�� �����մϴ�.
// 
// * Author : [likehood], Created : [2003-04-18 ���� 11:35:17]
// * Returns : [BOOL] -������ TRUE
// [lpszProvider] -���ι��̴�
// [EX_Oracle] "Provider=MSDAORA.1;Data Source=NEW_DSN;User ID=likehood;password=likehood; Persist Security Info=False;"
// [emOption] -����ɼ�
// 
// ==================================================================
BOOL CAdoControl::Connect(LPCTSTR lpszProvider,ExecuteOptionEnum emOption)
{
//imsi	TRACE0( " CAdoControl::Connect() entered\n" );
	
	if(m_pConnection!=NULL)
		return FALSE;

	try{
//		EnterCriticalSection(&m_csLock);
		m_pConnection.CreateInstance(__uuidof(Connection));
		m_pRecordset.CreateInstance(__uuidof(Recordset));


		m_pConnection->ConnectionTimeout=10;
		_bstr_t btProvider=(_bstr_t)lpszProvider;
		_bstr_t btEmpty=(_bstr_t)"";

		m_pConnection->Open(btProvider,btEmpty,btEmpty,emOption);
	}
	catch(_com_error &e)
	{
//		LeaveCriticalSection(&m_csLock);
		DumpError(e);
		return FALSE;
	}
//	LeaveCriticalSection(&m_csLock);
	m_bConnected = TRUE;
	return TRUE;
}

BOOL CAdoControl::Disconnect()
{
	try{
//		EnterCriticalSection(&m_csLock);
		if(m_pConnection->GetState() == adStateClosed)
		{
			if (m_pConnection != NULL)
				m_pConnection = NULL;
//			LeaveCriticalSection(&m_csLock);
			return TRUE;
		}
		if(m_pConnection->GetState()!=adStateClosed)
		{
			m_pConnection->Close();
			m_pConnection.Release();
		}
		if(m_pRecordset->GetState() == adStateClosed)
		{
			if (m_pRecordset != NULL)
				m_pRecordset = NULL;
			m_bConnected = FALSE;
//			LeaveCriticalSection(&m_csLock);
			return TRUE;
		}
		if(m_pRecordset->GetState()!=adStateClosed)
		{
			m_pRecordset->Close();
			m_pRecordset.Release();
		}

	}catch(_com_error &e)
	{
		DumpError(e);
//		LeaveCriticalSection(&m_csLock);
		return FALSE;
	}
//	LeaveCriticalSection(&m_csLock);
	m_bConnected = FALSE;
	return TRUE;
}

BOOL CAdoControl::InsUpDel(LPCTSTR lpszQry)
{
	if(m_pConnection->GetState()!=adStateOpen )
		return FALSE;
	try{
//		EnterCriticalSection(&m_csLock);
		_bstr_t btQry=(_bstr_t)lpszQry;
		m_pConnection->Execute(btQry,NULL,adOptionUnspecified);
	}
	catch(_com_error &e)
	{
		DumpError(e);
//		LeaveCriticalSection(&m_csLock);
		return FALSE;
	}
//	LeaveCriticalSection(&m_csLock);
	return TRUE;
}

// ==================================================================
// FUNCTION :  CAdoControl::ExcuteQryEx()
// * Description :CursorType, CursorLocation,LockType��������
// 
// * Author : [likehood], Created : [2003-04-18 ���� 11:09:43]
// * Returns : [BOOL] -������ TRUE
// [lpszQry] -������
// [CursorTypeEnum] -Ŀ��Ÿ�� (����� ���)
// [CursorLocationEnum] -Ŀ�������̼�(����� ���)
// [LockTypeEnum] -locktype (����� ���)
// 
// ==================================================================
BOOL CAdoControl::ExcuteQryEx(LPCTSTR lpszQry, CursorTypeEnum, CursorLocationEnum, LockTypeEnum)
{
	if(m_pConnection->GetState()!=adStateOpen)
		return FALSE;
	m_nRowSize=0;
	try{
//		EnterCriticalSection(&m_csLock);
		_bstr_t btQry=(_bstr_t)lpszQry;
        m_pRecordset->Open(btQry, _variant_t((IDispatch *) m_pConnection), adOpenStatic, adLockReadOnly, adCmdText);
		m_nColSize=m_pRecordset->GetFields()->GetCount();
		_variant_t vtQryData;
		if(m_pvtData!=NULL)
			delete [] m_pvtData;
		m_pvtData= new vector<_variant_t>[m_nColSize];
		while(!m_pRecordset->GetEndOfFile())
		{
			for(int i = 0 ; i < m_nColSize ; ++i)
			{
				vtQryData=m_pRecordset->GetCollect(m_pRecordset->Fields->GetItem( _variant_t( (long)i))->GetName());
				m_pvtData[i].push_back(vtQryData);
				vtQryData.Clear();
			}
			m_pRecordset->MoveNext();
			++m_nRowSize;
		}

	}
	catch(_com_error &e)
	{
		DumpError(e);
//		LeaveCriticalSection(&m_csLock);
		return FALSE;
	}
//	LeaveCriticalSection(&m_csLock);
	return TRUE;
}

BOOL CAdoControl::ExcuteQry(LPCTSTR lpszQry)
{	
	if(m_pConnection->GetState()!=adStateOpen)
		return FALSE;
	m_nRowSize=0;
	try{
//		EnterCriticalSection(&m_csLock);
		_bstr_t btQry=(_bstr_t)lpszQry;
		m_pRecordset=m_pConnection->Execute(btQry,NULL,adOptionUnspecified);
		m_nColSize=m_pRecordset->GetFields()->GetCount();
		_variant_t vtQryData;
		if(m_pvtData!=NULL)
			delete [] m_pvtData;
		m_pvtData= new vector<_variant_t>[m_nColSize];
		while(!m_pRecordset->GetEndOfFile())
		{
			for(int i = 0 ; i < m_nColSize ; ++i)
			{
				vtQryData=m_pRecordset->GetCollect(m_pRecordset->Fields->GetItem( _variant_t( (long)i))->GetName());
				m_pvtData[i].push_back(vtQryData);
				vtQryData.Clear();
			}
			m_pRecordset->MoveNext();
			++m_nRowSize;
		}

	}
	catch(_com_error &e)
	{
		DumpError(e);
//		LeaveCriticalSection(&m_csLock);
		return FALSE;
	}
//	LeaveCriticalSection(&m_csLock);
	return TRUE;
}

// ==================================================================
// FUNCTION :  CAdoControl::GetData()
// * Description :select������ ������ ����� ����
// 
// * Author : [likehood], Created : [2003-04-18 ���� 11:11:22]
// * Returns : [BOOL] -������ TRUE
// [nCol] -�÷���ġ
// [nRow] -����ġ
// [&nData] -�����
// 
// ==================================================================
BOOL CAdoControl::GetData(long nCol,long nRow,int &nData) const
{
//@	TRACE0( " CAdoControl::GetData() entered\n" );
	
	if(m_pvtData==NULL)
		return FALSE;
	
	try{
		vector<_variant_t>::iterator iter=m_pvtData[nCol].begin();
		_variant_t vtData=*(iter+nRow);
		switch(vtData.vt)
		{
		case VT_I4:
			nData=vtData.intVal;
			break;
		case (VT_ARRAY|VT_UI1):
			GetBinary(vtData,nData);

			break;
		default:
			return FALSE;
			break;
		}
	}catch(_com_error &e){
		DumpError(e);
		return FALSE;
	}
	return TRUE;
}

BOOL CAdoControl::GetData(long nCol,long nRow,CByteArray &baData) const
{
	if(m_pvtData==NULL)
		return FALSE;
	try{
		vector<_variant_t>::iterator iter=m_pvtData[nCol].begin();
		_variant_t vtData=*(iter+nRow);
		switch(vtData.vt)
		{
		case VT_NULL:
		case VT_EMPTY:
			baData.RemoveAll();
			break;
		case (VT_ARRAY|VT_UI1):
			GetBinary(vtData,baData);
			break;
		default:
			baData.RemoveAll();
			break;
		}
	}catch(_com_error &e){
		DumpError(e);
		return FALSE;
	}
	return TRUE;
}

// ==================================================================
// FUNCTION :  CAdoControl::GetBinary()
// * Description :���̳ʸ�Ÿ���� �����͸� �о�ö� ���(������)
// 
// * Author : [likehood], Created : [2003-04-18 ���� 11:12:27]
// * Returns : [BOOL] -������ TRUE
// [vtData] -����������
// [&baData] -������� ����� �ν��Ͻ�
// 
// ==================================================================
BOOL CAdoControl::GetBinary(const _variant_t& vtData,CByteArray &baData) const
{
	TRACE0( " CAdoControl::GetBinary() entered\n" );
	
	
	LONG nDataLength=vtData.parray->rgsabound[0].cElements;
	

	BYTE *pData= new BYTE[nDataLength];
	CByteArray ba;
	if(*pData!=NULL)
	{
		void *pArrayData;
		SafeArrayAccessData(vtData.parray,&pArrayData);
		memcpy(pData, pArrayData,nDataLength);
		SafeArrayUnaccessData(vtData.parray);
		for(int i = 0 ; i < nDataLength ; ++i)
		{
			baData.Add(pData[i]);
		}
		
		delete [] pData;
		return TRUE;
	}
	delete [] pData;
	return FALSE;
}

BOOL CAdoControl::GetData(long nCol,long nRow,CString &strData) const
{
	if(m_pvtData==NULL)
		return FALSE;
	try{
		vector<_variant_t>::iterator iter=m_pvtData[nCol].begin();
		_variant_t vtData=*(iter+nRow);
		switch(vtData.vt)
		{
		case VT_NULL:
		case VT_EMPTY:
			strData.Empty();
			break;
		case (VT_ARRAY|VT_UI1):
			strData=GetBinary(vtData);
			break;
		default:
			vtData.ChangeType(VT_BSTR);
			strData=vtData.bstrVal;
			break;
		}
	}catch(_com_error &e){
		DumpError(e);
		return FALSE;
	}
	return TRUE;
}

//50120.kang.add........
BOOL CAdoControl::GetData(long nCol, long nRow, COleDateTime &OleTime) const
{
	if(m_pvtData==NULL)
		return FALSE;
	try{
		vector<_variant_t>::iterator iter=m_pvtData[nCol].begin();
		_variant_t vtData=*(iter+nRow);
		switch(vtData.vt)
		{
		case VT_DATE:
			OleTime = vtData.date;
			break;
		}
	}catch(_com_error &e){
		DumpError(e);
		return FALSE;
	}
	return TRUE;	
}
//......................

BOOL CAdoControl::GetBinary(const _variant_t& vtData,int& nData) const
{
	LONG nDataLength=vtData.parray->rgsabound[0].cElements;
	BYTE *pData= new BYTE[nDataLength];
	if(*pData!=NULL)
	{
		void *pArrayData;
		SafeArrayAccessData(vtData.parray,&pArrayData);
		memcpy(pData, pArrayData,nDataLength);
		SafeArrayUnaccessData(vtData.parray);
		nData=pData[0];
		delete [] pData;
		return TRUE;
	}
	delete [] pData;
	return FALSE;

}

CString CAdoControl::GetBinary(const _variant_t& vtData) const
{
	LONG nDataLength=vtData.parray->rgsabound[0].cElements;
	BYTE *pData= new BYTE[nDataLength];
	if(*pData!=NULL)
	{
		void *pArrayData;
		SafeArrayAccessData(vtData.parray,&pArrayData);
		memcpy(pData, pArrayData,nDataLength);
		SafeArrayUnaccessData(vtData.parray);
		CString strData;
		CString strHexData("");
		for(int i = 0 ; i < nDataLength ; ++i)
		{
			strData.Format(_T("%02x"),pData[i]);	
			strHexData+=strData;
		}
		delete [] pData;
		return strHexData;
	}
	delete [] pData;
	return "";
}

// ==================================================================
// FUNCTION :  CAdoControl::SaveQryData()
// * Description :������ ���� �����͸� File�� ��ȯ�� �ݴϴ�.
//				  ***ExcuteEx()�Լ��� ����ؾ� ���� �����մϴ�.
// * Author : [likehood], Created : [2003-04-18 ���� 11:33:21]
// * Returns : [BOOL] -������ TRUE
// [szSaveDestination] -File�� ����� ��ġ
// [PersistFormat] -adPersistXML, adPersistADTG
// 
// ==================================================================
BOOL CAdoControl::SaveQryData(LPCTSTR szSaveDestination,enum PersistFormatEnum PersistFormat) const
{
	TRACE0( " CAdoControl::SaveQryData() entered\n" );
	
//@	EnterCriticalSection(&m_csLock);
	_variant_t vtDes;
	vtDes.SetString((LPCSTR)szSaveDestination);
	try{
		m_pRecordset->Save(vtDes,PersistFormat);
	}catch(_com_error &e){
		DumpError(e);
//@		LeaveCriticalSection(&m_csLock);
		return FALSE;
	}
//@	LeaveCriticalSection(&m_csLock);
	return TRUE;
}
void CAdoControl::DumpError(const _com_error &e) const
{
	_bstr_t btSource(e.Source());
	_bstr_t btDescription(e.Description());
	CString strMsg;
	strMsg.Format(	_T("DATE= %s\n")
					_T("%08lx= %s\n")
					_T("Source= %s\n")
					_T("Description= %s")
				,CTime::GetCurrentTime().Format("%Y/%m/%d %H:%M:%S")
				,e.Error(),e.ErrorMessage()
				,(LPCTSTR)btSource
				,(LPCTSTR)btDescription);
#ifdef _DEBUG
	//AfxMessageBox(strMsg);
#endif
}


BOOL CAdoControl::IsConnected()
{
	return m_bConnected;
}

void CAdoControl::SetLock(BOOL Lock)
{
	m_bLock = Lock;
}

BOOL CAdoControl::IsLock()
{
	return m_bLock;
}
