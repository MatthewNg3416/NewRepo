#ifndef MErrorMsg_H
#define MErrorMsg_H

#include "MFileManager.h"

/**
 * Final Data를 파일에서 읽어서 Member Variable이 그 값을 갖고 있다.
 */
class MErrorMsg  : public MFileManager 
{
public:
	/** 생성자1 */
	MErrorMsg(CString strFileName, CString strPath);
	/** 생성자2 */
	MErrorMsg(CString strFileName);
	/** 소멸자 */
	virtual ~MErrorMsg();

	/** File에서 에러 메세지를 읽어오는 함수 */
	CString GetErrorMessage(int iErrorCode, UINT uiLanguageType);    
};
#endif // MErrorMsg_H
