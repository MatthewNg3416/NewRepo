#if !defined(AFX_IVISION_H__D95B65B3_7070_48CA_B84A_9336B3165DEB__INCLUDED_)
#define AFX_IVISION_H__D95B65B3_7070_48CA_B84A_9336B3165DEB__INCLUDED_

#include "stdafx.h"
#include "ICommon.h"
#include "DefVisionExt.h"
#include <string>

typedef std::basic_string<_TCHAR> tstring;

/** @interface */
class IVision
{
	// Destructor
public:
	virtual ~IVision() {};

	// Methods
public:

	/* 주의 : Grab 이후에 SaveImage 함수를 호출할 때는 LockCamera() 로 Camera 자원에 대한
	 *		  보호가 이루어진 상태이어야 한다.
	 */
	virtual int SaveLastErrorImage(int iCamNo, const tstring& strPath) = 0;
	virtual int SaveImage(int iCamNo, const tstring& strPath, const tstring& strFile) = 0;
	virtual int SaveImage(int iCamNo, const tstring& strFile) = 0;

	/* 주의 : Thread 에서 Grab - 인식 함수를 호출할 때는
	 *        꼭 아래 함수로 사용하는 Camera 자원을 보호해야 한다.
	 */
	virtual void LockCamera(int iCamNo) = 0;
	virtual void UnlockCamera(int iCamNo) = 0;

	virtual BOOL WriteLog(const tstring& strFile, const tstring& strLog) = 0;

	// 확인 필요 : ICommon 에 대한 의존성 제거.
	virtual void CreateCommonObject(SCommonAttribute& commonData) = 0;

#ifdef DEF_USE_ICOMMON
	virtual ICommon* GetCommonObject() = 0;
#endif

	virtual int CreateTemporaryView(RECT* pWndPos, CWnd* pParentWnd) = 0;

	// 확인 필요 : 사이즈, Search Area 다른 Model 사용 시 이상 동작 발생.
	virtual int FindMultiModel(int iCamNo, int* rgiModel, long lNumModel) = 0;

	// 기존 대비 추가되거나 수정된 IF

	virtual void LoadImage(int iDisplayCamNo, tstring strPathName) = 0;

	virtual const tstring GetCameraName(const int iUserCamNo) const = 0;

	/** Local View 를 생성한다.
	 */
	virtual int CreateLocalView(RECT* pWndPos, CWnd* pParentWnd) = 0;

	virtual int DestroyView(int iViewID) = 0;

	/**
	 *
	 */
	virtual BOOL IsValidCam(const int iCamNo) const = 0;

	/** Camera 와 View Window 를 연결한다.
	 */
	virtual int CreateFixedView(int iViewID, RECT* pWndPos, CWnd* pParentWnd = NULL) = 0;

	/**
	 *	사용하는 카메라의 최대 번호 + 1 값을 return 한다.
	 */
	virtual const int GetMaxUseCameraNo() const = 0;

	virtual int Initialize(const tstring strVisionDataFilePath, const tstring strModelDataFilePath, const tstring strLogDataFilePath, const tstring strImageDataFilePath) = 0;

	/**	Model Change 시, 기존 Model 에 대한 Vision Model Data 를 제거하고,
	 *	새 Model 에 대한 Model Data 를 Load 한다.
	 */
	virtual int ChangeModel(const tstring strModelDataFilePath) = 0;


	/** 다른 Camera 에 Teaching 된 모델을 불러 들인다.
	 *
	 */
	virtual int ImportModel(EMarkType eMarkType, int iSrcCamNo, int iSrcMarkNo, int iDstCamNo, int iDstMarkNo) = 0;

	/** Vision Mark 이름을 설정한다. */
	virtual int SetModelName(int iCamNo, EMarkType eMarkType, int iModelNo, tstring strName) = 0;

/** ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
 *     Related Common Operations
 */
	/**
	 */
	virtual POINT GetDistribution(int iCamNo) = 0;

	/** Grab Operation 을 수행한다.
	 */
	virtual int Grab(int iCamNo) = 0;

	/** Grab Operatioin 완료 대기.
	 */
	virtual int GrabWait(int iCamNo) = 0;

	/** Write Vision Model Data
	 *
	 */
	virtual int WriteModelData(int iCamNo, EMarkType eMarkType, int iModelNo) = 0;

	/** Check Enabled Model & Recognition Area
	 *
	 */
	virtual int CheckModel(int iCamNo, EMarkType eMarkType, int iModelNo) = 0;

