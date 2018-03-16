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

// ��� ����
// 1st Cycle, 2nd Cycle : ������ Unit �� �۾��ؾ��� Panel �� ���� ��, TabMounter Group ���� �����ϴ� ù��° Work Cycle ����
//                        �ι�° Work Cycle ������ ������ �� ����Ѵ�.
// 1st Work Range, 2nd Work Range
//		: �۾� �����ʹ� ������� Tab ��ȣ�� ���� �����Ǵ� ������
//        �۾� ���� Tab ��ȣ���� �����ؼ� TabMounter Group �� ��ü TabMounter �� �ѹ��� �۾��� �� �ִ� Tab ��ȣ ������ 1st Work Range �� �ϰ�,
//        1st Work Range ������ Tab ��ȣ ������ 2nd Work Range �� �Ѵ�.
//      - ���⼭ �۾� ���� Tab ��ȣ��, ����ڰ� ���� �۾��ϵ��� ������ Tab ��ȣ �߿��� �۾� ���� �� ���� ���� �۾��Ǵ� Tab ��ȣ�� �ǹ��Ѵ�. 

//const int DEF_MAX_WORK_RANGE		= 2;
const int DEF_MAX_WORK_RANGE		= 4;	//KKY.Modify
const int DEF_WORK_RANGE_NONE		= -1;
const int DEF_WORK_RANGE_1			= 0;
const int DEF_WORK_RANGE_2			= 1;
const int DEF_WORK_RANGE_3			= 2;	//KKY.Add
const int DEF_WORK_RANGE_4			= 3;	//KKY.Add

const int DEF_MAX_SUB_LIST			= 16;

// Inspection Camera �� �ٸ� Inspection Camera �� �۾��� ����
// �ٸ� Inspection Camera �� �۾��ؾ� �ϴ� Tab �ٷ� �� Tab ��ġ��
// �̵��϶�� �����. (Source : Inspection Camera Carrier 2 ���� �ش�Ǵ� �����.)
const int DEF_MOVE_WAIT_POS_COMMAND	= -100;

typedef enum
{
	UNIT_TYPE_NONE = -1,
	UNIT_TYPE_TAB_CARRIER = 0,
    UNIT_TYPE_TAB_MOUNTER,
    UNIT_TYPE_INSPECTION_CAMERA,
	UNIT_TYPE_PANEL_ALIGNER,
} EUnitType;

/**	�ڵ����� ���� �� �ǽð����� Tab ���� ���� ���� Data �� ����ü
 *	�Ʒ� ����ü�� Instance �� ����� �ϴ� �۾��� ���� ���� Tab ������ŭ�� �迭�� �����Ǿ���ϰ�, 
 *	�� �迭���� Tab ��ȣ�� Index �� ������ �� �ִ�.
 */
