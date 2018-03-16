#ifndef DEF_MPROCESS_DATA_H
#define DEF_MPROCESS_DATA_H

#include <afxtempl.h>
#include <afxmt.h>

#include "MPos_XY.h"
#include "MPos_XYT.h"
#include "DefSystem.h"

class MSystemData;
class MPanelData;
class MTabData;
class MPreBonderData;
class MTrsPanelAligner;
class MTrsTabFeeder;
//class MPos_XYT;

const int DEF_NO_WORK				= -1;

const int DEF_PROCESS_DATA_SUCCESS	= 0;
const int DEF_PROCESS_DATA_ERROR	= 1;

// 용어 정의
// 1st Cycle, 2nd Cycle : 가압착 Unit 이 작업해야할 Panel 을 받은 후, TabMounter Group 으로 수행하는 첫번째 Work Cycle 인지
//                        두번째 Work Cycle 인지를 구분할 때 사용한다.
// 1st Work Range, 2nd Work Range
//		: 작업 순서와는 관계없이 Tab 번호에 따라 결정되는 값으로
//        작업 시작 Tab 번호부터 시작해서 TabMounter Group 내 전체 TabMounter 로 한번에 작업할 수 있는 Tab 번호 범위를 1st Work Range 라 하고,
//        1st Work Range 이후의 Tab 번호 범위를 2nd Work Range 라 한다.
//      - 여기서 작업 시작 Tab 번호란, 사용자가 실제 작업하도록 설정한 Tab 번호 중에서 작업 순서 상 가장 먼저 작업되는 Tab 번호를 의미한다. 

//const int DEF_MAX_WORK_RANGE		= 2;
const int DEF_MAX_WORK_RANGE		= 4;	//KKY.Modify
const int DEF_WORK_RANGE_NONE		= -1;
const int DEF_WORK_RANGE_1			= 0;
const int DEF_WORK_RANGE_2			= 1;
const int DEF_WORK_RANGE_3			= 2;	//KKY.Add
const int DEF_WORK_RANGE_4			= 3;	//KKY.Add

const int DEF_MAX_SUB_LIST			= 16;

// Inspection Camera 가 다른 Inspection Camera 의 작업을 위해
// 다른 Inspection Camera 가 작업해야 하는 Tab 바로 전 Tab 위치로
// 이동하라는 명령임. (Source : Inspection Camera Carrier 2 에만 해당되는 명령임.)
const int DEF_MOVE_WAIT_POS_COMMAND	= -100;

typedef enum
{
	UNIT_TYPE_NONE = -1,
	UNIT_TYPE_TAB_CARRIER = 0,
    UNIT_TYPE_TAB_MOUNTER,
    UNIT_TYPE_INSPECTION_CAMERA,
	UNIT_TYPE_PANEL_ALIGNER,
} EUnitType;

/**	자동운전 진행 중 실시간으로 Tab 부착 정보 관리 Data 의 구조체
 *	아래 구조체의 Instance 를 멤버로 하는 작업변 별로 부착 Tab 수량만큼의 배열이 생성되어야하고, 
 *	그 배열에는 Tab 번호를 Index 로 접근할 수 있다.
 */
