/* samsung */

#ifndef MTEACHING_POS_DATA_H
#define MTEACHING_POS_DATA_H

#include "MFileManager.h"
#include "MTeachingInfo.h"

/**
 * Teaching Posiont Data를 파일에서 읽어서 Member Variable이 그 값을 갖고 있다.
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
	/**경로가 임의 폴더면 실행하는 생성자*/
    MTeachingPosData(STeachingInfo sTeachingInfo, CString strFileName, CString strPath);
	/**경로가 Data 폴더면 실행하는 생성자*/
	MTeachingPosData(STeachingInfo sTeachingInfo, CString strFileName);
	/** 소멸자 */
	~MTeachingPosData();

	/** Data를 File에서 읽어 오는 함수 */
	BOOL ReadData();
	/** Data를 File에 쓰는 오는 함수 */
    BOOL WriteData();

	/** Path를 설정 하는 함수 */
	void SetPath(CString strPath);

private :
	CString	m_strKeyName[DEF_MAX_TEACHING_POS][DEF_MAX_TEACHING_AXIS];	// Key Name

	/** 데이타 초기화 */
	void initialize(STeachingInfo sTeachingInfo);

};
#endif //MTEACHING_POS_DATA_H
