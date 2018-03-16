/* samsung */
#ifndef IDACONVERTER_H
#define IDACONVERTER_H

#include "ICommon.h"
#include "IIO.h"
#include "MDAConverterData.h"

const	int	DEF_DACONVERTER_SUCCESS					= 0;		// Success
const	int DEF_MAX_DACONVERTER_BIT					= 16;
const	int DEF_MAX_DACONVERTER_VALUE				= 6000;

const	int	ERR_DACONVERTER_SUCCESS					= 0;
const	int	ERR_DACONVERTER_FAIL_IN_LOADING_PARAM	= 1;
const	int	ERR_DACONVERTER_FAIL_IN_SAVING_PARAM	= 2;
const	int	ERR_DACONVERTER_INVALID_VALUE			= 3;

/** @stereotype enumeration */
enum EDAConverterType {
        LIGHTENING,
        PRESSURE,
		TENSION
};

typedef struct
{
	/** 생성된 DAConverter 객체의 ID */
	int m_iDAConverterID;
	/** 생성된 DAConverter 객체의 이름 */
	WCHAR m_szName[256];
//	char m_szName[256];

	/** 생성된 DAConverter의 시작 Bit */
	unsigned short m_usOStarBit;
	unsigned short m_usIStarBit;

	/** 생성된 DAConverter의 BitSize */
	unsigned short m_usOBitSize;
	unsigned short m_usIBitSize;

	/** @link aggregation DAConverter 타입 */
	EDAConverterType m_lnkEDAConverterType;

}SDAConverterVal;


typedef struct tagSDAConverterData
{		
	SDAConverterVal m_sDAConverterVal;

	/** DAConverter Data가 저장될 File Name */
	CString m_strFileName;
	
} SDAConverterData, *pSDAConverterData;


/** @interface */
class IDAConverter : public virtual ICommon 
{
public :
    IIO *m_gpIIO;

	/** Regular Value return */
	virtual int GetValue() = 0;

    /** RegualaValue 값으로 동작시킴  */
    virtual int SetValue(unsigned short val) = 0;

	/** 원시 Data로 Set하는 함수*/
	virtual int SetDigit(unsigned short val) = 0;

/*------------------------ Common Interface -----------------------*/
    /**
	 * Assign component linkage
	 * 
	 * @param	pIO	: 참조할 IO Component Pointer
	 */
	virtual int AssignComponents(IIO *pIO) = 0;
  
    /** 
	 * Component Data 처리 
	 *
	 * @param	datComponent : 기록할 데이타
	 * @return	Error : 0 = Success, 그외=기타
	 */
	virtual int SetData(SDAConverterData datComponent) = 0;

    /** 
	 * Component Data 읽어올 데이타 인터 
	 *
	 * @param	datComponent : MDAConverterData Class에 대한 포인터
	 * @return	Error : 0 = Success, 그외=기타
	 */
	virtual int GetData(SDAConverterData *pData) = 0;

	/**
	 * DAConverter 관련 Data를 File에서 읽어온다.
	 *
	 * @param	pcylTimerData (읽어올 Data가 저장될 변수)
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	virtual int LoadParameter() = 0;
 
	/**
	 * DAConverter 관련 Data를 File에 저장한다.
	 *
	 * @param	pDAConverterData (저장할 Data)
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	virtual int SaveParameter(MDAConverterData* pDAConverterData) = 0;

	/**
	 * DAConverter 관련 Data를 얻어 온다.
	 *
	 * @param	pDAConverterData (읽어올 Data가 저장될 변수)
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	virtual void GetParameter(MDAConverterData* pDAConverterData) = 0;

	/**
	 * DAConverter 이름을 Return 한다.
	 * @return : DAConverter Name
	 */
	virtual CString GetDAConverterName() = 0;
};
#endif //IDACONVERTER_H