typedef struct
{
	/** 실시간 Data */

	// Error 발생 시 등 해당 Tab 관련 작업 Skip 여부, Init 처리?
	BOOL m_bWorkSkip;

	// Tab 버림 작업 여부
	BOOL m_bRemoveTabIC;

	// TabCarrier 가 TabFeeder 로부터 TabIC Load 완료 여부
	BOOL m_bCarrierLoadCompleted;

	// 해당 Tab 에 대한 Mounter Load 완료 여부
	// 자동운전 중 Tab Load 후 Tab 떨어짐 등의 문제가 발생하면, 이 Flag 를 Clear 해주는 등의 처리가 필요하다.
	BOOL m_bMounterLoadCompleted;

	// Tab Prealign 완료 여부 : Source 설비 불필요, Gate 설비 Inspection 위치 이동 전 확인해야 하는 사항임.
	BOOL m_bTabPrealignCompleted;

	// 해당 Tab Inspection 완료 여부
	BOOL m_bInspectionCompleted;

	// 해당 Tab 부착 완료 여부
	BOOL m_bMountCompleted;

	// 해당 Tab Mount After Inspection 완료 여부
	BOOL m_bAfterInspectionCompleted;

	// Carrier Load 동작 Retry 횟수 저장
	int m_iCarrierLoadRetryCount;

	// Mounter Load 동작 Retry 횟수 저장
	int m_iMounterLoadRetryCount;

	// Tab Prealign 동작 Retry 횟수 저장
	int m_iTabPrealignRetryCount;

	// 20081008 jdy
	// Inspection : TabIC Mark 인식 Error 관련 Retry 횟수 저장
	int m_iInspectionRetryCount;

	// Panel - TabIC LeadMark Inspection 결과 Offset 저장. - 관리용...
	// Tab Prealign 후 Panel X 방향 Offset 을 저장하고, 이 값은 Inspection 위치 이동 시 사용된다.
	MPos_XY m_posTabPrealignResult;

	// Panel - TabIC LeadMark Inspection 결과 Offset 저장. - 관리용...
	// Tab Inspection 후 Panel X 방향 Offset 을 저장하고, 이 값은 Mount 위치 이동 시 사용된다.
	MPos_XY m_posInspectionResult;

	// Before Inspection 시 이전 Panel Mark Vision 좌표 저장
	MPos_XY m_rgxyOldInspectionPanelMarkVisionPos[2];

	/** 사전 결정 Data */

	// 사용자가 사용하는 Tab 번호 : (STabProcessData 배열 Index + 1)
	int m_iTabNo;

	// 작업변
	EWorkingSide m_eWorkSide;

	// Tab 부착 여부
	BOOL m_bUseTab;

	// Tab 부착 여부 (Repair)
	BOOL m_bRepairUseTab;

	// 이 Tab 을 부착하는 TabMounter 번호
	int m_iWorkMounterNo;

	//KKY_____
	// 이 Tab을 공급하는 TabFeeder 번호(DEF_TABFEEDER1 ~ DEF_TABFEEDER2)
	int m_iWorkTabFeederNo;
	// 이 Tab을 공급하는 Carrier 번호(DEF_TABCARRIER1 ~ DEF_TABCARRIER4)
	int m_iWorkTabCarrierNo;
	//________

	// 이 Tab 을 부착하는 TabMounter 와 함께(동시에) Tab Prealign 작업을 하는 TabMounter 번호(0,1 / 2,3 / 4,5 / 6,7)
	int m_iPairWorkTabMounterNo;

	// 이 Tab 에 대한 Prealign 작업을 담당하는 TabAligner 의 작업 그룹 내에서의 번호. (Front/Rear : 0, 1)
	int m_iWorkTabAlignerNo;

	// 이 Tab 을 Inspection 할 Inspection Camera Unit 번호
	int m_iWorkInspectorNo;

	// 이 Tab 을 부착하는 TabMounter 의 부착 시 Model 좌표
	// 따라서 이 값은 Inspection CameraCarrier 축 Model 좌표와 동일.
	// Model Offset 을 두 군데서 관리하는 게 맞나? Repair 감안하면, 여기도 값이 있어야 하는데, 두 군데는 불합리...
	double m_dModelOffset;


} STabProcessData, *pSTabProcessData;

typedef struct
{
	STabProcessData* pTab;

	int iStart;
	int iEnd;
	int iGap;
} SWorkSideData;

/**	ProcessData 검증용 Data Structure : 사용 여부 ?
 */
/*
typedef struct
{
	// Carrier 가 흡착하고 있는 Tab 번호 (흡착하고 있지 않으면 -1)
	int m_iCarrierTabNo;

	// TabMounter1 이 흡착하고 있는 Tab 번호 (흡착하고 있지 않으면 -1)
	int m_iMounter1TabNo;

	// TabMounter2 가 흡착하고 있는 Tab 번호 (흡착하고 있지 않으면 -1)
	int m_iMounter2TabNo;

} SVerifyData, *pSVerifyData;
*/