	/** Delete Registered Model or Recognition Area
	 *
	 */
	virtual void DeleteMark(int iCamNo, EMarkType eMarkType, int iModelNo) = 0;

	/**	Delete Old Error Image Files
	 *
	 */
	virtual int DeleteOldImageFiles(const tstring& strPath = _T("")) = 0;

	/** Enable or Disable "Save Image" Fuction.
	 */
	virtual void EnableSaveImage(BOOL bFlag = FALSE) = 0;

	/** Get Grab Settling Time.
	 */
// 삭제 예정.	virtual int GetGrabSettlingTime(int iCamNo) = 0;

	/** Set Grab Settling Time.
	 */
// 삭제 예정.		virtual void SetGrabSettlingTime(int iCamNo, int iGrabSettlingTime) = 0;

	/** Get Camera Change Time.
	 */
	virtual int GetCameraChangeTime(int iCamNo) = 0;

	/** Set Camera Change Time.
	 */
	virtual void SetCameraChangeTime(int iCamNo, int iCameraChangeTime) = 0;

/** ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
 *     Related Pattern Matching Operations
 */

	/** Register NGC & GMF Model
	 * @return boolean type : TRUE or FALSE
	 * @param iCameraNo : Camera Number
	 * @param SelectMark : Model Mark Number
	 * @param SearchArea : Search Area Rectangle
	 * @param ModelArea : Model Area Rectangle
	 * @param ReferencePoint : Reference Point
	 */
	virtual int RegisterPatternMark(int iCamNo, int iModelNo, RECT& SearchArea, RECT& ModelArea, double dReferenceX, double dReferenceY) = 0;

	/**	Reset Search Area
	 * @param iCamNo : Camera Number
	 * @param iModelNo : Model Mark Number
	 * @param SArea : Search Area Rectangle
	 */
	virtual int	SetSearchArea(int iCamNo, int iModelNo, const RECT& SArea) = 0;

	/** Recognition Model Mark (NGC Algorithm)
	 * @return int type Error Code : 0 - DEF_VISION_SUCCESS, etc. - Error
	 * @param iCamNo : Camera Number
	 * @param iModelNo : Model Mark Number
	 *                  False - Not Use GMF(Geometry Model Finder) 
	 */
	virtual int RecognitionPatternMark(int iCameraNo, int iModelNo) = 0;

	/** Make a Mask Image & Apply the Mask Image to GMF Search Context
	 * @return int type Error Code : 0 - DEF_VISION_SUCCESS, etc. - Error
	 * @param iCamNo : Camera Number
	 * @param iModelNo : Model Mark Number
	 * @param MaskRect : Rectangle for masking
	 * @param ModelRect : GMF Model Rectangle
	 * @param bMakeEndFlag : TRUE - Stop making mask image & Apply the Mask Image to GMF Search Context
	 *                       FALSE - Continue making mask image
	 */
//	virtual int MaskImage(int iCamNo, int iModelNo, RECT& MaskRect, RECT& ModelRect, bool bMakeEndFlag) = 0;

	/** Return Pattern Matching Result : Reference Point X value
	 * @return double type : X coordinate
	 * @param iCamNo : Camera Number
	 * @param iModelNo : Model Mark Number
	 */
	virtual double GetSearchResultX(int iCamNo, int iModelNo) = 0;

	/** Return Pattern Matching Result : Reference Point Y value
	 * @return double type : Y coordinate
	 * @param iCamNo : Camera Number
	 * @param iModelNo : Model Mark Number
	 */
	virtual double GetSearchResultY(int iCamNo, int iModelNo) = 0;
	//20121004 SJ_HJG 횡전개

	/** Return Pattern Matching Result : Reference Point X value
	 * @return double type : X coordinate
	 * @param iCamNo : Camera Number
	 * @param iModelNo : Model Mark Number
	 */
	virtual double GetSearchBlobResultX(int iCamNo, int iModelNo) = 0;

	/** Return Pattern Matching Result : Reference Point Y value
	 * @return double type : Y coordinate
	 * @param iCamNo : Camera Number
	 * @param iModelNo : Model Mark Number
	 */
	virtual double GetSearchBlobResultY(int iCamNo, int iModelNo) = 0;
	//__________________
	/** Return Pattern Matching Search Area Rectangle
	 * @return RECT type : Search Area
	 * @param iCamNo : Camera Number
	 * @param iModelNo : Model Mark Number
	 */
	virtual RECT GetSearchAreaRect(int iCamNo, int iModelNo) = 0;

