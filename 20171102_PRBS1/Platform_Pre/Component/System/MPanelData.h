#ifndef MPANELDATA_H
#define MPANELDATA_H

#include "MFileManager.h"
#include "DefSystem.h"

#define REFERENCE_MODEL_SECTION	_T("Reference Data")

typedef enum
{
	INPUT_DIRECTION_NORMAL		= 0,
	INPUT_DIRECTION_REVERSE		= 1
} EInputDirection;

typedef enum
{
	OUTPUT_DIRECTION_NORMAL		= 0,
	OUTPUT_DIRECTION_REVERSE	= 1
} EOutputDirection;

/** Panel Data */
class MPanelData : public MFileManager 
{
public:
	/** X�� PanelSize */
    double	m_dPanelSize_X;
	
 	/** Y�� PanelSize */
	double	m_dPanelSize_Y;				

	/**
	 *	Xu �� Data
	 */
	
	/** Xu �� ��� ���� */
	BOOL	m_bUseFlag_Xu;

	/** Xu�� SubFidumark ��뿩�� */
 	BOOL	m_bUseFlag_SubMark_Xu;

	/** Xu�� FiduMark X ��ǥ */
    double	m_dX_FiduMark_Xu;

	/** Xu�� FiduMark Y ��ǥ */
    double	m_dY_FiduMark_Xu;

	/** Xu�� FiduMark �� �Ÿ� */
    double	m_dDistance_FiduMark_Xu;

	/** Xu�� Fidu SubMark X ��ǥ */
	double	m_dX_FiduSubMark_Xu;

	/** Xu�� Fidu SubMark Y ��ǥ */
	double	m_dY_FiduSubMark_Xu;

	/** Xu�� Fidu SubMark �� �Ÿ� */
	double	m_dDistance_FiduSubMark_Xu;

	/** Panel Mark�� Tab Mark �� Y�Ÿ� */
	double	m_dMarkDist_PanelToTab_Xu;

	/**
	 *	Xd �� Data
	 */
	
	BOOL	m_bUseFlag_Xd;
	BOOL	m_bUseFlag_SubMark_Xd;
    double	m_dX_FiduMark_Xd;
    double	m_dY_FiduMark_Xd;
    double	m_dDistance_FiduMark_Xd;  
	double	m_dX_FiduSubMark_Xd;
	double	m_dY_FiduSubMark_Xd;
	double	m_dDistance_FiduSubMark_Xd;
	double	m_dMarkDist_PanelToTab_Xd;

	/**
	 *	Yl �� Data
	 */
	
	BOOL	m_bUseFlag_Yl;
    BOOL	m_bUseFlag_SubMark_Yl;
    double	m_dX_FiduMark_Yl;
    double	m_dY_FiduMark_Yl;
    double	m_dDistance_FiduMark_Yl;
	double	m_dX_FiduSubMark_Yl;
	double	m_dY_FiduSubMark_Yl;
	double	m_dDistance_FiduSubMark_Yl;
	double	m_dMarkDist_PanelToTab_Yl;

	/**
	 *	Yr �� Data
	 */

	BOOL	m_bUseFlag_Yr;
 	BOOL	m_bUseFlag_SubMark_Yr;
    double	m_dX_FiduMark_Yr;
    double	m_dY_FiduMark_Yr;
    double	m_dDistance_FiduMark_Yr;
	double	m_dX_FiduSubMark_Yr;
	double	m_dY_FiduSubMark_Yr;
	double	m_dDistance_FiduSubMark_Yr;
	double	m_dMarkDist_PanelToTab_Yr;

	/** Panel Input ���� */
	UINT	m_ePanel_InDirection;

	/** Panel Output ���� */
	UINT	m_ePanel_OutDirection;

	/** Panel �β� */
	double	m_dPanelThickness;

	/** LowSide Panel �β� */
	double	m_dPanelLowSidePolThickness;

	//110816. SJ_YTH.
	/*Cell Mark �� Tab Mark �� �Ÿ�*/
	double	m_dCellTabDist;
	//_________________

	//@ Panel Align�� Y�� ���� �Ұ����� X������ �Ұ����� ����
//@	int		m_iPanelAlignMode;	//0=YMode, 1=XuMode
	BOOL	m_bUseAirVacuum[DEF_MAX_WORK_SCHEDULE][DEF_MAX_AIRVACUUM];
    
	//NSMC KJS
	ERunMode		m_eRunMode;

	//110114.kms
	CString m_strReferenceDevice;			// Reference Data - Device ID

	BOOL			m_bUseTHandlerVac[DEF_MAX_THANDLER_VAC];

private:
	CString m_strSection;

// Operations
	void initialize();

public:
	MPanelData();
	MPanelData(CString strFileName, CString strPath);
	MPanelData(CString strFileName);
	~MPanelData();

    BOOL ReadData();
    BOOL WriteData();
};
#endif //MPANELDATA_H