typedef struct
{
	/** �ǽð� Data */

	// Error �߻� �� �� �ش� Tab ���� �۾� Skip ����, Init ó��?
	BOOL m_bWorkSkip;

	// Tab ���� �۾� ����
	BOOL m_bRemoveTabIC;

	// TabCarrier �� TabFeeder �κ��� TabIC Load �Ϸ� ����
	BOOL m_bCarrierLoadCompleted;

	// �ش� Tab �� ���� Mounter Load �Ϸ� ����
	// �ڵ����� �� Tab Load �� Tab ������ ���� ������ �߻��ϸ�, �� Flag �� Clear ���ִ� ���� ó���� �ʿ��ϴ�.
	BOOL m_bMounterLoadCompleted;

	// Tab Prealign �Ϸ� ���� : Source ���� ���ʿ�, Gate ���� Inspection ��ġ �̵� �� Ȯ���ؾ� �ϴ� ������.
	BOOL m_bTabPrealignCompleted;

	// �ش� Tab Inspection �Ϸ� ����
	BOOL m_bInspectionCompleted;

	// �ش� Tab ���� �Ϸ� ����
	BOOL m_bMountCompleted;

	// �ش� Tab Mount After Inspection �Ϸ� ����
	BOOL m_bAfterInspectionCompleted;

	// Carrier Load ���� Retry Ƚ�� ����
	int m_iCarrierLoadRetryCount;

	// Mounter Load ���� Retry Ƚ�� ����
	int m_iMounterLoadRetryCount;

	// Tab Prealign ���� Retry Ƚ�� ����
	int m_iTabPrealignRetryCount;

	// 20081008 jdy
	// Inspection : TabIC Mark �ν� Error ���� Retry Ƚ�� ����
	int m_iInspectionRetryCount;

	// Panel - TabIC LeadMark Inspection ��� Offset ����. - ������...
	// Tab Prealign �� Panel X ���� Offset �� �����ϰ�, �� ���� Inspection ��ġ �̵� �� ���ȴ�.
	MPos_XY m_posTabPrealignResult;

	// Panel - TabIC LeadMark Inspection ��� Offset ����. - ������...
	// Tab Inspection �� Panel X ���� Offset �� �����ϰ�, �� ���� Mount ��ġ �̵� �� ���ȴ�.
	MPos_XY m_posInspectionResult;

	// Before Inspection �� ���� Panel Mark Vision ��ǥ ����
	MPos_XY m_rgxyOldInspectionPanelMarkVisionPos[2];

	/** ���� ���� Data */

	// ����ڰ� ����ϴ� Tab ��ȣ : (STabProcessData �迭 Index + 1)
	int m_iTabNo;

	// �۾���
	EWorkingSide m_eWorkSide;

	// Tab ���� ����
	BOOL m_bUseTab;

	// Tab ���� ���� (Repair)
	BOOL m_bRepairUseTab;

	// �� Tab �� �����ϴ� TabMounter ��ȣ
	int m_iWorkMounterNo;

	//KKY_____
	// �� Tab�� �����ϴ� TabFeeder ��ȣ(DEF_TABFEEDER1 ~ DEF_TABFEEDER2)
	int m_iWorkTabFeederNo;
	// �� Tab�� �����ϴ� Carrier ��ȣ(DEF_TABCARRIER1 ~ DEF_TABCARRIER4)
	int m_iWorkTabCarrierNo;
	//________

	// �� Tab �� �����ϴ� TabMounter �� �Բ�(���ÿ�) Tab Prealign �۾��� �ϴ� TabMounter ��ȣ(0,1 / 2,3 / 4,5 / 6,7)
	int m_iPairWorkTabMounterNo;

	// �� Tab �� ���� Prealign �۾��� ����ϴ� TabAligner �� �۾� �׷� �������� ��ȣ. (Front/Rear : 0, 1)
	int m_iWorkTabAlignerNo;

	// �� Tab �� Inspection �� Inspection Camera Unit ��ȣ
	int m_iWorkInspectorNo;

	// �� Tab �� �����ϴ� TabMounter �� ���� �� Model ��ǥ
	// ���� �� ���� Inspection CameraCarrier �� Model ��ǥ�� ����.
	// Model Offset �� �� ������ �����ϴ� �� �³�? Repair �����ϸ�, ���⵵ ���� �־�� �ϴµ�, �� ������ ���ո�...
	double m_dModelOffset;


} STabProcessData, *pSTabProcessData;

typedef struct
{
	STabProcessData* pTab;

	int iStart;
	int iEnd;
	int iGap;
} SWorkSideData;

/**	ProcessData ������ Data Structure : ��� ���� ?
 */
/*
typedef struct
{
	// Carrier �� �����ϰ� �ִ� Tab ��ȣ (�����ϰ� ���� ������ -1)
	int m_iCarrierTabNo;

	// TabMounter1 �� �����ϰ� �ִ� Tab ��ȣ (�����ϰ� ���� ������ -1)
	int m_iMounter1TabNo;

	// TabMounter2 �� �����ϰ� �ִ� Tab ��ȣ (�����ϰ� ���� ������ -1)
	int m_iMounter2TabNo;

} SVerifyData, *pSVerifyData;
*/

// �� �� �۾��� �ʿ��� �����͸� �����Ѵ�.
// �۾� �� ���� �����ؼ� ����ؾ� �Ѵ�.
class MProcessData
{
private:

	const MSystemData* m_plnkSystemData;
	const MPanelData* m_plnkPanelData;
	const MTabData* m_plnkTabData;

	// Carrier Load Complete Data ��ȣ Critical Section
	// : m_rgbNextPanelTabICLoad[] �� �����Ǵ� �κп��� ��� �ʿ�.
	CRITICAL_SECTION m_rgcsCarrierLoadWork[DEF_MAX_TABMOUNTER];

