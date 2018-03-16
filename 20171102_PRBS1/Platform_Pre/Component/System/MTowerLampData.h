#ifndef MTOWERLAMPDATA_H
#define MTOWERLAMPDATA_H

#include "MFileManager.h"
#include <assert.h>
#include "common.h"
#include "DefSystem.h"

 /** Panel Data */
class MTowerLampData : public MFileManager 
{
public:

	int	m_bUseRedLamp_NomalRun;
	int	m_bUseRedLamp_StepStop;
	int	m_bUseRedLamp_ErroeStop;
	int	m_bUseRedLamp_PanelEmpty;
	int	m_bUseRedLamp_MaterialChange;
	int	m_bUseRedLamp_OPCall;

	int	m_bUseYellowLamp_NomalRun;
	int	m_bUseYellowLamp_StepStop;
	int	m_bUseYellowLamp_ErroeStop;
	int	m_bUseYellowLamp_PanelEmpty;
	int	m_bUseYellowLamp_MaterialChange;
	int	m_bUseYellowLamp_OPCall;

	int	m_bUseGreenLamp_NomalRun;
	int	m_bUseGreenLamp_StepStop;
	int	m_bUseGreenLamp_ErroeStop;
	int	m_bUseGreenLamp_PanelEmpty;
	int	m_bUseGreenLamp_MaterialChange;
	int	m_bUseGreenLamp_OPCall;

	int	m_bUseBuzzer_NomalRun;
	int	m_bUseBuzzer_StepStop;
	int	m_bUseBuzzer_ErroeStop;
	int	m_bUseBuzzer_PanelEmpty;
	int	m_bUseBuzzer_MaterialChange;
	int	m_bUseBuzzer_OPCall;

	int	m_bUseStartLamp_NomalRun;
	int	m_bUseStartLamp_StepStop;
	int	m_bUseStartLamp_ErroeStop;
	int	m_bUseStartLamp_PanelEmpty;
	int	m_bUseStartLamp_MaterialChange;
	int	m_bUseStartLamp_OPCall;

	int	m_bUseStopLamp_NomalRun;
	int	m_bUseStopLamp_StepStop;
	int	m_bUseStopLamp_ErroeStop;
	int	m_bUseStopLamp_PanelEmpty;
	int	m_bUseStopLamp_MaterialChange;
	int	m_bUseStopLamp_OPCall;

	int	m_iDelayTime;

private:
	CString m_strSection;

public:
	MTowerLampData(CString strFileName, CString strPath);
	MTowerLampData(CString strFileName);
	~MTowerLampData();

	/** Data를 File에서 읽어 오는 함수 */
    BOOL ReadData();
	/** Data를 File에 쓰는 오는 함수 */
    BOOL WriteData();

	void Initialize();
};
#endif //MTOWERLAMPDATA_H
