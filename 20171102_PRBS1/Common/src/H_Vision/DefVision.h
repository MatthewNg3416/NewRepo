#ifndef _DEFVISION_HEADER
#define _DEFVISION_HEADER

#include "DefVisionExt.h"
#include <mil.h>
#include <vector>

// Message ID ���
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


const int DEF_NONE_CAMERA = -1;	// Camera ����.
const int DEF_DEF_NONE_MARK = -1;	//Mark ����.

const int DEF_KEY_NOT_FOUND_VALUE = -99;

const int DEF_MAX_CAMERA_NUM_PER_SYSTEM = 8;
const int DEF_METEOR_SYSTEM_MAX_CAM_NUM = 6;
const int DEF_SOLIOS_SYSTEM_MAX_CAM_NUM = 8;

const int DEF_MATROX_SYSTEM_CHANNEL_NUM = 2;

const int DEF_MATROX_SOLIOS_INDEPENDENT_INPUT_PATH_NUM = 4;

const int DEF_MAX_USE_MARK = 28;	// Mark Type �� Camera �� ���� �� �ִ� �ִ� Mark ����

const int DEF_MAX_MARK_NUM_FOR_MULTI_FIND = 5;	// Find Multiple Model �� �� ���� ����� �� �ִ� �ִ� Mark ����

const int DEF_MAX_DRAW_OBJECT = 5;		// �ϴ� string, rectangle, point ��� 5 �������� ����.
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
const int ERR_APPLICATION_ALLOC_FAILURE				= 1;	// 104001 = Application Allocation ����
const int ERR_SYSTEM_ALLOC_FAILURE					= 2;	// 104002 = System Allocation ����
const int ERR_DIGITIZER_ALLOC_FAILURE				= 3;	// 104003 = Digitizer Allocation ����
const int ERR_BUFFER_ALLOC_FAILURE					= 4;	// 104004 = Buffer Allocation ����
const int ERR_DCF_FILE_DOES_NOT_EXIST				= 5;	// 104005 = DCF File ����.
const int ERR_INVALID_SYSTEM_TYPE					= 6;	// 104006 = ����� �� ���� System Type ��.

const int ERR_INVALID_CAMERA_NO						= 11;	// 104011 = ����ϴ� Camera ��ȣ�� �ƴ�
const int ERR_NOT_REGISTERED_MARK					= 12;	// 104012 = ��ϵ� Mark �� �ƴ�
const int ERR_INVALID_MARK_INDEX					= 13;	// 104013 = Mark Index �� ���� �� ����
const int ERR_MARK_NO_RANGE_OVER					= 14;	// 104014 = ��� ���� Mark ��ȣ ���� �ƴ�
const int ERR_MODEL_DISPLAY_BUFFER_ALLOCATION_FAIL	= 15;	// 104015 = Model Display Buffer ���� ����
const int ERR_INVALID_MARK_ARRAY_FOR_MULTI_SEARCH	= 16;	// 104016 = Multiple Model Search �� ����� Mark ��ȣ ����
const int ERR_PATTERN_MATCHING_SEARCH_FAIL			= 17;	// 104017 = Pattern Matching �ν� ����
const int ERR_PATTERN_MATCHING_LOW_SCORE			= 18;	// 104018 = Pattern Matching �νķ� ����
const int ERR_BLOB_ANALYSIS_FAIL					= 19;	// 104019 = Blob �м� ��� Score ���� ���
const int ERR_MARK_POSITION_LIMIT_LEFT				= 20;	// 104020 = Mark ��ġ Limit Error [left]
const int ERR_MARK_POSITION_LIMIT_TOP				= 21;	// 104021 = Mark ��ġ Limit Error [top]
const int ERR_MARK_POSITION_LIMIT_RIGHT				= 22;	// 104022 = Mark ��ġ Limit Error [right]
const int ERR_MARK_POSITION_LIMIT_BOTTOM			= 23;	// 104023 = Mark ��ġ Limit Error [bottom]
const int ERR_INVALID_PARAMETER_VALUE				= 24;	// 104024 = parameter ���� ��� ������ �ʰ���.
const int ERR_NOT_SUPPORTED_OPERATION				= 25;	// 104025 = �����ϴ� Operation �� �ƴ�.
const int ERR_BLOB_MARK_SIZE_INVALID				= 26;	// 104026 = Blob ��ũ ����� �ʹ� �۽��ϴ�.

const int ERR_GRAB_FAIL								= 30;	// 104030 = Grab ���� �ȵ�. [PC Rebooting �ʿ�]

/************************************************************************/
/*                         Data Structure Define                        */
/************************************************************************/