// 한 변 작업에 필요한 데이터를 관리한다.
// 작업 변 별로 생성해서 사용해야 한다.
class MProcessData
{
private:

	const MSystemData* m_plnkSystemData;
	const MPanelData* m_plnkPanelData;
	const MTabData* m_plnkTabData;

	// Carrier Load Complete Data 보호 Critical Section
	// : m_rgbNextPanelTabICLoad[] 와 연관되는 부분에서 사용 필요.
	CRITICAL_SECTION m_rgcsCarrierLoadWork[DEF_MAX_TABMOUNTER];

	// Update Process Data 시 Data 접근 금지 Critical Section
	CRITICAL_SECTION m_csDataUpdate[DEF_MAX_WORK_SIDE];
	
	// 자동운전 초기화 후 바로 작업 시작하지 않도록 설정하는 변수
	// 자동운전 초기화를 위해 MProcessData::Initialize() 가 호출되면 False 로 설정되고,
	// Panel 이 준비되면 외부에서 True 로 설정해서 작업을 진행한다.
	BOOL m_bWorkStartAfterUnitInitialization;

	// 자동운전 초기화 후 최초 Process Data Update 작업이 수행되었는지 여부 저장.
	BOOL m_bInitialUpdateProcessDataComplete;

	// 다음 Panel 을 위한 TabIC 를 Carrier 가 Loading 해야 하는지 여부 저장.
	BOOL m_rgbGetNextPanelTabIC[DEF_MAX_WORKER_GROUP];

	// 다음 작업 Panel 에 대한 사전 Carrier Load Complete Flag 관리 변수
	BOOL* m_rgbNextPanelTabICLoad[DEF_MAX_WORKER_GROUP];

	// jdy2
	// Tab 간 최소 Pitch 가 Inspection Camera Safe Margin 보다 작은지 여부 저장.
	BOOL m_rgbIsTabPitchSmallerThanCameraCarrierSafeMargin[DEF_MAX_WORKER_GROUP];

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	// Xu 변 사전 Tab Data 동적 배열을 가리킬 Pointer
	STabProcessData* m_prgXuData;
	
	// Xd 변 사전 Tab Data 동적 배열을 가리킬 Pointer
	STabProcessData* m_prgXdData;

	// Yl 변 사전 Tab Data 동적 배열을 가리킬 Pointer
	STabProcessData* m_prgYlData;

	// Yr 변 사전 Tab Data 동적 배열을 가리킬 Pointer
	STabProcessData* m_prgYrData;

	// 실제 압착 작업을 수행하는 시작 Tab 번호 관리
	int m_rgiStartTabNo[DEF_MAX_WORK_SIDE];

	// 실제 압착 작업을 수행하는 마지막 Tab 번호 관리
	int m_rgiEndTabNo[DEF_MAX_WORK_SIDE];

private:
	void deletePocessData(void);

	SWorkSideData getWorkSideData(EWorkingSide eWorkSide);

	int getGroupNo(EWorkingSide eWorkSide);

	// 현재 Work Range 가 1st Work Range 인지 2nd WorkRange 인지 구분한다.
	const int getWorkRangeNo(EWorkingSide eWorkSide);

	// 지정된 Cycle 작업에 필요한 시작 Tab 번호와 종료 Tab 번호를 반환한다.
	void getWorkCycleOpTabNo(const EWorkingSide eWorkSide, const SWorkSideData* const ws, int* piStartNo, int* piEndNo);

	// 현재 설비가 Source 설비인 경우, 설비 번호에 따라 첫번째 작업 Cycle 을 수행하는데 필요한 시작 Tab 번호와 종료 Tab 번호를 반환한다.
	void getSourceUnit1stCycleOpTabNo(const SWorkSideData* const ws, int* piStartNo, int* piEndNo);

