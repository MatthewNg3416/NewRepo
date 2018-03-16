#if !defined(AFX_VISION_H__6DC657D8_C279_4141_884D_B1E351D7E3BB__INCLUDED_)
#define AFX_VISION_H__6DC657D8_C279_4141_884D_B1E351D7E3BB__INCLUDED_

#include "IVision.h"
#include "DefVision.h"

class ICommon;
class Msystem;
class Mdisplay;

class MdisplayWnd;

class MVision : public IVision  
{
	// Member Variables
private:
	ICommon* m_pICommon;	// ICommon Object
	long m_lAppID;	// MIL Application ID

	// Vision Component Member
	std::vector<Msystem*> m_vectorpSystem;	// Vision System Member (vector)
	std::vector<Mdisplay*> m_vectorpDisp;	// Display Object Pointer Array (vector)

	// Vision Data Managememt
	std::vector<SCameraData> m_vectorCamData;	// Camera Data : [Config + Display Data] (vector)
	std::vector<SMarkData> m_vectorMarkData;	// Vision Mark Data (vector) : Original Storage

// 확인 필요	std::vector<HWND> m_vectorOldWndHandle;	// 확인 필요.

	// Vision Component Config Option
	int* m_rgiVSystemType;	// Vision System Type Array (Vision Board Type : DEF_METEOR_II_MC, DEF_SOLIOS) : (Dynamic Allocation)
	int m_iCpuCount;	// CPU Count
	int m_iSystemCount;	// Vision System Count (ie. number of vision board)
	int m_iCameraCount;	// Camera Count (ie. 사용하는 Camera 개수)
	int m_iErrorPrint;	// MIL Error Display Option
	int m_iSaveImage;	// Save Image Option
	long m_lCameraChangeTime;	// Camera Channel Change Time (unit : ms)
	std::string m_strVisionDataFilePath;	// Vision Configuration Data, Vision Model(Mark) Data File Path
	std::string m_strModelDataFilePath;	// Panel Model Data File Path : Camera Data File
	std::string m_strLogDataFilePath;	// Vision Default Log Data File Storage Path
	std::string m_strImageDataFilePath;	// Vision Default Image Data File Storage Path

	// Log 처리 관련
	std::string m_strOldDay;
	CRITICAL_SECTION m_csLog;

	CRITICAL_SECTION m_csVisionLock;//110126.임시

	// Member Functions
private:
	int initialize();
	void loadConfigData();
	void loadCameraData();
	void loadModelData();
	void loadMarkRelationData();//120119.KKY

	int saveConfigData();
	int saveCameraData();
	int saveAllModelData();
	int saveModelData(SMarkData sMark);
	int saveMarkRelationData();//120119.KKY

	int getMarkIndex(int iCamNo, EMarkType eMarkType, const int iModelNo);
	int getErrorCode(const int iVisionErrorCode, int iUserCamNo = DEF_NONE_CAMERA);
	std::string getConsoleInput(const char* szOutMessage);
	void setModelDataFilePath(const std::string strModelDataFilePath);
	int makeDisplay(int iViewID, int iPriority, RECT* pWndPos, CWnd* pParentWnd);
	int getSystemIndex(int iUserCamNo);
	HWND getViewHandle(int iCamNo);
	void assignCameraDataAboutSystems();
	int checkAreaSize(const RECT& rectObject, const RECT& rectLimit);
	int getNewMarkStorageIndex();
	int restoreSearchMark(SMarkData& sMark);
	void removeCameraData(int iCamIndex);
	int cutModelLink(int iCamNo, EMarkType eMarkType, int iModelNo);
	void removeModelFile(int iModelStorageIndex);
	void removeModelElement(int iModelStorageIndex);
	int makeBackupFile(std::string strFile);
	BOOL isValidMarkNo(int iModelNo);
	void setDrawRectData(int iCamNo, RECT& rect, int iObjectType = DEF_FIXED_OBJECT);
	void setDrawPointData(int iCamNo, long lX, long lY, int iObjectType = DEF_FIXED_OBJECT);
	void setDrawTextData(int iCamNo, char* pszText, int iObjectType = DEF_FIXED_OBJECT);
	//110214.Add_____
	void setDrawPointData_Measure(int iCamNo, long lX, long lY, int iObjectType = DEF_FLOATING_OBJECT);
	void setDrawLineData_Measure(int iCamNo, long lStartX, long lStartY, long lEndX, long lEndY, int iObjectType = DEF_FLOATING_OBJECT);
	//_______________
	void redraw(int iCamNo);
	void checkUsingFolders();
	int findMultiModel(int iCamNo, int* rgiModel, long lNumModel);
	std::string getSingleCharacterString(tstring str);

public:
	MVision();
	~MVision();