	virtual double GetSearchAcceptanceThreshold(int iCamNo, int iModelNo) = 0;
	virtual int SetSearchAcceptanceThreshold(int iCamNo, int iModelNo, double dValue) = 0;


/** ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
 *     Related Blob Analysis Operations
 */

	/** Allocate the Blob Area for Blob Analysis
	 * @return int type Error Code : 0 - DEF_VISION_SUCCESS, etc. - Invalid Model Mark Number
	 * @param CameraNo : Camera Number
	 * @param MarkNo : Blob Area Number
	 * @param SearchArea : Search Area Rectangle - Not Used
	 * @param BlobArea : Blob Area Rectangel
	 */
//@	virtual int RegisterBLOBPattern(int CameraNo, int MarkNo, RECT& SearchArea, RECT& BlobArea) = 0;
	virtual int RegisterBLOBPattern(int CameraNo, int MarkNo, RECT& SearchArea, RECT& BlobArea, double dRefX = 0.0, double dRefY = 0.0) = 0;

	/** Operate Blob Analysis
	 * @param iCameraNo : Camera Number
	 * @param iModelNo : Blob Area Number
	 */
	virtual int RecognitionBLOBPattern(int iCameraNo, int iModelNo) = 0;
	//@130119.KKY____________
	virtual int RecognitionEdgeLineY(int iCameraNo, int iModelNo, BOOL bSettingMode, double dRefVX, double dRefVY) = 0;
	//@______________________
	virtual int SetBlobThreshold(int iCamNo, int iModelNo, double dThreshold) = 0;
	virtual double GetBlobThreshold(int iCamNo, int iModelNo) = 0;
	virtual int	SetBlobJudgeMaxPixelPercent(int iCamNo, int iModelNo, int iMaxPixelPercent) = 0;
	virtual	int GetBlobJudgeMaxPixelPercent(int iCamNo, int iModelNo) = 0;
	virtual int	SetBlobJudgeMinPixelPercent(int iCamNo, int iModelNo, int iMinPixelPercent) = 0;
	virtual int GetBlobJudgeMinPixelPercent(int iCamNo, int iModelNo) = 0;
	virtual int SetBlobPolarity(int iCamNo, int iModelNo, double dPolarity) = 0;
	virtual double GetBlobPolarity(int iCamNo, int iModelNo) = 0;
/*	virtual int SetBlobOverSign(int iCamNo, int iModelNo, BOOL bBlobOverSign) = 0;
	virtual double GetBlobOverSign(int iCamNo, int iModelNo) = 0;
*/
	//20121004 SJ_HJG 횡전개
	virtual int GetBlobHistogram(int iCameraNo, int iModelNo, unsigned char* rgucHist) = 0;
//	virtual int GetBlobHistogram(int iCameraNo, int iModelNo, unsigned char* rgucHist, int* rglHistVals1) = 0;

	virtual double GetBlobResultScore(int iCamNo, int iModelNo) = 0;
	virtual long GetBlobPixelsInModelArea(int iCamNo, int iModelNo) = 0;

/** ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
 *     Related Edge Finder (Caliper Tool) Operations
 */
	virtual int RegisterEdgeModel(int CameraNo, int MarkNo, RECT rectMeasurementBox) =0;
	virtual int FindEdge(int iCameraNo, int iModelNo) = 0;

	virtual int SetEdgeFinderPolarity(int iCameraNo, int iModelNo, double dPolarity) = 0;
	virtual int SetEdgeFinderThreshold(int iCameraNo, int iModelNo, double dThreshold) = 0;
//	virtual int SetEdgeFinderNumOfResults(int iCameraNo, int iModelNo, double dNumOfResults) = 0;
	virtual int SetEdgeFinderDirection(int iCameraNo, int iModelNo, double dSearchDirection) = 0;

	virtual double GetEdgeFinderPolarity(int iCameraNo, int iModelNo) = 0;
	virtual double GetEdgeFinderThreshold(int iCameraNo, int iModelNo) = 0;
//	virtual double GetEdgeFinderNumOfResults(int iCameraNo, int iModelNo) = 0;
	virtual double GetEdgeFinderDirection(int iCameraNo, int iModelNo) = 0;

/** ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
 *     Related Vision View Display Operations
 */