	// Update Process Data �� Data ���� ���� Critical Section
	CRITICAL_SECTION m_csDataUpdate[DEF_MAX_WORK_SIDE];
	
	// �ڵ����� �ʱ�ȭ �� �ٷ� �۾� �������� �ʵ��� �����ϴ� ����
	// �ڵ����� �ʱ�ȭ�� ���� MProcessData::Initialize() �� ȣ��Ǹ� False �� �����ǰ�,
	// Panel �� �غ�Ǹ� �ܺο��� True �� �����ؼ� �۾��� �����Ѵ�.
	BOOL m_bWorkStartAfterUnitInitialization;

	// �ڵ����� �ʱ�ȭ �� ���� Process Data Update �۾��� ����Ǿ����� ���� ����.
	BOOL m_bInitialUpdateProcessDataComplete;

	// ���� Panel �� ���� TabIC �� Carrier �� Loading �ؾ� �ϴ��� ���� ����.
	BOOL m_rgbGetNextPanelTabIC[DEF_MAX_WORKER_GROUP];

	// ���� �۾� Panel �� ���� ���� Carrier Load Complete Flag ���� ����
	BOOL* m_rgbNextPanelTabICLoad[DEF_MAX_WORKER_GROUP];

	// jdy2
	// Tab �� �ּ� Pitch �� Inspection Camera Safe Margin ���� ������ ���� ����.
	BOOL m_rgbIsTabPitchSmallerThanCameraCarrierSafeMargin[DEF_MAX_WORKER_GROUP];

	//����������������������������������������������������������������������������������

	// Xu �� ���� Tab Data ���� �迭�� ����ų Pointer
	STabProcessData* m_prgXuData;
	
	// Xd �� ���� Tab Data ���� �迭�� ����ų Pointer
	STabProcessData* m_prgXdData;

	// Yl �� ���� Tab Data ���� �迭�� ����ų Pointer
	STabProcessData* m_prgYlData;

	// Yr �� ���� Tab Data ���� �迭�� ����ų Pointer
	STabProcessData* m_prgYrData;

	// ���� ���� �۾��� �����ϴ� ���� Tab ��ȣ ����
	int m_rgiStartTabNo[DEF_MAX_WORK_SIDE];

	// ���� ���� �۾��� �����ϴ� ������ Tab ��ȣ ����
	int m_rgiEndTabNo[DEF_MAX_WORK_SIDE];

private:
	void deletePocessData(void);

	SWorkSideData getWorkSideData(EWorkingSide eWorkSide);

	int getGroupNo(EWorkingSide eWorkSide);

	// ���� Work Range �� 1st Work Range ���� 2nd WorkRange ���� �����Ѵ�.
	const int getWorkRangeNo(EWorkingSide eWorkSide);

	// ������ Cycle �۾��� �ʿ��� ���� Tab ��ȣ�� ���� Tab ��ȣ�� ��ȯ�Ѵ�.
	void getWorkCycleOpTabNo(const EWorkingSide eWorkSide, const SWorkSideData* const ws, int* piStartNo, int* piEndNo);

	// ���� ���� Source ������ ���, ���� ��ȣ�� ���� ù��° �۾� Cycle �� �����ϴµ� �ʿ��� ���� Tab ��ȣ�� ���� Tab ��ȣ�� ��ȯ�Ѵ�.
	void getSourceUnit1stCycleOpTabNo(const SWorkSideData* const ws, int* piStartNo, int* piEndNo);

	// �ش� Panel �۾������� ������ �۾��� �����ϴ� ù��° Tab ��ȣ�� ��´�.
	const int getStartTabNo(EWorkingSide eWorkSide);

	// �ش� Panel �۾������� ������ �۾��ϴ� ������ Tab ��ȣ�� ��´�.
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

	// ���ڷ� �Ѿ�� Tab �� �۾��ϴ� TabMounter ��ȣ�� ��´�.
	int GetWorkTabMounterNo(EWorkingSide eWorkSide, int iTabNo);

	// Manual Mode Teaching Dlg ���� ����ϱ� ���� �Լ��� : TabMounter ���� ����ϴ� Tab ��ȣ�� ��ȯ�Ѵ�.
	// ������ �ش� Unit �� 1st Work Cycle ���� ����ϴ� Tab ��ȣ�� ��ȯ�Ѵ�.
	// Carrier VacID �� TabMounter ID �� ��ġ�ϹǷ� �� �Լ��� �̿��ؼ�
	// �ش� Carrier Vacuum ID ���� �����ϰ� �� TabNo �� ��ȯ�� ���� �ִ�.
	int GetMounterDefaultTabNo(EWorkingSide eWorkSide, const int iTabMounterNo);

