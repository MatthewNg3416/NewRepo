#ifndef USEIO_H
#define USEIO_H

//======================== I/O define format ==============================
//  1. 도면의 function을 기본으로 함 (주의: 16진수 사용)
//  2. 단어와 단어 사이에는 '_'를 사용 함
//  3. Spare의 경우 뒤에 '_'와 No를 붙임.//  4. 입력부:
//     . Function 이름 앞에 IN_을 붙여 출력과 구분 함.
//     . Function 내용에 센서는 Detect
//     . Switch 의 경우 SW로 축약 함.
//  5. 출력부
//     . Function의 Sol은 생략 함
//     . 
//==========================================================================

const int INPUT_DUMMY								= 1999;
const int OUTPUT_DUMMY								= 2999;

#ifdef DEF_SOURCE_SYSTEM
//===================================================================

//==========================================================================
// Input
//==========================================================================
//______________________
const int IN_SPARE_1000							= 1000; // X000  
const int IN_E_STOP_RELAY_CHECK					= 1001; // X001  //170701 JSH 미사용
const int IN_E_STOP_RELAY_CHECK2				= 1002; // X002  //170701 JSH 미사용
const int IN_C_BOX_GAS_DETECT					= 1003; // X003  
const int IN_E_STOP_RELAY_CHECK3				= 1004; // X004  //170701 JSH 미사용
const int IN_SPARE_1005							= 1005; // X005  
const int IN_OVER_HEAT_DETECT_60				= 1006; // X006  
const int IN_OVER_HEAT_DETECT_80				= 1007; // X007

const int IN_FRONT_LOCAL_OVERHEAT				= 1008; // X008
const int IN_REAR_LOCAL_OVERHEAT				= 1009; // X009  
const int IN_C_BOX_VENT_FAN						= 1010; // X00A  
const int IN_C_BOX_INHALATION_FAN				= 1011; // X00B  
const int IN_C_BOX_SSR_VENT_FAN					= 1012; // X00C  
const int IN_C_BOX_SSR_INHALATION_FAN			= 1013; // X00D  
const int IN_C_BOX_PANEL_TEMP					= 1014; // X00E  
const int IN_C_BOX_LIGHT_ON						= 1015; // X00F  
//______________________

//______________________
const int IN_MAIN_AIR1							= 1016; // X010  
const int IN_MAIN_VACUUM1						= 1017;	// X011
const int IN_SPARE_1018							= 1018; // X012  
const int IN_SPARE_1019							= 1019; // X013  
const int IN_SPARE_1020							= 1020; // X014  
const int IN_SPARE_1021							= 1021; // X015  
const int IN_SPARE_1022							= 1022; // X016  
const int IN_SPARE_1023							= 1023; // X017  

//const int IN_TURNBASE_AIRRAIL_FOR1				= 1024;	// X0184
const int IN_SPARE_1024							= 1024;	// X018
const int IN_SPARE_1025							= 1025;	// X019
const int IN_SPARE_1026							= 1026;	// X01A
const int IN_SPARE_1027							= 1027;	// X01B
const int IN_TRANSFERIN_AIRRAIL_FOR1			= 1028; // X01C		//@신규추가
const int IN_TRANSFERIN_AIRRAIL_BACK1			= 1029; // X01D		//@신규추가
const int IN_TRANSFERIN_AIRRAIL_FOR2			= 1030; // X01E		//@신규추가
const int IN_TRANSFERIN_AIRRAIL_BACK2			= 1031; // X01F		//@신규추가
//______________________

//______________________
const int IN_STAGE_CELL_DETECT					= 1032;	// X020
const int IN_STAGE_VACUUM1						= 1033;	// X021
const int IN_STAGE_VACUUM2						= 1034;	// X022
const int IN_STAGE_VACUUM3						= 1035;	// X023
const int IN_PLATE_AIR_VACUUM1_ON				= 1036; // X024
const int IN_PLATE_AIR_LEVITATION1_ON			= 1037; // X025
const int IN_PLATE_AIR_VACUUM2_ON				= 1038;	// X026
const int IN_PLATE_AIR_LEVITATION2_ON			= 1039;	// X027

const int IN_TRANSFER_IN_1_UP					= 1040;	// X028
const int IN_TRANSFER_IN_1_DOWN					= 1041;	// X029
const int IN_TRANSFER_IN_2_UP					= 1042;	// X02A
const int IN_TRANSFER_IN_2_DOWN					= 1043;	// X02B
const int IN_TRANSFER_IN_VACUUM1				= 1044;	// X02C
const int IN_TRANSFER_IN_CELL_DETECT			= 1045;	// X02D
const int IN_PANEL_DETECT_WORKING				= 1046;	// X02E
const int IN_PANEL_DETECT_LOADING				= 1047;	// X02F
//______________________

//______________________
const int IN_TRANSFER_OUT_1_UP					= 1048;	// X030
const int IN_TRANSFER_OUT_1_DOWN				= 1049;	// X031
const int IN_TRANSFER_OUT_2_UP					= 1050;	// X032
const int IN_TRANSFER_OUT_2_DOWN				= 1051;	// X033
const int IN_TRANSFER_OUT_VACUUM1				= 1052;	// X034
const int IN_TRANSFER_OUT_CELL_DETECT			= 1053;	// X035
const int IN_TRANSFER_IN_VACUUM2				= 1054; // X036
const int IN_TRANSFER_OUT_VACUUM2				= 1055;	// X037

const int IN_TRANSFER_CENTER_1_UP				= 1056;	// X038
const int IN_TRANSFER_CENTER_1_DOWN				= 1057;	// X039
const int IN_TRANSFER_CENTER_2_UP				= 1058;	// X03A
const int IN_TRANSFER_CENTER_2_DOWN				= 1059;	// X03B
const int IN_TRANSFER_CENTER_CELL_DETECT		= 1060;	// X03C
const int IN_SPOT_CLEANER_BLOW_ON				= 1061;	// X03D
const int IN_PLATE_AIR_VACUUM3_ON				= 1062; // X03E
const int IN_PLATE_AIR_VACUUM4_ON				= 1063; // X03F
//______________________

//______________________
const int IN_SAFETY_KEY1_AUTO					= 1064; // X040
const int IN_SAFETY_KEY2_AUTO					= 1065; // X041
const int IN_SAFETY_KEY_F_MATERIAL				= 1066; // X042
const int IN_SAFETY_KEY_R_MATERIAL				= 1067; // X043
const int IN_TRANSFER_CENTER_VACUUM1			= 1068; // X044
const int IN_TRANSFER_CENTER_VACUUM2			= 1069; // X045
const int IN_SPARE_1070							= 1070; // X046
const int IN_SPARE_1071							= 1071; // X047


const int IN_TURN_HANDLER_CELL_DETECT			= 1072; // X048
const int IN_TURN_HANDLER_VACUUM1				= 1073; // X049
const int IN_TURN_HANDLER_VACUUM2				= 1074; // X04A
const int IN_TURN_HANDLER_VACUUM3				= 1075; // X04B
const int IN_TURN_HANDLER_VACUUM4				= 1076; // X04A
const int IN_SPARE_1077							= 1077; // X04B
const int IN_SPARE_1078							= 1078; // X04E
const int IN_IONIZER_STATE_ONOFF				= 1079; // X04F
//______________________

//______________________
const int IN_FRONT_START_SW						= 1080; // X050
const int IN_FRONT_STEP_STOP_SW 				= 1081; // X051k
const int IN_FRONT_RESET_SW						= 1082; // X052
const int IN_FRONT_JOG_L_SW						= 1083; // X053
const int IN_FRONT_JOG_R_SW						= 1084; // X054
const int IN_FRONT_JOG_F_SW						= 1085; // X055
const int IN_FRONT_JOG_B_SW						= 1086; // X056
const int IN_FRONT_JOG_CW_SW					= 1087; // X057

const int IN_FRONT_JOG_CCW_SW					= 1088; // X058
const int IN_FRONT_JOG_U_SW						= 1089; // X059
const int IN_FRONT_JOG_D_SW						= 1090; // X05A
const int IN_FRONT_JOG_E_STOP					= 1091; // X05B
const int IN_SPARE_1092							= 1092; // X05C
const int IN_SPARE_1093							= 1093; // X05D
const int IN_SPARE_1094							= 1094; // X05E
const int IN_FRONT_JOG_DETECT					= 1095; // X05F
//______________________

//______________________
const int IN_REAR_START_SW						= 1096; // X060  
const int IN_REAR_STEP_STOP_SW					= 1097; // X061  
const int IN_REAR_RESET_SW						= 1098; // X062  
const int IN_REAR_JOG_L_SW						= 1099; // X063  
const int IN_REAR_JOG_R_SW						= 1100; // X064  
const int IN_REAR_JOG_F_SW						= 1101; // X065  
const int IN_REAR_JOG_B_SW						= 1102; // X066  
const int IN_REAR_JOG_CW_SW						= 1103; // X067  

const int IN_REAR_JOG_CCW_SW					= 1104; // X068  
const int IN_REAR_JOG_U_SW						= 1105; // X069  
const int IN_REAR_JOG_D_SW						= 1106; // X06A  
const int IN_REAR_JOG_E_STOP					= 1107; // X06B  
const int IN_SPARE_1108							= 1108; // X06C  
const int IN_SPARE_1109							= 1109; // X06D  
const int IN_SPARE_1110							= 1110; // X06E  
const int IN_REAR_JOG_DETECT					= 1111; // X06F
//______________________

//______________________
const int IN_FRONT_DOOR1 						= 1112; // X070
const int IN_FRONT_DOOR2 						= 1113; // X071
const int IN_FRONT_DOOR3 						= 1114; // X072
const int IN_FRONT_DOOR4 						= 1115; // X073
const int IN_FRONT_DOOR5 						= 1116; // X074
const int IN_FRONT_DOOR6 						= 1117; // X075
const int IN_FRONT_MATERIAL_DOOR_LOCK			= 1118; // X076 //150326 SJ_YSH Add.
const int IN_FRONT_DOOR_LOCK					= 1119; // X077 //150326 SJ_YSH Add.


const int IN_REAR_DOOR1							= 1120; // X078  
const int IN_REAR_DOOR2							= 1121; // X079  
const int IN_REAR_DOOR3							= 1122; // X07A  
const int IN_REAR_DOOR4							= 1123; // X07B  
const int IN_REAR_DOOR5							= 1124; // X07C
const int IN_REAR_DOOR6							= 1125; // X07D  
const int IN_REAR_MATERIAL_DOOR_LOCK			= 1126; // X07E //150326 SJ_YSH Add.
const int IN_REAR_DOOR_LOCK						= 1127; // X07F //150326 SJ_YSH Add.
//______________________

//______________________
const int IN_INTERFACE_00 						= 1128; // X080  
const int IN_INTERFACE_01 						= 1129; // X081  
const int IN_INTERFACE_02 						= 1130; // X082  
const int IN_INTERFACE_03 						= 1131; // X083  
const int IN_INTERFACE_04 						= 1132; // X084  
const int IN_INTERFACE_05 						= 1133; // X085  
const int IN_INTERFACE_06 						= 1134; // X086  
const int IN_INTERFACE_07 						= 1135; // X087  


const int IN_INTERFACE_08 						= 1136; // X088  
const int IN_INTERFACE_09 						= 1137; // X089  
const int IN_INTERFACE_10 						= 1138; // X08A  
const int IN_INTERFACE_11 						= 1139; // X08B  
const int IN_INTERFACE_12 						= 1140; // X08C  
const int IN_INTERFACE_13 						= 1141; // X08D  
const int IN_INTERFACE_14 						= 1142; // X08E  
const int IN_INTERFACE_15 						= 1143; // X08F  
//______________________

//______________________
const int IN_TABMOUNTER1_THERMOSTAT_01 			= 1144; // X090  
const int IN_TABMOUNTER1_THERMOSTAT_02 			= 1145; // X091  
const int IN_TABMOUNTER1_THERMOSTAT_03 			= 1146; // X092  
const int IN_TABMOUNTER1_THERMOSTAT_04 			= 1147; // X093  
const int IN_TABMOUNTER1_HEATER_WIRE_CUT_01 	= 1148; // X094  
const int IN_TABMOUNTER1_HEATER_WIRE_CUT_02 	= 1149; // X095  
const int IN_TABMOUNTER1_HEATER_WIRE_CUT_03 	= 1150; // X096  
const int IN_TABMOUNTER1_HEATER_WIRE_CUT_04 	= 1151; // X097  

const int IN_TABMOUNTER1_PRESS1_1ST_UP			= 1152; // X098  
const int IN_TABMOUNTER1_PRESS2_1ST_UP			= 1153; // X099  
const int IN_TABMOUNTER1_PRESS3_1ST_UP			= 1154; // X09A  
const int IN_TABMOUNTER1_PRESS4_1ST_UP			= 1155; // X09B  
const int IN_TABMOUNTER1_PRESS1_1ST_DOWN		= 1156; // X09C  
const int IN_TABMOUNTER1_PRESS2_1ST_DOWN		= 1157; // X09D  
const int IN_TABMOUNTER1_PRESS3_1ST_DOWN		= 1158; // X09E  
const int IN_TABMOUNTER1_PRESS4_1ST_DOWN		= 1159; // X09F  
//______________________


//______________________
const int IN_TABMOUNTER1_THERMOSTAT_05 			= 1160; // X100  
const int IN_TABMOUNTER1_THERMOSTAT_06 			= 1161; // X101  
const int IN_TABMOUNTER1_THERMOSTAT_07 			= 1162; // X102  
const int IN_TABMOUNTER1_THERMOSTAT_08 			= 1163; // X103  
const int IN_TABMOUNTER1_HEATER_WIRE_CUT_05 	= 1164; // X104  
const int IN_TABMOUNTER1_HEATER_WIRE_CUT_06 	= 1165; // X105  
const int IN_TABMOUNTER1_HEATER_WIRE_CUT_07 	= 1166; // X106  
const int IN_TABMOUNTER1_HEATER_WIRE_CUT_08 	= 1167; // X107  

const int IN_TABMOUNTER1_PRESS5_1ST_UP			= 1168; // X108  
const int IN_TABMOUNTER1_PRESS6_1ST_UP			= 1169; // X109  
const int IN_TABMOUNTER1_PRESS7_1ST_UP			= 1170; // X10A  
const int IN_TABMOUNTER1_PRESS8_1ST_UP			= 1171; // X10B  
const int IN_TABMOUNTER1_PRESS5_1ST_DOWN		= 1172; // X10C  
const int IN_TABMOUNTER1_PRESS6_1ST_DOWN		= 1173; // X10D  
const int IN_TABMOUNTER1_PRESS7_1ST_DOWN		= 1174; // X10E  
const int IN_TABMOUNTER1_PRESS8_1ST_DOWN		= 1175; // X10F  
//______________________

//______________________
const int IN_TABMOUNTER1_PRESSURE1 				= 1176; // X110
const int IN_TABMOUNTER1_PRESSURE2 				= 1177; // X111
const int IN_TABMOUNTER1_PRESSURE3 				= 1178; // X112
const int IN_TABMOUNTER1_PRESSURE4 				= 1179; // X113
const int IN_TABMOUNTER1_VACUUM1 				= 1180; // X114
const int IN_TABMOUNTER1_VACUUM2 				= 1181; // X115
const int IN_TABMOUNTER1_VACUUM3 				= 1182; // X116
const int IN_TABMOUNTER1_VACUUM4 				= 1183; // X117

const int IN_TABMOUNTER1_PANEL_PUSHER			= 1184; // X118
const int IN_TABMOUNTER2_PANEL_PUSHER			= 1185; // X119
const int IN_TABMOUNTER3_PANEL_PUSHER			= 1186; // X11A
const int IN_TABMOUNTER4_PANEL_PUSHER			= 1187; // X11B
const int IN_SPARE_1188							= 1188; // X11C
const int IN_SPARE_1189							= 1189; // X11D
const int IN_SPARE_1190							= 1190; // X11E
const int IN_SPARE_1191							= 1191; // X11F

//______________________

//______________________
const int IN_TABMOUNTER1_PRESSURE5 				= 1192; // X120  
const int IN_TABMOUNTER1_PRESSURE6 				= 1193; // X121  
const int IN_TABMOUNTER1_PRESSURE7 				= 1194; // X122  
const int IN_TABMOUNTER1_PRESSURE8 				= 1195; // X123  
const int IN_TABMOUNTER1_VACUUM5 				= 1196; // X124  
const int IN_TABMOUNTER1_VACUUM6 				= 1197; // X125  
const int IN_TABMOUNTER1_VACUUM7 				= 1198; // X126  
const int IN_TABMOUNTER1_VACUUM8 				= 1199; // X127  


