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

	double	m_dDistTabEgdeToMark_Xtab;	//@TabID끝단에서 마크까지 거리
	double	m_dDistTabEgdeToMark_Ytab;	//@TabID끝단에서 마크까지 거리

	/** AutoHoming Data */
	double	m_dSprocket2_F_User_AutoHoming_Offset;
	double	m_dSprocket2_R_User_AutoHoming_Offset;

	//120329.kms_________
	double	m_dSprocket1_F_AutoHoming_Offset;
	double	m_dSprocket1_R_AutoHoming_Offset;
	double	m_dSprocket2_F_AutoHoming_Offset;
	double	m_dSprocket2_R_AutoHoming_Offset;
	//___________________


	/** TCP,COF 사용 선택 기능 사용여부 */
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
	double	m_dToleranceSRLine;		//@기준값과 측정값과의 차이의 허용 오차
	double	m_dToleranceTabEnd;		//@기준값과 측정값과의 차이의 허용 오차
	double	m_dNoCareValueTabEnd;	//@기준값과 측정값과의 차이가 이값 이하일 경우 보정하지 않음

	//20121004 SJ_HJG 횡전개
	double	m_dSRLineMarkDistance;
	double	m_dTabEndMarkDistance;
	BOOL	m_bMarkDistanceFunc;
	BOOL	m_bSRLineAlignFunc;
	//__________________
	//2009.08.14 CYJ
	/** Chip 중심과 Tab 끝단 간의 Length(즉, Tab 전체 길이의 절반) **/
	/** 4 Pitch Tab에 이 길이가 다른 2 Type 존재하여 **/
	/** 모델별 Carrier X Load 위치 모델 Data 반영시 별도 참조 Data **/
	double	m_dLength_between_ChipCenter_and_TabEnd;

	/** TabMark PanelAlign */
	BOOL			m_bUseTabPanelAlign;	//TabMark로 PanelAlign 여부

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