	// �ش� Tab ��ȣ�� ���� TabIC Prealign ����� ��´�.
	double GetTabPrealignOffset(EWorkingSide eWorkSide, int iTabNo);

	// �ش� �۾����� TabIC �ִ� ���� ������ ��ȯ�Ѵ�.
	const int GetMaxTabQty(EWorkingSide eWorkSide);

	// ���ڷ� �Ѿ�� TabMounter �� �ش� �۾��� �۾� �� �ش� Work Cycle �� ���� Group Leader TabMounter ���� ���θ� ��ȯ�Ѵ�.
	const BOOL IsWorkSideTabMounterGroupLeader(EWorkingSide eWorkSide, int iTabMounterNo);

	// �۾���(Front/Rear) ���� ��ȣ�� �ش� �۾��ο��� �۾��ϴ� �۾����� �˾Ƴ���.
	EWorkingSide GetCurrentWorkingSide(int iWorkGroupNo);

	// �ش� Panel �۾������� ������ �۾��� �����ϴ� ù��° Tab ��ȣ�� ��´�.
	const int GetStartTabNo(EWorkingSide eWorkSide);

	// ���ڷ� ������ �۾����� �ش� tab �� ���ؼ� Prealign �۾��� �����ϴ� TabAligner ID �� ��ȯ�Ѵ�.
	// ��, �� �� ��ȯ�Ǵ� ���� �۾���(Front/Rear) �� ���� 0 ���� ���۵Ǵ� ID ���̴�.
	const int GetWorkTabAlignerID(EWorkingSide eWorkSide, int iTabNo);

	// ���� �۾������� ���ڷ� ������ Tab ��ȣ �۾��� ����� TabMounter �� Tab Prealign �۾��� �Բ� �ϴ� TabMounter �� ��ȣ�� ��ȯ�Ѵ�.
	int GetPairTabAlignWorkTabMounterNo(EWorkingSide eWorkSide, int iTabNo);

	/* jdy sesl : ���ʿ� ����.
	// jdy2
	// �۾��ϴ� Tab �� �ִ��� ���θ� �����Ѵ�.
	const BOOL ProcessWorkExists();
	*/

	// �۾����� ���� ProcessData Log ��ȯ.
	CString GetDataLog(EWorkingSide eWorkSide);

	// Ȯ�� �ʿ� : ��� ���ڿ� Return ���� ���� ��ȿ�� �˻� �� ��.

	// Gate�� ��� ������ ���Ƽ� Inspection Camera�� 2�ุ ����ϴ� ���
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
	int		m_nWorkedGroup;				//Front, Rear. TabCarrier���� TabMounter�� Tab�Ѱ��ٶ� �Ǵ� �ʱ�ȭ�� TabMounter�� ���� ����
	int		m_nWorkedToolID;			//Source�϶��� WorkCarrierNo�� ����(0 ~ 3)
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
	int			m_nTabNo;		//Schedule�� TabNo
	int			m_nRealTabNo;	//TabData�� ���� TabNo
	double		m_dTabPitch;
	double		m_dModelOffset;				// �� Tab �� �����ϴ� TabMounter �� ���� �� Model ��ǥ. Inspection CameraCarrier �� Model ��ǥ�� ����
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

	//�� ����� �����Ǵ� Data
	MListWorkTab	m_OrgListWorkTab;
	//Panel Aligner�� �ִ� Panel�� WorkSchedule
	MListWorkTab	m_CurWorkSchedule;

	static BOOL m_bWorkPanelExist;	//�۾��� Panel�� �ִ���
	BOOL m_bUseWorkSide;			//PanelData�� UseFlag
	//�ʱ�ȭ�� Panel�ִ��� Ȯ���Ͽ� TRUE/FALSE ����.
	//Mount�۾� �Ϸ��ϸ� FALSE -> WorkClear ��Ų��, Transfer�� PanelAligner�� Panel�� ������ generateWork -> TRUE��Ŵ.
	BOOL m_bExistWorkList;		//Inspection���� �۾��� List�� �ִ��� Ȯ���ϴ� Flag
	BOOL m_bCompleteCurWorkList;//���� Mounting���� List�� �۾��Ϸ� �Ǿ����� Ȯ���ϴ� Flag