const int IN_RTABMOUNTER1_PANEL_PUSHER			= 1200; // X128
const int IN_RTABMOUNTER2_PANEL_PUSHER			= 1201; // X129
const int IN_RTABMOUNTER3_PANEL_PUSHER			= 1202; // X12A
const int IN_RTABMOUNTER4_PANEL_PUSHER			= 1203; // X12B
const int IN_SPARE_1204							= 1204; // X12C
const int IN_SPARE_1205							= 1205; // X12D
const int IN_SPARE_1206							= 1206; // X12E
const int IN_SPARE_1207							= 1207; // X12F

//______________________


//______________________
const int IN_TABCARRIER_UP						= 1208; // X130
const int IN_TABCARRIER_DOWN					= 1209; // X131
const int IN_TABCARRIER_FORWARD					= 1210; // X132
const int IN_TABCARRIER_BACKWARD				= 1211; // X133
const int IN_TABCARRIER1_VAC_ON_OFF				= 1212; // X134 
const int IN_TABCARRIER2_VAC_ON_OFF				= 1213; // X135 
const int IN_TABCARRIER3_VAC_ON_OFF				= 1214; // X136 
const int IN_TABCARRIER4_VAC_ON_OFF				= 1215; // X137 

const int IN_TABPRESS1_WEIGHT_TENSION_UP		= 1216; // X138 
const int IN_TABPRESS1_WEIGHT_TENSION_DOWN		= 1217; // X139 
const int IN_TABPRESS1_RECOVERY_SPROCKET_OPEN	= 1218; // X13A 
const int IN_TABPRESS1_RECOVERY_SPROCKET_CLOSE	= 1219; // X13B 
const int IN_TABPRESS1_PRESS_RFID_ALARM			= 1220; // X13C
const int IN_TABPRESS1_PRESS_MOLD_DETECT		= 1221; // X13D
const int IN_TABPRESS1_PRESS_VACUUM_ON			= 1222; // X13E
const int IN_TABPRESS1_PRESS_DOWN_CHECK			= 1223; // X13F
//____________________

//____________________
const int IN_TABPRESS1_SUPPLIER_LARGE_DETECT		= 1224; // X140
const int IN_TABPRESS1_SUPPLIER_MIDDLE_DETECT		= 1225; // X141
const int IN_TABPRESS1_SUPPLIER_SMALL_DETECT		= 1226; // X142
const int IN_TABPRESS1_RAIL_UNFOLD_DETECT			= 1227; // X143
const int IN_TABPRESS1_RECOVERY_PROTECT_ARM_DOWN	= 1228; // X144
const int IN_TABPRESS1_RECOVERY_PROTECT_ARM_UP		= 1229; // X145
const int IN_FRONT_RECOVERY_BOX						= 1230; // X146
const int IN_SPARE_1231								= 1231; // X147

const int IN_TABPRESS1_BADTAB_TCP_DETECT			= 1232; // X148
const int IN_TABPRESS1_BADTAB_COF_DETECT			= 1233; // X149
const int IN_TABCARRIER_UP2							= 1234; // X14A
const int IN_TABCARRIER_DOWN2						= 1235; // X14B
const int IN_PREALIGN_CAM1_1_EXPAND					= 1236; // X14C
const int IN_PREALIGN_CAM1_1_SHRINK					= 1237; // X14D
const int IN_PREALIGN_CAM1_2_EXPAND					= 1238;	// X14E
const int IN_PREALIGN_CAM1_2_SHRINK					= 1239;	// X14F
//____________________

//____________________
const int IN_R_TABCARRIER_UP						= 1240; // X150
const int IN_R_TABCARRIER_DOWN						= 1241; // X151
const int IN_R_TABCARRIER_FORWARD					= 1242; // X152
const int IN_R_TABCARRIER_BACKWARD					= 1243; // X153
const int IN_R_TABCARRIER1_VAC_ON_OFF				= 1244; // X154 
const int IN_R_TABCARRIER2_VAC_ON_OFF				= 1245; // X155 
const int IN_R_TABCARRIER3_VAC_ON_OFF				= 1246; // X156 
const int IN_R_TABCARRIER4_VAC_ON_OFF				= 1247; // X157 

const int IN_TABPRESS2_WEIGHT_TENSION_UP			= 1248; // X158 
const int IN_TABPRESS2_WEIGHT_TENSION_DOWN			= 1249; // X159 
const int IN_TABPRESS2_RECOVERY_SPROCKET_OPEN		= 1250; // X15A 
const int IN_TABPRESS2_RECOVERY_SPROCKET_CLOSE		= 1251; // X15B 
const int IN_TABPRESS2_PRESS_RFID_ALARM				= 1252; // X15C
const int IN_TABPRESS2_PRESS_MOLD_DETECT			= 1253; // X15D
const int IN_TABPRESS2_PRESS_VACUUM_ON				= 1254; // X15E
const int IN_TABPRESS2_PRESS_DOWN_CHECK				= 1255; // X15F
//____________________

//____________________
const int IN_TABPRESS2_SUPPLIER_LARGE_DETECT		= 1256; // X160
const int IN_TABPRESS2_SUPPLIER_MIDDLE_DETECT		= 1257; // X161
const int IN_TABPRESS2_SUPPLIER_SMALL_DETECT		= 1258; // X162
const int IN_TABPRESS2_RAIL_UNFOLD_DETECT			= 1259; // X163
const int IN_TABPRESS2_RECOVERY_PROTECT_ARM_DOWN	= 1260; // X164
const int IN_TABPRESS2_RECOVERY_PROTECT_ARM_UP		= 1261; // X165
const int IN_REAR_RECOVERY_BOX						= 1262; // X166
const int IN_SPARE_1263								= 1263; // X167

const int IN_TABPRESS2_BADTAB_TCP_DETECT			= 1264; // X168
const int IN_TABPRESS2_BADTAB_COF_DETECT			= 1265; // X169
const int IN_R_TABCARRIER_UP2						= 1266; // X16A
const int IN_R_TABCARRIER_DOWN2						= 1267; // X16B
const int IN_PREALIGN_CAM2_1_EXPAND					= 1268; // X16C
const int IN_PREALIGN_CAM2_1_SHRINK					= 1269; // X16D
const int IN_PREALIGN_CAM2_2_EXPAND					= 1270; // X16E
const int IN_PREALIGN_CAM2_2_SHRINK					= 1271; // X16F
//____________________

//==========================================================================
// Output
//==========================================================================

//____________________
const int OUT_SPARE_2000								= 2000; // Y000  
const int OUT_TOWER_LAMP_RED							= 2001; // Y001  
const int OUT_TOWER_LAMP_YELLOW							= 2002; // Y002  
const int OUT_TOWER_LAMP_GREEN							= 2003; // Y003  
const int OUT_BUZZER_1 									= 2004; // Y004  
const int OUT_BUZZER_2 									= 2005; // Y005  
const int OUT_BUZZER_3 									= 2006; // Y006  
const int OUT_BUZZER_4 									= 2007; // Y007  

const int OUT_SPARE_2008								= 2008; // Y008  
const int OUT_SPARE_2009								= 2009; // Y009  
const int OUT_SPARE_2010								= 2010; // Y00A  
const int OUT_SPARE_2011								= 2011; // Y00B  
//170426 JSH.s
const int OUT_POWER_RESET								= 2012; // Y00C  
const int OUT_POWER_RESET_SECOND						= 2013; // Y00D  
const int OUT_SERVO_CONTROL_POWER_OFF					= 2014; // Y00E  
const int OUT_HEATER_POWER_RESET						= 2015; // Y00F  
//170426 JSh.e
//____________________

//____________________
const int OUT_STAGE_VACUUM1								= 2016; // Y010  
const int OUT_STAGE_VACUUM2								= 2017; // Y011  
const int OUT_STAGE_VACUUM3								= 2018; // Y012  
const int OUT_STAGE_BLOW								= 2019; // Y013  
const int OUT_PLATE_AIR_LEVITATION1_ON					= 2020; // Y014 
const int OUT_PLATE_AIR_VACUUM1_ON						= 2021; // Y015
const int OUT_PLATE_AIR_LEVITATION2_ON					= 2022; // Y016
const int OUT_PLATE_AIR_VACUUM2_ON						= 2023; // Y017  

const int OUT_TRANSFER_IN_UP							= 2024; // Y018  
const int OUT_TRANSFER_IN_DOWN							= 2025; // Y019  
const int OUT_TRANSFER_IN_VACUUM1						= 2026; // Y01A
const int OUT_TRANSFER_IN_VACUUM2						= 2027; // Y01B
const int OUT_TRANSFERIN_AIRRAIL_FOR1					= 2028; // Y01C		//@신규추가
const int OUT_TRANSFERIN_AIRRAIL_BACK1					= 2029; // Y01D		//@신규추가
const int OUT_TRANSFERIN_AIRRAIL_FOR2				= 2030; // Y01E		//@신규추가
const int OUT_TRANSFERIN_AIRRAIL_BACK2				= 2031; // Y01F		//@신규추가

//____________________

//____________________
const int OUT_TRANSFER_OUT_UP							= 2032; // Y020
const int OUT_TRANSFER_OUT_DOWN							= 2033; // Y021
const int OUT_TRANSFER_OUT_VACUUM1						= 2034; // Y022
const int OUT_TRANSFER_OUT_VACUUM2						= 2035; // Y023
const int OUT_PLATE_AIR_VACUUM3_ON					= 2036; // Y024
const int OUT_PLATE_AIR_VACUUM4_ON					= 2037; // Y025
const int OUT_REAR_SAFETY_KEY_UNLOCK					= 2038; // Y026
const int OUT_SPARE_2039								= 2039; // Y027

const int OUT_TRANSFER_IN_LOW_VACUUM1					= 2040; // Y028  //SJ_YYK 150109 저진공..추가..
const int OUT_TRANSFER_IN_LOW_VACUUM2					= 2041; // Y029
const int OUT_TRANSFER_OUT_LOW_VACUUM1					= 2042; // Y02A
const int OUT_TRANSFER_OUT_LOW_VACUUM2					= 2043; // Y02B
const int OUT_FRONT_SAFETY_KEY_UNLOCK					= 2044; // Y02C //150326 SJ_YSH Add.
const int OUT_FRONT_MATERIAL_KEY_UNLOCK					= 2045; // Y02D //150326 SJ_YSH Add.
const int OUT_SPOT_CLEANER_BLOW_ON						= 2046; // Y02E //150326 SJ_YSH Add.
const int OUT_REAR_MATERIAL_KEY_UNLOCK					= 2047; // Y02F //150326 SJ_YSH Add.
/*/
const int OUT_DOOR_LOCK_OPENCLOSE						= 2036; // Y024
const int OUT_FRONT_MATERIALCHANGE_DOOR_LOCK			= 2037;
const int OUT_REAR_MATERIALCHANGE_DOOR_LOCK				= 2038;
const int OUT_SPARE_2039								= 2039; // Y027

//const int OUT_PANEL_PUSHER_UPDOWN						= 2040; // Y028
const int OUT_PANEL_PUSHER_UP							= 2040; // Y028
const int OUT_PANEL_PUSHER_DOWN							= 2041; // Y029
const int OUT_SPARE_2042								= 2042; // Y02A
const int OUT_SPARE_2043								= 2043; // Y02B
const int OUT_PLATE_AIR_LEVITATION3_ON					= 2044; // Y02C
const int OUT_PLATE_AIR_VACUUM3_ON						= 2045; // Y02D
const int OUT_PLATE_AIR_LEVITATION4_ON					= 2046; // Y02E
const int OUT_PLATE_AIR_VACUUM4_ON						= 2047; // Y02F
/*/
//____________________

//____________________
const int OUT_TOUCH_MONITOR_CHANGE						= 2048; // Y030
const int OUT_FRONT_DOOR_LOCK_OPEN						= 2049; // Y031
const int OUT_REAR_DOOR_LOCK_OPEN						= 2050; // Y032
const int OUT_REAR_METERIAL_DOOR_LOCK_OPEN				= 2051; // Y033  // DUMMY
const int OUT_FRONT_METERIAL_DOOR_LOCK_OPEN				= 2052; // Y034  // DUMMY
const int OUT_TOUCH_MONITOR_CHANGE_R					= 2053; // Y035  // DUMMY
const int OUT_TOUCH_MONITOR_CHANGE_PRESS				= 2054; // Y036  // DUMMY 
const int OUT_TOUCH_MONITOR_CHANGE_PRESS_R				= 2055; // Y037  // DUMMY

const int OUT_FRONT_START_SW_LAMP						= 2056; // Y038  
const int OUT_FRONT_STEP_STOP_SW_LAMP					= 2057; // Y039  
const int OUT_FRONT_RESET_SW_LAMP						= 2058; // Y03A  
const int OUT_SPARE_2059								= 2059; // Y03B
const int OUT_REAR_START_SW_LAMP						= 2060; // Y03C  
const int OUT_REAR_STEP_STOP_SW_LAMP					= 2061; // Y03D  
const int OUT_REAR_RESET_SW_LAMP						= 2062; // Y03E  
const int OUT_SPARE_2063								= 2063; // Y03F  
//____________________


//____________________
const int OUT_INTERFACE_00 									= 2064; // Y040 
const int OUT_INTERFACE_01 									= 2065; // Y041 
const int OUT_INTERFACE_02 									= 2066; // Y042 
const int OUT_INTERFACE_03 									= 2067; // Y043 
const int OUT_INTERFACE_04 									= 2068; // Y044 
const int OUT_INTERFACE_05 									= 2069; // Y045 
const int OUT_INTERFACE_06 									= 2070; // Y046 
const int OUT_INTERFACE_07 									= 2071; // Y047 

const int OUT_INTERFACE_08									= 2072; // Y048 
const int OUT_INTERFACE_09 									= 2073; // Y049 
const int OUT_INTERFACE_10 									= 2074; // Y04A 
const int OUT_INTERFACE_11 									= 2075; // Y04B 
const int OUT_INTERFACE_12 									= 2076; // Y04C 
const int OUT_INTERFACE_13 									= 2077; // Y04D 
const int OUT_INTERFACE_14 									= 2078; // Y04E 
const int OUT_INTERFACE_15 									= 2079; // Y04F 
//____________________

//____________________
const int OUT_INSPECTION_CAM1_EXPAND						= 2080; // Y050
const int OUT_INSPECTION_CAM1_SHRINK						= 2081; // Y051 
const int OUT_INSPECTION_CAM2_EXPAND						= 2082; // Y052 
const int OUT_INSPECTION_CAM2_SHRINK						= 2083; // Y053 
const int OUT_INSPECTION_CAM3_EXPAND						= 2084; // Y054 
const int OUT_INSPECTION_CAM3_SHRINK						= 2085; // Y055 
const int OUT_INSPECTION_CAM4_EXPAND						= 2086; // Y056 
const int OUT_INSPECTION_CAM4_SHRINK						= 2087; // Y057

const int OUT_R_INSPECTION_CAM1_EXPAND						= 2088; // Y058
const int OUT_R_INSPECTION_CAM1_SHRINK						= 2089; // Y059 
const int OUT_R_INSPECTION_CAM2_EXPAND						= 2090; // Y05A 
const int OUT_R_INSPECTION_CAM2_SHRINK						= 2091; // Y05B 
const int OUT_R_INSPECTION_CAM3_EXPAND						= 2092; // Y05C 
const int OUT_R_INSPECTION_CAM3_SHRINK						= 2093; // Y05D 
const int OUT_R_INSPECTION_CAM4_EXPAND						= 2094; // Y05E 
const int OUT_R_INSPECTION_CAM4_SHRINK						= 2095; // Y05F
//____________________

//____________________
const int OUT_TABMOUNTER1_PRESS1_1ST_DOWN					= 2096; // Y060  
const int OUT_TABMOUNTER1_PRESS2_1ST_DOWN					= 2097; // Y061  
const int OUT_TABMOUNTER1_PRESS3_1ST_DOWN					= 2098; // Y062  
const int OUT_TABMOUNTER1_PRESS4_1ST_DOWN					= 2099; // Y063  
const int OUT_TABMOUNTER1_BLOW1_ON							= 2100; // Y064 
const int OUT_TABMOUNTER1_BLOW2_ON							= 2101; // Y065  
const int OUT_TABMOUNTER1_BLOW3_ON							= 2102; // Y066  
const int OUT_TABMOUNTER1_BLOW4_ON							= 2103; // Y067

const int OUT_TABMOUNTER1_VAC1_ON							= 2104; // Y068  
const int OUT_TABMOUNTER1_VAC2_ON							= 2105; // Y069  
const int OUT_TABMOUNTER1_VAC3_ON							= 2106; // Y06A  
const int OUT_TABMOUNTER1_VAC4_ON							= 2107; // Y06B  
const int OUT_TABMOUNTER1_PANEL_PUSHER						= 2108; // Y06C
const int OUT_TABMOUNTER2_PANEL_PUSHER						= 2109; // Y06D
const int OUT_TABMOUNTER3_PANEL_PUSHER						= 2110; // Y06E
const int OUT_TABMOUNTER4_PANEL_PUSHER						= 2111; // Y06F
//____________________