	int SaveLastErrorImage(int iCamNo, const tstring& strPath);
	int SaveImage(int iCamNo, const tstring& strPath, const tstring& strFile);
	int SaveImage(int iCamNo, const tstring& strFile);

	void LockCamera(int iCamNo);
	void UnlockCamera(int iCamNo);

	int FindMultiModel(int iCamNo, int* rgiModel, long lNumModel);

	void CreateCommonObject(SCommonAttribute& commonData);
#ifdef DEF_USE_ICOMMON
	ICommon* GetCommonObject();
#endif

	int CreateFixedView(int iViewID, RECT* pWndPos, CWnd* pParentWnd = NULL);
	BOOL IsValidCam(const int iCamNo) const;
	const tstring GetCameraName(const int iUserCamNo) const;
	const int GetMaxUseCameraNo() const;

	POINT GetDistribution(int iCamNo);

	int CreateTemporaryView(RECT* pWndPos, CWnd* pParentWnd);

	int CreateLocalView(RECT* pWndPos, CWnd* pParentWnd);
	int DestroyView(int iViewID);

	int Initialize(const tstring strVisionDataFilePath, const tstring strModelDataFilePath, const tstring strLogDataFilePath, const tstring strImageDataFilePath);

	void RedrawView(int iViewID);
	void DrawOverlayAreaRect(int iCamNo, RECT& rect, int iObjectType = DEF_FIXED_OBJECT);
	void DrawOverlayCrossMark(int iCamNo, long lX, long lY, int iObjectType = DEF_FIXED_OBJECT);
	void DrawOverlayText(int iCamNo, char* pszText, int iObjectType = DEF_FIXED_OBJECT);
	//110214.Add_____
	void DrawOverlayLine_Measure(int iCamNo, long lStartX, long lStartY, long lEndX, long lEndY, int iObjectType = DEF_FLOATING_OBJECT);
	void DrawOverlayCross_Measure(int iCamNo, long lX, long lY, int iObjectType = DEF_FLOATING_OBJECT);
	//_______________

	int ImportModel(EMarkType eMarkType, int iSrcCamNo, int iSrcMarkNo, int iDstCamNo, int iDstMarkNo);

	void LoadImage(int iDisplayCamNo, tstring strPathName);
	
	//20121004 SJ_HJG 횡전개
	int GetBlobHistogram(int iCameraNo, int iModelNo, unsigned char* rgucHist);
	//int GetBlobHistogram(int iCameraNo, int iModelNo, unsigned char* rgucHist, int* rglHistVals1);

/* 삭제 예정.
	int		GetGrabSettlingTime(int iCamNo);
	void	SetGrabSettlingTime(int iCamNo, int iGrabSettlingTime);
*/

	int SetModelName(int iCamNo, EMarkType eMarkType, int iModelNo, tstring strName);

	int GetCameraChangeTime(int iCamNo);
	void SetCameraChangeTime(int iCamNo, int iCameraChangeTime);

	int ChangeModel(const tstring strModelDataFilePath);

	int WriteModelData(int iCamNo, EMarkType eMarkType, int iModelNo);

	int RegisterEdgeModel(int CameraNo, int MarkNo, RECT rectMeasurementBox);
	int FindEdge(int iCameraNo, int iModelNo);

	void ClearOverlay(int iCamNo);
	int Grab(int iCamNo);
	int GrabWait(int iCamNo);
	
