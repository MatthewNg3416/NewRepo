#ifndef MFILEMANAGER_H
#define MFILEMANAGER_H

/**
 * MFileManager Class�� ���Ϸ� ���� Ư�� Item�� ��� ���ų� �ݴ�� Ư�� Item�� ���� ������ �ϴ� Class �̴�. 
 * Data Mananger�� ���� ������� ���� �� Class�� ��� �޴´�.
 */
class MFileManager 
{
protected:    
    CString m_strFileName;
    CString m_strPath;
	CString m_strFile;

public:
    /**
     * ������ strFileName : �аų� �����ϴ� File �̸� 
     */
    MFileManager(CString strFileName);    

    /**
     * ������ strFileName : �аų� �����ϴ� File �̸�
     *             strPath : File�� ��θ� 
     */
    MFileManager(CString strFileName, CString strPath);

    /**
     * ������ 
     */
    MFileManager();

    /**
     * �Ҹ��� 
     */
    virtual ~MFileManager();

	/** File Path�� �ٽ� ���� �ϴ� �Լ� */
	void SetFilePath(CString strPath); 

	static void CreateBackupFolder(LPCTSTR strDest, LPCTSTR strSrc, BOOL bOverWrite);

	static void AutoDeleteFile(CString Target, int Duration, BOOL bDelelteDir = FALSE);
 
protected:
   /**
     * File�� ���ڷ� ���� Section�� Item�� �������� ���� �Լ� 
     */
    int SetValue(CString strSection, CString strItem, int nValue, BOOL bMakeHistory = TRUE);

	/**
     * File�� ���ڷ� ���� Section�� Item�� �������� ���� �Լ� 
     */
    int SetValue(CString strSection, CString strItem, UINT nValue, BOOL bMakeHistory = TRUE);

    /**
     * File�� ���ڷ� ���� Section�� Item�� �Ǽ����� ���� �Լ� 
     */
    int SetValue(CString strSection, CString strItem, double dValue, BOOL bMakeHistory = TRUE);

    /**
     * File�� ���ڷ� ���� Section�� Item�� ���ڰ��� ���� �Լ� 
     */
    int SetValue(CString strSection, CString strItem, CString strValue, BOOL bMakeHistory = TRUE);
	 /**
     * File�� ���ڷ� ���� Section�� Item�� ���ڰ��� ���� �Լ� 
     */
    int SetValue(CString strSection, CString strItem, WCHAR* pchValue, UINT nSize, BOOL bMakeHistory = TRUE);

    /**
     * File���� ���ڷ� ���� Section�� Item�� ���� �о� ���� �Լ� 
     */
    int GetValue(CString strSection, CString strItem, int * pnValue);

	/**
     * File���� ���ڷ� ���� Section�� Item�� ���� �о� ���� �Լ� 
     */
    int GetValue(CString strSection, CString strItem, UINT * pnValue);

    /**
     * File���� ���ڷ� ���� Section�� Item�� ���� �о� ���� �Լ� 
     */
    int GetValue(CString strSection, CString strItem, double * pdValue);

    /**
     * File���� ���ڷ� ���� Section�� Item�� ���� �о� ���� �Լ� 
     */
    int GetValue(CString strSection, CString strItem, CString * pstrValue);

	/**
     * File���� ���ڷ� ���� Section�� Item�� ���� �о� ���� �Լ� 
     */
    int GetValue(CString strSection, CString strItem, WCHAR * pchValue, UINT nSize);

	/**
     * Backup File�� ����� �Լ�
     */
	int MakeBackUpFile();

	/**
	 * Data ���� ���θ� Ȯ���ϴ� �Լ�
	 */
	void checkValue(CString strSection, CString strItem, CString strValue);

	/**
	 * Data ���� ������ �����ϴ� �Լ�
	 */
	void writeChangeHistory(CString strSection, CString strItem, WCHAR* chrBeforData, CString strValue);

	/**
	 * ������ ũ�Ⱑ Ŭ ��� ����� �� ���ο� ������ �����.
	 */
	void resizeFile(CString strDirName);

	/**cys_100420
	  * Model ���� ������ �Լ�
	  */
	CString getModelName();

	/**cys_100420
	 * Data ���� ������ �����ϴ� �Լ� - ��ü ���� ���׿� ���Ͽ� �ϳ��� ���Ϸ� ����
	 */
	void writeChangeWholeHistory(CString strSection, CString strItem, WCHAR* chrBeforData, CString strValue);

public:
	static BOOL m_bResizeFile;
};
#endif //MFILEMANAGER_H
