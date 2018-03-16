// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__CFC254C1_998F_40AD_B8B5_1B5E18136E27__INCLUDED_)
#define AFX_STDAFX_H__CFC254C1_998F_40AD_B8B5_1B5E18136E27__INCLUDED_

//LKH_20090114 : VC++ 2008 warning 대비
#if _MSC_VER > 1200	//VC++ 6.0 : 1200, VC++ 2008(9.0) : 1500
//#define WINVER 0x0501 //Windows Xp
#pragma warning(disable:4996)
#pragma warning(disable:4819)
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//#include <afxsock.h>		// MFC socket extensions
#ifdef _WIN32_WCE
#include <winsock.h>
#include <ipexport.h>
#include <icmpapi.h>
#elif defined( WIN32 )
#include <winsock2.h>
#include <ws2tcpip.h>
#endif


///#include "htmlhelp.h"
#include "resource.h"

#define UINT16	unsigned short

///#define	HELP_FILE_PATH	"./Help/PreBonder.chm"

#define WM_SW_STATUS	(WM_USER + 1)
#define WM_AUTO_STATUS	(WM_USER + 2)
#define WM_ALARM_MSG	(WM_USER + 3)

#define	WM_MSGBOX_MSG	(WM_USER + 100)		// MyMessageBox 띄우기
#define WM_SWITCH_VIEW	(WM_USER + 101)		// View Switch 알림

//#define SIMULATION
//#define VISION_SIMULATION

/** 운전 모드 변화 MSG
*	wparam : 운전 모드
*/
#define WM_DISP_RUN_MODE			(WM_USER + 300)

/** 운전 상태 변화 MSG
*	wparam : 운전 상태
*/
#define WM_DISP_OPERATE_STATUS	    (WM_USER + 301)

/** 생산 모델 Msg
*	lparam : 생산 모델
*/
#define WM_DISP_MODEL_NAME			(WM_USER + 302)

/** 설비 TackTime Msg
*	lparam : 설비 Tack Tiem
*/
#define WM_DISP_EQ_TACK_TIME		(WM_USER + 303)

/** Line TackTime Msg
*	lparam : Line Tack Time
*/
#define WM_DISP_LINE_TACK_TIME		(WM_USER + 304)

/** 생산 수량 Msg
*	wparam : 생산 수량 
*/
#define WM_DISP_PRODUCT_QTY			(WM_USER + 305)

/** Inspect Camera All Move
*	Align Pos
*/
#define	WM_ALL_INSPECT_CAM_MOVE_ALIGN	(WM_USER + 306)

/** Feeder1 Tab Info
*	wparam : Feeder1 Tab Info
*/
#define WM_DISP_TABFEEDER1_INFO		(WM_USER + 309)

/** Feeder2 Tab Info
*	wparam : Feeder2 Tab Info
*/
#define WM_DISP_TABFEEDER2_INFO		(WM_USER + 310)

/** Mold1 Tab Info
*	wparam : Mold1 Tab Info
*/
#define WM_DISP_MOLD1_INFO			(WM_USER + 312)

/** Mold2 Tab Info */
#define WM_DISP_MOLD2_INFO			(WM_USER + 313)

/** Terminal Message Display */
#define WM_DISP_TERMINAL_MSG		(WM_USER + 314)

/** OP Call Message Display */
#define WM_DISP_OPCALL_MSG			(WM_USER + 315)

/** EQ State Message Display */
#define WM_DISP_EQ_STATE			(WM_USER + 316)

/** EQP State Message Display */
#define WM_DISP_EQP_STATE			(WM_USER + 317)

// Panel Scrap Dlg Display Request
#define WM_DISP_PANEL_SCRAP_DLG		(WM_USER + 318)

// Panel Unscrap Dlg Display Request
#define WM_DISP_PANEL_UNSCRAP_DLG	(WM_USER + 319)

// Material Info Dlg Display Request
#define WM_DISP_MATERIAL_INFO_DLG	(WM_USER + 320)

// Start TabFeeder Auto Homing Request
#define WM_START_TAB_FEEDER_AUTO_HOMING_REQ	(WM_USER + 321)

// TabMounter 회피 요청.
#define WM_MOVE_TAB_MOUNTER_REQ		(WM_USER + 323)

// Remove TabIC Request
#define WM_REMOVE_TABIC_REQ			(WM_USER + 324)

// Display Cylinder Move Time Dialog Request
#define WM_DISP_CYLINDER_MOVE_TIME_DLG	(WM_USER + 325)

// (Source) Inspection Camera 회피 요청.
#define WM_MOVE_INSPECTION_CAMERA_REQ	(WM_USER + 326)

// Display E-Stop Status Request
#define WM_DISP_E_STOP_STATUS		(WM_USER + 327)

// Display Teaching Pendant 연결 상태
#define WM_DISP_TEACH_PENDANT_CONNECTING_STATUS		(WM_USER + 328)

// Log Main Window Refresh Request
#define WM_REFRESH_LOG_MAIN_WINDOW	(WM_USER + 329)

// Display Material Exhaust Pre Alarm
#define WM_DISP_MATERIAL_EXHAUST_PRE_ALARM_REQ		(WM_USER + 330)

/** Load Conveyor Panel ID Msg
*	lparam : Load Conveyor Panel ID
*/
#define WM_DISP_LOAD_CONVEYOR_PANEL_ID		(WM_USER + 331)
 
/** Panel Transfer Panel ID Msg
*	lparam : Panel Transfer Panel ID
*/
#define WM_DISP_PANEL_TRANSFER_PANEL_ID		(WM_USER + 332)
 