//____________________
const int OUT_TABMOUNTER1_PRESS5_1ST_DOWN					= 2112; // Y070  
const int OUT_TABMOUNTER1_PRESS6_1ST_DOWN					= 2113; // Y071  
const int OUT_TABMOUNTER1_PRESS7_1ST_DOWN					= 2114; // Y072  
const int OUT_TABMOUNTER1_PRESS8_1ST_DOWN					= 2115; // Y073  
const int OUT_TABMOUNTER1_BLOW5_ON							= 2116; // Y074 
const int OUT_TABMOUNTER1_BLOW6_ON							= 2117; // Y075  
const int OUT_TABMOUNTER1_BLOW7_ON							= 2118; // Y076  
const int OUT_TABMOUNTER1_BLOW8_ON							= 2119; // Y077

const int OUT_TABMOUNTER1_VAC5_ON							= 2120; // Y078  
const int OUT_TABMOUNTER1_VAC6_ON							= 2121; // Y079  
const int OUT_TABMOUNTER1_VAC7_ON							= 2122; // Y07A  
const int OUT_TABMOUNTER1_VAC8_ON							= 2123; // Y07B  
const int OUT_RTABMOUNTER1_PANEL_PUSHER						= 2124; // Y07C
const int OUT_RTABMOUNTER2_PANEL_PUSHER						= 2125; // Y07D
const int OUT_RTABMOUNTER3_PANEL_PUSHER						= 2126; // Y07E
const int OUT_RTABMOUNTER4_PANEL_PUSHER						= 2127; // Y07F
//____________________

//____________________
const int OUT_TABCARRIER_UP_SOL								= 2128; // Y080 
const int OUT_TABCARRIER_FORWARD_SOL						= 2129; // Y081
const int OUT_TABCARRIER_UP_SOL2							= 2130; // Y082 
const int OUT_SPARE_2339									= 2131; // Y083 
const int OUT_TABCARRIER1_VAC_ON_OFF						= 2132; // Y084  
const int OUT_TABCARRIER2_VAC_ON_OFF						= 2133; // Y085  
const int OUT_TABCARRIER3_VAC_ON_OFF						= 2134; // Y086  
const int OUT_TABCARRIER4_VAC_ON_OFF						= 2135; // Y087  
				
const int OUT_TABCARRIER1_BLOW_ON							= 2136; // Y088
const int OUT_TABCARRIER2_BLOW_ON							= 2137; // Y089
const int OUT_TABCARRIER3_BLOW_ON							= 2138; // Y08A
const int OUT_TABCARRIER4_BLOW_ON							= 2139; // Y08B
const int OUT_TABPRESS1_RECOVERY_SPROCKET_OPEN				= 2140; // Y08C
const int OUT_TABPRESS1_RECOVERY_SPROCKET_CLOSE				= 2141; // Y08D
const int OUT_TABPRESS1_PRESS_MOLD_BLOW						= 2142; // Y08E 
const int OUT_TABPRESS1_RECOVERY_TENSION_SPROCKET_MOTOR_ON	= 2143; // Y08F
//____________________									

//____________________
const int OUT_TABPRESS1_RECOVERY_TENSION_MOTOR_ON			= 2144; // Y090
const int OUT_TABPRESS1_RECOVERY_TENSION_MOTOR_CW			= 2145; // Y091
const int OUT_TABPRESS1_RECOVERY_TENSION_MOTOR_CCW			= 2146; // Y092
const int OUT_TABPRESS1_PRESS_RFID_RESET					= 2147; // Y093
const int OUT_TABPRESS1_PRESS_VACUUM_ON						= 2148; // Y094
const int OUT_TABPRESS1_PRESS_VACUUM_BLOW					= 2149; // Y095
const int OUT_TRANSFER_CENTER_UP							= 2150; // Y096
const int OUT_TRANSFER_CENTER_DOWN							= 2151; // Y097

const int OUT_PREALIGN_CAM1_EXPAND							= 2152; // Y098 
const int OUT_PREALIGN_CAM1_SHRINK							= 2153; // Y099 
const int OUT_TURN_HANDLER_VACUUM1							= 2154; // Y09A
const int OUT_TURN_HANDLER_VACUUM2							= 2155; // Y09B
const int OUT_TURN_HANDLER_VACUUM3							= 2156; // Y09C
const int OUT_TRANSFER_CENTER_VACUUM1						= 2157; // Y09D
const int OUT_TRANSFER_CENTER_VACUUM2						= 2158; // Y09E
const int OUT_TABPRESS1_SPROCKET_CLUTCH_ONOFF				= 2159; // Y09F
//____________________

//____________________
const int OUT_R_TABCARRIER_UP_SOL							= 2160; // Y100 
const int OUT_R_TABCARRIER_FORWARD_SOL						= 2161; // Y101
const int OUT_R_TABCARRIER_UP_SOL2							= 2162; // Y102 
const int OUT_SPARE_2371									= 2163; // Y103 
const int OUT_R_TABCARRIER1_VAC_ON_OFF						= 2164; // Y104  
const int OUT_R_TABCARRIER2_VAC_ON_OFF						= 2165; // Y105  
const int OUT_R_TABCARRIER3_VAC_ON_OFF						= 2166; // Y106  
const int OUT_R_TABCARRIER4_VAC_ON_OFF						= 2167; // Y107  
				
const int OUT_R_TABCARRIER1_BLOW_ON							= 2168; // Y108
const int OUT_R_TABCARRIER2_BLOW_ON							= 2169; // Y109
const int OUT_R_TABCARRIER3_BLOW_ON							= 2170; // Y10A
const int OUT_R_TABCARRIER4_BLOW_ON							= 2171; // Y10B
const int OUT_TABPRESS2_RECOVERY_SPROCKET_OPEN				= 2172; // Y10C
const int OUT_TABPRESS2_RECOVERY_SPROCKET_CLOSE				= 2173; // Y10D
const int OUT_TABPRESS2_PRESS_MOLD_BLOW						= 2174; // Y10E 
const int OUT_TABPRESS2_RECOVERY_TENSION_SPROCKET_MOTOR_ON	= 2175; // Y10F
//____________________

//____________________
const int OUT_TABPRESS2_RECOVERY_TENSION_MOTOR_ON			= 2176; // Y110
const int OUT_TABPRESS2_RECOVERY_TENSION_MOTOR_CW			= 2177; // Y111
const int OUT_TABPRESS2_RECOVERY_TENSION_MOTOR_CCW			= 2178; // Y112
const int OUT_TABPRESS2_PRESS_RFID_RESET					= 2179; // Y113
const int OUT_TABPRESS2_PRESS_VACUUM_ON						= 2180; // Y114
const int OUT_TABPRESS2_PRESS_VACUUM_BLOW					= 2181; // Y115
const int OUT_SPARE_2390									= 2182; // Y116
const int OUT_SPARE_2391									= 2183; // Y117

const int OUT_PREALIGN_CAM2_EXPAND							= 2184; // Y118 
const int OUT_PREALIGN_CAM2_SHRINK							= 2185; // Y119 
const int OUT_SPARE_2394									= 2186; // Y11A
const int OUT_SPARE_2395									= 2187; // Y11B
const int OUT_SPARE_2396									= 2188; // Y11C
const int OUT_SPARE_2397									= 2189; // Y11D
const int OUT_SPARE_2398									= 2190; // Y11E
const int OUT_TABPRESS2_SPROCKET_CLUTCH_ONOFF				= 2191; // Y11F
//____________________


//===================================================================

#else	//#ifdef DEF_SOURCE_SYSTEM

//===================================================================

//______________________
const int IN_E_STOP_RELAY_CHECK					= 1000; // X000  
const int IN_SPARE_1001							= 1001; // X001  
const int IN_SPARE_1002							= 1002; // X002  
const int IN_OVER_HEAT_DETECT_80				= 1003; // X003  
const int IN_OVER_HEAT_DETECT_60				= 1004; // X004  
const int IN_CBOX_OVERHEAT_DETECT				= 1005; // X005  
const int IN_OCR_DISCONNECT_DETECT				= 1006; // X006  
const int IN_SPARE_1007							= 1007; // X007  

const int IN_SPARE_1008							= 1008; // X008  
const int IN_SPARE_1009							= 1009; // X009  
const int IN_SPARE_1010							= 1010; // X00A  
const int IN_SPARE_1011							= 1011; // X00B  
const int IN_SPARE_1012							= 1012; // X00C  
const int IN_SPARE_1013							= 1013; // X00D  
const int IN_SPARE_1014							= 1014; // X00E  
const int IN_SPARE_1015							= 1015; // X00F  
//______________________

//______________________
const int IN_MAIN_AIR1							= 1016; // X010  
const int IN_MAIN_VACUUM1						= 1017; // X011
const int IN_SPARE_1018							= 1018; // X012 
const int IN_SPARE_1019							= 1019; // X013  
const int IN_SPARE_1020							= 1020; // X014  
const int IN_SPARE_1021							= 1021; // X015  
const int IN_SPARE_1022							= 1022; // X016  
const int IN_SPARE_1023							= 1023; // X017  

const int IN_SPARE_1024							= 1024; // X018  
const int IN_SPARE_1025							= 1025; // X019  
const int IN_SPARE_1026							= 1026; // X01A  
const int IN_SPARE_1027							= 1027; // X01B
const int IN_SPARE_1028							= 1028; // X01C
const int IN_SPARE_1029							= 1029; // X01D
const int IN_SPARE_1030							= 1030; // X01E
const int IN_SPARE_1031							= 1031; // X01F
//______________________

//______________________
const int IN_STAGE_CELL_DETECT					= 1032; // X020 
const int IN_STAGE_VACUUM1						= 1033; // X021 
const int IN_STAGE_VACUUM2						= 1034; // X022 
const int IN_STAGE_VACUUM3						= 1035; // X023 
const int IN_PLATE_AIR_VACUUM1_ON				= 1036; // X024
const int IN_PLATE_AIR_LEVITATION1_ON			= 1037; // X025
const int IN_PLATE_AIR_VACUUM2_ON				= 1038; // X026
const int IN_PLATE_AIR_LEVITATION2_ON			= 1039; // X027

const int IN_SPARE_1040							= 1040; // X028
const int IN_SPARE_1041							= 1041; // X029
const int IN_SPARE_1042							= 1042; // X02A  
const int IN_SPARE_1043							= 1043; // X02B  
const int IN_SPARE_1044							= 1044; // X02C 
const int IN_SPARE_1045							= 1045; // X02D 
const int IN_PANEL_DETECT_WORKING				= 1046;	// X02E
const int IN_PANEL_DETECT_LOADING				= 1047;	// X02F
//______________________ 

//______________________
const int IN_TRANSFER_OUT_1_UP					= 1048; // X030
const int IN_TRANSFER_OUT_1_DOWN				= 1049; // X031 
const int IN_TRANSFER_OUT_2_UP					= 1050; // X032 
const int IN_TRANSFER_OUT_2_DOWN				= 1051; // X033 
const int IN_TRANSFER_OUT_VACUUM1				= 1052; // X034 
const int IN_TRANSFER_OUT_CELL_DETECT			= 1053; // X035
const int IN_TRANSFER_IN_VACUUM2				= 1054; // X036
const int IN_TRANSFER_OUT_VACUUM2				= 1055;	// X037

const int IN_SPARE_1056							= 1056; // X038
const int IN_SPARE_1057							= 1057; // X039 
const int IN_SPARE_1058							= 1058; // X03A 
const int IN_SPARE_1059							= 1059; // X03B 
const int IN_SPARE_1060							= 1060; // X03C 
const int IN_SPARE_1061							= 1061; // X03D 
const int IN_PLATE_AIR_VACUUM3_ON				= 1062; // X03E
const int IN_PLATE_AIR_VACUUM4_ON				= 1063; // X03F
//______________________

//______________________
const int IN_SAFETY_KEY1_AUTO					= 1064; // X040
const int IN_SAFETY_KEY2_AUTO					= 1065; // X041
const int IN_SAFETY_KEY_F_MATERIAL				= 1066; // X042
const int IN_SAFETY_KEY_R_MATERIAL				= 1067; // X043
const int IN_SPARE_1068							= 1068; // X044
const int IN_SPARE_1069							= 1069; // X045
const int IN_SPARE_1070							= 1070; // X046
const int IN_SPARE_1071							= 1071; // X047

const int IN_FRONT_START_SW_PRESS				= 1072; // X048
const int IN_FRONT_STEP_STOP_SW_PRESS			= 1073; // X049
const int IN_FRONT_RESET_SW_PRESS				= 1074; // X04A
const int IN_REAR_START_SW_PRESS				= 1075; // X04B
const int IN_REAR_STEP_STOP_SW_PRESS			= 1076; // X04A
const int IN_REAR_RESET_SW_PRESS				= 1077; // X04B
const int IN_SPARE_1078							= 1078; // X04E
const int IN_SPARE_1079							= 1079; // X04F
//______________________

//______________________
const int IN_FRONT_START_SW						= 1080; // X050  
const int IN_FRONT_STEP_STOP_SW 				= 1081; // X051  
const int IN_FRONT_RESET_SW						= 1082; // X052  
const int IN_FRONT_JOG_L_SW						= 1083; // X053  
const int IN_FRONT_JOG_R_SW						= 1084; // X054  
const int IN_FRONT_JOG_F_SW						= 1085; // X055  
const int IN_FRONT_JOG_B_SW						= 1086; // X056  
const int IN_FRONT_JOG_CW_SW					= 1087; // X057  


const int IN_FRONT_JOG_CCW_SW					= 1088; // X058  
const int IN_FRONT_JOG_U_SW						= 1089; // X059  
const int IN_FRONT_JOG_D_SW						= 1090; // X05A  
const int IN_FRONT_JOG_E_STOP					= 1091; // X05B  
const int IN_SPARE_1092							= 1092; // X05C  
const int IN_SPARE_1093							= 1093; // X05D  
const int IN_SPARE_1094							= 1094; // X05E  
const int IN_FRONT_JOG_DETECT					= 1095; // X05F
//______________________

//______________________
const int IN_REAR_START_SW						= 1096; // X060  
const int IN_REAR_STEP_STOP_SW					= 1097; // X061  
const int IN_REAR_RESET_SW						= 1098; // X062  
const int IN_REAR_JOG_L_SW						= 1099; // X063  
const int IN_REAR_JOG_R_SW						= 1100; // X064  
const int IN_REAR_JOG_F_SW						= 1101; // X065  
const int IN_REAR_JOG_B_SW						= 1102; // X066  
const int IN_REAR_JOG_CW_SW						= 1103; // X067  


const int IN_REAR_JOG_CCW_SW					= 1104; // X068  
const int IN_REAR_JOG_U_SW						= 1105; // X069  
const int IN_REAR_JOG_D_SW						= 1106; // X06A  
const int IN_REAR_JOG_E_STOP					= 1107; // X06B  
const int IN_SPARE_1108							= 1108; // X06C  
const int IN_SPARE_1109							= 1109; // X06D  
const int IN_SPARE_1110							= 1110; // X06E  
const int IN_REAR_JOG_DETECT					= 1111; // X06F
//______________________

//______________________
const int IN_FRONT_DOOR1 						= 1112; // X070
const int IN_FRONT_DOOR2 						= 1113; // X071
const int IN_FRONT_DOOR3 						= 1114; // X072
const int IN_FRONT_DOOR4 						= 1115; // X073
const int IN_FRONT_DOOR5 						= 1116; // X074
const int IN_FRONT_DOOR6 						= 1117; // X075
const int IN_SPARE_1118							= 1118; // X076
const int IN_ALL_DOOR_LOCK						= 1119; // X077

const int IN_REAR_DOOR1							= 1120; // X078
const int IN_REAR_DOOR2							= 1121; // X079  
const int IN_REAR_DOOR3							= 1122; // X07A  
const int IN_REAR_DOOR4							= 1123; // X07B  
const int IN_REAR_DOOR5							= 1124; // X07C  
const int IN_REAR_DOOR6							= 1125; // X07D  
const int IN_FRONT_MATERIAL_DOOR_LOCK			= 1126; // X07E  
const int IN_REAR_MATERIAL_DOOR_LOCK			= 1127; // X07F
//______________________

