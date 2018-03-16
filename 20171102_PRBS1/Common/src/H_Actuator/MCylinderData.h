#ifndef MCYLINDERDATA_H
#define MCYLINDERDATA_H

#include "MFileManager.h"

class MCylinderData : public MFileManager 
{
private:
	/** Section Name */
	CString m_strSection;

public:
	/** ���� ���� �ð� */
	double m_dMovingTime;

	/** 1���� �� ����ȭ �ð� */
    double m_dSettlingTime1;

	/** 2���� �� ����ȭ �ð� */
	double m_dSettlingTime2;

	/** Sensor�� ���� ���� ���� ���� �ð� */
    double m_dNoSenMovingTime;

	/** Simulation Mode ������ Cylinder ���� �ҿ� �ð�*/
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
