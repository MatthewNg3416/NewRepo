#ifndef MVACUUMDATA_H
#define MVACUUMDATA_H

#include "MFileManager.h"


class MVacuumData : public MFileManager 
{
private:
	/** Section Name */
	CString m_strSection;

public:
	/** ���� ���� �ð� */
	double	m_dTurningTime;

	/** ON ���� �� ����ȭ �ð� */
	double	m_dOnSettlingTime;

	/** OFF ���� �� ����ȭ �ð� */
	double	m_dOffSettlingTime;

	/** Simulation Mode ������ Vacuum ���� �ҿ� �ð�*/
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
