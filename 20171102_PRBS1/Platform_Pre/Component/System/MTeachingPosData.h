/* samsung */

#ifndef MTEACHING_POS_DATA_H
#define MTEACHING_POS_DATA_H

#include "MFileManager.h"
#include "MTeachingInfo.h"

/**
 * Teaching Posiont Data�� ���Ͽ��� �о Member Variable�� �� ���� ���� �ִ�.
 */
class MTeachingPosData : public MFileManager 
{
public:

	CString		m_strUnitName[3];		// GUI Name
	CString		m_strSectionName;		// Section Name
	int			m_iNumAxis;
	CString		m_strSectionPostfix[DEF_MAX_TEACHING_AXIS];
	int			m_iNumTeachingPos;
	SPosition	m_sPosition[DEF_MAX_TEACHING_POS];
	double		m_TeachingPos[DEF_MAX_TEACHING_POS][DEF_MAX_TEACHING_AXIS];

public:    
	MTeachingPosData(){};
	/**��ΰ� ���� ������ �����ϴ� ������*/
    MTeachingPosData(STeachingInfo sTeachingInfo, CString strFileName, CString strPath);
	/**��ΰ� Data ������ �����ϴ� ������*/
	MTeachingPosData(STeachingInfo sTeachingInfo, CString strFileName);
	/** �Ҹ��� */
	~MTeachingPosData();

	/** Data�� File���� �о� ���� �Լ� */
	BOOL ReadData();
	/** Data�� File�� ���� ���� �Լ� */
    BOOL WriteData();

	/** Path�� ���� �ϴ� �Լ� */
	void SetPath(CString strPath);

private :
	CString	m_strKeyName[DEF_MAX_TEACHING_POS][DEF_MAX_TEACHING_AXIS];	// Key Name

	/** ����Ÿ �ʱ�ȭ */
	void initialize(STeachingInfo sTeachingInfo);

};
#endif //MTEACHING_POS_DATA_H