//______________________
const int IN_INTERFACE_00 						= 1128; // X080  
const int IN_INTERFACE_01 						= 1129; // X081  
const int IN_INTERFACE_02 						= 1130; // X082  
const int IN_INTERFACE_03 						= 1131; // X083  
const int IN_INTERFACE_04 						= 1132; // X084  
const int IN_INTERFACE_05 						= 1133; // X085  
const int IN_INTERFACE_06 						= 1134; // X086  
const int IN_INTERFACE_07 						= 1135; // X087  


const int IN_INTERFACE_08 						= 1136; // X088  
const int IN_INTERFACE_09 						= 1137; // X089  
const int IN_INTERFACE_10 						= 1138; // X08A  
const int IN_INTERFACE_11 						= 1139; // X08B  
const int IN_INTERFACE_12 						= 1140; // X08C  
const int IN_INTERFACE_13 						= 1141; // X08D  
const int IN_INTERFACE_14 						= 1142; // X08E  
const int IN_INTERFACE_15 						= 1143; // X08F  
//______________________

//______________________
const int IN_TABMOUNTER1_THERMOSTAT_01 			= 1144; // X090  
const int IN_TABMOUNTER1_THERMOSTAT_02 			= 1145; // X091  
const int IN_SPARE_1146							= 1146; // X092  
const int IN_SPARE_1147							= 1147; // X093  
const int IN_TABMOUNTER1_HEATER_WIRE_CUT_01 	= 1148; // X094  
const int IN_TABMOUNTER1_HEATER_WIRE_CUT_02 	= 1149; // X095  
const int IN_SPARE_1150							= 1150; // X096  
const int IN_SPARE_1151							= 1151; // X097  

const int IN_TABMOUNTER1_PRESS1_1ST_UP 			= 1152; // X098  
const int IN_TABMOUNTER1_PRESS2_1ST_UP 			= 1153; // X099  
const int IN_SPARE_1154							= 1154; // X09A  
const int IN_SPARE_1155							= 1155; // X09B  
const int IN_TABMOUNTER1_PRESS1_1ST_DOWN		= 1156; // X09C  
const int IN_TABMOUNTER1_PRESS2_1ST_DOWN		= 1157; // X09D  
const int IN_MODELCHANGE_AIRRAIL_FOR1			= 1158; // X09E		//@신규추가
const int IN_MODELCHANGE_AIRRAIL_BACK1			= 1159; // X09F		//@신규추가
//______________________

//______________________
const int IN_TABMOUNTER1_THERMOSTAT_03			= 1160; // X100  
const int IN_TABMOUNTER1_THERMOSTAT_04			= 1161; // X101  
const int IN_SPARE_1162							= 1162; // X102  
const int IN_SPARE_1163							= 1163; // X103  
const int IN_TABMOUNTER1_HEATER_WIRE_CUT_03		= 1164; // X104  
const int IN_TABMOUNTER1_HEATER_WIRE_CUT_04		= 1165; // X105  
const int IN_SPARE_1166							= 1166; // X106
const int IN_SPARE_1167							= 1167; // X107

const int IN_TABMOUNTER1_PRESS3_1ST_UP			= 1168; // X108  
const int IN_TABMOUNTER1_PRESS4_1ST_UP			= 1169; // X109  
const int IN_SPARE_1170							= 1170; // X10A  
const int IN_SPARE_1171							= 1171; // X10B  
const int IN_TABMOUNTER1_PRESS3_1ST_DOWN		= 1172; // X10C  
const int IN_TABMOUNTER1_PRESS4_1ST_DOWN		= 1173; // X10D  
const int IN_MODELCHANGE_AIRRAIL_FOR2			= 1174; // X10E		//@신규추가
const int IN_MODELCHANGE_AIRRAIL_BACK2			= 1175; // X10F		//@신규추가
//______________________

//______________________
const int IN_TABMOUNTER1_PRESSURE1 				= 1176; // X110  
const int IN_TABMOUNTER1_PRESSURE2 				= 1177; // X111  
const int IN_SPARE_1178			 				= 1178; // X112  
const int IN_SPARE_1179 						= 1179; // X113  
const int IN_TABMOUNTER1_VACUUM1 				= 1180; // X114  
const int IN_TABMOUNTER1_VACUUM2 				= 1181; // X115  
const int IN_SPARE_1182			 				= 1182; // X116
const int IN_SPARE_1183 						= 1183; // X117


const int IN_TABMOUNTER1_PANEL_PUSHER			= 1184; // X118
const int IN_TABMOUNTER2_PANEL_PUSHER			= 1185; // X119
const int IN_SPARE_1186 						= 1186; // X11A
const int IN_SPARE_1187 						= 1187; // X11B
const int IN_SPARE_1188 						= 1188; // X11C  
const int IN_SPARE_1189 						= 1189; // X11D  
const int IN_SPARE_1190 						= 1190; // X11E
const int IN_SPARE_1191 						= 1191; // X11F
//______________________

//______________________
const int IN_TABMOUNTER1_PRESSURE3 				= 1192;// X120
const int IN_TABMOUNTER1_PRESSURE4 				= 1193;  
const int IN_SPARE_1194			 				= 1194;  
const int IN_SPARE_1195 						= 1195;  
const int IN_TABMOUNTER1_VACUUM3 				= 1196;  
const int IN_TABMOUNTER1_VACUUM4 				= 1197;  
const int IN_SPARE_1198			 				= 1198;
const int IN_SPARE_1199 						= 1199;

const int IN_RTABMOUNTER1_PANEL_PUSHER			= 1200; // X128
const int IN_RTABMOUNTER2_PANEL_PUSHER			= 1201; // X129
const int IN_SPARE_1202 						= 1202; // X12A
const int IN_SPARE_1203 						= 1203; // X12B
const int IN_SPARE_1204 						= 1204;  
const int IN_SPARE_1205 						= 1205;  
const int IN_SPARE_1206 						= 1206;
const int IN_IONIZER_STATE_ONOFF				= 1207;	//X12F
//______________________

//______________________
const int IN_TABCARRIER_UP						= 1208; // X130 
const int IN_TABCARRIER_DOWN					= 1209; // X131 
const int IN_TABCARRIER_FORWARD					= 1210; // X132 
const int IN_TABCARRIER_BACKWARD				= 1211; // X133 
const int IN_TABCARRIER1_VAC_ON_OFF				= 1212; // X134 
const int IN_TABCARRIER2_VAC_ON_OFF				= 1213; // X135 
const int IN_TABCARRIER3_VAC_ON_OFF				= 1214; // X136 
const int IN_TABCARRIER4_VAC_ON_OFF				= 1215; // X137 

const int IN_TABPRESS1_WEIGHT_TENSION_UP		= 1216; // X138 
const int IN_TABPRESS1_WEIGHT_TENSION_DOWN		= 1217; // X139 
const int IN_TABPRESS1_RECOVERY_SPROCKET_OPEN	= 1218; // X13A 
const int IN_TABPRESS1_RECOVERY_SPROCKET_CLOSE	= 1219; // X13B 
const int IN_TABPRESS1_PRESS_RFID_ALARM			= 1220; // X13C  
const int IN_TABPRESS1_PRESS_MOLD_DETECT		= 1221; // X13D  
const int IN_TABPRESS1_PRESS_VACUUM_ON			= 1222; // X13E
const int IN_TABPRESS1_DOWN_CHECK				= 1223;	// X13F		//신규추가
//____________________

//____________________
const int IN_TABPRESS1_SUPPLIER_LARGE_DETECT		= 1224; // X140  
const int IN_TABPRESS1_SUPPLIER_MIDDLE_DETECT		= 1225; // X141  
const int IN_TABPRESS1_SUPPLIER_SMALL_DETECT		= 1226; // X142  
const int IN_TABPRESS1_RAIL_UNFOLD_DETECT			= 1227; // X143  
const int IN_TABPRESS1_RECOVERY_PROTECT_ARM_DOWN	= 1228; // X144
const int IN_TABPRESS1_RECOVERY_PROTECT_ARM_UP		= 1229; // X145 
const int IN_FRONT_RECOVERY_BOX						= 1230; // X146
const int IN_SPARE_1231								= 1231; // X147

const int IN_TABPRESS1_BADTAB_TCP_DETECT			= 1232; // X148
const int IN_TABPRESS1_BADTAB_COF_DETECT			= 1233; // X149
const int IN_SPARE_1234								= 1234; // X14A
const int IN_SPARE_1235								= 1235; // X14B
const int IN_PREALIGN_CAM1_1_EXPAND					= 1236; // X14C
const int IN_PREALIGN_CAM1_1_SHRINK					= 1237; // X14D
const int IN_PREALIGN_CAM1_2_EXPAND					= 1238;	// X14E
const int IN_PREALIGN_CAM1_2_SHRINK					= 1239;	// X14F

//____________________

//____________________
const int IN_R_TABCARRIER_UP						= 1240; // X150 
const int IN_R_TABCARRIER_DOWN						= 1241; // X151 
const int IN_R_TABCARRIER_FORWARD					= 1242; // X152 
const int IN_R_TABCARRIER_BACKWARD					= 1243; // X153 
const int IN_R_TABCARRIER1_VAC_ON_OFF				= 1244; // X154   
const int IN_R_TABCARRIER2_VAC_ON_OFF				= 1245; // X155   
const int IN_R_TABCARRIER3_VAC_ON_OFF				= 1246; // X156   
const int IN_R_TABCARRIER4_VAC_ON_OFF				= 1247; // X157

const int IN_TABPRESS2_WEIGHT_TENSION_UP			= 1248;	// X158
const int IN_TABPRESS2_WEIGHT_TENSION_DOWN			= 1249; // X159  
const int IN_TABPRESS2_RECOVERY_SPROCKET_OPEN		= 1250; // X15A  
const int IN_TABPRESS2_RECOVERY_SPROCKET_CLOSE		= 1251; // X15B
const int IN_TABPRESS2_PRESS_RFID_ALARM				= 1252; // X15C  
const int IN_TABPRESS2_PRESS_MOLD_DETECT			= 1253; // X15D  
const int IN_TABPRESS2_PRESS_VACUUM_ON				= 1254; // X15E  
const int IN_TABPRESS2_DOWN_CHECK					= 1255; // X15F		//신규추가
//____________________

//____________________
const int IN_TABPRESS2_SUPPLIER_LARGE_DETECT			= 1256; // X160  
const int IN_TABPRESS2_SUPPLIER_MIDDLE_DETECT			= 1257; // X161  
const int IN_TABPRESS2_SUPPLIER_SMALL_DETECT			= 1258; // X162  
const int IN_TABPRESS2_RAIL_UNFOLD_DETECT				= 1259; // X163  
const int IN_TABPRESS2_RECOVERY_PROTECT_ARM_DOWN		= 1260; // X164 
const int IN_TABPRESS2_RECOVERY_PROTECT_ARM_UP			= 1261; // X165  
const int IN_REAR_RECOVERY_BOX							= 1262; // X166
const int IN_SPARE_1263									= 1263; // X167

const int IN_TABPRESS2_BADTAB_TCP_DETECT				= 1264; // X168
const int IN_TABPRESS2_BADTAB_COF_DETECT				= 1265; // X169
const int IN_SPARE_1266									= 1266; // X16A
const int IN_SPARE_1267									= 1267; // X16B
const int IN_PREALIGN_CAM2_1_EXPAND						= 1268; // X16C
const int IN_PREALIGN_CAM2_1_SHRINK						= 1269; // X16D
const int IN_PREALIGN_CAM2_2_EXPAND						= 1270; // X16E
const int IN_PREALIGN_CAM2_2_SHRINK						= 1271; // X16F
//____________________

//____________________
const int OUT_SPARE_2000								= 2000; // Y000  
const int OUT_TOWER_LAMP_RED							= 2001; // Y001  
const int OUT_TOWER_LAMP_YELLOW							= 2002; // Y002  
const int OUT_TOWER_LAMP_GREEN							= 2003; // Y003  
const int OUT_BUZZER_1 									= 2004; // Y004  
const int OUT_BUZZER_2 									= 2005; // Y005  
const int OUT_BUZZER_3 									= 2006; // Y006  
const int OUT_BUZZER_4 									= 2007; // Y007  

const int OUT_SPARE_2008								= 2008; // Y008  
const int OUT_SPARE_2009								= 2009; // Y009  
const int OUT_SPARE_2010								= 2010; // Y00A  
const int OUT_SPARE_2011								= 2011; // Y00B  
const int OUT_SPARE_2012								= 2012; // Y00C  
const int OUT_POWER_RESET								= 2013; // Y00D  
const int OUT_SPARE_2014								= 2014; // Y00E  
const int OUT_SERVO_CONTROL_POWER_OFF					= 2015; // Y00F  
//____________________

//____________________
const int OUT_STAGE_VACUUM1								= 2016; // Y010  
const int OUT_STAGE_VACUUM2								= 2017; // Y011  
const int OUT_STAGE_VACUUM3								= 2018; // Y012  
const int OUT_STAGE_BLOW								= 2019; // Y013  
const int OUT_PLATE_AIR_LEVITATION1_ON					= 2020; // Y014 
const int OUT_PLATE_AIR_VACUUM1_ON						= 2021; // Y015
const int OUT_PLATE_AIR_LEVITATION2_ON					= 2022; // Y016
const int OUT_PLATE_AIR_VACUUM2_ON						= 2023; // Y017  

const int OUT_SPARE_2024								= 2024; // Y018  
const int OUT_SPARE_2025								= 2025; // Y019  
const int OUT_SPARE_2026								= 2026; // Y01A  
const int OUT_SPARE_2027								= 2027; // Y01B  
const int OUT_SPARE_2028								= 2028; // Y01C  
const int OUT_SPARE_2029								= 2029; // Y01D  
const int OUT_SPARE_2030								= 2030; // Y01E  
const int OUT_SPARE_2031								= 2031; // Y01F  
//____________________

//____________________
const int OUT_TRANSFER_OUT_UP							= 2032; // Y020
const int OUT_TRANSFER_OUT_DOWN							= 2033; // Y021
const int OUT_TRANSFER_OUT_VACUUM1						= 2034; // Y022
const int OUT_TRANSFER_OUT_VACUUM2						= 2035; // Y023
const int OUT_PLATE_AIR_VACUUM3_ON						= 2036; // Y024
const int OUT_PLATE_AIR_VACUUM4_ON						= 2037; // Y025
const int OUT_SPARE_2038								= 2038; // Y026
const int OUT_SPARE_2039								= 2039; // Y027

const int OUT_SPARE_2040								= 2040; // Y028
const int OUT_SPARE_2041								= 2041; // Y029
const int OUT_SPARE_2042								= 2042; // Y02A
const int OUT_SPARE_2043								= 2043; // Y02B
const int OUT_SPARE_2044								= 2044; // Y02C
const int OUT_SPARE_2045								= 2045; // Y02D
const int OUT_SPARE_2046								= 2046; // Y02E
const int OUT_SPARE_2047								= 2047; // Y02F
//____________________

//____________________
const int OUT_ALL_DOOR_LOCK_OPEN						= 2048;
const int DEF_SPARE_2049								= 2049; // Y031
const int OUT_REAR_METERIAL_DOOR_LOCK_OPEN				= 2050; // Y032
const int OUT_FRONT_METERIAL_DOOR_LOCK_OPEN				= 2051; // Y033
const int OUT_TOUCH_MONITOR_CHANGE						= 2052; // Y034
const int OUT_TOUCH_MONITOR_CHANGE_R					= 2053; // Y035
const int OUT_TOUCH_MONITOR_CHANGE_PRESS				= 2054; // Y036
const int OUT_TOUCH_MONITOR_CHANGE_PRESS_R				= 2055; // Y037

const int OUT_FRONT_START_SW_LAMP						= 2056; // Y038  
const int OUT_FRONT_STEP_STOP_SW_LAMP					= 2057; // Y039  
const int OUT_FRONT_RESET_SW_LAMP						= 2058; // Y03A  
const int OUT_SPARE_2059								= 2059; // Y03B
const int OUT_REAR_START_SW_LAMP						= 2060; // Y03C  
const int OUT_REAR_STEP_STOP_SW_LAMP					= 2061; // Y03D  
const int OUT_REAR_RESET_SW_LAMP						= 2062; // Y03E  
const int OUT_SPARE_2063								= 2063; // Y03F  
//____________________

//____________________
const int OUT_INTERFACE_00 								= 2064; // Y040 
const int OUT_INTERFACE_01 								= 2065; // Y041  
const int OUT_INTERFACE_02 								= 2066; // Y042  
const int OUT_INTERFACE_03 								= 2067; // Y043  
const int OUT_INTERFACE_04 								= 2068; // Y044  
const int OUT_INTERFACE_05 								= 2069; // Y045  
const int OUT_INTERFACE_06 								= 2070; // Y046  
const int OUT_INTERFACE_07 								= 2071; // Y047  

