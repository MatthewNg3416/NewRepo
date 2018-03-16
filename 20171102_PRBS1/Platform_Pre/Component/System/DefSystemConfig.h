#ifndef DEFSYSTEMCONFIG_H
#define DEFSYSTEMCONFIG_H



//
// Setup �Ⱓ ��, Setup ���Ǹ� ���� �ʿ��� ��� ���� ���� Define.
//  ex) Safety Sensor ��� ���� ���� etc.
//#define DEF_SETUP

//170711 JSH.s
#ifdef DEF_SETUP
//#define DEF_SETUP_NONE_IN_LINE
#undef SETUP_NONE_IN_LINE
#endif
//170711 JSH.e

// ���� � ����忡 ���� Define Macro
//  : ����� �� ���� ������ �ٸ� �׸� ���� ó���� ���� Define.
//    ex.) Lamp ��� ���wlrma
#define DEF_TANGJUNG_SITE	// ���������
#undef DEF_SESL_SITE		// SESL
///////////////////////////////////////////////////////////////

// Mirror Line �� ���� Define Macro
#undef DEF_MIRROR_ //���� DEF_MIRROR_LINE ��ü
///////////////////////////////////////////////////////////////

#define DEF_MELSEC_VER_2014		

#define DEF_1ST_EQ	//1ȣ��
//#define  DEF_2ND_EQ	//2ȣ�� 
 
//���� �۾����� ���� Define Macro
#define  DEF_SOURCE_SYSTEM
#undef	DEF_GATE_SYSTEM
///////////////////////////////////////////////////////////////

// Unit Type Define  
#undef DEF_PLATFORM_ARP_SYSTEM
#define DEF_PLATFORM_PREBONDER_SYSTEM
#undef DEF_PLATFORM_FINALBONDER_SYSTEM
//////////////////////////////////////////////////////////////

//���� ��� Define Macro
#define DEF_USE_REVERSE_TAB_SIDE				//���� ����

#ifdef	DEF_1ST_EQ
	#define  DEF_USE_TURN_HANDLER					//Turn Handler ��� ����
	#define  DEF_USE_TRANSFER_CENTER				//Transfer Center ��� ����
	#define	 DEF_USE_TRANSFER_OUT_MULTI_AXIS		//Transfer Out Multi Axis ��� ���� 
#else
	#undef DEF_USE_TURN_HANDLER					//Turn Handler ��� ����
	#undef DEF_USE_TRANSFER_CENTER				//Transfer Center ��� ����
	#undef DEF_USE_TRANSFER_OUT_MULTI_AXIS		//Transfer Out Multi Axis ��� ���� 
#endif	

#undef DEF_USE_TRANSFER_DONT_MOVE_SIGNAL		//Transfer Center�� Panel�� ������ ������ �ռ��� Unload Handler�� �������� ���ϵ��� �ϴ� Define, #if (DEF_LINE_NO >= DEF_LINE_NO_11TH)
#undef DEF_PROCESS_MC_FIRST						//Panel�� �����ϴ� ������ ��쿡 ���(���������� Panel�� ���ʷ� ���ԵǴ� ���� ex)Loader. )
//_____________________________________________________



//���⼭���ʹ� ���� ������ ����, (ifdef�� �ǵ����̸� ������� ����!!!)s
//Tab Mounter
const double DEF_TABMOUNTER_STANDARD_GAP = 77.0;
const double DEF_TABMOUNTER_SAFE_GAP = 77.0;	//@73.0;
const double DEF_TABMOUNTER_TOOL_SIZE = 73.0;
const double DEF_TABMOUNTER_COLLISION_GAP = DEF_TABMOUNTER_STANDARD_GAP - 0.5;
const double DEF_GATE_EQUIP_MARGIN = 20.0;
const double DEF_TABMOUNTER_SAFE_HEIGHT = 2.0;//100930.TabMark�ν��� ��³���
//_____________________________________
//Aligner
const double C_TYPE_PANEL_SIZE_MIDDLE = 903.4;// 40" 
const double C_TYPE_PANEL_SIZE_LARGE = 1038.3;// 46"
const double D_TYPE_PANEL_SIZE_MIDDLE = 1181.2; // 52"
const double D_TYPE_PANEL_SIZE_LARGE = 1278.0; // 57"

const double COMPARE_PANEL_SIZE_MIDDLE = D_TYPE_PANEL_SIZE_MIDDLE;
const double COMPARE_PANEL_SIZE_LARGE = D_TYPE_PANEL_SIZE_LARGE;

// PanelAligner ȸ���߽ɻ����� �Ÿ� ////////////////////////////////////////
const double DEF_PANELALIGNER_DIST_CENTERTOEDGE = 75.0;		//PanelAligner ȸ�� �߽ɿ��� ���ܱ��� �Ÿ�
const double DEF_PANELALIGNER_DIST_PROJECT = 270.0;			//Panel���ⷮ
const double DEF_PANELALIGNER_DIST_CENTERTOCENTER = 500.0;	//PanelAligner Pad�߽ɰ� �Ÿ�

const double DEF_AIR_PLATE_LENGTH = 1300.0;					//Air�λ� ����. 1320mm											
const double DEF_ALIGNER_WORK_DIST_TO_PLATE_CENTER = 450.0;	//PanelAligner ���� ��ġ�� �λ� �߽ɿ����� ������ �Ÿ�

const double DEF_MARKDIST_FIDUCIALTOTAB = 0.8;				//FiducialMark�� TabMark�� �Ÿ�
const double DEF_TABMARK_VISION_CENTER_OFFSET = 0.8;		//TabAlign�� TabMark�߽��� ȭ�� �߽ɿ��� ������ �Ÿ�


//__________________________________________
//Align Camera
//170825 JSH.s
// const int DEF_CAMERACARRIER_SAFE_MARGIN = 142;
// const double DEF_INSPECTION_STANDARD_GAP = 145.0; //@130710.KKY.140�ϰ�� �浹_______
// const double DEF_INSPECTION_SAFE_GAP = 70.0;
// const double DEF_INSPECTIOM_TOOL_SIZE = 120.0;
const double DEF_INSPECTION_STANDARD_GAP = 165.0;
const int DEF_CAMERACARRIER_SAFE_MARGIN = (int)DEF_INSPECTION_STANDARD_GAP; //170825 JSH ���� ����
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

const double DEF_TABCARRIER_SHIFT_Y = 20.5;	//@ TabCarrier ȸ���߽ɿ��� Tip���ܱ��� �Ÿ�
//________________________________________________
//Etc.
const double DEF_DIVIDER_FOR_LOW_SPEED = 10.0; // ª�� ���� �̵� ��, ���� �̵��� ���� ������ ����.

//Panel Aligner Vac1		: 32" ����, Vac1/2		: 46" ����, Vac1/2/3	: 46" �ʰ�
//Air �λ�		Plate1		: 40" �̸�, Plate1/2	: 40" �̻�
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
const double DEF_PANEL_SIZE_82INCH		= 1650.0;		//@Size �ٽ��Է��ؾ� ��
const double DEF_PANEL_SIZE_105INCH		= 2350.0;
const double DEF_PANEL_SIZE_105INCH_Y	= 1450.0;



///////////////////////////////////////////////////////////////////////
//


// ���� ���� Define - ���� Version ������ : 20090814 ~ (����)
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
