#ifndef MDACONVERTERDATA_H
#define MDACONVERTERDATA_H

#include "MFileManager.h"
#include "common.h"

class MDAConverterData : public MFileManager 
{
private:
	/** Section Name */
	CString m_strSection;

public:
	/** Y���� */
	double	m_dOffset;
	/** ���� */
	double	m_dSlant;

public:

	MDAConverterData(){};

    /**
     * @stereotype constructor	
	 *
	 * @param	CString strSection (Section Name)
	 * @param	CString strFileName (File Name)
     */
	MDAConverterData(CString strSection, CString strFileName);

    /**
     * @stereotype constructor	
	 *
	 * @param	CString strSection (Section Name)
	 * @param	CString strFileName (File Name)
	 * @param	CString strPath (File Path)
     */
	MDAConverterData(CString strSection, CString strFileName, CString strPath);

    /**
     * @stereotype destructor 
     */
	~MDAConverterData();

	/** 
	 * DAConverter Data�� �ʱ�ȭ�Ѵ�.
	 *
	 * @return	void
	 */
	void Initialize();

	/** 
	 * DAConverter Data�� File���� �о�´�.
	 *
	 * @return	BOOL (TRUE = Success, FALSE = Fail)
	 */
    BOOL ReadData();

	/** 
	 * DAConverter Data�� File�� �����Ѵ�.
	 *
	 * @return	BOOL (TRUE = Success, FALSE = Fail)
	 */
    BOOL WriteData();
};

#endif //MDACONVERTERDATA_H
