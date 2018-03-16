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
	/** Y절편 */
	double	m_dOffset;
	/** 기울기 */
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
	 * DAConverter Data를 초기화한다.
	 *
	 * @return	void
	 */
	void Initialize();

	/** 
	 * DAConverter Data를 File에서 읽어온다.
	 *
	 * @return	BOOL (TRUE = Success, FALSE = Fail)
	 */
    BOOL ReadData();

	/** 
	 * DAConverter Data를 File에 저장한다.
	 *
	 * @return	BOOL (TRUE = Success, FALSE = Fail)
	 */
    BOOL WriteData();
};

#endif //MDACONVERTERDATA_H
