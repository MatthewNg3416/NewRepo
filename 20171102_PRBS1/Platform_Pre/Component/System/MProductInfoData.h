#ifndef MPRODUCT_INFO_DATA_H
#define MPRODUCT_INFO_DATA_H

#include "MFileManager.h"
#include "DefSystem.h"
#include "MLCNetData.h"
#include "MNetHData.h"

const int DEF_MAX_QUEUE_SIZE = 20;

typedef
/**
 *	Log Main Form 에서 생산 Panel 관련 정보를 보여주기 위해 관리하는 Data Structure
 */
struct
{
	double	m_dLineTact;
	double	m_dMachineTact;
	CTime m_timePanelIn;
	CTime m_timePanelOut;
	CString m_strPanelID;
} SPanelInfo;

/**
 *	Production Info 데이타를 관리한다.
 */
class MProductInfoData : public MFileManager 
{
private:
	CString m_strSection;
	int m_iPanelInfoIndex;
	SPanelInfo m_rgsPanelInfoQueue[DEF_MAX_QUEUE_SIZE];
	int			m_iPanelTurnStatus;//0:회전되어 있지 않은 상태(Return)	1:회전되어 있는 상태(Turn)

public:
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// 파일 미보관 데이타
	double	m_dEquipTact;

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// 파일 보관 데이타
	int		m_iProductCount;

	// Doolin kwangilshin 2017. 09. 12.
	//
	int		m_nPanelInputQty;
	int		m_nPanelScrapQty;
	// End

	// TabFeeder Data
	MMaterial	m_matTabICInfo[DEF_MAX_TABFEEDER][DEF_MAX_REEL];
	MMaterial	m_matZigInfo[DEF_MAX_TABFEEDER];
	int		m_iPreHandQty;
	int		m_iWarningQty;

	// Panel Data
	MGlassDatas	m_Glass[DEF_MAX_PANEL_UNIT];

	void	SetPanelStatus(BOOL bTurn);
	BOOL	GetPanelStatus();

	//110620.KKY.Add_________
	// Product Plan Data - 0:Current Plan, 1:Next Plan, 2:End Plan
	MProductPlanBatchData	m_CurProductBatchInfo;
	MProductPlanBatchData	m_NextProductBatchInfo[DEF_MAX_PRODUCT_PLAN_CNT];
	MProductPlanBatchData	m_EndProductBatchInfo[DEF_MAX_PRODUCT_PLAN_CNT];
	MMaterialCodeInfo		m_MaterialCodeInfo_S[DEF_MAX_PRODUCT_PLAN];
	MMaterialCodeInfo		m_MaterialCodeInfo_G[DEF_MAX_PRODUCT_PLAN];
	//_______________________

public:
    MProductInfoData(CString strFileName, CString strPath);
	MProductInfoData(CString strFileName);
	~MProductInfoData();

	void Initialize();

	BOOL ReadData();
	BOOL ReadMaterialData();
	BOOL ReadGlassData();
	BOOL ReadStatusData();
	//110620.KKY.Add__________
	BOOL ReadProductPlan();
	BOOL ReadMaterialCodeInfo();
	//________________________
	
    BOOL WriteData();
	BOOL WriteMaterialData();
	BOOL WriteGlassData();
	BOOL WriteStatusData();
	//110620.KKY.Add__________
	BOOL WriteProductPlan();
	BOOL WriteMaterialCodeInfo();
	//________________________

	BOOL IncreaseProductionCount();

	void MProductInfoData::SetPanelInfo(double dLineTact, CString strPanelID, CTime timePanelIn, CTime timePanelOut);
	double GetMeanLineTact();

	SPanelInfo* GetPanelInfo(int iQueueIndex);
	int GetCurrentQueueIndex();
};
#endif // MPRODUCT_INFO_DATA_H
