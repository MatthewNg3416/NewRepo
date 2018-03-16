#if !defined(AFX_VISION_SYSTEM_H__2A882A32_9030_4F3D_95B0_B30AEABE1DB7__INCLUDED_)
#define AFX_VISION_SYSTEM_H__2A882A32_9030_4F3D_95B0_B30AEABE1DB7__INCLUDED_

#include "stdafx.h"
#include <Afxmt.h>
#include "DefVision.h"
#include "Shlwapi.h"
#pragma comment(lib, "Shlwapi.lib")

// Msystem Class Decleration
class Msystem  
{
	// Member Variables
protected:
	// MIL ID
	long m_lSysID;	// MIL System ID Array (Dynamic Allocation) : Frame Grabber H/W
	long* m_rglDigID;	// MIL Digitizer ID Array (Dynamic Allocation) : Camera Module

	// MIL Buffer ID
	long* m_rglGrabParent;	// Grab Parent Color buffer Array for MeteorII M/C (Dynamic Allocation)
	long* m_rglGrab;	// Grab Buffer ID Array (Dynamic Allocation)
	long* m_rglSource;	// Recognition Source Image Buffer ID Array (Dynamic Allocation)

	// Data Members
	int m_iSystemType;	// SystemType : Meteor2 M/C(DEF_METEOR_II_MC(0)), Solios(DEF_SOLIOS(1))
	int m_iInstanceNo;	// System Instance No : 1st system = 0, 2nd system = 1, ...
	int m_iMaxCamNum;	// Max Camera Num / 1 System [in agreement with System Type(Meteor II MC, Solios, ...)]
	int m_iStartCamNo;	// 현재 System 에서의 1st Camera No.
	std::vector<SCameraData>* m_pvectorCamData;	// MVsion::m_vectorCamData 에 대한 link pointer

	// Member Functions
protected:
	virtual long getDigitizer(int iSysCamNo);
	void initMembers(SSystemData* pSystemData);

public:
	Msystem();
	virtual ~Msystem();

	// System Allocation
	virtual int AllocSystem(SSystemData* pSystemData) = 0;

	// Get Functions
	long GetSysID();
	long GetSourceBuffer(int iUserCamNo);
	long GetGrabBuffer(int iUserCamNo);
	int GetStartCamNo();
	int GetLastCamNo();
	int WaitGrabComplete(int iUserCamNo);

	// Grab Functions
	virtual int Grab(SCameraData* pCamData) = 0;
	virtual void GrabContinuous(SCameraData* pCamData) = 0;
	virtual void Halt(SCameraData* pCamData) = 0;

	// Etc. Functions
	virtual void FreeAllDigitizer() = 0;
	virtual int ChangeCamera(int iCamNo, long lCameraChangeTime) = 0;
	virtual void EnterCameraCriticalSection(int iCamNo);
	virtual void LeaveCameraCriticalSection(int iCamNo);
};

#endif // !defined(AFX_VISION_SYSTEM_H__2A882A32_9030_4F3D_95B0_B30AEABE1DB7__INCLUDED_)