	// 해당 Panel 작업변에서 실제로 작업을 시작하는 첫번째 Tab 번호를 얻는다.
	const int getStartTabNo(EWorkingSide eWorkSide);

	// 해당 Panel 작업변에서 실제로 작업하는 마지막 Tab 번호를 얻는다.
	const int getEndTabNo(EWorkingSide eWorkSide);

	const int calculateInspectorNo(EWorkingSide eWorkSide, int iTabNo);

	const int calculatePairTabMounterNo(EWorkingSide eWorkSide, int iRefTabMounterNo);

	const int calculateWorkTabAlignerNo(EWorkingSide eWorkSide, int iRefTabMounterNo);

	// jdy2
	const BOOL isTabPitchSmallerThanCameraCarrierSafeMargin(int iTabQty, const double rgdTabPitch[DEF_MAX_TAB]);

public:
	MProcessData(MSystemData* pSystemData, MPanelData* pPanelData, MTabData* pTabData);
	~MProcessData(void);

	void ChangeProcessData(void);

	MPos_XY* GetOldInspectionPanelMarkVisionPosStoragePointer(EWorkingSide eWorkSide, int iTabNo);

	// 인자로 넘어온 Tab 을 작업하는 TabMounter 번호를 얻는다.
	int GetWorkTabMounterNo(EWorkingSide eWorkSide, int iTabNo);

	// Manual Mode Teaching Dlg 에서 사용하기 위한 함수임 : TabMounter 에서 사용하는 Tab 번호를 반환한다.
	// 무조건 해당 Unit 의 1st Work Cycle 에서 사용하는 Tab 번호를 반환한다.
	// Carrier VacID 와 TabMounter ID 는 일치하므로 이 함수를 이용해서
	// 해당 Carrier Vacuum ID 에서 흡착하게 될 TabNo 를 반환할 수도 있다.
	int GetMounterDefaultTabNo(EWorkingSide eWorkSide, const int iTabMounterNo);

	// 해당 Tab 번호에 대한 TabIC Prealign 결과를 얻는다.
	double GetTabPrealignOffset(EWorkingSide eWorkSide, int iTabNo);

	// 해당 작업변의 TabIC 최대 부착 개수를 반환한다.
	const int GetMaxTabQty(EWorkingSide eWorkSide);

	// 인자로 넘어온 TabMounter 가 해당 작업변 작업 시 해당 Work Cycle 에 대한 Group Leader TabMounter 인지 여부를 반환한다.
	const BOOL IsWorkSideTabMounterGroupLeader(EWorkingSide eWorkSide, int iTabMounterNo);

	// 작업부(Front/Rear) 구분 번호로 해당 작업부에서 작업하는 작업변을 알아낸다.
	EWorkingSide GetCurrentWorkingSide(int iWorkGroupNo);

	// 해당 Panel 작업변에서 실제로 작업을 시작하는 첫번째 Tab 번호를 얻는다.
	const int GetStartTabNo(EWorkingSide eWorkSide);

	// 인자로 지정된 작업변의 해당 tab 에 대해서 Prealign 작업을 수행하는 TabAligner ID 를 반환한다.
	// 단, 이 때 반환되는 값은 작업부(Front/Rear) 에 따라 0 부터 시작되는 ID 값이다.
	const int GetWorkTabAlignerID(EWorkingSide eWorkSide, int iTabNo);

	// 현재 작업변에서 인자로 지정된 Tab 번호 작업을 담당한 TabMounter 와 Tab Prealign 작업을 함께 하는 TabMounter 의 번호를 반환한다.
	int GetPairTabAlignWorkTabMounterNo(EWorkingSide eWorkSide, int iTabNo);

	/* jdy sesl : 불필요 삭제.
	// jdy2
	// 작업하는 Tab 이 있는지 여부를 조사한다.
	const BOOL ProcessWorkExists();
	*/

	// 작업변에 대한 ProcessData Log 반환.
	CString GetDataLog(EWorkingSide eWorkSide);

	// 확인 필요 : 모든 인자와 Return 값에 대한 유효성 검사 할 것.

