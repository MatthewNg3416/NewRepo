#if !defined(AFX_VISION_SYSTEM_METEOR_H__2A882A32_9030_4F3D_95B0_B30AEABE1DB7__INCLUDED_)
#define AFX_VISION_SYSTEM_METEOR_H__2A882A32_9030_4F3D_95B0_B30AEABE1DB7__INCLUDED_

#include "stdafx.h"
#include <Afxmt.h>
#include "Msystem.h"

typedef struct
{
	long* plGrabR;
	long* plGrabG;
	long* plGrabB;
	long lSourceR;
	long lSourceG;
	long lSourceB;

	SCameraData* pDataR;
	SCameraData* pDataG;
	SCameraData* pDataB;
} SMeteorHookData;

// Msystem_meteor Class Decleration
class Msystem_meteor : public Msystem
{
	// Member Variables
private:
	int m_iOldChNo;
	int m_iMaxCamPerChannel;
	SMeteorHookData* m_pHookData;

	// Hooking Counter Array Pointer;
	int* m_rgiDigHookCount;	// Digitizer Hooking Counter

	// Member Functions
private:
	int getFirstCamNoPerChannel(int iChID);

protected:
	long getDigitizer(int iSysCamNo);
	static long MFTYPE hookHandler(long lHookType, long lEventID, void MPTYPE *pUserData);
	static long MFTYPE hookLiveHandler(long lHookType, long lEventID, void MPTYPE *pUserData);

public:
	Msystem_meteor();
	virtual ~Msystem_meteor();

	// System Allocation
	int AllocSystem(SSystemData* pSystemData);

	// Grab Functions
	int Grab(SCameraData* pCamData);
	void GrabContinuous(SCameraData* pCamData);
	void Halt(SCameraData* pCamData);

	// Etc. Functions
	void FreeAllDigitizer();
	int ChangeCamera(int iCamNo, long lCameraChangeTime);
};

#endif // !defined(AFX_VISION_SYSTEM_METEOR_H__2A882A32_9030_4F3D_95B0_B30AEABE1DB7__INCLUDED_)
