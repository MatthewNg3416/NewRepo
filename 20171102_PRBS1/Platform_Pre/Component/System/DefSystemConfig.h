#ifndef DEFSYSTEMCONFIG_H
#define DEFSYSTEMCONFIG_H



//
// Setup 기간 중, Setup 편의를 위해 필요한 기능 구현 관련 Define.
//  ex) Safety Sensor 사용 유무 설정 etc.
//#define DEF_SETUP

//170711 JSH.s
#ifdef DEF_SETUP
//#define DEF_SETUP_NONE_IN_LINE
#undef SETUP_NONE_IN_LINE
#endif
//170711 JSH.e

// 설비 운영 사업장에 대한 Define Macro
//  : 사업장 별 적용 기준이 다른 항목에 대한 처리를 위한 Define.
//    ex.) Lamp 운용 방식wlrma
#define DEF_TANGJUNG_SITE	// 탕정사업장
#undef DEF_SESL_SITE		// SESL
///////////////////////////////////////////////////////////////

// Mirror Line 에 대한 Define Macro
#undef DEF_MIRROR_ //기존 DEF_MIRROR_LINE 대체
///////////////////////////////////////////////////////////////

#define DEF_MELSEC_VER_2014		

#define DEF_1ST_EQ	//1호기
//#define  DEF_2ND_EQ	//2호기 
 
//설비 작업변에 따른 Define Macro
#define  DEF_SOURCE_SYSTEM
#undef	DEF_GATE_SYSTEM
///////////////////////////////////////////////////////////////

// Unit Type Define  
#undef DEF_PLATFORM_ARP_SYSTEM
#define DEF_PLATFORM_PREBONDER_SYSTEM
#undef DEF_PLATFORM_FINALBONDER_SYSTEM
//////////////////////////////////////////////////////////////

//설비 기능 Define Macro
#define DEF_USE_REVERSE_TAB_SIDE				//삭제 보류

#ifdef	DEF_1ST_EQ
	#define  DEF_USE_TURN_HANDLER					//Turn Handler 사용 여부
	#define  DEF_USE_TRANSFER_CENTER				//Transfer Center 사용 여부
	#define	 DEF_USE_TRANSFER_OUT_MULTI_AXIS		//Transfer Out Multi Axis 사용 여부 
#else
	#undef DEF_USE_TURN_HANDLER					//Turn Handler 사용 여부
	#undef DEF_USE_TRANSFER_CENTER				//Transfer Center 사용 여부
	#undef DEF_USE_TRANSFER_OUT_MULTI_AXIS		//Transfer Out Multi Axis 사용 여부 
#endif	

#undef DEF_USE_TRANSFER_DONT_MOVE_SIGNAL		//Transfer Center가 Panel를 가지고 있을때 앞설비 Unload Handler가 진입하지 못하도록 하는 Define, #if (DEF_LINE_NO >= DEF_LINE_NO_11TH)
#undef DEF_PROCESS_MC_FIRST						//Panel을 투입하는 설비일 경우에 사용(공정단위로 Panel이 최초로 투입되는 설비 ex)Loader. )
//_____________________________________________________



//여기서부터는 관련 데이터 설정, (ifdef은 되도록이면 사용하지 말것!!!)s
//Tab Mounter
const double DEF_TABMOUNTER_STANDARD_GAP = 77.0;
const double DEF_TABMOUNTER_SAFE_GAP = 77.0;	//@73.0;
const double DEF_TABMOUNTER_TOOL_SIZE = 73.0;
const double DEF_TABMOUNTER_COLLISION_GAP = DEF_TABMOUNTER_STANDARD_GAP - 0.5;
const double DEF_GATE_EQUIP_MARGIN = 20.0;
const double DEF_TABMOUNTER_SAFE_HEIGHT = 2.0;//100930.TabMark인식후 상승높이
//_____________________________________
//Aligner
const double C_TYPE_PANEL_SIZE_MIDDLE = 903.4;// 40" 
const double C_TYPE_PANEL_SIZE_LARGE = 1038.3;// 46"
const double D_TYPE_PANEL_SIZE_MIDDLE = 1181.2; // 52"
const double D_TYPE_PANEL_SIZE_LARGE = 1278.0; // 57"

const double COMPARE_PANEL_SIZE_MIDDLE = D_TYPE_PANEL_SIZE_MIDDLE;
const double COMPARE_PANEL_SIZE_LARGE = D_TYPE_PANEL_SIZE_LARGE;

// PanelAligner 회전중심사이의 거리 ////////////////////////////////////////
const double DEF_PANELALIGNER_DIST_CENTERTOEDGE = 75.0;		//PanelAligner 회전 중심에서 끝단까지 거리
const double DEF_PANELALIGNER_DIST_PROJECT = 270.0;			//Panel돌출량
const double DEF_PANELALIGNER_DIST_CENTERTOCENTER = 500.0;	//PanelAligner Pad중심간 거리

