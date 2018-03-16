#ifndef MTABDATA_H
#define MTABDATA_H

#include "MFileManager.h"
#include "DefSystem.h"

/** Tab Data */
class MTabData : public MFileManager 
{
public:
	int		m_iXuTabQty;
	double	m_dXuTabPitch[DEF_MAX_TAB_XU];
	BOOL	m_bUseXuTab[DEF_MAX_TAB_XU];

	int		m_iXdTabQty;
	double	m_dXdTabPitch[DEF_MAX_TAB_XD];
	BOOL	m_bUseXdTab[DEF_MAX_TAB_XD];

	int		m_iYlTabQty;
	double	m_dYlTabPitch[DEF_MAX_TAB_YL];
	BOOL	m_bUseYlTab[DEF_MAX_TAB_YL];

	int		m_iYrTabQty;
	double	m_dYrTabPitch[DEF_MAX_TAB_YR];
	BOOL	m_bUseYrTab[DEF_MAX_TAB_YR];

	BOOL	m_bHalfandHalf;  //120625.kms

private:
	void initialize();

public:
	MTabData();
	MTabData(CString strFileName, CString strPath);
	MTabData(CString strFileName);
	~MTabData();

    BOOL ReadData();
    BOOL WriteData();

	MTabData& CopyData(MTabData& tabData);
};
#endif //MTABDATA_H
