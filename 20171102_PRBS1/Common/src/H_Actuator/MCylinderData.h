#ifndef MCYLINDERDATA_H
#define MCYLINDERDATA_H

#include "MFileManager.h"

class MCylinderData : public MFileManager 
{
private:
	/** Section Name */
	CString m_strSection;

public:
	/** 동작 제한 시간 */
	double m_dMovingTime;

	/** 1동작 후 안정화 시간 */
    double m_dSettlingTime1;

	/** 2동작 후 안정화 시간 */
	double m_dSettlingTime2;

	/** Sensor가 없을 때의 동작 제한 시간 */
    double m_dNoSenMovingTime;

	/** Simulation Mode 에서의 Cylinder 동작 소요 시간*/
	double m_dSimulationTime;
    
public:

	MCylinderData(){};

	MCylinderData(CString strSection, CString strFileName);

	MCylinderData(CString strSection, CString strFileName, CString strPath);

	~MCylinderData();

	void Initialize();

    BOOL ReadData();

    BOOL WriteData();
};

#endif //MCYLINDERDATA_H