const int OUT_INTERFACE_08								= 2072; // Y048  
const int OUT_INTERFACE_09 								= 2073; // Y049  
const int OUT_INTERFACE_10 								= 2074; // Y04A  
const int OUT_INTERFACE_11 								= 2075; // Y04B  
const int OUT_INTERFACE_12 								= 2076; // Y04C 
const int OUT_INTERFACE_13 								= 2077; // Y04D 
const int OUT_INTERFACE_14 								= 2078; // Y04E 
const int OUT_INTERFACE_15 								= 2079; // Y04F 
//____________________

//____________________
const int OUT_INSPECTION_CAM1_EXPAND					= 2080; // Y050
const int OUT_INSPECTION_CAM1_SHRINK					= 2081; // Y051 
const int OUT_INSPECTION_CAM2_EXPAND					= 2082; // Y052 
const int OUT_INSPECTION_CAM2_SHRINK					= 2083; // Y053 
const int OUT_R_INSPECTION_CAM1_EXPAND					= 2084; // Y054
const int OUT_R_INSPECTION_CAM1_SHRINK					= 2085; // Y055 
const int OUT_R_INSPECTION_CAM2_EXPAND					= 2086; // Y056 
const int OUT_R_INSPECTION_CAM2_SHRINK					= 2087; // Y057 

const int OUT_SPARE_2200 								= 2088; // Y058 
const int OUT_SPARE_2201 								= 2089; // Y059 
const int OUT_SPARE_2202 								= 2090; // Y05A 
const int OUT_SPARE_2203 								= 2091; // Y05B 
const int OUT_SPARE_2204 								= 2092; // Y05C 
const int OUT_SPARE_2205 								= 2093; // Y05D 
const int OUT_SPARE_2206 								= 2094; // Y05E 
const int OUT_SPARE_2207 								= 2095; // Y05F 
//____________________

//____________________
const int OUT_TABMOUNTER1_PRESS1_1ST_DOWN				= 2096; // Y060  
const int OUT_TABMOUNTER1_PRESS2_1ST_DOWN				= 2097; // Y061  
const int OUT_SPARE_2210 								= 2098; // Y062 
const int OUT_SPARE_2211 								= 2099; // Y063 
const int OUT_TABMOUNTER1_BLOW1_ON						= 2100; // Y064  
const int OUT_TABMOUNTER1_BLOW2_ON						= 2101; // Y065 
const int OUT_SPARE_2214 								= 2102; // Y066 
const int OUT_SPARE_2215 								= 2103; // Y067 
 
const int OUT_TABMOUNTER1_VAC1_ON						= 2104; // Y068  
const int OUT_TABMOUNTER1_VAC2_ON						= 2105; // Y069
const int OUT_MODELCHANGE_AIRRAIL_FOR1					= 2106; // Y06A		//@신규추가
const int OUT_MODELCHANGE_AIRRAIL_BACK1					= 2107; // Y06B		//@신규추가
const int OUT_TABMOUNTER1_PANEL_PUSHER					= 2108; // Y06C
const int OUT_TABMOUNTER2_PANEL_PUSHER					= 2109; // Y06D
const int OUT_SPARE_2222 								= 2110; // Y06E 
const int OUT_SPARE_2223 								= 2111; // Y06F 
//____________________

//____________________
const int OUT_TABMOUNTER1_PRESS3_1ST_DOWN				= 2112; // Y070  
const int OUT_TABMOUNTER1_PRESS4_1ST_DOWN				= 2113; // Y071  
const int OUT_SPARE_2226 								= 2114; // Y072 
const int OUT_SPARE_2227 								= 2115; // Y073 
const int OUT_TABMOUNTER1_BLOW3_ON						= 2116; // Y074  
const int OUT_TABMOUNTER1_BLOW4_ON						= 2117; // Y075 
const int OUT_SPARE_2230 								= 2118; // Y076 
const int OUT_SPARE_2231 								= 2119;	// Y077 
 
const int OUT_TABMOUNTER1_VAC3_ON						= 2120; // Y078  
const int OUT_TABMOUNTER1_VAC4_ON						= 2121; // Y079   
const int OUT_MODELCHANGE_AIRRAIL_FOR2				= 2122; // Y07A		//@신규추가
const int OUT_MODELCHANGE_AIRRAIL_BACK2				= 2123; // Y07B		//@신규추가
const int OUT_RTABMOUNTER1_PANEL_PUSHER				= 2124; // Y07C
const int OUT_RTABMOUNTER2_PANEL_PUSHER				= 2125; // Y07D
const int OUT_SPARE_2238 								= 2126; // Y07E 
const int OUT_SPARE_2239 								= 2127; // Y07F 
//____________________

//____________________
const int OUT_TABCARRIER_UP_SOL							= 2128; // Y080 
const int OUT_TABCARRIER_FORWARD_SOL					= 2129; // Y081
const int OUT_SPARE_2242 								= 2130; // Y082 
const int OUT_SPARE_2243 								= 2131; // Y083 
const int OUT_TABCARRIER1_VAC_ON_OFF					= 2132; // Y084  
const int OUT_TABCARRIER2_VAC_ON_OFF					= 2133; // Y085  
const int OUT_TABCARRIER3_VAC_ON_OFF					= 2134; // Y086  
const int OUT_TABCARRIER4_VAC_ON_OFF					= 2135; // Y087 
 
const int OUT_TABCARRIER1_BLOW_ON						= 2136; // Y088
const int OUT_TABCARRIER2_BLOW_ON						= 2137; // Y089
const int OUT_TABCARRIER3_BLOW_ON						= 2138; // Y08A
const int OUT_TABCARRIER4_BLOW_ON						= 2139; // Y08B
const int OUT_TABPRESS1_RECOVERY_SPROCKET_OPEN			= 2140; // Y08C
const int OUT_TABPRESS1_RECOVERY_SPROCKET_CLOSE			= 2141; // Y08D  
const int OUT_TABPRESS1_PRESS_MOLD_BLOW					= 2142; // Y08E  
const int OUT_TABPRESS1_RECOVERY_TENSION_SPROCKET_MOTOR_ON	= 2143; // Y08F //?????
//____________________

//____________________
const int OUT_TABPRESS1_RECOVERY_TENSION_MOTOR_ON		= 2144; // Y090
const int OUT_TABPRESS1_RECOVERY_TENSION_MOTOR_CW		= 2145; // Y091	//???
const int OUT_TABPRESS1_RECOVERY_TENSION_MOTOR_CCW		= 2146; // Y092	//???
const int OUT_TABPRESS1_PRESS_RFID_RESET				= 2147; // Y093  
const int OUT_TABPRESS1_PRESS_VACUUM_ON					= 2148; // Y094  
const int OUT_TABPRESS1_PRESS_VACUUM_BLOW				= 2149; // Y095  
const int OUT_SPARE_2262 								= 2150; // Y096 
const int OUT_SPARE_2263 								= 2151; // Y097 

const int OUT_PREALIGN_CAM1_EXPAND						= 2152; // Y098
const int OUT_PREALIGN_CAM1_SHRINK						= 2153; // Y099
const int OUT_SPARE_2266 								= 2154; // Y09A 
const int OUT_SPARE_2267 								= 2155; // Y09B 
const int OUT_SPARE_2268 								= 2156; // Y09C 
const int OUT_SPARE_2269 								= 2157; // Y09D 
const int OUT_SPARE_2270 								= 2158; // Y09E 
const int OUT_SPARE_2271 								= 2159; // Y09F 
//____________________

//____________________
const int OUT_R_TABCARRIER_UP_SOL						= 2160; // Y100
const int OUT_R_TABCARRIER_FORWARD_SOL					= 2161; // Y101
const int OUT_SPARE_2274								= 2162; // Y102
const int OUT_SPARE_2275								= 2163; // Y103
const int OUT_R_TABCARRIER1_VAC_ON_OFF					= 2164; // Y104 
const int OUT_R_TABCARRIER2_VAC_ON_OFF					= 2165; // Y105 
const int OUT_R_TABCARRIER3_VAC_ON_OFF					= 2166; // Y106
const int OUT_R_TABCARRIER4_VAC_ON_OFF					= 2167; // Y107 
														
const int OUT_R_TABCARRIER1_BLOW_ON_OFF					= 2168; // Y108 
const int OUT_R_TABCARRIER2_BLOW_ON_OFF					= 2169; // Y109 
const int OUT_R_TABCARRIER3_BLOW_ON_OFF					= 2170; // Y10A 
const int OUT_R_TABCARRIER4_BLOW_ON_OFF					= 2171; // Y10B 
const int OUT_TABPRESS2_RECOVERY_SPROCKET_OPEN			= 2172; // Y10C
const int OUT_TABPRESS2_RECOVERY_SPROCKET_CLOSE			= 2173; // Y10D  
const int OUT_TABPRESS2_PRESS_MOLD_BLOW					= 2174; // Y10E  
const int OUT_TABPRESS2_RECOVERY_TENSION_SPROCKET_MOTOR_ON		= 2175; // Y10F
//____________________

//____________________
const int OUT_TABPRESS2_RECOVERY_TENSION_MOTOR_ON		= 2176; // Y110
const int OUT_TABPRESS2_RECOVERY_TENSION_MOTOR_CW		= 2177; // Y111  
const int OUT_TABPRESS2_RECOVERY_TENSION_MOTOR_CCW		= 2178; // Y112  
const int OUT_TABPRESS2_PRESS_RFID_RESET				= 2179; // Y113  
const int OUT_TABPRESS2_PRESS_VACUUM_ON					= 2180; // Y114  
const int OUT_TABPRESS2_PRESS_VACUUM_BLOW				= 2181; // Y115  
const int OUT_SPARE_2294 								= 2182; // Y116 
const int OUT_SPARE_2295 								= 2183; // Y117 

const int OUT_PREALIGN_CAM2_EXPAND						= 2184; // Y118
const int OUT_PREALIGN_CAM2_SHRINK						= 2185; // Y119
const int OUT_SPARE_2298 								= 2186; // Y11A 
const int OUT_SPARE_2299 								= 2187; // Y11B 
const int OUT_SPARE_2300 								= 2188; // Y11C
const int OUT_SPARE_2301 								= 2189; // Y11D
const int OUT_SPARE_2302 								= 2190; // Y11E
const int OUT_SPARE_2303 								= 2191; // Y11F 
//____________________

#endif
//===================================================================

#ifdef DEF_SOURCE_SYSTEM

	const int IN_DUMP_BASKET_FRONT_FOR				= INPUT_DUMMY;
	const int IN_DUMP_BASKET_FRONT_BACK				= INPUT_DUMMY;
	const int IN_DUMP_BASKET_REAR_FOR				= INPUT_DUMMY;
	const int IN_DUMP_BASKET_REAR_BACK				= INPUT_DUMMY;

	const int IN_TABPRESS1_PRESS_REEL_UP				= INPUT_DUMMY; // X150 //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS1_PRESS_REEL_DOWN				= INPUT_DUMMY; // X151 //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS1_PRESS_CLAMP_UP				= INPUT_DUMMY; // X152 //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS1_PRESS_CLAMP_DOWN				= INPUT_DUMMY; // X153 //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS1_RECOVERY_PROTECT_DOWN_DETECT	= INPUT_DUMMY; // X158	//DEF_MECH_VER1일때만 사용

	const int IN_TABPRESS1_ATTACH_CUTTING_SUPPORT_LEFT_UP		= INPUT_DUMMY; // X160  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS1_ATTACH_CUTTING_SUPPORT_LEFT_DOWN		= INPUT_DUMMY; // X161  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS1_ATTACH_CUTTING_SUPPORT_RIGHT_UP		= INPUT_DUMMY; // X162  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS1_ATTACH_CUTTING_SUPPORT_RIGHT_DOWN	= INPUT_DUMMY; // X163  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS1_ATTACH_PRESS_UP						= INPUT_DUMMY; // X164  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS1_ATTACH_PRESS_DOWN					= INPUT_DUMMY; // X165  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS1_ATTACH_REEL_CUTTER_CUTTING			= INPUT_DUMMY; // X166  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS1_ATTACH_REEL_CUTTER_OPEN				= INPUT_DUMMY; // X167  //DEF_MECH_VER1일때만 사용


	const int IN_TABPRESS1_ATTACH_REEL_FOR					= INPUT_DUMMY; // X168  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS1_ATTACH_REEL_BACK					= INPUT_DUMMY; // X169  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS1_ATTACH_NEW_REEL_VACCUM_ON		= INPUT_DUMMY; // X16A  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS1_SUPPLIER_CHUCK_EXPAND			= INPUT_DUMMY; // X16B  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS1_SUPPLIER_CHUCK_SHRINK			= INPUT_DUMMY; // X16C  //DEF_MECH_VER1일때만 사용

	const int IN_TABPRESS1_SUPPLIER_CURRENT_REEL_DETECT		= INPUT_DUMMY; // X170  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS1_SUPPLIER_NEW_REEL_DETECT			= INPUT_DUMMY; // X171  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS1_RAIL_DISCARD_REEL_DETECT			= INPUT_DUMMY; // X179  //DEF_MECH_VER1일때만 사용

	const int IN_TABPRESS2_PRESS_REEL_UP				= INPUT_DUMMY; // X150 //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS2_PRESS_REEL_DOWN				= INPUT_DUMMY; // X151 //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS2_PRESS_CLAMP_UP				= INPUT_DUMMY; // X152 //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS2_PRESS_CLAMP_DOWN				= INPUT_DUMMY; // X153 //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS2_RECOVERY_PROTECT_DOWN_DETECT	= INPUT_DUMMY; // X158	//DEF_MECH_VER1일때만 사용

	const int IN_TABPRESS2_ATTACH_CUTTING_SUPPORT_LEFT_UP		= INPUT_DUMMY; // X160  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS2_ATTACH_CUTTING_SUPPORT_LEFT_DOWN		= INPUT_DUMMY; // X161  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS2_ATTACH_CUTTING_SUPPORT_RIGHT_UP		= INPUT_DUMMY; // X162  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS2_ATTACH_CUTTING_SUPPORT_RIGHT_DOWN	= INPUT_DUMMY; // X163  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS2_ATTACH_PRESS_UP						= INPUT_DUMMY; // X164  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS2_ATTACH_PRESS_DOWN					= INPUT_DUMMY; // X165  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS2_ATTACH_REEL_CUTTER_CUTTING			= INPUT_DUMMY; // X166  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS2_ATTACH_REEL_CUTTER_OPEN				= INPUT_DUMMY; // X167  //DEF_MECH_VER1일때만 사용


	const int IN_TABPRESS2_ATTACH_REEL_FOR					= INPUT_DUMMY; // X168  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS2_ATTACH_REEL_BACK					= INPUT_DUMMY; // X169  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS2_ATTACH_NEW_REEL_VACCUM_ON		= INPUT_DUMMY; // X16A  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS2_SUPPLIER_CHUCK_EXPAND			= INPUT_DUMMY; // X16B  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS2_SUPPLIER_CHUCK_SHRINK			= INPUT_DUMMY; // X16C  //DEF_MECH_VER1일때만 사용

	const int IN_TABPRESS2_SUPPLIER_CURRENT_REEL_DETECT		= INPUT_DUMMY; // X170  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS2_SUPPLIER_NEW_REEL_DETECT			= INPUT_DUMMY; // X171  //DEF_MECH_VER1일때만 사용
	const int IN_TABPRESS2_RAIL_DISCARD_REEL_DETECT			= INPUT_DUMMY; // X179  //DEF_MECH_VER1일때만 사용


//170426 JSh del	const int OUT_HEATER_POWER_RESET						= OUT_SERVO_CONTROL_POWER_OFF; // Y00D