	// Gate의 경우 간격이 좁아서 Inspection Camera를 2축만 사용하는 경우
	BOOL	UseTwoInspectionCamera();

};



//###########################################################################
typedef struct  tagSWorkScheduleRefData
{
	MSystemData*		m_plnkSystemData;
	MPanelData*			m_plnkPanelData;
	MTabData*			m_plnkTabData;
	MPreBonderData*		m_plnkPreBonderData;
	int					m_iFirstTabNo;
	int					m_iLastTabNo;
} SWorkScheduleRefData;

typedef struct  tagSWorkScheduleRefProcess
{
	MTrsPanelAligner*	m_plnkTrsPanelAligner;
	MTrsTabFeeder*		m_plnkTrsTabFeeder[DEF_MAX_TABFEEDER];
} SWorkScheduleRefProcess;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
template <class T>
class MVarList
{
private:
	CList<T, T&>	m_list;
	int				m_iWorkGroup;//0:Front, 1:Rear
	int				m_iSupplyGroup;//TabCarrier Group
	int				m_iKeyValue;
	BOOL			m_bReservedSupply;
	BOOL			m_bLastGroup;
public:
	MVarList();
	MVarList(MVarList& list);
	virtual ~MVarList();

	void Clear();
	int GetCount();
	void AddHead(T& info);
	void Append(T& info);
	void Delete(int no);
	T GetInfo(int no);
	void SetInfo(int no, T& info);
	MVarList& operator=(MVarList& list);
	int	 GetWorkGroup();
	void SetWorkGroup(int iGroup);
	int	 GetKeyValue();
	void SetKeyValue(int iValue);
	BOOL GetReservedSupply();
	void SetReservedSupply(BOOL bReserve);
	void SetLastGroup(BOOL bLast);
	BOOL GetLastGroup();
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class MJobInfo
{
public:
	MJobInfo();
	MJobInfo(MJobInfo& info);
	virtual ~MJobInfo();

	MJobInfo& operator=(MJobInfo& info);
	int Init();
	void SetPressComplete();
public:
	int		m_nWorkedGroup;				//Front, Rear. TabCarrier에서 TabMounter로 Tab넘겨줄때 또는 초기화시 TabMounter에 따라 지정
	int		m_nWorkedToolID;			//Source일때는 WorkCarrierNo와 같음(0 ~ 3)
//	int		m_nWorkedCarrierID[DEF_MAX_GROUP];
//	int		m_nWorkedToolID[DEF_MAX_GROUP];
	int		m_nWorkedInspectionID;
	BOOL	m_bToolLoadComplete;
	BOOL	m_bInspectionComplete;
	BOOL	m_bPressComplete;
	BOOL	m_bSkip;
	MPos_XYT	m_OffsetPrealign;
	MPos_XYT	m_OffsetInspection;
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class MTabInfo
{
public:
	int			m_nTabNo;		//Schedule의 TabNo
	int			m_nRealTabNo;	//TabData의 실제 TabNo
	double		m_dTabPitch;
	double		m_dModelOffset;				// 이 Tab 을 부착하는 TabMounter 의 부착 시 Model 좌표. Inspection CameraCarrier 축 Model 좌표와 동일
	MJobInfo	m_jobInfo;
public:
	MTabInfo();
	MTabInfo(MTabInfo& info);
	virtual ~MTabInfo();
	MTabInfo& operator=(MTabInfo& info);
	void Init();
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class MInspectCarrierInfo
{
public:
	int		m_nWorkTabNo;
	int		m_nRealTabNo;
	double	m_dInspectPos;
	BOOL	m_bWorkSkipAfterInspect;
	int		m_nCarrierNo;
public:
	MInspectCarrierInfo();
	virtual ~MInspectCarrierInfo();
	MInspectCarrierInfo& operator=(MInspectCarrierInfo& info);
	void Init();
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class MListWorkTab
{
private:
	int					m_nTotalWorkTab;
	int					m_nLastKeyValue;
private:
	//CTypedPtrList<CPtrList, MWorkTabList*> m_list;
	CTypedPtrList<CPtrList, MVarList<MTabInfo>*> m_list;
public:
	MListWorkTab();
	virtual ~MListWorkTab();
	void Clear();
	int GetCount();
	void AddHead(MVarList<MTabInfo>* pl);
	void Append(MVarList<MTabInfo>* pl);
	void Insert(int no, MVarList<MTabInfo>* pl);
	void Delete(int no);
	MVarList<MTabInfo>* GetSubList(int no);
	int	GetTotalWorkTabCount();
	MListWorkTab& CopyData(MListWorkTab& list);
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class MWorkSchedule
{
private:
	BOOL m_bLock;
	static int	m_nInstanceCnt;
	MSystemData* m_plnkSystemData;
	MPanelData* m_plnkPanelData;
	MTabData* m_plnkTabData;
	MPreBonderData* m_plnkPreBonderData;
	EWorkingSide	m_eWorkSide;
	const int m_iWorkGroup;
	const int m_iWorkScheduleNo;
	int	m_iFirstTabNo;
	int	m_iLastTabNo;

	CCriticalSection m_csLock;
	SWorkScheduleRefProcess m_RefProcess;

	//모델 변경시 생성되는 Data
	MListWorkTab	m_OrgListWorkTab;
	//Panel Aligner에 있는 Panel의 WorkSchedule
	MListWorkTab	m_CurWorkSchedule;

	static BOOL m_bWorkPanelExist;	//작업할 Panel이 있는지
	BOOL m_bUseWorkSide;			//PanelData의 UseFlag
	//초기화시 Panel있는지 확인하여 TRUE/FALSE 해줌.
	//Mount작업 완료하면 FALSE -> WorkClear 시킨후, Transfer나 PanelAligner에 Panel이 있으면 generateWork -> TRUE시킴.
	BOOL m_bExistWorkList;		//Inspection에서 작업할 List가 있는지 확인하는 Flag
	BOOL m_bCompleteCurWorkList;//현재 Mounting중인 List가 작업완료 되었는지 확인하는 Flag

	BOOL m_bCompleteSubWorkList[DEF_MAX_SUB_LIST];
	BOOL m_bExistSupplyList[DEF_MAX_GROUP];//TabCarrier에서 공급할 List가 결정되었는지 확인하는 Flag

	//TrsTabCarrier관련____________________________________________
	//101223.Add작업할 Panel이 없거나, Tab공급을 완료했을 경우 TabCarrier가 연속으로 Tab를 공급하기 위해 미리 Supplylist를 만든다
//	int								m_iSupplyListNo;
//	int								m_iTabSupplyCnt[DEF_MAX_SUB_LIST];
	MVarList<MTabInfo>				*m_pCurrentSupplyList[DEF_MAX_GROUP];
	//_____________________________________________________________

	//TrsInspectionCamera관련 _____________________________________
	//TrsInspectionCamera에서 검색한 현재 TabMounter할 List
	MVarList<MTabInfo>				*m_pCurrentWorkList;
	MVarList<MInspectCarrierInfo>	m_InspectCarrierList;//현재 작업할 CameraCarrier정보 List
	MVarList<MInspectCarrierInfo>	m_FirstInspectCarrierList;//첫번째 작업할 CameraCarrier정보 List
	//_____________________________________________________________

	//TabCarrier에서 처음 공급할 List
	MVarList<MTabInfo>				*m_pFirstSupplyList[DEF_MAX_GROUP];

	MVarList<MInspectCarrierInfo>	m_TabMounterListSupply;	

	BOOL							m_bFeederReady[DEF_MAX_GROUP];

	void	SetLock();
	void	SetUnlock();
	void	DoEvents();

private:
	void	sortingInspectCarrierPos(MVarList<MInspectCarrierInfo> *pList, BOOL bLast = FALSE);
	double	GetPitchSum(int nTabNo);

	//@130824_____________
	MVarList<MTabInfo>				*getCurCarrierSupplyList(int iWorkGroup);
	//@___________________
public:
	MWorkSchedule(const EWorkingSide eWSide, const int iWorkGroup, const int iScheduleNo);
	virtual ~MWorkSchedule();

/***********************************************************************************************
/*
/* Process Assign Interface
/*
/************************************************************************************************/
	void AssignData(SWorkScheduleRefData RefData);
	void AssignProcess(SWorkScheduleRefProcess listRefTrs);

public:
	int				ChangeModel();
	void			GetOrgWorkList(MListWorkTab *pList);
	BOOL			GetUseWorkSide();
	//___________________________________________________________________
	
	//작업패널이 들어와서 처음 WorkSchedule Data를 만들때
	int		GenerateWorkSchedule();

	BOOL	IsGeneratedWorkSchedule();

	//Mount작업이 완료된후 작업 List를 Clear시킨다.
	int		ClearWorkSchedule();

	//PanelAligner에 Panel이 있을 경우 MJobInfo의 m_bPress 여부에 따라 WorkSchedule을 초기화 한다
	int		GenerateWorkScheduleByPanel();

	//Tab정보를 변경한다. 고유한 번호는 Tab번호이므로, List에 접근시 Tab번호를 사용한다
	int		UpdateWorkTabInfo(MTabInfo info);

	void	SetExistWorkPanel(BOOL bExist);
	BOOL	GetExistWorkPanel();

	void	SetFeederReady(int iWorkGroup, BOOL bReady);
	//_____________________________________________________________

	
	//TrsTabCarrier관련 함수_______________________________________	
public:
	int					IsAllSupplyComplete(BOOL bCheckGroup, int iWorkGroup);
	int					GetSupplyTabCnt(int iWorkGroup);
	BOOL				IsExist_SupplyWorkList(int iWorkGroup);
	int					GetKeyValue_SupplyWorkList(int iWorkGroup);
	int					GetCarrierToTool_FromCurrentSupplyList(int iMountGroup, int iCarrierGroup, int iCarrierNo);
	BOOL				IsSupplyTab(int iWorkGroup, int iCarrierNo);
	int					GetWorkGroup_CarrierSupply(int iCarrierWorkGroup);//Carrier에서 현재 공급하는 Tab의 작업 그룹. 8Mounter Mode에서 사용

	//@Carrier가 Ready위치 대기중 TabLoading할 다음 Carrier가 있는지 확인
	BOOL				IsExistNextSupplyCarrier(int iWorkGroup, int iCarrierNo);

private:
	BOOL				isLoadComplete_SupplyWorkList(int iWorkGroup);
	//_____________________________________________________________

	//TrsTabMounter관련 함수_______________________________________
public:
	MTabInfo			GetTabInfo_SupplyWorkList(int iWorkGroup, int iToolID);
	int					SetMounterTabLoadComplete_SupplyWorkList(int iWorkGroup, int iToolID);
	int					PickOut_SupplyWorkList(int iWorkGroup);
	int					GetFirstWorkToolNo_FromCurrentSupplyList(int iWorkGroup);
	//현재 Supply작업중인 List에서 iTabNo를 넘겨주고, 작업할 Carrier번호를 얻는다
	int					GetCarrierNo_FromCurrentSupplyList(int iWorkGroup, int iTabNo);
	BOOL				IsLoadComplete_SupplyList(int iKeyValue);
	BOOL				IsInspectionSkip_CurrentWorkList(int iWorkInstance);
	BOOL				IsInspectionCompleteGroup_CurrentWorkList();
	int					GetLastPickoutGroup();
	//_____________________________________________________________
	
	//TrsInspectionCamera관련 함수_________________________________
public:
	MVarList<MInspectCarrierInfo> GetFirstInspectionCarrierList();
	//TabMount해야할 List를 뽑아낸다(MTrsInspectionCamera에서 호출함)
	int					PickOut_CurrentWorkList();
	BOOL				IsExist_CurrentWorkList();
	MInspectCarrierInfo	GetInspectCarrierInfo_FromCurrentWorkList(int iInstanceNo);
	MInspectCarrierInfo	GetInspectCarrierInfo_FromFirstWorkList(int iInstanceNo);
	MInspectCarrierInfo	GetFirstWorkTabCarrierInfo_FromFirstWorkList();
	MInspectCarrierInfo	GetLastWorkTabCarrierInfo_FromFirstWorkList();

	//현재 Press작업중인 List에 WorkGroup이 지정되었는지 확인. TrsCameraCarrier에서 ITITabMounter전 확인함.
	BOOL				IsSpecifiedWorkGroup();
	//현재 Press작업중인 List에서 iTabNo를 넘겨주고, 작업할 Mounter번호를 얻는다
	int					GetTabMounterNoFromCurrentWorkList(int iTabNo);
	//현재 Press작업중인 List에서 iTabNo를 넘겨주고, 작업할 Inspection Carrier번호를 얻는다
	int					GetInspectionNoFromCurrentWorkList(int iTabNo);
	//Inspection중 인식에러로 인해 TabSkip해야할 경우
	void				SetTabSkipToCurrentWorkList(int iTabNo);
	//Tool이			Inspection 완료후
	void				SetInspectionComplete(int iTabNo);	
	//Tool이 Tab Remove동작후 ToolGroupStarter에서 
	void				ResetInspectionComplete_CurWorkList();

	void				SetPressComplete(int iTabNo);

	BOOL				IsPressComplete_CurrentWorkList();

	void				AppendSkipTabAfterInspection();

	BOOL				IsPressComplete_AllWorkList();

	BOOL				IsPressCompleteSelectWorkList(int iKeyValue);

	int					GetKeyValue_CurrentWorkList();

	MTabInfo			GetTabInfo_CurrentWorkList(int iWorkInstance);

	double				GetTabPos(int iTabNo);

	void				SetInspectionOffset(int iTabNo, MPos_XYT posoffset);
	MPos_XYT			GetInspectionOffset(int iTabNo);

	//PickOut_CurrentWorkList()함수에서 m_pCurrentWorkList의 Tab과 작업할 InspectionCamera를 지정한다
	int	FittingTabNoToInspectionNo(MVarList<MTabInfo> *tList, MVarList<MInspectCarrierInfo> *cList);

	//작업할 TabList로 InspectionCarrier의 정보를 구한다
	MVarList<MInspectCarrierInfo> CreateInspectionCarrierInfo(MVarList<MTabInfo> varlist, BOOL bTrace=TRUE);

private:
	//iKeyValue에 해당하는 List의 Group을 지정한다
	//TrsCarrier에서 작업 List를 요구하거나 초기화 할때 호출되며,
	//이때 각Tab에 해당하는 WorkTool번호도 구하여(FittingTabNoToMountNo()) 준다
	int		setWorkGroup(int iWorkGroup, int iKeyValue);

	BOOL	isAllFeederReady();
	//_____________________________________________________________


	//공통함수_____________________________________________________
public:
	int				GetFirstTabNo() { return m_iFirstTabNo; }
	int				GetLastTabNo() { return m_iLastTabNo; }

	// 해당 작업변의 TabIC 최대 부착 개수를 반환한다.
	int GetMaxTabQty(EWorkingSide eWorkSide);

	//Press하지 않은 Tab이 있는지 확인(m_CurWorkSchedule)
	BOOL	IsMoreWorkForCurrentPanel();

	BOOL	IsLastWorkGroup();//현재 Press작업중인 WorkGroup이 마지막인지

	//Front, Rear에 따라서 작업할 Tool및 TabCarrier의 번호를 구한다
	void	FittingTabNoToMountNo(MVarList<MTabInfo> *tList, MVarList<MInspectCarrierInfo> *cList);
	//_____________________________________________________________

	int		GetWorkScheculeNo() { return m_iWorkScheduleNo; }
};
//###########################################################################

#endif	// DEF_MPROCESS_DATA_H