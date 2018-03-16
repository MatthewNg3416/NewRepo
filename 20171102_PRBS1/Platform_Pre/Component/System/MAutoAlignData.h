#ifndef MAUTOALIGNDATA_H
#define MAUTOALIGNDATA_H

#include "MFileManager.h"
#include "DefSystem.h"


#define SYSTEM_SECTION			_T("System")
#define ALIGN_SECTION			_T("Align")


/**
 * Align Data를 파일에서 읽어서 Member Variable 이 그 값을 갖고 있다.
 */
class MAutoAlignData : public MFileManager
{
private:
	CString			m_strSection;

public:
/************************************************************************/
/*        Common Section											    */
/************************************************************************/

	BOOL m_bAutoAlignSystem;

	double XuTabOffset_X[50];
	double XuTabOffset_Y[50];
	double XuTabOffset_T[50];

	double XdTabOffset_X[50];
	double XdTabOffset_Y[50];
	double XdTabOffset_T[50];

	double YlTabOffset_X[50];
	double YlTabOffset_Y[50];
	double YlTabOffset_T[50];

	double YrTabOffset_X[50];
	double YrTabOffset_Y[50];
	double YrTabOffset_T[50];


private:
	void initialize();

public:
    MAutoAlignData(CString strFileName, CString strPath);
	MAutoAlignData(CString strFileName);
	~MAutoAlignData();

	int ReadSystem();  
	int ReadSourceAlignData();
	int ReadGateAlignData();

};
#endif // MAutoAlignData_H