//	const int OUT_TABPRESS1_SPROCKET_CLUTCH_ONOFF	= OUTPUT_DUMMY; // Y160
//	const int OUT_TABPRESS2_SPROCKET_CLUTCH_ONOFF	= OUTPUT_DUMMY; // Y160

	const int IN_INSPECTION_CAM1_EXPAND				= INPUT_DUMMY; // X118
	const int IN_INSPECTION_CAM1_SHRINK				= INPUT_DUMMY; // X119
	const int IN_INSPECTION_CAM2_EXPAND				= INPUT_DUMMY; // X11A
	const int IN_INSPECTION_CAM2_SHRINK				= INPUT_DUMMY; // X11B
	const int IN_INSPECTION_CAM3_EXPAND				= INPUT_DUMMY; // X11C
	const int IN_INSPECTION_CAM3_SHRINK				= INPUT_DUMMY; // X11D
	const int IN_INSPECTION_CAM4_EXPAND				= INPUT_DUMMY; // X11E
	const int IN_INSPECTION_CAM4_SHRINK				= INPUT_DUMMY; // X11F

	const int IN_R_INSPECTION_CAM1_EXPAND			= INPUT_DUMMY; // X128
	const int IN_R_INSPECTION_CAM1_SHRINK			= INPUT_DUMMY; // X129
	const int IN_R_INSPECTION_CAM2_EXPAND			= INPUT_DUMMY; // X12A
	const int IN_R_INSPECTION_CAM2_SHRINK			= INPUT_DUMMY; // X12B
	const int IN_R_INSPECTION_CAM3_EXPAND			= INPUT_DUMMY; // X12C
	const int IN_R_INSPECTION_CAM3_SHRINK			= INPUT_DUMMY; // X12D
	const int IN_R_INSPECTION_CAM4_EXPAND			= INPUT_DUMMY; // X12E
	const int IN_R_INSPECTION_CAM4_SHRINK			= INPUT_DUMMY; // X12F

	const int IN_FRONT_START_SW_PRESS				= INPUT_DUMMY; // X048
	const int IN_FRONT_STEP_STOP_SW_PRESS			= INPUT_DUMMY; // X049
	const int IN_FRONT_RESET_SW_PRESS				= INPUT_DUMMY; // X04A
	const int IN_REAR_START_SW_PRESS				= INPUT_DUMMY; // X04B
	const int IN_REAR_STEP_STOP_SW_PRESS			= INPUT_DUMMY; // X04A
	const int IN_REAR_RESET_SW_PRESS				= INPUT_DUMMY; // X04B

//____________________
const int OUT_ILLUMINATION1_DIGITAL_INPUT_1 			= OUTPUT_DUMMY; // Y040  
const int OUT_ILLUMINATION1_DIGITAL_INPUT_2 			= OUTPUT_DUMMY; // Y041  
const int OUT_ILLUMINATION1_DIGITAL_INPUT_3 			= OUTPUT_DUMMY; // Y042  
const int OUT_ILLUMINATION1_DIGITAL_INPUT_4 			= OUTPUT_DUMMY; // Y043  
const int OUT_ILLUMINATION1_DIGITAL_INPUT_5 			= OUTPUT_DUMMY; // Y044  
const int OUT_ILLUMINATION1_DIGITAL_INPUT_6 			= OUTPUT_DUMMY; // Y045  
const int OUT_ILLUMINATION1_DIGITAL_INPUT_7 			= OUTPUT_DUMMY; // Y046  
const int OUT_ILLUMINATION1_DIGITAL_INPUT_8 			= OUTPUT_DUMMY; // Y047  

const int OUT_ILLUMINATION2_DIGITAL_INPUT_1				= OUTPUT_DUMMY; // Y048  
const int OUT_ILLUMINATION2_DIGITAL_INPUT_2				= OUTPUT_DUMMY; // Y049  
const int OUT_ILLUMINATION2_DIGITAL_INPUT_3				= OUTPUT_DUMMY; // Y04A
const int OUT_ILLUMINATION2_DIGITAL_INPUT_4				= OUTPUT_DUMMY; // Y04B  
const int OUT_ILLUMINATION2_DIGITAL_INPUT_5				= OUTPUT_DUMMY; // Y04C  
const int OUT_ILLUMINATION2_DIGITAL_INPUT_6				= OUTPUT_DUMMY; // Y04D  
const int OUT_ILLUMINATION2_DIGITAL_INPUT_7				= OUTPUT_DUMMY; // Y04E  
const int OUT_ILLUMINATION2_DIGITAL_INPUT_8				= OUTPUT_DUMMY; // Y04F
//____________________

//____________________
const int OUT_ILLUMINATION3_DIGITAL_INPUT_1				= OUTPUT_DUMMY; // Y050
const int OUT_ILLUMINATION3_DIGITAL_INPUT_2				= OUTPUT_DUMMY; // Y051
const int OUT_ILLUMINATION3_DIGITAL_INPUT_3				= OUTPUT_DUMMY; // Y052
const int OUT_ILLUMINATION3_DIGITAL_INPUT_4 			= OUTPUT_DUMMY; // Y053  
const int OUT_ILLUMINATION3_DIGITAL_INPUT_5 			= OUTPUT_DUMMY; // Y054  
const int OUT_ILLUMINATION3_DIGITAL_INPUT_6				= OUTPUT_DUMMY; // Y055  
const int OUT_ILLUMINATION3_DIGITAL_INPUT_7				= OUTPUT_DUMMY; // Y056
const int OUT_ILLUMINATION3_DIGITAL_INPUT_8				= OUTPUT_DUMMY; // Y057

const int OUT_ILLUMINATION4_DIGITAL_INPUT_1				= OUTPUT_DUMMY; // Y058
const int OUT_ILLUMINATION4_DIGITAL_INPUT_2				= OUTPUT_DUMMY; // Y059  
const int OUT_ILLUMINATION4_DIGITAL_INPUT_3				= OUTPUT_DUMMY; // Y05A  
const int OUT_ILLUMINATION4_DIGITAL_INPUT_4				= OUTPUT_DUMMY; // Y05B  
const int OUT_ILLUMINATION4_DIGITAL_INPUT_5				= OUTPUT_DUMMY; // Y05C  
const int OUT_ILLUMINATION4_DIGITAL_INPUT_6				= OUTPUT_DUMMY; // Y05D  
const int OUT_ILLUMINATION4_DIGITAL_INPUT_7				= OUTPUT_DUMMY; // Y05E  
const int OUT_ILLUMINATION4_DIGITAL_INPUT_8				= OUTPUT_DUMMY; // Y05F
//____________________

//____________________
const int OUT_ILLUMINATION5_DIGITAL_INPUT_1				= OUTPUT_DUMMY; // Y060  
const int OUT_ILLUMINATION5_DIGITAL_INPUT_2				= OUTPUT_DUMMY; // Y061  
const int OUT_ILLUMINATION5_DIGITAL_INPUT_3				= OUTPUT_DUMMY; // Y062  
const int OUT_ILLUMINATION5_DIGITAL_INPUT_4				= OUTPUT_DUMMY; // Y063  
const int OUT_ILLUMINATION5_DIGITAL_INPUT_5				= OUTPUT_DUMMY; // Y064  
const int OUT_ILLUMINATION5_DIGITAL_INPUT_6				= OUTPUT_DUMMY; // Y065  
const int OUT_ILLUMINATION5_DIGITAL_INPUT_7				= OUTPUT_DUMMY; // Y066  
const int OUT_ILLUMINATION5_DIGITAL_INPUT_8				= OUTPUT_DUMMY; // Y067  

const int OUT_ILLUMINATION6_DIGITAL_INPUT_1				= OUTPUT_DUMMY; // Y068  
const int OUT_ILLUMINATION6_DIGITAL_INPUT_2				= OUTPUT_DUMMY; // Y069  
const int OUT_ILLUMINATION6_DIGITAL_INPUT_3				= OUTPUT_DUMMY; // Y06A
const int OUT_ILLUMINATION6_DIGITAL_INPUT_4				= OUTPUT_DUMMY; // Y06B  
const int OUT_ILLUMINATION6_DIGITAL_INPUT_5				= OUTPUT_DUMMY; // Y06C  
const int OUT_ILLUMINATION6_DIGITAL_INPUT_6				= OUTPUT_DUMMY; // Y06D  
const int OUT_ILLUMINATION6_DIGITAL_INPUT_7				= OUTPUT_DUMMY; // Y06E  
const int OUT_ILLUMINATION6_DIGITAL_INPUT_8				= OUTPUT_DUMMY; // Y06F
//____________________

//____________________
const int OUT_ILLUMINATION7_DIGITAL_INPUT_1				= OUTPUT_DUMMY; // Y070
const int OUT_ILLUMINATION7_DIGITAL_INPUT_2				= OUTPUT_DUMMY; // Y071
const int OUT_ILLUMINATION7_DIGITAL_INPUT_3				= OUTPUT_DUMMY; // Y072
const int OUT_ILLUMINATION7_DIGITAL_INPUT_4				= OUTPUT_DUMMY; // Y073  
const int OUT_ILLUMINATION7_DIGITAL_INPUT_5				= OUTPUT_DUMMY; // Y074  
const int OUT_ILLUMINATION7_DIGITAL_INPUT_6				= OUTPUT_DUMMY; // Y075  
const int OUT_ILLUMINATION7_DIGITAL_INPUT_7				= OUTPUT_DUMMY; // Y076
const int OUT_ILLUMINATION7_DIGITAL_INPUT_8				= OUTPUT_DUMMY; // Y077

const int OUT_ILLUMINATION8_DIGITAL_INPUT_1				= OUTPUT_DUMMY; // Y078
const int OUT_ILLUMINATION8_DIGITAL_INPUT_2				= OUTPUT_DUMMY; // Y079  
const int OUT_ILLUMINATION8_DIGITAL_INPUT_3				= OUTPUT_DUMMY; // Y07A  
const int OUT_ILLUMINATION8_DIGITAL_INPUT_4				= OUTPUT_DUMMY; // Y07B  
const int OUT_ILLUMINATION8_DIGITAL_INPUT_5				= OUTPUT_DUMMY; // Y07C  
const int OUT_ILLUMINATION8_DIGITAL_INPUT_6				= OUTPUT_DUMMY; // Y07D  
const int OUT_ILLUMINATION8_DIGITAL_INPUT_7				= OUTPUT_DUMMY; // Y07E  
const int OUT_ILLUMINATION8_DIGITAL_INPUT_8				= OUTPUT_DUMMY; // Y07F  
//____________________

//____________________
const int OUT_ILLUMINATION9_DIGITAL_INPUT_1				= OUTPUT_DUMMY; // Y080
const int OUT_ILLUMINATION9_DIGITAL_INPUT_2				= OUTPUT_DUMMY; // Y081
const int OUT_ILLUMINATION9_DIGITAL_INPUT_3				= OUTPUT_DUMMY; // Y082
const int OUT_ILLUMINATION9_DIGITAL_INPUT_4				= OUTPUT_DUMMY; // Y083
const int OUT_ILLUMINATION9_DIGITAL_INPUT_5				= OUTPUT_DUMMY; // Y084
const int OUT_ILLUMINATION9_DIGITAL_INPUT_6				= OUTPUT_DUMMY; // Y085
const int OUT_ILLUMINATION9_DIGITAL_INPUT_7				= OUTPUT_DUMMY; // Y086
const int OUT_ILLUMINATION9_DIGITAL_INPUT_8				= OUTPUT_DUMMY; // Y087

const int OUT_ILLUMINATION10_DIGITAL_INPUT_1			= OUTPUT_DUMMY; // Y088
const int OUT_ILLUMINATION10_DIGITAL_INPUT_2			= OUTPUT_DUMMY; // Y089
const int OUT_ILLUMINATION10_DIGITAL_INPUT_3			= OUTPUT_DUMMY; // Y08A
const int OUT_ILLUMINATION10_DIGITAL_INPUT_4			= OUTPUT_DUMMY; // Y08B
const int OUT_ILLUMINATION10_DIGITAL_INPUT_5			= OUTPUT_DUMMY; // Y08C
const int OUT_ILLUMINATION10_DIGITAL_INPUT_6			= OUTPUT_DUMMY; // Y08D
const int OUT_ILLUMINATION10_DIGITAL_INPUT_7			= OUTPUT_DUMMY; // Y08E
const int OUT_ILLUMINATION10_DIGITAL_INPUT_8			= OUTPUT_DUMMY; // Y08F
//____________________

//____________________
const int OUT_ILLUMINATION11_DIGITAL_INPUT_1			= OUTPUT_DUMMY; // Y090
const int OUT_ILLUMINATION11_DIGITAL_INPUT_2			= OUTPUT_DUMMY; // Y091
const int OUT_ILLUMINATION11_DIGITAL_INPUT_3			= OUTPUT_DUMMY; // Y092
const int OUT_ILLUMINATION11_DIGITAL_INPUT_4			= OUTPUT_DUMMY; // Y093
const int OUT_ILLUMINATION11_DIGITAL_INPUT_5			= OUTPUT_DUMMY; // Y094
const int OUT_ILLUMINATION11_DIGITAL_INPUT_6			= OUTPUT_DUMMY; // Y095
const int OUT_ILLUMINATION11_DIGITAL_INPUT_7			= OUTPUT_DUMMY; // Y096
const int OUT_ILLUMINATION11_DIGITAL_INPUT_8			= OUTPUT_DUMMY; // Y097

const int OUT_ILLUMINATION12_DIGITAL_INPUT_1			= OUTPUT_DUMMY; // Y098
const int OUT_ILLUMINATION12_DIGITAL_INPUT_2			= OUTPUT_DUMMY; // Y099
const int OUT_ILLUMINATION12_DIGITAL_INPUT_3			= OUTPUT_DUMMY; // Y09A
const int OUT_ILLUMINATION12_DIGITAL_INPUT_4			= OUTPUT_DUMMY; // Y09B
const int OUT_ILLUMINATION12_DIGITAL_INPUT_5			= OUTPUT_DUMMY; // Y09C
const int OUT_ILLUMINATION12_DIGITAL_INPUT_6			= OUTPUT_DUMMY; // Y09D
const int OUT_ILLUMINATION12_DIGITAL_INPUT_7			= OUTPUT_DUMMY; // Y09E
const int OUT_ILLUMINATION12_DIGITAL_INPUT_8			= OUTPUT_DUMMY; // Y09F
//____________________

//____________________
const int OUT_ILLUMINATION13_DIGITAL_INPUT_1			= OUTPUT_DUMMY; // Y100
const int OUT_ILLUMINATION13_DIGITAL_INPUT_2			= OUTPUT_DUMMY; // Y101
const int OUT_ILLUMINATION13_DIGITAL_INPUT_3			= OUTPUT_DUMMY; // Y102
const int OUT_ILLUMINATION13_DIGITAL_INPUT_4			= OUTPUT_DUMMY; // Y103
const int OUT_ILLUMINATION13_DIGITAL_INPUT_5			= OUTPUT_DUMMY; // Y104
const int OUT_ILLUMINATION13_DIGITAL_INPUT_6			= OUTPUT_DUMMY; // Y105
const int OUT_ILLUMINATION13_DIGITAL_INPUT_7			= OUTPUT_DUMMY; // Y106
const int OUT_ILLUMINATION13_DIGITAL_INPUT_8			= OUTPUT_DUMMY; // Y107

const int OUT_ILLUMINATION14_DIGITAL_INPUT_1			= OUTPUT_DUMMY; // Y108
const int OUT_ILLUMINATION14_DIGITAL_INPUT_2			= OUTPUT_DUMMY; // Y109
const int OUT_ILLUMINATION14_DIGITAL_INPUT_3			= OUTPUT_DUMMY; // Y10A
const int OUT_ILLUMINATION14_DIGITAL_INPUT_4			= OUTPUT_DUMMY; // Y10B
const int OUT_ILLUMINATION14_DIGITAL_INPUT_5			= OUTPUT_DUMMY; // Y10C
const int OUT_ILLUMINATION14_DIGITAL_INPUT_6			= OUTPUT_DUMMY; // Y10D
const int OUT_ILLUMINATION14_DIGITAL_INPUT_7			= OUTPUT_DUMMY; // Y10E
const int OUT_ILLUMINATION14_DIGITAL_INPUT_8			= OUTPUT_DUMMY; // Y10F
//____________________

//____________________
const int OUT_ILLUMINATION15_DIGITAL_INPUT_1			= OUTPUT_DUMMY; // Y110
const int OUT_ILLUMINATION15_DIGITAL_INPUT_2			= OUTPUT_DUMMY; // Y111
const int OUT_ILLUMINATION15_DIGITAL_INPUT_3			= OUTPUT_DUMMY; // Y112
const int OUT_ILLUMINATION15_DIGITAL_INPUT_4			= OUTPUT_DUMMY; // Y113
const int OUT_ILLUMINATION15_DIGITAL_INPUT_5			= OUTPUT_DUMMY; // Y114
const int OUT_ILLUMINATION15_DIGITAL_INPUT_6			= OUTPUT_DUMMY; // Y115
const int OUT_ILLUMINATION15_DIGITAL_INPUT_7			= OUTPUT_DUMMY; // Y116
const int OUT_ILLUMINATION15_DIGITAL_INPUT_8			= OUTPUT_DUMMY; // Y117

const int OUT_ILLUMINATION16_DIGITAL_INPUT_1			= OUTPUT_DUMMY; // Y118
const int OUT_ILLUMINATION16_DIGITAL_INPUT_2			= OUTPUT_DUMMY; // Y119
const int OUT_ILLUMINATION16_DIGITAL_INPUT_3			= OUTPUT_DUMMY; // Y11A
const int OUT_ILLUMINATION16_DIGITAL_INPUT_4			= OUTPUT_DUMMY; // Y11B
const int OUT_ILLUMINATION16_DIGITAL_INPUT_5			= OUTPUT_DUMMY; // Y11C
const int OUT_ILLUMINATION16_DIGITAL_INPUT_6			= OUTPUT_DUMMY; // Y11D
const int OUT_ILLUMINATION16_DIGITAL_INPUT_7			= OUTPUT_DUMMY; // Y11E
const int OUT_ILLUMINATION16_DIGITAL_INPUT_8			= OUTPUT_DUMMY; // Y11F
//____________________

