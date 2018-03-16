#ifndef _DEFVISION_HEADER
#define _DEFVISION_HEADER

#include "DefVisionExt.h"
#include <mil.h>
#include <vector>

// Message ID 등록
#define WM_DISPLAY_SOURCE_IMAGE (WM_APP - 1)

// System Type Define
enum
{
	DEF_NONE_SYSTEM		= -1,
	DEF_METEOR_II_MC	= 0,
	DEF_SOLIOS,
};

// Dispaly Window Priority Define
enum
{
	DEF_NONE_PRIORITY				= -1,
	DEF_IMAGE_DISPLAY_VIEW_PRIORITY	= 0,
	DEF_TEMPORARY_VIEW_PRIORITY,
	DEF_LOCAL_VIEW_PRIORITY,
	DEF_FIXED_VIEW_PRIORITY
};


const int DEF_NONE_CAMERA = -1;	// Camera 없음.
const int DEF_DEF_NONE_MARK = -1;	//Mark 없음.

const int DEF_KEY_NOT_FOUND_VALUE = -99;

const int DEF_MAX_CAMERA_NUM_PER_SYSTEM = 8;
const int DEF_METEOR_SYSTEM_MAX_CAM_NUM = 6;
const int DEF_SOLIOS_SYSTEM_MAX_CAM_NUM = 8;

const int DEF_MATROX_SYSTEM_CHANNEL_NUM = 2;

const int DEF_MATROX_SOLIOS_INDEPENDENT_INPUT_PATH_NUM = 4;

const int DEF_MAX_USE_MARK = 28;	// Mark Type 별 Camera 가 가질 수 있는 최대 Mark 개수

const int DEF_MAX_MARK_NUM_FOR_MULTI_FIND = 5;	// Find Multiple Model 시 한 번에 사용할 수 있는 최대 Mark 개수

const int DEF_MAX_DRAW_OBJECT = 5;		// 일단 string, rectangle, point 모두 5 개씩으로 설정.
const int DEF_MAX_TEXT_LENGTH = 128;

const int DEF_MAX_TEXT_OBJECT = DEF_MAX_DRAW_OBJECT;
const int DEF_MAX_RECT_OBJECT = DEF_MAX_DRAW_OBJECT * 2;
const int DEF_MAX_POINT_OBJECT = DEF_MAX_DRAW_OBJECT;
const int DEF_MAX_LINE_OBJECT = DEF_MAX_DRAW_OBJECT;//110214.Add

const int DEF_MAX_CHR_LENGTH = 512;	//110411.Add

/************************************************************************/
/*                          Error Code Define                           */
/************************************************************************/

// Allocation Failure
const int ERR_APPLICATION_ALLOC_FAILURE				= 1;	// 104001 = Application Allocation 실패
const int ERR_SYSTEM_ALLOC_FAILURE					= 2;	// 104002 = System Allocation 실패
const int ERR_DIGITIZER_ALLOC_FAILURE				= 3;	// 104003 = Digitizer Allocation 실패
const int ERR_BUFFER_ALLOC_FAILURE					= 4;	// 104004 = Buffer Allocation 실패
const int ERR_DCF_FILE_DOES_NOT_EXIST				= 5;	// 104005 = DCF File 없음.
const int ERR_INVALID_SYSTEM_TYPE					= 6;	// 104006 = 사용할 수 없는 System Type 임.

const int ERR_INVALID_CAMERA_NO						= 11;	// 104011 = 사용하는 Camera 번호가 아님
const int ERR_NOT_REGISTERED_MARK					= 12;	// 104012 = 등록된 Mark 가 아님
const int ERR_INVALID_MARK_INDEX					= 13;	// 104013 = Mark Index 를 얻을 수 없음
const int ERR_MARK_NO_RANGE_OVER					= 14;	// 104014 = 사용 가능 Mark 번호 범위 아님
const int ERR_MODEL_DISPLAY_BUFFER_ALLOCATION_FAIL	= 15;	// 104015 = Model Display Buffer 생성 실패
const int ERR_INVALID_MARK_ARRAY_FOR_MULTI_SEARCH	= 16;	// 104016 = Multiple Model Search 에 사용할 Mark 번호 오류
const int ERR_PATTERN_MATCHING_SEARCH_FAIL			= 17;	// 104017 = Pattern Matching 인식 실패
const int ERR_PATTERN_MATCHING_LOW_SCORE			= 18;	// 104018 = Pattern Matching 인식률 낮음
const int ERR_BLOB_ANALYSIS_FAIL					= 19;	// 104019 = Blob 분석 결과 Score 범위 벗어남
const int ERR_MARK_POSITION_LIMIT_LEFT				= 20;	// 104020 = Mark 위치 Limit Error [left]
const int ERR_MARK_POSITION_LIMIT_TOP				= 21;	// 104021 = Mark 위치 Limit Error [top]
const int ERR_MARK_POSITION_LIMIT_RIGHT				= 22;	// 104022 = Mark 위치 Limit Error [right]
const int ERR_MARK_POSITION_LIMIT_BOTTOM			= 23;	// 104023 = Mark 위치 Limit Error [bottom]
const int ERR_INVALID_PARAMETER_VALUE				= 24;	// 104024 = parameter 값이 사용 범위를 초과함.
const int ERR_NOT_SUPPORTED_OPERATION				= 25;	// 104025 = 지원하는 Operation 이 아님.
const int ERR_BLOB_MARK_SIZE_INVALID				= 26;	// 104026 = Blob 마크 사이즈가 너무 작습니다.

