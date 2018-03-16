#ifndef MErrorMsg_H
#define MErrorMsg_H

#include "MFileManager.h"

/**
 * Final Data�� ���Ͽ��� �о Member Variable�� �� ���� ���� �ִ�.
 */
class MErrorMsg  : public MFileManager 
{
public:
	/** ������1 */
	MErrorMsg(CString strFileName, CString strPath);
	/** ������2 */
	MErrorMsg(CString strFileName);
	/** �Ҹ��� */
	virtual ~MErrorMsg();

	/** File���� ���� �޼����� �о���� �Լ� */
	CString GetErrorMessage(int iErrorCode, UINT uiLanguageType);    
};
#endif // MErrorMsg_H
