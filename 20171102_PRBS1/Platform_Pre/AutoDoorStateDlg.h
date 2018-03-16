#pragma once
#include "btnenh.h"

// CAutoDoorStateDlg ��ȭ �����Դϴ�.
const int DEF_MAX_DOOR_BTN		= 16;
const int DEF_MAX_DOOR_KEY_BTN	= 4;

class CAutoDoorStateDlg : public CDialog
{
	DECLARE_DYNAMIC(CAutoDoorStateDlg)

public:
	CAutoDoorStateDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAutoDoorStateDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_AUTO_DOOR_STATE };

protected:
	UINT m_uiTimerID;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	DECLARE_EVENTSINK_MAP()
	void ClickExit();

	virtual BOOL OnInitDialog();
	
	CBtnEnh m_lblDoorState[DEF_MAX_DOOR_BTN];
	CBtnEnh m_lblKeyState[DEF_MAX_DOOR_KEY_BTN];

	// ���� Safety Door�� ����ϰ� �ִ��� ���� �Ǵ� TRUE : ���, FALSE : �̻��
	BOOL	m_bDoorStateFlg[DEF_MAX_DOOR_BTN];
	// Door�� �̸�
	CString m_strDoorName[DEF_MAX_DOOR_BTN];
	UINT	m_uiDoorAddr[DEF_MAX_DOOR_BTN];

	BOOL	m_bKeyStateFlg[DEF_MAX_DOOR_KEY_BTN];
	CString m_strKeyName[DEF_MAX_DOOR_KEY_BTN];
	UINT	m_uiKeyAddr[DEF_MAX_DOOR_KEY_BTN];
};