/** Panel Aligner Panel ID Msg
*	lparam : Panel Aligner Panel ID
*/
#define WM_DISP_PANEL_ALIGNER_PANEL_ID		(WM_USER + 333)
 
/** Unload Conveyor Panel ID Msg
*	lparam : Unload Conveyor Panel ID
*/
#define WM_DISP_UNLOAD_CONVEYOR_PANEL_ID	(WM_USER + 334)

// TabFeeder Current Reel Recovery Rail Unfold Request
#define WM_TABFEEDER_RAIL_UNFOLD_REQ		(WM_USER + 335)

// Display My Message
#define WM_DISP_MY_MESSAGE					(WM_USER + 336)

// NSMC
#define WM_DISP_PROGRESS_DLG_FOR_AUTO_MODEL_CHANGE	(WM_USER + 337)

// NSMC
// Panel ID Display Request
#define WM_DISP_PANEL_ID		(WM_USER + 338)

// Material Check Request
#define WM_DISP_MAT_CHECK		(WM_USER + 339)

// For Align Image Display Request
#define WM_DISP_MANUAL_PANEL_MARK_SEARCH_DLG_REQ	(WM_USER + 340)

/** 설비 Stop 유실 보고 Msg */ 
#define WM_DISP_EQSTOP_MSG			(WM_USER + 341) // 100825

#define WM_DISP_INSPECTION_JUDGE_DLG_REQ	(WM_USER + 342)

#define WM_AUTO_UNIT_INITIALIZE				(WM_USER + 343)

//110620.KKY_____
#define WM_DISP_PRODUCT_PLAN_MSG			(WM_USER + 344)
#define WM_DISP_MATERIAL_CODEINFO			(WM_USER + 345)

#define WM_SHOW_MATERIAL_CODEINFO_DLG		(WM_USER + 347)
#define WM_SHOW_PRODUCT_PLAN_DLG			(WM_USER + 348)
//_______________

// ML_hongju
// For Multi Language Request
#define WM_DISP_MULTI_LANGUAGE_DLG_REQ		(WM_USER + 350)

// 2011.12.14 KWC ------------------------------------
/** 생산 DeviceID Msg
*	lparam : 생산 DeviceID
*/
#define WM_DISP_MODEL_DEVICEID			(WM_USER + 360)
// ---------------------------------------------------

/** 설비 상태 Operater Change Msg */ 
#define WM_DISP_OPERATER_MSG			(WM_USER + 361) // 120508.kms

#define WM_DISP_MANUAL_SEARCH_REQUEST		(WM_USER + 362)
#define WM_DISP_MANUAL_SEARCH_POINT			(WM_USER + 363)

// Doolin kwangilshin 2017. 09. 12.
//
#define WM_DISP_PANEL_IN_QTY				(WM_USER + 364)
#define WM_DISP_PANEL_SCRAP_QTY				(WM_USER + 365)
// End

//@KKY.Compile창에 Log 찍기___________________
#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : Warning Msg From KKY: "
#define __LOC2__ __FILE__ "("__STR1__(__LINE__)") : "
///#pragma message(__LOC__ "여기 중요한 부분! 나중에 수정해야 함.")
///#pragma message(__LOC2__ "error C9901: 이부분은 오류로 만들고 싶구나...")
//@___________________________________________
#define DEF_MIL_VER_90
//#undef DEF_MIL_VER_90
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
const COLORREF BACKGROUND_COLOR = RGB(230, 230, 230);
const COLORREF CLOUDBLUE		= RGB(128, 184, 223);
const COLORREF WHITE			= RGB(255, 255, 255);
const COLORREF BLACK			= RGB(1, 1, 1);
const COLORREF DKGRAY			= RGB(128, 128, 128);
const COLORREF LTGRAY			= RGB(192, 192, 192);
const COLORREF YELLOW			= RGB(255, 255, 0);
const COLORREF RED				= RGB(255, 0, 0);
const COLORREF DKRED			= RGB(128, 0, 0);
const COLORREF BLUE				= RGB(0, 0, 255);
const COLORREF DKBLUE			= RGB(0, 0, 128);
const COLORREF CYAN				= RGB(0, 255, 255);
const COLORREF DKCYAN			= RGB(0, 128, 128);
const COLORREF GREEN			= RGB(0, 255, 0);
const COLORREF DKGREEN			= RGB(0, 128, 0);
const COLORREF MAGENTA			= RGB(255, 0, 255);
const COLORREF DKMAGENTA		= RGB(128, 0, 128);
const COLORREF BASIC			= RGB(212, 208, 200);
const COLORREF STATIC_BACKGROUND = RGB(237, 231, 218);

const COLORREF DATA_ITEM_CHANGE_BACKGROUND_COLOR= CYAN;
const COLORREF DATA_ITEM_CHANGE_TEXT_COLOR		= BLACK;
const COLORREF DATA_ITEM_TEXT_COLOR				= BLACK;
const COLORREF DATA_ITEM_BACKGROUND_COLOR		= CYAN;

const int DEF_WS_EX_LAYERED	= 0x00080000;
const int DEF_LWA_COLORKEY	= 0x00000001;
const int DEF_LWA_ALPHA		= 0x00000002;
const int DEF_ULW_COLORKEY	= 0x00000001;
const int DEF_ULW_ALPHA		= 0x00000002;
const int DEF_ULW_OPAQUE	= 0x00000004;

#endif // !defined(AFX_STDAFX_H__CFC254C1_998F_40AD_B8B5_1B5E18136E27__INCLUDED_)