const double DEF_AIR_PLATE_LENGTH = 1300.0;					//Air부상 길이. 1320mm											
const double DEF_ALIGNER_WORK_DIST_TO_PLATE_CENTER = 450.0;	//PanelAligner 압착 위치의 부상 중심에서의 떨어진 거리

const double DEF_MARKDIST_FIDUCIALTOTAB = 0.8;				//FiducialMark와 TabMark간 거리
const double DEF_TABMARK_VISION_CENTER_OFFSET = 0.8;		//TabAlign시 TabMark중심이 화면 중심에서 떨어진 거리


//__________________________________________
//Align Camera
//170825 JSH.s
// const int DEF_CAMERACARRIER_SAFE_MARGIN = 142;
// const double DEF_INSPECTION_STANDARD_GAP = 145.0; //@130710.KKY.140일경우 충돌_______
// const double DEF_INSPECTION_SAFE_GAP = 70.0;
// const double DEF_INSPECTIOM_TOOL_SIZE = 120.0;
const double DEF_INSPECTION_STANDARD_GAP = 165.0;
const int DEF_CAMERACARRIER_SAFE_MARGIN = (int)DEF_INSPECTION_STANDARD_GAP; //170825 JSH 설계 변경
const double DEF_INSPECTION_SAFE_GAP = DEF_INSPECTION_STANDARD_GAP;
const double DEF_INSPECTIOM_TOOL_SIZE = 120.0;
const double DEF_INSPECTIOM_BONDING_CENTER_TO_ALIGN_CENTER = -68.0; //170826 JSH											
//170825 JSH.e
//__________________________________________________
//Tab Carrier
const double DEF_TABCARRIER_STANDARD_GAP = 80.0;
const double DEF_TABCARRIER_SAFE_GAP = 80.0;
const double DEF_TABCARRIER_TOOL_SIZE = 60.0;
#define	DEF_TABCARRIER_PICKUP_UNIT_INTERVAL		(DEF_TABMOUNTER_STANDARD_GAP > DEF_TABCARRIER_STANDARD_GAP ? DEF_TABMOUNTER_STANDARD_GAP : DEF_TABCARRIER_STANDARD_GAP)
const double DEF_TABCARRIER_COLLISION_GAP = DEF_TABCARRIER_PICKUP_UNIT_INTERVAL - 0.5;

const double DEF_TABCARRIER_SHIFT_Y = 20.5;	//@ TabCarrier 회전중심에서 Tip끝단까지 거리
//________________________________________________
//Etc.
const double DEF_DIVIDER_FOR_LOW_SPEED = 10.0; // 짧은 구간 이동 시, 저속 이동을 위한 나눗셈 인자.

//Panel Aligner Vac1		: 32" 이하, Vac1/2		: 46" 이하, Vac1/2/3	: 46" 초과
//Air 부상		Plate1		: 40" 미만, Plate1/2	: 40" 이상
const double DEF_PANEL_SIZE_32INCH		= 713.0;
const double DEF_PANEL_SIZE_40INCH		= 906.4;
const double DEF_PANEL_SIZE_46INCH		= 1041.3;
const double DEF_PANEL_SIZE_52INCH		= 1184.2;
const double DEF_PANEL_SIZE_57INCH		= 1272.8;
const double DEF_PANEL_SIZE_60INCH		= 1354.0;
const double DEF_PANEL_SIZE_70INCH		= 1586.7;
const double DEF_PANEL_SIZE_70INCH_Y	= 908.8;
const double DEF_PANEL_SIZE_78INCH		= 1743.0;		//150527 SJ_YSH
const double DEF_PANEL_SIZE_78INCH_Y	= 987.6;
const double DEF_PANEL_SIZE_82INCH		= 1650.0;		//@Size 다시입력해야 함
const double DEF_PANEL_SIZE_105INCH		= 2350.0;
const double DEF_PANEL_SIZE_105INCH_Y	= 1450.0;



///////////////////////////////////////////////////////////////////////
//


// 설비 버전 Define - 현재 Version 갱신일 : 20090814 ~ (탕정)
#define	DEF_VERSION		"ver.20170919"
///////////////////////////////////////////////////////////////
#	ifdef DEF_MIRROR_
#		ifdef DEF_SOURCE_SYSTEM
#			define DEF_VERSION_STRING DEF_VERSION"sm"
#		else
#			define DEF_VERSION_STRING DEF_VERSION"gm"
#		endif
#	else
#		ifdef DEF_SOURCE_SYSTEM
#			define DEF_VERSION_STRING DEF_VERSION"sn"
#		else
#			define DEF_VERSION_STRING DEF_VERSION"gn"
#		endif
#	endif
#endif // DEFSYSTEMCONFIG_H
