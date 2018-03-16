#pragma once
#include "btnenh.h"

// CAutoDoorStateDlg 대화 상자입니다.
const int DEF_MAX_DOOR_BTN		= 16;
const int DEF_MAX_DOOR_KEY_BTN	= 4;

class CAutoDoorStateDlg : public CDialog
{
	DECLARE_DYNAMIC(CAutoDoorStateDlg)

public:
	CAutoDoorStateDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAutoDoorStateDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_AUTO_DOOR_STATE };

protected:
	UINT m_uiTimerID;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	DECLARE_EVENTSINK_MAP()
	void ClickExit();

	virtual BOOL OnInitDialog();
	
	CBtnEnh m_lblDoorState[DEF_MAX_DOOR_BTN];
	CBtnEnh m_lblKeyState[DEF_MAX_DOOR_KEY_BTN];

	// 현재 Safety Door가 사용하고 있는지 여부 판단 TRUE : 사용, FALSE : 미사용
	BOOL	m_bDoorStateFlg[DEF_MAX_DOOR_BTN];
	// Door의 이름
	CString m_strDoorName[DEF_MAX_DOOR_BTN];
	UINT	m_uiDoorAddr[DEF_MAX_DOOR_BTN];

	BOOL	m_bKeyStateFlg[DEF_MAX_DOOR_KEY_BTN];
	CString m_strKeyName[DEF_MAX_DOOR_KEY_BTN];
	UINT	m_uiKeyAddr[DEF_MAX_DOOR_KEY_BTN];
};