	int SetSearchAcceptanceThreshold(int iCamNo, int iModelNo, double dValue);
	double GetSearchAcceptanceThreshold(int iCamNo, int iModelNo);
	double GetSearchResultX(int iCamNo, int iModelNo);
	double GetSearchResultY(int iCamNo, int iModelNo);
	//20121004 SJ_HJG 횡전개
	double GetSearchBlobResultX(int iCamNo, int iModelNo);
	double GetSearchBlobResultY(int iCamNo, int iModelNo);
	//_______________
	RECT GetSearchAreaRect(int iCamNo, int iModelNo);
	
	int SetBlobThreshold(int iCamNo, int iModelNo, double dThreshold);
	double GetBlobThreshold(int iCamNo, int iModelNo);
	int SetBlobJudgeMinPixelPercent(int iCamNo, int iModelNo, int iMinPixelPercent);
	int GetBlobJudgeMinPixelPercent(int iCamNo, int iModelNo);
	int SetBlobJudgeMaxPixelPercent(int iCamNo, int iModelNo, int iMaxPixelPercent);
	int GetBlobJudgeMaxPixelPercent(int iCamNo, int iModelNo);
	int SetBlobPolarity(int iCamNo, int iModelNo, double dPolarity);
	double GetBlobPolarity(int iCamNo, int iModelNo);

	double GetBlobResultScore(int iCamNo, int iModelNo);
	long GetBlobPixelsInModelArea(int iCamNo, int iModelNo);

	double GetEdgeFinderPolarity(int iCameraNo, int iModelNo);
	double GetEdgeFinderThreshold(int iCameraNo, int iModelNo);
//	double GetEdgeFinderNumOfResults(int iCameraNo, int iModelNo);
	double GetEdgeFinderDirection(int iCameraNo, int iModelNo);
	
	int SetEdgeFinderPolarity(int iCameraNo, int iModelNo, double dPolarity);
	int SetEdgeFinderThreshold(int iCameraNo, int iModelNo, double dThreshold);
//	int SetEdgeFinderNumOfResults(int iCameraNo, int iModelNo, double dNumOfResults);
	int SetEdgeFinderDirection(int iCameraNo, int iModelNo, double dSearchDirection);
	
	BOOL RecognitionBLOBPattern(int iCameraNo, int iModelNo);
	int RecognitionPatternMark(int iCameraNo, int iModelNo);

	//@130119.KKY____________
	int RecognitionEdgeLineY(int iCameraNo, int iModelNo, BOOL bSettingMode, double dRefVX, double dRefVY);
	//@______________________
	
	int DisplayPatternImage(int iCamNo, EMarkType eMarkType, int iModelNo);
	void EnableSaveImage(BOOL bFlag = FALSE);
	int DeleteOldImageFiles(const tstring& strPath = _T(""));
	void deleteOldFile(struct tm* tmTime, const std::string& strPath, std::string& currentFile);
	
	int RegisterPatternMark(int iCamNo, int iModelNo, RECT& SearchArea, RECT& ModelArea, double dReferenceX, double dReferenceY);

	int SetSearchArea(int iCamNo, int iModelNo, const RECT& SArea);
//@	int RegisterBLOBPattern(int CameraNo, int MarkNo, RECT& SearchArea, RECT& BlobArea);
	int RegisterBLOBPattern(int CameraNo, int MarkNo, RECT& SearchArea, RECT& BlobArea, double dRefX = 0.0, double dRefY = 0.0);
	
	void HaltVideo(int iCamNo);
	int LiveVideo(int iCamNo);
	
	int CheckModel(int iCamNo, EMarkType eMarkType, int iModelNo);
	void DeleteMark(int iCamNo, EMarkType eMarkType, int iModelNo);

	int WriteLog(const tstring& strFile, const tstring& strLog);

	void DrawSourceImage(int iCamNo);

//	int* MVision::SetNumOfUseCam(int* rgiCamNum, int iNumOfUseCam, ...);
};

#endif // !defined(AFX_VISION_H__6DC657D8_C279_4141_884D_B1E351D7E3BB__INCLUDED_)
