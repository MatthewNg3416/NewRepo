#if !defined(AFX_VISION_SYSTEM_SOLIOS_H__2A882A32_9030_4F3D_95B0_B30AEABE1DB7__INCLUDED_)
#define AFX_VISION_SYSTEM_SOLIOS_H__2A882A32_9030_4F3D_95B0_B30AEABE1DB7__INCLUDED_

#include "stdafx.h"
#include <Afxmt.h>
#include "Msystem.h"

// Msystem_solios Class Decleration
class Msystem_solios : public Msystem
{
	// Member Variables
private:
	static BOOL m_bDisplayImg;//DisplayȽ���� ���̱� ����(100901.KKY)

	// Hooking Counter Array Pointer;
	int* m_rgiDigGrabHookCount;	// Grab Hooking Counter
	int* m_rgiDigLiveHookCount;	// Continuous Grab Hooking Counter

	// ���� Input Path �� �����ϴ� �ٸ� Camera ��ȣ ���� Array
	int m_rgiPairCamNo[DEF_SOLIOS_SYSTEM_MAX_CAM_NUM];

	// Solios �� ����Ǵ� Camera �� Device (Data Input Path) ��ȣ ����
	long m_rglDeviceNo[DEF_SOLIOS_SYSTEM_MAX_CAM_NUM];

	// Solios �� ����Ǵ� Camera �� Channel ��ȣ ����
	long m_rglCamCh[DEF_SOLIOS_SYSTEM_MAX_CAM_NUM];

	// Independent Data Input Path Critical Section
	CRITICAL_SECTION m_csDataInput[DEF_MATROX_SOLIOS_INDEPENDENT_INPUT_PATH_NUM];

	// Member Functions
protected:
	static long MFTYPE hookHandler(long lHookType, long lEventID, void MPTYPE *pUserData);
	static long MFTYPE hookLiveHandler(long lHookType, long lEventID, void MPTYPE *pUserData);

public:
	Msystem_solios();
	virtual ~Msystem_solios();

	// System Allocation
	int AllocSystem(SSystemData* pSystemData);

	// Grab Functions
	int Grab(SCameraData* pCamData);
	void GrabContinuous(SCameraData* pCamData);
	void Halt(SCameraData* pCamData);

	// Etc. Functions
	void FreeAllDigitizer();
	int ChangeCamera(int iCamNo, long lCameraChangeTime = 0);
	virtual void EnterCameraCriticalSection(int iCamNo);
	virtual void LeaveCameraCriticalSection(int iCamNo);
};

#endif // !defined(AFX_VISION_SYSTEM_SOLIOS_H__2A882A32_9030_4F3D_95B0_B30AEABE1DB7__INCLUDED_)