//____________________
const int OUT_ILLUMINATION17_DIGITAL_INPUT_1			= OUTPUT_DUMMY; // Y120
const int OUT_ILLUMINATION17_DIGITAL_INPUT_2			= OUTPUT_DUMMY; // Y121
const int OUT_ILLUMINATION17_DIGITAL_INPUT_3			= OUTPUT_DUMMY; // Y122
const int OUT_ILLUMINATION17_DIGITAL_INPUT_4			= OUTPUT_DUMMY; // Y123
const int OUT_ILLUMINATION17_DIGITAL_INPUT_5			= OUTPUT_DUMMY; // Y124
const int OUT_ILLUMINATION17_DIGITAL_INPUT_6			= OUTPUT_DUMMY; // Y125
const int OUT_ILLUMINATION17_DIGITAL_INPUT_7			= OUTPUT_DUMMY; // Y126
const int OUT_ILLUMINATION17_DIGITAL_INPUT_8			= OUTPUT_DUMMY; // Y127

const int OUT_ILLUMINATION18_DIGITAL_INPUT_1			= OUTPUT_DUMMY; // Y128
const int OUT_ILLUMINATION18_DIGITAL_INPUT_2			= OUTPUT_DUMMY; // Y129
const int OUT_ILLUMINATION18_DIGITAL_INPUT_3			= OUTPUT_DUMMY; // Y12A
const int OUT_ILLUMINATION18_DIGITAL_INPUT_4			= OUTPUT_DUMMY; // Y12B
const int OUT_ILLUMINATION18_DIGITAL_INPUT_5			= OUTPUT_DUMMY; // Y12C
const int OUT_ILLUMINATION18_DIGITAL_INPUT_6			= OUTPUT_DUMMY; // Y12D
const int OUT_ILLUMINATION18_DIGITAL_INPUT_7			= OUTPUT_DUMMY; // Y12E
const int OUT_ILLUMINATION18_DIGITAL_INPUT_8			= OUTPUT_DUMMY; // Y12F
//____________________

//____________________
const int OUT_ILLUMINATION19_DIGITAL_INPUT_1			= OUTPUT_DUMMY; // Y130
const int OUT_ILLUMINATION19_DIGITAL_INPUT_2			= OUTPUT_DUMMY; // Y131
const int OUT_ILLUMINATION19_DIGITAL_INPUT_3			= OUTPUT_DUMMY; // Y132
const int OUT_ILLUMINATION19_DIGITAL_INPUT_4			= OUTPUT_DUMMY; // Y133
const int OUT_ILLUMINATION19_DIGITAL_INPUT_5			= OUTPUT_DUMMY; // Y134
const int OUT_ILLUMINATION19_DIGITAL_INPUT_6			= OUTPUT_DUMMY; // Y135
const int OUT_ILLUMINATION19_DIGITAL_INPUT_7			= OUTPUT_DUMMY; // Y136
const int OUT_ILLUMINATION19_DIGITAL_INPUT_8			= OUTPUT_DUMMY; // Y137

const int OUT_ILLUMINATION20_DIGITAL_INPUT_1			= OUTPUT_DUMMY; // Y138
const int OUT_ILLUMINATION20_DIGITAL_INPUT_2			= OUTPUT_DUMMY; // Y139
const int OUT_ILLUMINATION20_DIGITAL_INPUT_3			= OUTPUT_DUMMY; // Y13A
const int OUT_ILLUMINATION20_DIGITAL_INPUT_4			= OUTPUT_DUMMY; // Y13B
const int OUT_ILLUMINATION20_DIGITAL_INPUT_5			= OUTPUT_DUMMY; // Y13C
const int OUT_ILLUMINATION20_DIGITAL_INPUT_6			= OUTPUT_DUMMY; // Y13D
const int OUT_ILLUMINATION20_DIGITAL_INPUT_7			= OUTPUT_DUMMY; // Y13E
const int OUT_ILLUMINATION20_DIGITAL_INPUT_8			= OUTPUT_DUMMY; // Y13F
//____________________				

//____________________
const int OUT_ILLUMINATION21_DIGITAL_INPUT_1			= OUTPUT_DUMMY; // Y140
const int OUT_ILLUMINATION21_DIGITAL_INPUT_2			= OUTPUT_DUMMY; // Y141
const int OUT_ILLUMINATION21_DIGITAL_INPUT_3			= OUTPUT_DUMMY; // Y142
const int OUT_ILLUMINATION21_DIGITAL_INPUT_4			= OUTPUT_DUMMY; // Y143
const int OUT_ILLUMINATION21_DIGITAL_INPUT_5			= OUTPUT_DUMMY; // Y144
const int OUT_ILLUMINATION21_DIGITAL_INPUT_6			= OUTPUT_DUMMY; // Y145
const int OUT_ILLUMINATION21_DIGITAL_INPUT_7			= OUTPUT_DUMMY; // Y146  
const int OUT_ILLUMINATION21_DIGITAL_INPUT_8			= OUTPUT_DUMMY; // Y147

const int OUT_ILLUMINATION22_DIGITAL_INPUT_1			= OUTPUT_DUMMY; // Y148
const int OUT_ILLUMINATION22_DIGITAL_INPUT_2			= OUTPUT_DUMMY; // Y149
const int OUT_ILLUMINATION22_DIGITAL_INPUT_3			= OUTPUT_DUMMY; // Y14A
const int OUT_ILLUMINATION22_DIGITAL_INPUT_4			= OUTPUT_DUMMY; // Y14B
const int OUT_ILLUMINATION22_DIGITAL_INPUT_5			= OUTPUT_DUMMY; // Y14C
const int OUT_ILLUMINATION22_DIGITAL_INPUT_6			= OUTPUT_DUMMY; // Y14D
const int OUT_ILLUMINATION22_DIGITAL_INPUT_7			= OUTPUT_DUMMY; // Y14E
const int OUT_ILLUMINATION22_DIGITAL_INPUT_8			= OUTPUT_DUMMY; // Y14F
//____________________

//____________________
const int OUT_ILLUMINATION23_DIGITAL_INPUT_1			= OUTPUT_DUMMY; // Y150
const int OUT_ILLUMINATION23_DIGITAL_INPUT_2			= OUTPUT_DUMMY; // Y151
const int OUT_ILLUMINATION23_DIGITAL_INPUT_3			= OUTPUT_DUMMY; // Y152
const int OUT_ILLUMINATION23_DIGITAL_INPUT_4			= OUTPUT_DUMMY; // Y153
const int OUT_ILLUMINATION23_DIGITAL_INPUT_5			= OUTPUT_DUMMY; // Y154
const int OUT_ILLUMINATION23_DIGITAL_INPUT_6			= OUTPUT_DUMMY; // Y155
const int OUT_ILLUMINATION23_DIGITAL_INPUT_7			= OUTPUT_DUMMY; // Y156
const int OUT_ILLUMINATION23_DIGITAL_INPUT_8			= OUTPUT_DUMMY; // Y157

const int OUT_ILLUMINATION24_DIGITAL_INPUT_1			= OUTPUT_DUMMY; // Y158
const int OUT_ILLUMINATION24_DIGITAL_INPUT_2			= OUTPUT_DUMMY; // Y159
const int OUT_ILLUMINATION24_DIGITAL_INPUT_3			= OUTPUT_DUMMY; // Y15A
const int OUT_ILLUMINATION24_DIGITAL_INPUT_4			= OUTPUT_DUMMY; // Y15B
const int OUT_ILLUMINATION24_DIGITAL_INPUT_5			= OUTPUT_DUMMY; // Y15C
const int OUT_ILLUMINATION24_DIGITAL_INPUT_6			= OUTPUT_DUMMY; // Y15D
const int OUT_ILLUMINATION24_DIGITAL_INPUT_7			= OUTPUT_DUMMY; // Y15E
const int OUT_ILLUMINATION24_DIGITAL_INPUT_8			= OUTPUT_DUMMY; // Y15F
//____________________

//____________________
const int OUT_ILLUMINATION25_DIGITAL_INPUT_1			= OUTPUT_DUMMY; // Y160
const int OUT_ILLUMINATION25_DIGITAL_INPUT_2			= OUTPUT_DUMMY; // Y161
const int OUT_ILLUMINATION25_DIGITAL_INPUT_3			= OUTPUT_DUMMY; // Y162
const int OUT_ILLUMINATION25_DIGITAL_INPUT_4			= OUTPUT_DUMMY; // Y163
const int OUT_ILLUMINATION25_DIGITAL_INPUT_5			= OUTPUT_DUMMY; // Y164
const int OUT_ILLUMINATION25_DIGITAL_INPUT_6			= OUTPUT_DUMMY; // Y165
const int OUT_ILLUMINATION25_DIGITAL_INPUT_7			= OUTPUT_DUMMY; // Y166
const int OUT_ILLUMINATION25_DIGITAL_INPUT_8			= OUTPUT_DUMMY; // Y167

const int OUT_ILLUMINATION26_DIGITAL_INPUT_1			= OUTPUT_DUMMY; // Y168
const int OUT_ILLUMINATION26_DIGITAL_INPUT_2			= OUTPUT_DUMMY; // Y169
const int OUT_ILLUMINATION26_DIGITAL_INPUT_3			= OUTPUT_DUMMY; // Y16A
const int OUT_ILLUMINATION26_DIGITAL_INPUT_4			= OUTPUT_DUMMY; // Y16B
const int OUT_ILLUMINATION26_DIGITAL_INPUT_5			= OUTPUT_DUMMY; // Y16C
const int OUT_ILLUMINATION26_DIGITAL_INPUT_6			= OUTPUT_DUMMY; // Y16D
const int OUT_ILLUMINATION26_DIGITAL_INPUT_7			= OUTPUT_DUMMY; // Y16E
const int OUT_ILLUMINATION26_DIGITAL_INPUT_8			= OUTPUT_DUMMY; // Y16F
//____________________