	BOOL m_bCompleteSubWorkList[DEF_MAX_SUB_LIST];
	BOOL m_bExistSupplyList[DEF_MAX_GROUP];//TabCarrier���� ������ List�� �����Ǿ����� Ȯ���ϴ� Flag

	//TrsTabCarrier����____________________________________________
	//101223.Add�۾��� Panel�� ���ų�, Tab������ �Ϸ����� ��� TabCarrier�� �������� Tab�� �����ϱ� ���� �̸� Supplylist�� �����
//	int								m_iSupplyListNo;
//	int								m_iTabSupplyCnt[DEF_MAX_SUB_LIST];
	MVarList<MTabInfo>				*m_pCurrentSupplyList[DEF_MAX_GROUP];
	//_____________________________________________________________

	//TrsInspectionCamera���� _____________________________________
	//TrsInspectionCamera���� �˻��� ���� TabMounter�� List
	MVarList<MTabInfo>				*m_pCurrentWorkList;
	MVarList<MInspectCarrierInfo>	m_InspectCarrierList;//���� �۾��� CameraCarrier���� List
	MVarList<MInspectCarrierInfo>	m_FirstInspectCarrierList;//ù��° �۾��� CameraCarrier���� List
	//_____________________________________________________________

	//TabCarrier���� ó�� ������ List
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
	
	//�۾��г��� ���ͼ� ó�� WorkSchedule Data�� ���鶧
	int		GenerateWorkSchedule();

	BOOL	IsGeneratedWorkSchedule();

	//Mount�۾��� �Ϸ���� �۾� List�� Clear��Ų��.
	int		ClearWorkSchedule();

	//PanelAligner�� Panel�� ���� ��� MJobInfo�� m_bPress ���ο� ���� WorkSchedule�� �ʱ�ȭ �Ѵ�
	int		GenerateWorkScheduleByPanel();

	//Tab������ �����Ѵ�. ������ ��ȣ�� Tab��ȣ�̹Ƿ�, List�� ���ٽ� Tab��ȣ�� ����Ѵ�
	int		UpdateWorkTabInfo(MTabInfo info);

	void	SetExistWorkPanel(BOOL bExist);
	BOOL	GetExistWorkPanel();

	void	SetFeederReady(int iWorkGroup, BOOL bReady);
	//_____________________________________________________________

	
	//TrsTabCarrier���� �Լ�_______________________________________	
public:
	int					IsAllSupplyComplete(BOOL bCheckGroup, int iWorkGroup);
	int					GetSupplyTabCnt(int iWorkGroup);
	BOOL				IsExist_SupplyWorkList(int iWorkGroup);
	int					GetKeyValue_SupplyWorkList(int iWorkGroup);
	int					GetCarrierToTool_FromCurrentSupplyList(int iMountGroup, int iCarrierGroup, int iCarrierNo);
	BOOL				IsSupplyTab(int iWorkGroup, int iCarrierNo);
	int					GetWorkGroup_CarrierSupply(int iCarrierWorkGroup);//Carrier���� ���� �����ϴ� Tab�� �۾� �׷�. 8Mounter Mode���� ���

	//@Carrier�� Ready��ġ ����� TabLoading�� ���� Carrier�� �ִ��� Ȯ��
	BOOL				IsExistNextSupplyCarrier(int iWorkGroup, int iCarrierNo);

private:
	BOOL				isLoadComplete_SupplyWorkList(int iWorkGroup);
	//_____________________________________________________________

	//TrsTabMounter���� �Լ�_______________________________________
public:
	MTabInfo			GetTabInfo_SupplyWorkList(int iWorkGroup, int iToolID);
	int					SetMounterTabLoadComplete_SupplyWorkList(int iWorkGroup, int iToolID);
	int					PickOut_SupplyWorkList(int iWorkGroup);
	int					GetFirstWorkToolNo_FromCurrentSupplyList(int iWorkGroup);
	//���� Supply�۾����� List���� iTabNo�� �Ѱ��ְ�, �۾��� Carrier��ȣ�� ��´�
	int					GetCarrierNo_FromCurrentSupplyList(int iWorkGroup, int iTabNo);
	BOOL				IsLoadComplete_SupplyList(int iKeyValue);
	BOOL				IsInspectionSkip_CurrentWorkList(int iWorkInstance);
	BOOL				IsInspectionCompleteGroup_CurrentWorkList();
	int					GetLastPickoutGroup();
	//_____________________________________________________________
	
