// wrapping class create by likehood
// Create Date/Ver : 2003.03.19 /1.0
// Modify Date/Ver : 2003.04.18 / 1.5

// you must include in your main source 
// follow:
// ::CoInitialize(NULL); and ::CoUnInitialize();
// CoInitialize�� �ܺη� �� ������ ���������� �����Ҹ��Ű�� �˼� ���� 
// ������ ����� �� �ؼ� �Դϴ�.(Ȯ��ġ�� �ʾƿ�...)
// _ConnectionPtr,�� _RecordsetPtr�� ��� ����� ->Close(), .Release()�Լ���
// �ڵ�ȣ�� �ǹǷ� ���ġ �ʾҽ��ϴ�.
// 1.0 RecordSet�� CreateInstance������.
// 1.5 RecordSet������ ����  CreateInstance�� ����߽��ϴ�.(ExcuteQryEx)
// 1.5 SaveQryData�Լ� �߰�
// 1.5 ExcuteQryEx�Լ� �߰�(���ڵ�� ������ ����...)
// ��Ÿ ���� ������  �Ʒ��� �����ּ���
// http://likehood.com 
// likehood@hanmail.net

#pragma warning(push)
#pragma warning(disable:4146)

//#pragma once
// compile with: /MP /c C2813.cpp C2813.cpp  
//#import "c:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF", "EndOfFile")
#include "msado15.tlh" //171228 JSH.....
#pragma warning(pop)
#include <vector>
using namespace std;

class simpleAdo
{
// Functions	
public:
	simpleAdo();
	~simpleAdo();
	BOOL Connect(LPCTSTR lpszProvider,ExecuteOptionEnum emOption=adOptionUnspecified);
	BOOL Disconnect();
	BOOL ExcuteQry(LPCTSTR lpszQry);

//  04.18 release
	BOOL ExcuteQryEx(LPCTSTR lpszQry, CursorTypeEnum=adOpenStatic, CursorLocationEnum=adUseClient, LockTypeEnum=adLockReadOnly);
//*******************************************************************************************
//	cursor or lock type		����							����
//*******************************************************************************************
//  adOpenForwardOnly		Low resource requirements		Cannot scroll backward 
//															No data concurrency 
//*******************************************************************************************
//	adOpenStatic			Scrollable						No data concurrency 
//*******************************************************************************************
//	adOpenKeyset			Some data concurrency			Higher resource requirements 
//							Scrollable						Not available in disconnected scenario
//*******************************************************************************************
//	adOpenDynamic			High data concurrency			Highest resource requirements 
//							Scrollable						Not available in disconnected scenario 
//*******************************************************************************************
//	adLockReadOnly			Low resource requirements		Data not updatable through cursor
//							Highly scalable 
//*******************************************************************************************
//	adLockBatchOptimistic	Batch updates					Data can be changed by multiple users at once 
//							Allows disconnected scenarios 
//							Other users able to access data 
//*******************************************************************************************
//	adLockPessimistic		Data cannot be changed by other users while locked		Prevents other users from accessing data while locked 
//*******************************************************************************************
// 	adLockOptimistic		Other users able to access data							Data can be changed by multiple users at once 
//******************************************************************************************* 
	BOOL InsUpDel(LPCTSTR lpszQry);	// Insert, Update, Delete 
	BOOL GetData(long nCol,long nRow,CString &strData) const;	
	BOOL GetData(long nCol,long nRow,int &nData) const;
	BOOL GetBinary(const _variant_t& vtData,int& nData) const;
	BOOL GetData(long nCol,long nRow,CByteArray &baData) const;
	LONG BeginTrans() const { return m_pConnection->BeginTrans();}
	HRESULT RollbackTrans() const { return m_pConnection->RollbackTrans();}
	HRESULT CommitTrans() const { return m_pConnection->CommitTrans();}
	LONG GetColSize() const {return m_nColSize;}
	LONG GetRowSize() const {return m_nRowSize;}
	BOOL SaveQryData(LPCTSTR szSaveDestination,enum PersistFormatEnum PersistFormat) const;

private:
	void DumpError(const _com_error &e) const;
	CString GetBinary(const _variant_t& vtData) const;
	BOOL GetBinary(const _variant_t& vtData,CByteArray& baData) const;
//Variables
public:
	_ConnectionPtr	m_pConnection;
	_RecordsetPtr	m_pRecordset;

private:	
	LONG m_nColSize;
	LONG m_nRowSize;
//	_ConnectionPtr	m_pConnection;
//	_RecordsetPtr	m_pRecordset;
	vector<_variant_t> *m_pvtData;	
};