	/** Dispaly the registered Model Image or Model Area like below ;
	 *		NGC Pattern Matching Model Image
	 *		GMF Model Edge information
	 *		Blob Area
	 *		OCR Reading Area
	 * @return Error Code : 0[DEF_VISION_SUCCESS] - Success, etc. - Error
	 */
	virtual int DisplayPatternImage(int iCamNo, EMarkType eMarkType, int iModelNo) = 0;

	/** Stop Live & Grap Image
	 * @return 
	 * @param iCamNo : Camera Number
	 */
	virtual void HaltVideo(int iCamNo) = 0;

	/** Start Live : Grab Continuous
	 * @return int type Error Code : 0 - DEF_VISION_SUCCESS
	 * @param iCamNo : Camera Number
	 */
	virtual int	LiveVideo(int iCamNo) = 0;

	/** Clear Overlay Display
	 * @return
	 * @param iCamNo :
	 * @param bCenterLineFlag : TRUE - Draw Center Cross Mark when clear overlay display
	 *                          FALSE - Not Draw Center Cross Mark 
	 */
	virtual void ClearOverlay(int iCamNo) = 0;

	/**	Redraw View
	 * @return void
	 * @param iViewID : View ID
	 */
	virtual void RedrawView(int iViewID) = 0;

	/** Draw Rectangle on the Overlay Display
	 *
	 */
	virtual void DrawOverlayAreaRect(int iCamNo, RECT& rect, int iObjectType = DEF_FIXED_OBJECT) = 0;

	/** Draw Line On the Overlay Display
	 *
	 */
//	virtual void DrawOverlayLine(int iCamNo, const CPoint& ptStart, const CPoint& ptEnd, int iObjectType = DEF_FIXED_OBJECT) = 0;

	/** Draw Cross Mark on the Overlay Display
	 *
	 */
	virtual void DrawOverlayCrossMark(int iCamNo, long lX, long lY, int iObjectType = DEF_FIXED_OBJECT) = 0;	//, int color = 1) = 0;

	/** Draw Text on the View Image Display
	 *
	 */
	virtual void DrawOverlayText(int iCamNo, char* pszText, int iObjectType = DEF_FIXED_OBJECT) = 0;

//	virtual void StopDisplayLocalView() = 0;

	virtual void DrawSourceImage(int iCamNo) = 0;

	//110214.Add_____
	virtual void DrawOverlayLine_Measure(int iCamNo, long lStartX, long lStartY, long lEndX, long lEndY, int iObjectType = DEF_FLOATING_OBJECT) = 0;
	virtual void DrawOverlayCross_Measure(int iCamNo, long lX, long lY, int iObjectType = DEF_FLOATING_OBJECT) = 0;
	//_______________
/** ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
 *     Related OCR Operations
 */

	/** Read OCR Operation (internal reading operation)
	 * @return int type Error Code : 0 - DEF_VISION_SUCCESS, etc. - Error
	 * @param iCamNo : Camera Number
	 * @param iModelNo : OCR Reading Area Number
	 */
//	virtual int ReadOCR(int iCamNo, int iModelNo) = 0;

	/** Get OCR Read Result String
	 * @return int type Error Code : 0 - DEF_VISION_SUCCESS, etc. - Error
	 * @param iCamNo
	 * @param iModelNo : OCR Read Area Number
	 * @param szString : Character Pointer pointing to Character Array to save OCR Result String
	 */
//	virtual int GetOcrString(int iCamNo, int iModelNo, char* szString) = 0;

	/** Prepare OCR Operation
	 * @return int type Error Code : 0 - DEF_VISION_SUCCESS, etc. - Error
	 * @param iCamNo : Camera Number
	 * @param iModelNo : OCR Reading Area Number
	 * @param OcrReadingArea : Reading Area Rectangle
	 * @param iCharNum : String length in OCR Reading Area
	 * @param
	 * @param
	 */
//	virtual int CalibrateOCR(int iCamNo, int iModelNo, RECT OcrReadingArea, int iCharNum, RECT CharSize, double dSpacing) = 0;

	/**	Read OCR Calibration Data
	 */
//	virtual int ReadOcrCalibData(int iCamNo, int iModelNo) = 0;


/** ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
 *     Temporary Functions for Operating Test
 */
	
	/**	Camera 번호 배열을 동적으로 생성한다. 
	 */
	//virtual int* SetNumOfUseCam(int* rgiCamNum, int iNumOfUseCam, ...) = 0;


// Protected constructor
protected:
	IVision() {};
};

#endif // !defined(AFX_IVISION_H__D95B65B3_7070_48CA_B84A_9336B3165DEB__INCLUDED_)