	//TrsInspectionCamera���� �Լ�_________________________________
public:
	MVarList<MInspectCarrierInfo> GetFirstInspectionCarrierList();
	//TabMount�ؾ��� List�� �̾Ƴ���(MTrsInspectionCamera���� ȣ����)
	int					PickOut_CurrentWorkList();
	BOOL				IsExist_CurrentWorkList();
	MInspectCarrierInfo	GetInspectCarrierInfo_FromCurrentWorkList(int iInstanceNo);
	MInspectCarrierInfo	GetInspectCarrierInfo_FromFirstWorkList(int iInstanceNo);
	MInspectCarrierInfo	GetFirstWorkTabCarrierInfo_FromFirstWorkList();
	MInspectCarrierInfo	GetLastWorkTabCarrierInfo_FromFirstWorkList();

	//���� Press�۾����� List�� WorkGroup�� �����Ǿ����� Ȯ��. TrsCameraCarrier���� ITITabMounter�� Ȯ����.
	BOOL				IsSpecifiedWorkGroup();
	//���� Press�۾����� List���� iTabNo�� �Ѱ��ְ�, �۾��� Mounter��ȣ�� ��´�
	int					GetTabMounterNoFromCurrentWorkList(int iTabNo);
	//���� Press�۾����� List���� iTabNo�� �Ѱ��ְ�, �۾��� Inspection Carrier��ȣ�� ��´�
	int					GetInspectionNoFromCurrentWorkList(int iTabNo);
	//Inspection�� �νĿ����� ���� TabSkip�ؾ��� ���
	void				SetTabSkipToCurrentWorkList(int iTabNo);
	//Tool��			Inspection �Ϸ���
	void				SetInspectionComplete(int iTabNo);	
	//Tool�� Tab Remove������ ToolGroupStarter���� 
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

	//PickOut_CurrentWorkList()�Լ����� m_pCurrentWorkList�� Tab�� �۾��� InspectionCamera�� �����Ѵ�
	int	FittingTabNoToInspectionNo(MVarList<MTabInfo> *tList, MVarList<MInspectCarrierInfo> *cList);

	//�۾��� TabList�� InspectionCarrier�� ������ ���Ѵ�
	MVarList<MInspectCarrierInfo> CreateInspectionCarrierInfo(MVarList<MTabInfo> varlist, BOOL bTrace=TRUE);

private:
	//iKeyValue�� �ش��ϴ� List�� Group�� �����Ѵ�
	//TrsCarrier���� �۾� List�� �䱸�ϰų� �ʱ�ȭ �Ҷ� ȣ��Ǹ�,
	//�̶� ��Tab�� �ش��ϴ� WorkTool��ȣ�� ���Ͽ�(FittingTabNoToMountNo()) �ش�
	int		setWorkGroup(int iWorkGroup, int iKeyValue);

	BOOL	isAllFeederReady();
	//_____________________________________________________________


	//�����Լ�_____________________________________________________
public:
	int				GetFirstTabNo() { return m_iFirstTabNo; }
	int				GetLastTabNo() { return m_iLastTabNo; }

	// �ش� �۾����� TabIC �ִ� ���� ������ ��ȯ�Ѵ�.
	int GetMaxTabQty(EWorkingSide eWorkSide);

	//Press���� ���� Tab�� �ִ��� Ȯ��(m_CurWorkSchedule)
	BOOL	IsMoreWorkForCurrentPanel();

	BOOL	IsLastWorkGroup();//���� Press�۾����� WorkGroup�� ����������

	//Front, Rear�� ���� �۾��� Tool�� TabCarrier�� ��ȣ�� ���Ѵ�
	void	FittingTabNoToMountNo(MVarList<MTabInfo> *tList, MVarList<MInspectCarrierInfo> *cList);
	//_____________________________________________________________

	int		GetWorkScheculeNo() { return m_iWorkScheduleNo; }
};
//###########################################################################

#endif	// DEF_MPROCESS_DATA_H