typedef struct
{
	//��������������������������������������������������������������������������������������
	// Configuration Data
	//��������������������������������������������������������������������������������������
	int			m_iUserCameraNo;	// User �� ����ϴ� ���� Camera ��ȣ : 0 ������ ����
	int			m_iInternalCameraNo;	// Vision Component ���������� ����ϴ� Camera ��ȣ : 0 ������ ����

//	WCHAR*		m_pszDCFPath;	// DCF Path
//	WCHAR*		m_pszDCFName;	// DCF Name : DCF Name �� ������, ������� �ʴ� Camera �� �����Ѵ�.
//	WCHAR*		m_pszCameraName;	// Camera Name
	char*		m_pszDCFPath;	// DCF Path
	char*		m_pszDCFName;	// DCF Name : DCF Name �� ������, ������� �ʴ� Camera �� �����Ѵ�.
	char*		m_pszCameraName;	// Camera Name

	long		m_lBufferSizeX;	// Grab Buffer Size : Camera Input Image Resolution
	long		m_lBufferSizeY;

// ���� ����.	long		m_lGrabSettlingTime;	// Grab Settling Time (ms)

	int			m_iFixedViewID;	// �� ī�޶��� ������ ���������� Display ���ִ� Display Window ID

	int			m_rgiMarkRelation[DEF_MAX_MARK_TYPE][DEF_MAX_USE_MARK];	// Original Mark Storage �� ���� ���� index ����

	//��������������������������������������������������������������������������������������
	// Display Data
	//��������������������������������������������������������������������������������������
	int			m_iConnectedSystemID;	// �ش� Camera �� ����� System �� Index
	long		m_lSource;	// �ش� Camera �� ����� System ���� Source Buffer ID
	long		m_lGrab;	// �ش� Camera �� ����� System ���� Grab Buffer ID

	int			m_iTextIndex;
	int			m_rgiTextType[DEF_MAX_TEXT_OBJECT];	// Display Type ���� : NONE, FIXED, FLOATING
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

	//��������������������������������������������������������������������������������������
	// Critical Section
	//��������������������������������������������������������������������������������������
	CRITICAL_SECTION m_csGrab;	// Camera Grab Buffer ���� Critical Section
	CRITICAL_SECTION m_csSource;	// Camera Source Buffer ���� Critical Section

	//��������������������������������������������������������������������������������������
	// Grab ���� Hook Data
	//��������������������������������������������������������������������������������������
	HWND		m_hWnd;

} SCameraData;

typedef struct
{
	int m_iCpuCount;	// PC �� ������ CPU ���� : Solios type �� ��� �̸� �������� Multi Thread ����
	int* m_rgiType;	// �����ϴ� ��ü System �� System Type �� �����ϰ� �ִ� MVision::m_rgiVSystemType Array Link pointer
	int m_iInstanceNo;	// System Instance No
	std::vector<SCameraData>* m_pvectorCamData;	// ����ϴ� Camera ��ü ������ �����ϰ� �ִ� MVision::m_vectorCamData Link pointer

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
	//������������������������������������������������������������������
	// Common data
	int			m_iMarkIndex;	// Mark Array Index

	BOOL		m_bIsModel;	// Mark ��� ����
	BOOL		m_bSuccess;	// Search �۾� ���� ����

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

	double		m_dPolarity;	// BLOB - ��ǥ Pixel �� �ؼ� (Black : 0 / White : 1), CALIPER - whether an edge is rising or falling (0 : M_NEGATIVE, 1 : M_POSITIVE)

	long		m_lModelID;		// Vision Model ID

	long		m_lResultID;	// Result Buffer Identifier
	
	double		m_dScore;	// Recognition Result Score

	//������������������������������������������������������������������
	// Pattern mark

	int			m_iSystemIndex;	// Mark �� ��ϵ� System �� ���� System Index

	long		m_lSearchAreaLeft;	// Search Area
	long		m_lSearchAreaTop;
	long		m_lSearchAreaRight;
	long		m_lSearchAreaBottom;

	double		m_dReferenceX;	// Reference Point
	double		m_dReferenceY;

	double		m_dCertaintyThreshold;	// Certainty Threshold

//	double		m_dSearchAngle;	// Search Angle : MIL ���� bug �� ����ϸ� Error �߻�, Ȯ�� �ʿ�.

	double		m_dResultX;	// Vision ��ǥ�踦 ������ �ν� ��� ��ǥ [Display View ���� ��� ��ǥ : (0.0, 0.0)]
	double		m_dResultY;

	//������������������������������������������������������������������
	// Blob mark

	double		m_dMaxLimit;	// �˻��� OK �� ��ȯ�ϴ� �ִ� m_dScore �� (0 ~ 100 %) 
	double		m_dMinLimit;	// �˻��� OK �� ��ȯ�ϴ� �ּ� m_dScore �� (0 ~ 100 %) 

	//������������������������������������������������������������������
	// Caliper mark

	double		m_dSearchDirection;	// Search Direction

/* Ȯ�� �ʿ� : OCR ��� ����.
	//������������������������������������������������������������������
	// OCR mark

	double		m_dCharSizeX;	// Character Size X
	double		m_dCharSizeY;	// Character Size Y

	double		m_dCharSpacing;	// OCR Font ���ڰ� Spacing = Char Size X + ���� 

	int			m_iInitialCalibCharNum;

	long		m_lSubImage;	// Sub-image buffer identifier for Every OCR Reading.
	long		m_lFont;	// OCR font identifier

	char*		m_szResultString;	// character pointer for the read characters

	long		m_lForegroundColor;	// ������ ���ؼ� ���� ��Ⱑ ������ ��ο����� ��Ÿ��.
*/
} SMarkData;

#endif	// _DEFVISION_HEADER
