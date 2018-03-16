#ifndef MVACUUMDATA_H
#define MVACUUMDATA_H

#include "MFileManager.h"


class MVacuumData : public MFileManager 
{
private:
	/** Section Name */
	CString m_strSection;

public:
	/** 동작 제한 시간 */
	double	m_dTurningTime;

	/** ON 동작 후 안정화 시간 */
	double	m_dOnSettlingTime;

	/** OFF 동작 후 안정화 시간 */
	double	m_dOffSettlingTime;

	/** Simulation Mode 에서의 Vacuum 동작 소요 시간*/
	double m_dSimulationTime;
    
public:
	MVacuumData(){};
    /**
     * @stereotype constructor	
	 *
	 * @param	CString strSection (Section Name)
	 * @param	CString strFileName (File Name)
     */
	MVacuumData(CString strSection, CString strFileName);

    /**
     * @stereotype constructor	
	 *
	 * @param	CString strSection (Section Name)
	 * @param	CString strFileName (File Name)
	 * @param	CString strPath (File Path)
     */	
	MVacuumData(CString strSection, CString strFileName, CString strPath);

    /**
     * @stereotype destructor 
     */	
	~MVacuumData();

	void Initialize();

    BOOL ReadData();

    BOOL WriteData();
};

#endif //MVACUUMDATA_H
