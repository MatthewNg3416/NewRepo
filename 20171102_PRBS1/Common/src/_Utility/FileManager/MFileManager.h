#ifndef MFILEMANAGER_H
#define MFILEMANAGER_H

/**
 * MFileManager Class는 파일로 부터 특정 Item을 얻어 오거나 반대로 특정 Item에 쓰는 역할을 하는 Class 이다. 
 * Data Mananger는 파일 입출력을 위해 이 Class를 상속 받는다.
 */
class MFileManager 
{
protected:    
    CString m_strFileName;
    CString m_strPath;
	CString m_strFile;

public:
    /**
     * 생성자 strFileName : 읽거나 쓰야하는 File 이름 
     */
    MFileManager(CString strFileName);    

    /**
     * 생성자 strFileName : 읽거나 쓰야하는 File 이름
     *             strPath : File의 경로명 
     */
    MFileManager(CString strFileName, CString strPath);

    /**
     * 생성자 
     */
    MFileManager();

    /**
     * 소멸자 
     */
    virtual ~MFileManager();

	/** File Path를 다시 설정 하는 함수 */
	void SetFilePath(CString strPath); 

	static void CreateBackupFolder(LPCTSTR strDest, LPCTSTR strSrc, BOOL bOverWrite);

	static void AutoDeleteFile(CString Target, int Duration, BOOL bDelelteDir = FALSE);
 
protected:
   /**
     * File에 인자로 받은 Section과 Item에 정수값을 쓰는 함수 
     */
    int SetValue(CString strSection, CString strItem, int nValue, BOOL bMakeHistory = TRUE);

	/**
     * File에 인자로 받은 Section과 Item에 정수값을 쓰는 함수 
     */
    int SetValue(CString strSection, CString strItem, UINT nValue, BOOL bMakeHistory = TRUE);

    /**
     * File에 인자로 받은 Section과 Item에 실수값을 쓰는 함수 
     */
    int SetValue(CString strSection, CString strItem, double dValue, BOOL bMakeHistory = TRUE);

    /**
     * File에 인자로 받은 Section과 Item에 문자값을 쓰는 함수 
     */
    int SetValue(CString strSection, CString strItem, CString strValue, BOOL bMakeHistory = TRUE);
	 /**
     * File에 인자로 받은 Section과 Item에 문자값을 쓰는 함수 
     */
    int SetValue(CString strSection, CString strItem, WCHAR* pchValue, UINT nSize, BOOL bMakeHistory = TRUE);

    /**
     * File에서 인자로 받은 Section과 Item의 값을 읽어 오는 함수 
     */
    int GetValue(CString strSection, CString strItem, int * pnValue);

	/**
     * File에서 인자로 받은 Section과 Item의 값을 읽어 오는 함수 
     */
    int GetValue(CString strSection, CString strItem, UINT * pnValue);

    /**
     * File에서 인자로 받은 Section과 Item의 값을 읽어 오는 함수 
     */
    int GetValue(CString strSection, CString strItem, double * pdValue);

    /**
     * File에서 인자로 받은 Section과 Item의 값을 읽어 오는 함수 
     */
    int GetValue(CString strSection, CString strItem, CString * pstrValue);

	/**
     * File에서 인자로 받은 Section과 Item의 값을 읽어 오는 함수 
     */
    int GetValue(CString strSection, CString strItem, WCHAR * pchValue, UINT nSize);

	/**
     * Backup File을 만드는 함수
     */
	int MakeBackUpFile();

	/**
	 * Data 변경 여부를 확인하는 함수
	 */
	void checkValue(CString strSection, CString strItem, CString strValue);

	/**
	 * Data 변경 사항을 저장하는 함수
	 */
	void writeChangeHistory(CString strSection, CString strItem, WCHAR* chrBeforData, CString strValue);

	/**
	 * 파일의 크기가 클 경우 백업한 후 새로운 파일을 만든다.
	 */
	void resizeFile(CString strDirName);

	/**cys_100420
	  * Model 명을 얻어오는 함수
	  */
	CString getModelName();

	/**cys_100420
	 * Data 변경 사항을 저장하는 함수 - 전체 변경 사항에 대하여 하나의 파일로 통합
	 */
	void writeChangeWholeHistory(CString strSection, CString strItem, WCHAR* chrBeforData, CString strValue);

public:
	static BOOL m_bResizeFile;
};
#endif //MFILEMANAGER_H
