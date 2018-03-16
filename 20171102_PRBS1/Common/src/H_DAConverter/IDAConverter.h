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
	/** ������ DAConverter ��ü�� ID */
	int m_iDAConverterID;
	/** ������ DAConverter ��ü�� �̸� */
	WCHAR m_szName[256];
//	char m_szName[256];

	/** ������ DAConverter�� ���� Bit */
	unsigned short m_usOStarBit;
	unsigned short m_usIStarBit;

	/** ������ DAConverter�� BitSize */
	unsigned short m_usOBitSize;
	unsigned short m_usIBitSize;

	/** @link aggregation DAConverter Ÿ�� */
	EDAConverterType m_lnkEDAConverterType;

}SDAConverterVal;


typedef struct tagSDAConverterData
{		
	SDAConverterVal m_sDAConverterVal;

	/** DAConverter Data�� ����� File Name */
	CString m_strFileName;
	
} SDAConverterData, *pSDAConverterData;


/** @interface */
class IDAConverter : public virtual ICommon 
{
public :
    IIO *m_gpIIO;

	/** Regular Value return */
	virtual int GetValue() = 0;

    /** RegualaValue ������ ���۽�Ŵ  */
    virtual int SetValue(unsigned short val) = 0;

	/** ���� Data�� Set�ϴ� �Լ�*/
	virtual int SetDigit(unsigned short val) = 0;

/*------------------------ Common Interface -----------------------*/
    /**
	 * Assign component linkage
	 * 
	 * @param	pIO	: ������ IO Component Pointer
	 */
	virtual int AssignComponents(IIO *pIO) = 0;
  
    /** 
	 * Component Data ó�� 
	 *
	 * @param	datComponent : ����� ����Ÿ
	 * @return	Error : 0 = Success, �׿�=��Ÿ
	 */
	virtual int SetData(SDAConverterData datComponent) = 0;

    /** 
	 * Component Data �о�� ����Ÿ ���� 
	 *
	 * @param	datComponent : MDAConverterData Class�� ���� ������
	 * @return	Error : 0 = Success, �׿�=��Ÿ
	 */
	virtual int GetData(SDAConverterData *pData) = 0;

	/**
	 * DAConverter ���� Data�� File���� �о�´�.
	 *
	 * @param	pcylTimerData (�о�� Data�� ����� ����)
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	virtual int LoadParameter() = 0;
 
	/**
	 * DAConverter ���� Data�� File�� �����Ѵ�.
	 *
	 * @param	pDAConverterData (������ Data)
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	virtual int SaveParameter(MDAConverterData* pDAConverterData) = 0;

	/**
	 * DAConverter ���� Data�� ��� �´�.
	 *
	 * @param	pDAConverterData (�о�� Data�� ����� ����)
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	virtual void GetParameter(MDAConverterData* pDAConverterData) = 0;

	/**
	 * DAConverter �̸��� Return �Ѵ�.
	 * @return : DAConverter Name
	 */
	virtual CString GetDAConverterName() = 0;
};
#endif //IDACONVERTER_H