const int ERR_GRAB_FAIL								= 30;	// 104030 = Grab 수행 안됨. [PC Rebooting 필요]

/************************************************************************/
/*                         Data Structure Define                        */
/************************************************************************/

typedef struct
{
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Configuration Data
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	int			m_iUserCameraNo;	// User 가 사용하는 실제 Camera 번호 : 0 번부터 시작
	int			m_iInternalCameraNo;	// Vision Component 내부적으로 사용하는 Camera 번호 : 0 번부터 시작

//	WCHAR*		m_pszDCFPath;	// DCF Path
//	WCHAR*		m_pszDCFName;	// DCF Name : DCF Name 이 없으면, 사용하지 않는 Camera 로 간주한다.
//	WCHAR*		m_pszCameraName;	// Camera Name
	char*		m_pszDCFPath;	// DCF Path
	char*		m_pszDCFName;	// DCF Name : DCF Name 이 없으면, 사용하지 않는 Camera 로 간주한다.
	char*		m_pszCameraName;	// Camera Name

	long		m_lBufferSizeX;	// Grab Buffer Size : Camera Input Image Resolution
	long		m_lBufferSizeY;

// 삭제 예정.	long		m_lGrabSettlingTime;	// Grab Settling Time (ms)

	int			m_iFixedViewID;	// 이 카메라의 영상을 고정적으로 Display 해주는 Display Window ID

	int			m_rgiMarkRelation[DEF_MAX_MARK_TYPE][DEF_MAX_USE_MARK];	// Original Mark Storage 에 대한 연결 index 저장

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Display Data
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	int			m_iConnectedSystemID;	// 해당 Camera 가 연결된 System 의 Index
	long		m_lSource;	// 해당 Camera 와 연결된 System 상의 Source Buffer ID
	long		m_lGrab;	// 해당 Camera 와 연결된 System 상의 Grab Buffer ID

	int			m_iTextIndex;
	int			m_rgiTextType[DEF_MAX_TEXT_OBJECT];	// Display Type 종류 : NONE, FIXED, FLOATING
	char		m_rgszText[DEF_MAX_TEXT_OBJECT][DEF_MAX_TEXT_LENGTH];

	int			m_iRectIndex;
	int			m_rgiRectType[DEF_MAX_RECT_OBJECT];
	double		m_rgdRectLeft[DEF_MAX_RECT_OBJECT];
	double		m_rgdRectTop[DEF_MAX_RECT_OBJECT];
	double		m_rgdRectRight[DEF_MAX_RECT_OBJECT];
	double		m_rgdRectBottom[DEF_MAX_RECT_OBJECT];

	int			m_iPointIndex;
	int			m_rgiPointType[DEF_MAX_POINT_OBJECT];
	double		m_rgdPointX[DEF_MAX_POINT_OBJECT];
	double		m_rgdPointY[DEF_MAX_POINT_OBJECT];
	
	//110214.Add_____
	int			m_iPointIndex_Measure;
	int			m_rgiPointType_Measure[DEF_MAX_POINT_OBJECT];
	double		m_rgdPointX_Measure[DEF_MAX_POINT_OBJECT];
	double		m_rgdPointY_Measure[DEF_MAX_POINT_OBJECT];	

	int			m_iLineIndex_Measure;
	int			m_rgiLineType_Measure[DEF_MAX_LINE_OBJECT];
	double		m_rgdLineStartX_Measure[DEF_MAX_LINE_OBJECT];
	double		m_rgdLineStartY_Measure[DEF_MAX_LINE_OBJECT];
	double		m_rgdLineEndX_Measure[DEF_MAX_LINE_OBJECT];
	double		m_rgdLineEndY_Measure[DEF_MAX_LINE_OBJECT];
	//_______________

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Critical Section
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	CRITICAL_SECTION m_csGrab;	// Camera Grab Buffer 관련 Critical Section
	CRITICAL_SECTION m_csSource;	// Camera Source Buffer 관련 Critical Section

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Grab 관련 Hook Data
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	HWND		m_hWnd;

} SCameraData;

