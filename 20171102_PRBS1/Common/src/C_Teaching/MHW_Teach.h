/* samsung */

#ifndef MHW_TEACH_H
#define MHW_TEACH_H

#include "DefSystem.h"
#include "ICommon.h"
#include "MPos_XYT.h"

class MPanelData;
class IVision;
class MPanelAligner;
class MVisionCalibration;

/**
 * Error Code Define
 */
const	int	ERR_HW_TEACH_SUCCESS		= 0;

const	int	DEF_HW_TEACH_NONE			= -1;
const	int	DEF_HW_TEACH_STAGE			= 0;
const	int	DEF_HW_TEACH_PCBHANDLER		= 1;
const	int DEF_NO_POSID				= -1;

#ifndef RAD2DEG
#define	RAD2DEG(x)		((x) * 180.0 / 3.1415926535)
#endif

#ifndef DEG2RAD
#define	DEG2RAD(x)		((x) * 3.1415926535 / 180.0)
#endif

typedef
/** HW_Teach Component가 가지는 Component List */
struct tagSHW_TeachRefCompList
{
	/** Panel Aligner Component */
    MPanelAligner*		m_plnkPanelAligner[DEF_MAX_STAGE];

	/** Vision Component */
	IVision*			m_plnkVision;

	/** VisionCalibration Cmoponent */
	MVisionCalibration*	m_plnkVisionCalibration;

} SHW_TeachRefCompList, *pSHW_TeachRefCompList;

typedef
/** HW_Teach Data */
struct tagSHW_TeachData
{
	MPanelData*	m_plnkPanelData;

} SHW_TeachData, *pHW_TeachData;

class MHW_Teach : public virtual ICommon
{
/** Attribute */
private:

	// Related Component
    MPanelAligner*			m_plnkPanelAligner[DEF_MAX_STAGE];

	IVision*				m_plnkVision;

	/** VisionCalibration Cmoponent */
	MVisionCalibration*		m_plnkVisionCalibration;

	// Component Data
	MPanelData*				m_plnkPanelData;

	/** Constructor, Destructor, 초기화 관련 */
public:

	/**
	 * Constructor
	 * @stereotype constructor 
	 */
	/** Standard 생성자
	  */
	MHW_Teach(SCommonAttribute	commonData,	
		      SHW_TeachRefCompList listRefComponents, 
			  SHW_TeachData datComponent);

	/**
	 * Destructor
	 * @stereotype destructor 
	 */
	virtual ~MHW_Teach();

	int GetCameraFixedPosition(int iStageID, 
		                       int iCamera, 
							   int iMark,
							   double dTAngle,
							   double* pX,
							   double* pY);

	int GetTabCarrierCameraFixedPosition(int iCarrierID, 
								int iCamNo,
								int iMarkNo,
								double dTAngle,
								double* pX,
								double* pY,
								double* pCamDistX);									
	
	int GetCameraDistance(int iStageID,
		                  int iCamera, 
						  int iMark, 
						  int iOtherCam, 
						  int iOtherMark,
						  double* pdX,
						  double* pdY);
	
	int Initialize();

	/***************** Common Implementation *************************************/

	virtual int AssignComponents(SHW_TeachRefCompList listRefComponents);

	virtual int SetData(SHW_TeachData datComponent);

	virtual int GetData(SHW_TeachData *datComponent);

private:
	
	void FindCenter(double dTAngle, 
		            double dRadius, 
					MPos_XYT posFirst, 
					MPos_XYT posSecond, 
					MPos_XYT* posCenter);

	void Rotation(double dAngle, 
		          double dOldX, 
				  double dOldY, 
				  double* dNewX,
				  double* dNewY);
};

#endif //MHW_TEACH_H