#else
//@const int IN_TABPRESS1_WEIGHT_TENSION_UP_LIMIT	= INPUT_DUMMY;
//@const int IN_TABPRESS1_WEIGHT_TENSION_DOWN_LIMIT	= INPUT_DUMMY;
	const int IN_TABPRESS1_PRESS_REEL_UP				= INPUT_DUMMY;
	const int IN_TABPRESS1_PRESS_REEL_DOWN				= INPUT_DUMMY;
	const int IN_TABPRESS1_PRESS_CLAMP_UP				= INPUT_DUMMY;
	const int IN_TABPRESS1_PRESS_CLAMP_DOWN				= INPUT_DUMMY;

	const int IN_TABPRESS1_ATTACH_CUTTING_SUPPORT_LEFT_UP		= INPUT_DUMMY;	//자동교체 관련 없어짐
	const int IN_TABPRESS1_ATTACH_CUTTING_SUPPORT_LEFT_DOWN		= INPUT_DUMMY;
	const int IN_TABPRESS1_ATTACH_CUTTING_SUPPORT_RIGHT_UP		= INPUT_DUMMY;
	const int IN_TABPRESS1_ATTACH_CUTTING_SUPPORT_RIGHT_DOWN	= INPUT_DUMMY;
	const int IN_TABPRESS1_ATTACH_PRESS_UP						= INPUT_DUMMY;
	const int IN_TABPRESS1_ATTACH_PRESS_DOWN					= INPUT_DUMMY;
	const int IN_TABPRESS1_ATTACH_REEL_CUTTER_CUTTING			= INPUT_DUMMY;
	const int IN_TABPRESS1_ATTACH_REEL_CUTTER_OPEN				= INPUT_DUMMY;
	const int IN_TABPRESS1_ATTACH_REEL_FOR						= INPUT_DUMMY;
	const int IN_TABPRESS1_ATTACH_REEL_BACK						= INPUT_DUMMY;

	const int IN_TABPRESS1_ATTACH_NEW_REEL_VACCUM_ON			= INPUT_DUMMY;
	const int IN_TABPRESS1_SUPPLIER_CHUCK_EXPAND				= INPUT_DUMMY;
	const int IN_TABPRESS1_SUPPLIER_CHUCK_SHRINK				= INPUT_DUMMY;
	const int IN_TABPRESS1_SUPPLIER_CURRENT_REEL_DETECT			= INPUT_DUMMY;
	const int IN_TABPRESS1_SUPPLIER_NEW_REEL_DETECT				= INPUT_DUMMY;
	const int IN_TABPRESS1_BADTAB_LEFT_TCP_DETECT			= INPUT_DUMMY;
	const int IN_TABPRESS1_BADTAB_LEFT_COF_DETECT			= INPUT_DUMMY;
	const int IN_TABPRESS1_BADTAB_RIGHT_TCP_DETECT			= INPUT_DUMMY;
	const int IN_TABPRESS1_BADTAB_RIGHT_COF_DETECT			= INPUT_DUMMY;

	const int OUT_SAFETY_INVALIDATION1						= OUTPUT_DUMMY;
	const int OUT_SAFETY_INVALIDATION2						= OUTPUT_DUMMY;

	const int OUT_TABPRESS1_PRESS_REEL_UP					= OUTPUT_DUMMY;
	const int OUT_TABPRESS1_PRESS_REEL_DOWN					= OUTPUT_DUMMY;
	const int OUT_TABPRESS1_PRESS_CLAMP_UP					= OUTPUT_DUMMY;
	const int OUT_TABPRESS1_PRESS_CLAMP_DOWN				= OUTPUT_DUMMY;

	const int OUT_TABPRESS1_ATTACH_CUTTING_SUPPORT_LEFT_UP		= OUTPUT_DUMMY;
	const int OUT_TABPRESS1_ATTACH_CUTTING_SUPPORT_LEFT_DOWN	= OUTPUT_DUMMY;
	const int OUT_TABPRESS1_ATTACH_CUTTING_SUPPORT_RIGHT_UP		= OUTPUT_DUMMY;
	const int OUT_TABPRESS1_ATTACH_CUTTING_SUPPORT_RIGHT_DOWN	= OUTPUT_DUMMY;
	const int OUT_TABPRESS1_ATTACH_PRESS_UP						= OUTPUT_DUMMY;
	const int OUT_TABPRESS1_ATTACH_PRESS_DOWN					= OUTPUT_DUMMY;
	const int OUT_TABPRESS1_ATTACH_REEL_CUTTER_CUTTING			= OUTPUT_DUMMY;
	const int OUT_TABPRESS1_ATTACH_REEL_FOR						= OUTPUT_DUMMY;
	const int OUT_TABPRESS1_ATTACH_REEL_BACK					= OUTPUT_DUMMY;
	const int OUT_TABPRESS1_ATTACH_NEW_REEL_VACCUM_ON			= OUTPUT_DUMMY;
	const int OUT_TABPRESS1_SUPPLIER_CHUCK_EXPANDSHRINK			= OUTPUT_DUMMY;
	const int OUT_TABPRESS1_SUPPLIER_LARGE_TENSION				= OUTPUT_DUMMY; // Y27C  
	const int OUT_TABPRESS1_SUPPLIER_MIDDLE_TENSION				= OUTPUT_DUMMY; // Y27D  
	const int OUT_TABPRESS1_SUPPLIER_SMALL_TENSION				= OUTPUT_DUMMY; // Y27E  
	const int OUT_TABPRESS1_SUPPLIER_TENSION_MOTOR_ON			= OUTPUT_DUMMY; // Y27F  
	const int OUT_TABPRESS1_P_REEL_MOTOR_ON						= OUTPUT_DUMMY;
	const int OUT_TABPRESS1_CLEANING_MOTOR_ON					= OUTPUT_DUMMY;

	const int OUT_TABPRESS2_ATTACH_CUTTING_SUPPORT_LEFT_UP		= OUTPUT_DUMMY;  
	const int OUT_TABPRESS2_ATTACH_CUTTING_SUPPORT_LEFT_DOWN	= OUTPUT_DUMMY;
	const int OUT_TABPRESS2_ATTACH_CUTTING_SUPPORT_RIGHT_UP		= OUTPUT_DUMMY;
	const int OUT_TABPRESS2_ATTACH_CUTTING_SUPPORT_RIGHT_DOWN	= OUTPUT_DUMMY;
	const int OUT_TABPRESS2_ATTACH_PRESS_UP						= OUTPUT_DUMMY;
	const int OUT_TABPRESS2_ATTACH_PRESS_DOWN					= OUTPUT_DUMMY;
	const int OUT_TABPRESS2_ATTACH_REEL_CUTTER_CUTTING			= OUTPUT_DUMMY;
	const int OUT_TABPRESS2_ATTACH_REEL_FOR						= OUTPUT_DUMMY;
	const int OUT_TABPRESS2_ATTACH_REEL_BACK					= OUTPUT_DUMMY;
	const int OUT_TABPRESS2_ATTACH_NEW_REEL_VACCUM_ON			= OUTPUT_DUMMY;
	const int OUT_TABPRESS2_SUPPLIER_CHUCK_EXPANDSHRINK			= OUTPUT_DUMMY;
	const int OUT_TABPRESS2_SUPPLIER_LARGE_TENSION				= OUTPUT_DUMMY;
	const int OUT_TABPRESS2_SUPPLIER_MIDDLE_TENSION				= OUTPUT_DUMMY;
	const int OUT_TABPRESS2_SUPPLIER_SMALL_TENSION				= OUTPUT_DUMMY;
	const int OUT_TABPRESS2_SUPPLIER_TENSION_MOTOR_ON			= OUTPUT_DUMMY;
	const int OUT_TABPRESS2_P_REEL_MOTOR_ON						= OUTPUT_DUMMY;
	const int OUT_TABPRESS2_CLEANING_MOTOR_ON					= OUTPUT_DUMMY;

	const int OUT_HEATER_POWER_RESET						= OUT_SERVO_CONTROL_POWER_OFF; // Y00D

	const int OUT_TABPRESS1_SPROCKET_CLUTCH_ONOFF	= OUTPUT_DUMMY; // Y160
	const int OUT_TABPRESS2_SPROCKET_CLUTCH_ONOFF	= OUTPUT_DUMMY; // Y160

	const int IN_INSPECTION_CAM1_EXPAND				= INPUT_DUMMY; // X118
	const int IN_INSPECTION_CAM1_SHRINK				= INPUT_DUMMY; // X119
	const int IN_INSPECTION_CAM2_EXPAND				= INPUT_DUMMY; // X11A
	const int IN_INSPECTION_CAM2_SHRINK				= INPUT_DUMMY; // X11B

	const int IN_R_INSPECTION_CAM1_EXPAND			= INPUT_DUMMY; // X128
	const int IN_R_INSPECTION_CAM1_SHRINK			= INPUT_DUMMY; // X129
	const int IN_R_INSPECTION_CAM2_EXPAND			= INPUT_DUMMY; // X12A
	const int IN_R_INSPECTION_CAM2_SHRINK			= INPUT_DUMMY; // X12B
		//____________________
	const int OUT_ILLUMINATION1_DIGITAL_INPUT_1 			= OUTPUT_DUMMY; // Y040  
	const int OUT_ILLUMINATION1_DIGITAL_INPUT_2 			= OUTPUT_DUMMY; // Y041  
	const int OUT_ILLUMINATION1_DIGITAL_INPUT_3 			= OUTPUT_DUMMY; // Y042  
	const int OUT_ILLUMINATION1_DIGITAL_INPUT_4 			= OUTPUT_DUMMY; // Y043  
	const int OUT_ILLUMINATION1_DIGITAL_INPUT_5 			= OUTPUT_DUMMY; // Y044  
	const int OUT_ILLUMINATION1_DIGITAL_INPUT_6 			= OUTPUT_DUMMY; // Y045  
	const int OUT_ILLUMINATION1_DIGITAL_INPUT_7 			= OUTPUT_DUMMY; // Y046  
	const int OUT_ILLUMINATION1_DIGITAL_INPUT_8 			= OUTPUT_DUMMY; // Y047  

	const int OUT_ILLUMINATION2_DIGITAL_INPUT_1				= OUTPUT_DUMMY; // Y048  
	const int OUT_ILLUMINATION2_DIGITAL_INPUT_2				= OUTPUT_DUMMY; // Y049  
	const int OUT_ILLUMINATION2_DIGITAL_INPUT_3				= OUTPUT_DUMMY; // Y04A
	const int OUT_ILLUMINATION2_DIGITAL_INPUT_4				= OUTPUT_DUMMY; // Y04B  
	const int OUT_ILLUMINATION2_DIGITAL_INPUT_5				= OUTPUT_DUMMY; // Y04C  
	const int OUT_ILLUMINATION2_DIGITAL_INPUT_6				= OUTPUT_DUMMY; // Y04D  
	const int OUT_ILLUMINATION2_DIGITAL_INPUT_7				= OUTPUT_DUMMY; // Y04E  
	const int OUT_ILLUMINATION2_DIGITAL_INPUT_8				= OUTPUT_DUMMY; // Y04F
	//____________________

	//____________________
	const int OUT_ILLUMINATION3_DIGITAL_INPUT_1				= OUTPUT_DUMMY; // Y050
	const int OUT_ILLUMINATION3_DIGITAL_INPUT_2				= OUTPUT_DUMMY; // Y051
	const int OUT_ILLUMINATION3_DIGITAL_INPUT_3				= OUTPUT_DUMMY; // Y052
	const int OUT_ILLUMINATION3_DIGITAL_INPUT_4 			= OUTPUT_DUMMY; // Y053  
	const int OUT_ILLUMINATION3_DIGITAL_INPUT_5 			= OUTPUT_DUMMY; // Y054  
	const int OUT_ILLUMINATION3_DIGITAL_INPUT_6				= OUTPUT_DUMMY; // Y055  
	const int OUT_ILLUMINATION3_DIGITAL_INPUT_7				= OUTPUT_DUMMY; // Y056
	const int OUT_ILLUMINATION3_DIGITAL_INPUT_8				= OUTPUT_DUMMY; // Y057

	const int OUT_ILLUMINATION4_DIGITAL_INPUT_1				= OUTPUT_DUMMY; // Y058
	const int OUT_ILLUMINATION4_DIGITAL_INPUT_2				= OUTPUT_DUMMY; // Y059  
	const int OUT_ILLUMINATION4_DIGITAL_INPUT_3				= OUTPUT_DUMMY; // Y05A  
	const int OUT_ILLUMINATION4_DIGITAL_INPUT_4				= OUTPUT_DUMMY; // Y05B  
	const int OUT_ILLUMINATION4_DIGITAL_INPUT_5				= OUTPUT_DUMMY; // Y05C  
	const int OUT_ILLUMINATION4_DIGITAL_INPUT_6				= OUTPUT_DUMMY; // Y05D  
	const int OUT_ILLUMINATION4_DIGITAL_INPUT_7				= OUTPUT_DUMMY; // Y05E  
	const int OUT_ILLUMINATION4_DIGITAL_INPUT_8				= OUTPUT_DUMMY; // Y05F  
	//____________________

	//____________________
	const int OUT_ILLUMINATION5_DIGITAL_INPUT_1				= OUTPUT_DUMMY; // Y060  
	const int OUT_ILLUMINATION5_DIGITAL_INPUT_2				= OUTPUT_DUMMY; // Y061  
	const int OUT_ILLUMINATION5_DIGITAL_INPUT_3				= OUTPUT_DUMMY; // Y062  
	const int OUT_ILLUMINATION5_DIGITAL_INPUT_4				= OUTPUT_DUMMY; // Y063  
	const int OUT_ILLUMINATION5_DIGITAL_INPUT_5				= OUTPUT_DUMMY; // Y064  
	const int OUT_ILLUMINATION5_DIGITAL_INPUT_6				= OUTPUT_DUMMY; // Y065  
	const int OUT_ILLUMINATION5_DIGITAL_INPUT_7				= OUTPUT_DUMMY; // Y066  
	const int OUT_ILLUMINATION5_DIGITAL_INPUT_8				= OUTPUT_DUMMY; // Y067  

	const int OUT_ILLUMINATION6_DIGITAL_INPUT_1				= OUTPUT_DUMMY; // Y068  
	const int OUT_ILLUMINATION6_DIGITAL_INPUT_2				= OUTPUT_DUMMY; // Y069  
	const int OUT_ILLUMINATION6_DIGITAL_INPUT_3				= OUTPUT_DUMMY; // Y06A
	const int OUT_ILLUMINATION6_DIGITAL_INPUT_4				= OUTPUT_DUMMY; // Y06B  
	const int OUT_ILLUMINATION6_DIGITAL_INPUT_5				= OUTPUT_DUMMY; // Y06C  
	const int OUT_ILLUMINATION6_DIGITAL_INPUT_6				= OUTPUT_DUMMY; // Y06D  
	const int OUT_ILLUMINATION6_DIGITAL_INPUT_7				= OUTPUT_DUMMY; // Y06E  
	const int OUT_ILLUMINATION6_DIGITAL_INPUT_8				= OUTPUT_DUMMY; // Y06F
	//____________________

	//____________________
	const int OUT_ILLUMINATION7_DIGITAL_INPUT_1				= OUTPUT_DUMMY; // Y070
	const int OUT_ILLUMINATION7_DIGITAL_INPUT_2				= OUTPUT_DUMMY; // Y071
	const int OUT_ILLUMINATION7_DIGITAL_INPUT_3				= OUTPUT_DUMMY; // Y072
	const int OUT_ILLUMINATION7_DIGITAL_INPUT_4				= OUTPUT_DUMMY; // Y073  
	const int OUT_ILLUMINATION7_DIGITAL_INPUT_5				= OUTPUT_DUMMY; // Y074  
	const int OUT_ILLUMINATION7_DIGITAL_INPUT_6				= OUTPUT_DUMMY; // Y075  
	const int OUT_ILLUMINATION7_DIGITAL_INPUT_7				= OUTPUT_DUMMY; // Y076
	const int OUT_ILLUMINATION7_DIGITAL_INPUT_8				= OUTPUT_DUMMY; // Y077

	const int OUT_ILLUMINATION8_DIGITAL_INPUT_1				= OUTPUT_DUMMY; // Y078
	const int OUT_ILLUMINATION8_DIGITAL_INPUT_2				= OUTPUT_DUMMY; // Y079  
	const int OUT_ILLUMINATION8_DIGITAL_INPUT_3				= OUTPUT_DUMMY; // Y07A  
	const int OUT_ILLUMINATION8_DIGITAL_INPUT_4				= OUTPUT_DUMMY; // Y07B  
	const int OUT_ILLUMINATION8_DIGITAL_INPUT_5				= OUTPUT_DUMMY; // Y07C  
	const int OUT_ILLUMINATION8_DIGITAL_INPUT_6				= OUTPUT_DUMMY; // Y07D  
	const int OUT_ILLUMINATION8_DIGITAL_INPUT_7				= OUTPUT_DUMMY; // Y07E  
	const int OUT_ILLUMINATION8_DIGITAL_INPUT_8				= OUTPUT_DUMMY; // Y07F  
	//____________________

	//____________________
	const int OUT_ILLUMINATION9_DIGITAL_INPUT_1				= OUTPUT_DUMMY; // Y080
	const int OUT_ILLUMINATION9_DIGITAL_INPUT_2				= OUTPUT_DUMMY; // Y081
	const int OUT_ILLUMINATION9_DIGITAL_INPUT_3				= OUTPUT_DUMMY; // Y082
	const int OUT_ILLUMINATION9_DIGITAL_INPUT_4				= OUTPUT_DUMMY; // Y083
	const int OUT_ILLUMINATION9_DIGITAL_INPUT_5				= OUTPUT_DUMMY; // Y084
	const int OUT_ILLUMINATION9_DIGITAL_INPUT_6				= OUTPUT_DUMMY; // Y085
	const int OUT_ILLUMINATION9_DIGITAL_INPUT_7				= OUTPUT_DUMMY; // Y086
	const int OUT_ILLUMINATION9_DIGITAL_INPUT_8				= OUTPUT_DUMMY; // Y087

	const int OUT_ILLUMINATION10_DIGITAL_INPUT_1			= OUTPUT_DUMMY; // Y088
	const int OUT_ILLUMINATION10_DIGITAL_INPUT_2			= OUTPUT_DUMMY; // Y089
	const int OUT_ILLUMINATION10_DIGITAL_INPUT_3			= OUTPUT_DUMMY; // Y08A
	const int OUT_ILLUMINATION10_DIGITAL_INPUT_4			= OUTPUT_DUMMY; // Y08B
	const int OUT_ILLUMINATION10_DIGITAL_INPUT_5			= OUTPUT_DUMMY; // Y08C
	const int OUT_ILLUMINATION10_DIGITAL_INPUT_6			= OUTPUT_DUMMY; // Y08D
	const int OUT_ILLUMINATION10_DIGITAL_INPUT_7			= OUTPUT_DUMMY; // Y08E
	const int OUT_ILLUMINATION10_DIGITAL_INPUT_8			= OUTPUT_DUMMY; // Y08F
	//____________________

	//____________________
	const int OUT_ILLUMINATION11_DIGITAL_INPUT_1			= OUTPUT_DUMMY; // Y090
	const int OUT_ILLUMINATION11_DIGITAL_INPUT_2			= OUTPUT_DUMMY; // Y091
	const int OUT_ILLUMINATION11_DIGITAL_INPUT_3			= OUTPUT_DUMMY; // Y092
	const int OUT_ILLUMINATION11_DIGITAL_INPUT_4			= OUTPUT_DUMMY; // Y093
	const int OUT_ILLUMINATION11_DIGITAL_INPUT_5			= OUTPUT_DUMMY; // Y094
	const int OUT_ILLUMINATION11_DIGITAL_INPUT_6			= OUTPUT_DUMMY; // Y095
	const int OUT_ILLUMINATION11_DIGITAL_INPUT_7			= OUTPUT_DUMMY; // Y096
	const int OUT_ILLUMINATION11_DIGITAL_INPUT_8			= OUTPUT_DUMMY; // Y097

	const int OUT_ILLUMINATION12_DIGITAL_INPUT_1			= OUTPUT_DUMMY; // Y098
	const int OUT_ILLUMINATION12_DIGITAL_INPUT_2			= OUTPUT_DUMMY; // Y099
	const int OUT_ILLUMINATION12_DIGITAL_INPUT_3			= OUTPUT_DUMMY; // Y09A
	const int OUT_ILLUMINATION12_DIGITAL_INPUT_4			= OUTPUT_DUMMY; // Y09B
	const int OUT_ILLUMINATION12_DIGITAL_INPUT_5			= OUTPUT_DUMMY; // Y09C
	const int OUT_ILLUMINATION12_DIGITAL_INPUT_6			= OUTPUT_DUMMY; // Y09D
	const int OUT_ILLUMINATION12_DIGITAL_INPUT_7			= OUTPUT_DUMMY; // Y09E
	const int OUT_ILLUMINATION12_DIGITAL_INPUT_8			= OUTPUT_DUMMY; // Y09F
	//____________________

	//____________________
	const int OUT_ILLUMINATION13_DIGITAL_INPUT_1			= OUTPUT_DUMMY; // Y100
	const int OUT_ILLUMINATION13_DIGITAL_INPUT_2			= OUTPUT_DUMMY; // Y101
	const int OUT_ILLUMINATION13_DIGITAL_INPUT_3			= OUTPUT_DUMMY; // Y102
	const int OUT_ILLUMINATION13_DIGITAL_INPUT_4			= OUTPUT_DUMMY; // Y103
	const int OUT_ILLUMINATION13_DIGITAL_INPUT_5			= OUTPUT_DUMMY; // Y104
	const int OUT_ILLUMINATION13_DIGITAL_INPUT_6			= OUTPUT_DUMMY; // Y105
	const int OUT_ILLUMINATION13_DIGITAL_INPUT_7			= OUTPUT_DUMMY; // Y106
	const int OUT_ILLUMINATION13_DIGITAL_INPUT_8			= OUTPUT_DUMMY; // Y107

	const int OUT_ILLUMINATION14_DIGITAL_INPUT_1			= OUTPUT_DUMMY; // Y108
	const int OUT_ILLUMINATION14_DIGITAL_INPUT_2			= OUTPUT_DUMMY; // Y109
	const int OUT_ILLUMINATION14_DIGITAL_INPUT_3			= OUTPUT_DUMMY; // Y10A
	const int OUT_ILLUMINATION14_DIGITAL_INPUT_4			= OUTPUT_DUMMY; // Y10B
	const int OUT_ILLUMINATION14_DIGITAL_INPUT_5			= OUTPUT_DUMMY; // Y10C
	const int OUT_ILLUMINATION14_DIGITAL_INPUT_6			= OUTPUT_DUMMY; // Y10D
	const int OUT_ILLUMINATION14_DIGITAL_INPUT_7			= OUTPUT_DUMMY; // Y10E
	const int OUT_ILLUMINATION14_DIGITAL_INPUT_8			= OUTPUT_DUMMY; // Y10F
//____________________

#endif

	const int OUT_IONAIZER_BLOW_ONOFF				= OUTPUT_DUMMY;



#endif // USEIO_H