typedef struct
{
	int m_iCpuCount;	// PC 에 장착된 CPU 개수 : Solios type 의 경우 이를 기준으로 Multi Thread 설정
	int* m_rgiType;	// 존재하는 전체 System 의 System Type 을 보관하고 있는 MVision::m_rgiVSystemType Array Link pointer
	int m_iInstanceNo;	// System Instance No
	std::vector<SCameraData>* m_pvectorCamData;	// 사용하는 Camera 전체 정보를 보관하고 있는 MVision::m_vectorCamData Link pointer

} SSystemData;

typedef struct
{
	int m_iID;
	RECT m_rectWndPos;
	int m_iPriority;
	CWnd* m_pParentWnd;

} SDisplayAttributeData;

typedef struct
{
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Common data
	int			m_iMarkIndex;	// Mark Array Index

	BOOL		m_bIsModel;	// Mark 등록 여부
	BOOL		m_bSuccess;	// Search 작업 성공 여부

	char*		m_pszModelName;	// Model Name
	char*		m_pszFileName;	// Model File Name
//	WCHAR*		m_pszModelName;	// Model Name
//	WCHAR*		m_pszFileName;	// Model File Name

	EMarkType	m_eMarkType;	// Mark Type : DEF_NONE_MARK / DEF_PM_MARK / DEF_BLOB_MARK / DEF_CALIPER_MARK

	long		m_lModelLeft;	// Model Area : PM - Model, BLOB - Blob Area, CALIPER - Measurement Box, OCR - OCR Reading Area
	long		m_lModelTop;
	long		m_lModelRight;
	long		m_lModelBottom;

	double		m_dThreshold;	// Threshold : PM - Acceptance Threshold, BLOB - Binary Threshold, CALIPER - Edge Threshold

	double		m_dPolarity;	// BLOB - 목표 Pixel 의 극성 (Black : 0 / White : 1), CALIPER - whether an edge is rising or falling (0 : M_NEGATIVE, 1 : M_POSITIVE)

	long		m_lModelID;		// Vision Model ID

	long		m_lResultID;	// Result Buffer Identifier
	
	double		m_dScore;	// Recognition Result Score

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Pattern mark

	int			m_iSystemIndex;	// Mark 가 등록된 System 에 대한 System Index

	long		m_lSearchAreaLeft;	// Search Area
	long		m_lSearchAreaTop;
	long		m_lSearchAreaRight;
	long		m_lSearchAreaBottom;

	double		m_dReferenceX;	// Reference Point
	double		m_dReferenceY;

	double		m_dCertaintyThreshold;	// Certainty Threshold

//	double		m_dSearchAngle;	// Search Angle : MIL 내부 bug 로 사용하면 Error 발생, 확인 필요.

	double		m_dResultX;	// Vision 좌표계를 따르는 인식 결과 좌표 [Display View 좌측 상단 좌표 : (0.0, 0.0)]
	double		m_dResultY;

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Blob mark

	double		m_dMaxLimit;	// 검사결과 OK 를 반환하는 최대 m_dScore 값 (0 ~ 100 %) 
	double		m_dMinLimit;	// 검사결과 OK 를 반환하는 최소 m_dScore 값 (0 ~ 100 %) 

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Caliper mark

	double		m_dSearchDirection;	// Search Direction

/* 확인 필요 : OCR 사용 안함.
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// OCR mark

	double		m_dCharSizeX;	// Character Size X
	double		m_dCharSizeY;	// Character Size Y

	double		m_dCharSpacing;	// OCR Font 글자간 Spacing = Char Size X + 여백 

	int			m_iInitialCalibCharNum;

	long		m_lSubImage;	// Sub-image buffer identifier for Every OCR Reading.
	long		m_lFont;	// OCR font identifier

	char*		m_szResultString;	// character pointer for the read characters

	long		m_lForegroundColor;	// 배경색에 비해서 문자 밝기가 밝은지 어두운지를 나타냄.
*/
} SMarkData;

#endif	// _DEFVISION_HEADER
