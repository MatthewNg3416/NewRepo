#ifndef MPREBONDERDATA_H
#define MPREBONDERDATA_H

#include "MFileManager.h"
#include "DefSystem.h"

/** PreBonder Data */
class MPreBonderData : public MFileManager 
{
public:
	double	m_dLead_Width_Xtab;
	UINT	m_uiTab_Pitch_Count_Xtab;
	UINT	m_uiSprocket_Pitch_Count_Xtab;
	UINT	m_uiSprocket_Pitch_Count_Xtab_Second; //SJ_YYK 150213
	UINT	m_uiBad_Tab_Count_Xtab;
	UINT	m_uiAutoChange_Move_Tab_Count_Xtab;

	int		m_rgiNumberOfUsingSensorForWorker[DEF_MAX_GROUP];//??? KKY.Comment

	double	m_dLead_Width_Ytab;
	UINT	m_uiTab_Pitch_Count_Ytab;
	UINT	m_uiSprocket_Pitch_Count_Ytab;
	UINT	m_uiSprocket_Pitch_Count_Ytab_Second;//SJ_YYK 150213
	UINT	m_uiBad_Tab_Count_Ytab;

	double	m_dDistTabEgdeToMark_Xtab;	//@TabID���ܿ��� ��ũ���� �Ÿ�
	double	m_dDistTabEgdeToMark_Ytab;	//@TabID���ܿ��� ��ũ���� �Ÿ�

	/** AutoHoming Data */
	double	m_dSprocket2_F_User_AutoHoming_Offset;
	double	m_dSprocket2_R_User_AutoHoming_Offset;

	//120329.kms_________
	double	m_dSprocket1_F_AutoHoming_Offset;
	double	m_dSprocket1_R_AutoHoming_Offset;
	double	m_dSprocket2_F_AutoHoming_Offset;
	double	m_dSprocket2_R_AutoHoming_Offset;
	//___________________


	/** TCP,COF ��� ���� ��� ��뿩�� */
	BOOL	m_bUseTCP;

	/** Lighting Data */
	int		m_rgiLightingValue[DEF_MAX_LIGHTING];

	/** Roller Conveyor Speed Data */
	double	m_rgdConveyorNormalSpeed[DEF_MAX_CONVEYOR];
	double	m_rgdConveyorReducedSpeed[DEF_MAX_CONVEYOR];
	double	m_rgdConveyorTransferSpeed[DEF_MAX_CONVEYOR];

//@	double	m_uiPermissionXLeftValue13;
//@	double	m_uiPermissionXRightValue13;
//@	double	m_uiPermissionXLeftValue24;
//@	double	m_uiPermissionXRightValue24;
	double	m_dToleranceSRLine;		//@���ذ��� ���������� ������ ��� ����
	double	m_dToleranceTabEnd;		//@���ذ��� ���������� ������ ��� ����
	double	m_dNoCareValueTabEnd;	//@���ذ��� ���������� ���̰� �̰� ������ ��� �������� ����

	//20121004 SJ_HJG Ⱦ����
	double	m_dSRLineMarkDistance;
	double	m_dTabEndMarkDistance;
	BOOL	m_bMarkDistanceFunc;
	BOOL	m_bSRLineAlignFunc;
	//__________________
	//2009.08.14 CYJ
	/** Chip �߽ɰ� Tab ���� ���� Length(��, Tab ��ü ������ ����) **/
	/** 4 Pitch Tab�� �� ���̰� �ٸ� 2 Type �����Ͽ� **/
	/** �𵨺� Carrier X Load ��ġ �� Data �ݿ��� ���� ���� Data **/
	double	m_dLength_between_ChipCenter_and_TabEnd;

	/** TabMark PanelAlign */
	BOOL			m_bUseTabPanelAlign;	//TabMark�� PanelAlign ����

//@	double			m_dInspectionMarkXPermission;
//@	double			m_dInspectionMarkYPermission;

private:
	void initialize();

public:
	MPreBonderData();
	MPreBonderData(CString strFileName, CString strPath);
	MPreBonderData(CString strFileName);
	~MPreBonderData();

    BOOL ReadData();
    BOOL WriteData();
};
#endif //MPREBONDERDATA_H
