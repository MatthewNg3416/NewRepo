# Microsoft Developer Studio Project File - Name="MPlatformOlb" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MPlatformOlb - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MPlatformOlb.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MPlatformOlb.mak" CFG="MPlatformOlb - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MPlatformOlb - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MPlatformOlb - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "MPlatformOlb"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MPlatformOlb - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".\Component\P_TrsInspectionCamera" /I ".\Component\C_CtrlInspectionCamera" /I "..\Common\Src\H_Melsec" /I "..\Common\Src\H_Serial" /I "..\Common\Src\H_Lighting" /I "..\Common\Src\H_Inverter" /I "..\Common\Src\H_Motion" /I "..\Common\Src\H_Motion\H_SERCOS" /I "..\Common\Src\H_Motion\H_MMC" /I "..\Common\Src\H_Motion\H_CAN" /I "..\Common\Src\H_Motion\H_ACS" /I "..\Common\LIB\MMC" /I "..\Common\LIB\NMC" /I "..\Common\LIB\CAN" /I "..\Common\LIB\ACS" /I "..\Common\LIB\CAN\INC" /I ".\Component\M_TabAligner" /I "..\Common\Src\M_PanelTransfer" /I "..\Common\Src\P_TrsPanelTransfer" /I "..\Common\Src\P_TrsLoadConveyor" /I "..\Common\Src\P_TrsUnloadConveyor" /I ".\Component\P_TrsTabMounter" /I ".\Component\C_CtrlTabMounter" /I ".\Component\M_TabMounter" /I ".\Component\M_RFID" /I "..\Common\Src\M_RollerConveyor" /I ".\Component\System" /I "..\Common\src\M_OpPanel" /I "..\Common\LIB\MIL\include" /I "..\Common\LIB\MIL\lib" /I "..\Common\src\H_Vision" /I "..\Common\src\_Utility\MTickTimer" /I ".\\" /I "..\Common\src\_Utility\MFileUtil" /I "..\Common\src\_Utility\ViewUtil" /I "..\Common\src\_Utility\ETC_Util" /I "..\Common\src\_Utility\FileManager" /I "..\Common\src\_Utility\MLog" /I "..\Common\src\_CommonIF" /I "..\Common\LIB\HilscherDnet" /I "..\Common\src\H_IO" /I "..\Common\src\H_Actuator" /I "..\Common\src\H_Serial" /I "..\Common\src\_Utility\MPosition" /I "..\Common\Src\M_PanelAligner" /I "..\Common\Src\M_Online" /I "..\Common\Src\M_ModelChange" /I "..\Common\src\_Utility\MBaseClass" /I "..\Common\src\C_ManageOpPanel" /I ".\Component\M_TabCarrier" /I ".\Component\P_TrsAutoManager" /I ".\Component\P_TrsPanelAligner" /I "..\Common\Src\C_CtrlPanelAligner" /I "..\Common\src\C_VisionCalibration" /I "..\Common\src\P_TrsJog" /I ".\Component\M_CameraCarrier" /I ".\Component\M_TabFeeder" /I ".\Component\C_CtrlTabFeeder" /I ".\Component\C_CtrlTabCarrier" /I ".\Component\P_TrsTabFeeder" /I ".\Component\P_TrsTabCarrier" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"Release/Pre_v2.1ngn.exe"

!ELSEIF  "$(CFG)" == "MPlatformOlb - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\DataBase" /I ".\DataBase\TabICLoss" /I ".\Component\P_TrsInspectionCamera" /I ".\Component\C_CtrlInspectionCamera" /I "..\Common\Src\H_Melsec" /I "..\Common\Src\H_Serial" /I "..\Common\Src\H_Lighting" /I "..\Common\Src\H_Inverter" /I "..\Common\Src\H_Motion" /I "..\Common\Src\H_Motion\H_SERCOS" /I "..\Common\Src\H_Motion\H_MMC" /I "..\Common\Src\H_Motion\H_CAN" /I "..\Common\Src\H_Motion\H_ACS" /I "..\Common\LIB\MMC" /I "..\Common\LIB\NMC" /I "..\Common\LIB\CAN" /I "..\Common\LIB\ACS" /I "..\Common\LIB\CAN\INC" /I ".\Component\M_TabAligner" /I "..\Common\Src\M_PanelTransfer" /I "..\Common\Src\P_TrsPanelTransfer" /I "..\Common\Src\P_TrsLoadConveyor" /I "..\Common\Src\P_TrsUnloadConveyor" /I ".\Component\P_TrsTabMounter" /I ".\Component\P_TrsTabMounterSchedule" /I ".\Component\C_CtrlTabMounter" /I ".\Component\M_TabMounter" /I ".\Component\M_RFID" /I "..\Common\Src\M_RollerConveyor" /I ".\Component\System" /I "..\Common\src\M_OpPanel" /I "..\Common\LIB\MIL\include" /I "..\Common\LIB\MIL\lib" /I "..\Common\src\H_Vision" /I "..\Common\src\_Utility\MTickTimer" /I ".\\" /I "..\Common\src\_Utility\MFileUtil" /I "..\Common\src\_Utility\ViewUtil" /I "..\Common\src\_Utility\ETC_Util" /I "..\Common\src\_Utility\FileManager" /I "..\Common\src\_Utility\MLog" /I "..\Common\src\_CommonIF" /I "..\Common\LIB\HilscherDnet" /I "..\Common\src\H_IO" /I "..\Common\src\H_Actuator" /I "..\Common\src\H_Serial" /I "..\Common\src\_Utility\MPosition" /I "..\Common\Src\M_PanelAligner" /I "..\Common\Src\M_Online" /I "..\Common\Src\M_ModelChange" /I "..\Common\src\_Utility\MBaseClass" /I "..\Common\src\C_ManageOpPanel" /I ".\Component\M_TabCarrier" /I ".\Component\P_TrsAutoManager" /I ".\Component\P_TrsPanelAligner" /I "..\Common\Src\C_CtrlPanelAligner" /I "..\Common\src\C_VisionCalibration" /I "..\Common\src\C_Teaching" /I "..\Common\src\P_TrsJog" /I ".\Component\M_CameraCarrier" /I ".\Component\M_TabFeeder" /I ".\Component\C_CtrlTabFeeder" /I ".\Component\C_CtrlTabCarrier" /I ".\Component\P_TrsTabFeeder" /I ".\Component\P_TrsTabCarrierSchedule" /I ".\Component\P_TrsTabCarrier" /I "..\Common\src\P_TrsPanelTransferCenter" /I "..\Common\src\P_TrsPanelTransferOut" /I "..\Common\src\H_Motion\H_MRS_NMC" /I "..\Common\LIB\MRS_NMC" /I ".\Component\M_Handler" /I ".\Component\C_CtrlHandler" /I ".\Component\P_TrsTHandler" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "NO_SIMULATION" /D "NO_VISION_SIMULATION" /D "NO_ACS_SIMULATION" /FR /Yu"stdafx.h" /FD /I /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "MPlatformOlb - Win32 Release"
# Name "MPlatformOlb - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AutoDoorStateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoElapseTimeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoInspectJudgeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoMainForm.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoMaterialCmdGroupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoMaterialInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoModeSelectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoMotorOriginDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoNSMCInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoOperationCmdGroupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoPanelMarkManualSearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoRepairTabSelectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoScrapGlassDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoStepDispDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoTabICInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoZigInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BottomForm.cpp
# End Source File
# Begin Source File

SOURCE=.\btnenh.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonErrMsgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonEStopNotifyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonGetNumDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonGetValueDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonMatCodeCheckDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonMsgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonNotifyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonOPCallDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonPendantNotifyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonSelectUserGroupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonStepStopNotifyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataActuatorSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataCommonDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataECDBDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataFuncOnOffDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataMainForm.cpp
# End Source File
# Begin Source File

SOURCE=.\DataMatchingTableDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataModelChangeDistDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataModelDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataModelDlgNew.cpp
# End Source File
# Begin Source File

SOURCE=.\DataModelIDList.cpp
# End Source File
# Begin Source File

SOURCE=.\DataMoldInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataPanelAlignerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataPanelDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataSearchMajorSpec.cpp
# End Source File
# Begin Source File

SOURCE=.\DataTabICDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataTabICImageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataTabICOffsetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataTabMounterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataTabOffsetImageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataToolOffsetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataTowerLampDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFixedVision.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSheetMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSheetTactTime.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTabICLoss.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTabICLoss.h
# End Source File
# Begin Source File

SOURCE=.\DlgTabOffsetTable.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgViewWorkSchedule.cpp
# End Source File
# Begin Source File

SOURCE=.\EqStopCodeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorNotSendDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\LogConfigDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LogFileSelectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LogMainForm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ManualCameraExpShrDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ManualHandlerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ManualIOMonitoringDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ManualLimitSensorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ManualMainForm.cpp
# End Source File
# Begin Source File

SOURCE=.\ManualMarkRecogDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ManualPanelAlignerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ManualPanelTransferDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ManualTabCarrierDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ManualTabFeederDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ManualTabMounterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MaterialChangeDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\C_Teaching\MHW_Teach.cpp
# End Source File
# Begin Source File

SOURCE=.\MLCNetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MNetHDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelNDeviceIDListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ModulePortIDDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MoldZigInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\MPlatformOlb.cpp
# End Source File
# Begin Source File

SOURCE=.\MPlatformOlb.rc
# End Source File
# Begin Source File

SOURCE=.\MPlatformOlbDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\MPlatformOlbView.cpp
# End Source File
# Begin Source File

SOURCE=.\msflexgrid.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiViewSplitter.cpp
# End Source File
# Begin Source File

SOURCE=.\PageEtcTact.cpp
# End Source File
# Begin Source File

SOURCE=.\PageMaterialInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\PageMCodeInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\PageMCodeInfo2ND.cpp
# End Source File
# Begin Source File

SOURCE=.\PageTabCarrierTact.cpp
# End Source File
# Begin Source File

SOURCE=.\PageTabMounterTact.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelValidationCheckDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\picture.cpp
# End Source File
# Begin Source File

SOURCE=.\PlateVacModelDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PMFullDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\ProductCreateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProductPlanDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\rowcursor.cpp
# End Source File
# Begin Source File

SOURCE=.\simpleAdo.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\surfacecolor.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemBtnDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemFourBtnDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemThreeBtnDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TeachBadTabDetectorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TeachCameraCarrierDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TeachCarrierCameraExpandDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TeachCarrierDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TeachHandlerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TeachMainForm.cpp
# End Source File
# Begin Source File

SOURCE=.\TeachModelChangeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TeachMoldPinDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TeachPanelAlignerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TeachPanelAlignerOffsetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TeachPressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TeachTabMounterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TeachTransferDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\textdescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\texture.cpp
# End Source File
# Begin Source File

SOURCE=.\TitleForm.cpp
# End Source File
# Begin Source File

SOURCE=.\VersionDisplayDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VisionBlobThresholdDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VisionCalibDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VisionIlluminationDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VisionJogKeyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VisionMonitorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VisionOptionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VisionSearchAreaDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VisionSelectMarkDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VisionViewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.cpp
# End Source File
# Begin Source File

SOURCE=.\XHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\XHeaderCtrl.h
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AutoDoorStateDlg.h
# End Source File
# Begin Source File

SOURCE=.\AutoElapseTimeDlg.h
# End Source File
# Begin Source File

SOURCE=.\AutoInspectJudgeDlg.h
# End Source File
# Begin Source File

SOURCE=.\AutoMainForm.h
# End Source File
# Begin Source File

SOURCE=.\AutoMaterialCmdGroupDlg.h
# End Source File
# Begin Source File

SOURCE=.\AutoMaterialInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\AutoModeSelectDlg.h
# End Source File
# Begin Source File

SOURCE=.\AutoMotorOriginDlg.h
# End Source File
# Begin Source File

SOURCE=.\AutoNSMCInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\AutoOperationCmdGroupDlg.h
# End Source File
# Begin Source File

SOURCE=.\AutoPanelMarkManualSearchDlg.h
# End Source File
# Begin Source File

SOURCE=.\AutoRepairTabSelectDlg.h
# End Source File
# Begin Source File

SOURCE=.\AutoScrapGlassDlg.h
# End Source File
# Begin Source File

SOURCE=.\AutoStepDispDlg.h
# End Source File
# Begin Source File

SOURCE=.\AutoTabICInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\AutoZigInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\BottomForm.h
# End Source File
# Begin Source File

SOURCE=.\btnenh.h
# End Source File
# Begin Source File

SOURCE=.\CommonErrMsgDlg.h
# End Source File
# Begin Source File

SOURCE=.\CommonEStopNotifyDlg.h
# End Source File
# Begin Source File

SOURCE=.\CommonGetNumDlg.h
# End Source File
# Begin Source File

SOURCE=.\CommonGetValueDlg.h
# End Source File
# Begin Source File

SOURCE=.\CommonMatCodeCheckDlg.h
# End Source File
# Begin Source File

SOURCE=.\CommonMsgDlg.h
# End Source File
# Begin Source File

SOURCE=.\CommonNotifyDlg.h
# End Source File
# Begin Source File

SOURCE=.\CommonOPCallDlg.h
# End Source File
# Begin Source File

SOURCE=.\CommonPendantNotifyDlg.h
# End Source File
# Begin Source File

SOURCE=.\CommonSelectUserGroupDlg.h
# End Source File
# Begin Source File

SOURCE=.\CommonStepStopNotifyDlg.h
# End Source File
# Begin Source File

SOURCE=.\DataActuatorSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\DataCommonDlg.h
# End Source File
# Begin Source File

SOURCE=.\DataECDBDlg.h
# End Source File
# Begin Source File

SOURCE=.\DataFuncOnOffDlg.h
# End Source File
# Begin Source File

SOURCE=.\DataMainForm.h
# End Source File
# Begin Source File

SOURCE=.\DataMatchingTableDlg.h
# End Source File
# Begin Source File

SOURCE=.\DataModelChangeDistDlg.h
# End Source File
# Begin Source File

SOURCE=.\DataModelDlg.h
# End Source File
# Begin Source File

SOURCE=.\DataModelDlgNew.h
# End Source File
# Begin Source File

SOURCE=.\DataModelIDList.h
# End Source File
# Begin Source File

SOURCE=.\DataMoldInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\DataPanelAlignerDlg.h
# End Source File
# Begin Source File

SOURCE=.\DataPanelDlg.h
# End Source File
# Begin Source File

SOURCE=.\DataSearchMajorSpec.h
# End Source File
# Begin Source File

SOURCE=.\DataTabICDlg.h
# End Source File
# Begin Source File

SOURCE=.\DataTabICImageDlg.h
# End Source File
# Begin Source File

SOURCE=.\DataTabICOffsetDlg.h
# End Source File
# Begin Source File

SOURCE=.\DataTabMounterDlg.h
# End Source File
# Begin Source File

SOURCE=.\DataTabOffsetImageDlg.h
# End Source File
# Begin Source File

SOURCE=.\DataToolOffsetDlg.h
# End Source File
# Begin Source File

SOURCE=.\DataTowerLampDlg.h
# End Source File
# Begin Source File

SOURCE=.\DlgFixedVision.h
# End Source File
# Begin Source File

SOURCE=.\DlgSheetMaterial.h
# End Source File
# Begin Source File

SOURCE=.\DlgSheetTactTime.h
# End Source File
# Begin Source File

SOURCE=.\DlgTabOffsetTable.h
# End Source File
# Begin Source File

SOURCE=.\DlgViewWorkSchedule.h
# End Source File
# Begin Source File

SOURCE=.\EqStopCodeDlg.h
# End Source File
# Begin Source File

SOURCE=.\ErrorNotSendDlg.h
# End Source File
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\LogConfigDlg.h
# End Source File
# Begin Source File

SOURCE=.\LogFileSelectDlg.h
# End Source File
# Begin Source File

SOURCE=.\LogMainForm.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\ManualCameraExpShrDlg.h
# End Source File
# Begin Source File

SOURCE=.\ManualHandlerDlg.h
# End Source File
# Begin Source File

SOURCE=.\ManualIOMonitoringDlg.h
# End Source File
# Begin Source File

SOURCE=.\ManualLimitSensorDlg.h
# End Source File
# Begin Source File

SOURCE=.\ManualMainForm.h
# End Source File
# Begin Source File

SOURCE=.\ManualMarkRecogDlg.h
# End Source File
# Begin Source File

SOURCE=.\ManualPanelAlignerDlg.h
# End Source File
# Begin Source File

SOURCE=.\ManualPanelTransferDlg.h
# End Source File
# Begin Source File

SOURCE=.\ManualTabCarrierDlg.h
# End Source File
# Begin Source File

SOURCE=.\ManualTabFeederDlg.h
# End Source File
# Begin Source File

SOURCE=.\ManualTabMounterDlg.h
# End Source File
# Begin Source File

SOURCE=.\MaterialChangeDlg.h
# End Source File
# Begin Source File

SOURCE=.\MLCNetDlg.h
# End Source File
# Begin Source File

SOURCE=.\MNetHDlg.h
# End Source File
# Begin Source File

SOURCE=.\ModelNDeviceIDListDlg.h
# End Source File
# Begin Source File

SOURCE=.\ModulePortIDDlg.h
# End Source File
# Begin Source File

SOURCE=.\MoldZigInfo.h
# End Source File
# Begin Source File

SOURCE=.\MPlatformOlb.h
# End Source File
# Begin Source File

SOURCE=.\MPlatformOlbDoc.h
# End Source File
# Begin Source File

SOURCE=.\MPlatformOlbView.h
# End Source File
# Begin Source File

SOURCE=.\msflexgrid.h
# End Source File
# Begin Source File

SOURCE=.\MultiViewSplitter.h
# End Source File
# Begin Source File

SOURCE=.\PageEtcTact.h
# End Source File
# Begin Source File

SOURCE=.\PageMaterialInfo.h
# End Source File
# Begin Source File

SOURCE=.\PageMCodeInfo.h
# End Source File
# Begin Source File

SOURCE=.\PageMCodeInfo2ND.h
# End Source File
# Begin Source File

SOURCE=.\PageTabCarrierTact.h
# End Source File
# Begin Source File

SOURCE=.\PageTabMounterTact.h
# End Source File
# Begin Source File

SOURCE=.\PanelValidationCheckDlg.h
# End Source File
# Begin Source File

SOURCE=.\picture.h
# End Source File
# Begin Source File

SOURCE=.\PlateVacModelDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\PMFullDisplay.h
# End Source File
# Begin Source File

SOURCE=.\ProductCreateDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProductPlanDlg.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\rowcursor.h
# End Source File
# Begin Source File

SOURCE=.\simpleAdo.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\surfacecolor.h
# End Source File
# Begin Source File

SOURCE=.\SystemBtnDlg.h
# End Source File
# Begin Source File

SOURCE=.\SystemFourBtnDlg.h
# End Source File
# Begin Source File

SOURCE=.\SystemThreeBtnDlg.h
# End Source File
# Begin Source File

SOURCE=.\TeachBadTabDetectorDlg.h
# End Source File
# Begin Source File

SOURCE=.\TeachCameraCarrierDlg.h
# End Source File
# Begin Source File

SOURCE=.\TeachCarrierCameraExpandDlg.h
# End Source File
# Begin Source File

SOURCE=.\TeachCarrierDlg.h
# End Source File
# Begin Source File

SOURCE=.\TeachHandlerDlg.h
# End Source File
# Begin Source File

SOURCE=.\TeachMainForm.h
# End Source File
# Begin Source File

SOURCE=.\TeachModelChangeDlg.h
# End Source File
# Begin Source File

SOURCE=.\TeachMoldPinDlg.h
# End Source File
# Begin Source File

SOURCE=.\TeachPanelAlignerDlg.h
# End Source File
# Begin Source File

SOURCE=.\TeachPanelAlignerOffsetDlg.h
# End Source File
# Begin Source File

SOURCE=.\TeachPressDlg.h
# End Source File
# Begin Source File

SOURCE=.\TeachTabMounterDlg.h
# End Source File
# Begin Source File

SOURCE=.\TeachTransferDlg.h
# End Source File
# Begin Source File

SOURCE=.\textdescriptor.h
# End Source File
# Begin Source File

SOURCE=.\texture.h
# End Source File
# Begin Source File

SOURCE=.\TitleForm.h
# End Source File
# Begin Source File

SOURCE=.\VersionDisplayDlg.h
# End Source File
# Begin Source File

SOURCE=.\VisionBlobThresholdDlg.h
# End Source File
# Begin Source File

SOURCE=.\VisionCalibDlg.h
# End Source File
# Begin Source File

SOURCE=.\VisionIlluminationDlg.h
# End Source File
# Begin Source File

SOURCE=.\VisionJogKeyDlg.h
# End Source File
# Begin Source File

SOURCE=.\VisionMonitorDlg.h
# End Source File
# Begin Source File

SOURCE=.\VisionOptionDlg.h
# End Source File
# Begin Source File

SOURCE=.\VisionSearchAreaDlg.h
# End Source File
# Begin Source File

SOURCE=.\VisionSelectMarkDlg.h
# End Source File
# Begin Source File

SOURCE=.\VisionViewDlg.h
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor2.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor3.cur
# End Source File
# Begin Source File

SOURCE=.\res\erricon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\infoicon.bmp
# End Source File
# Begin Source File

SOURCE=..\Common\src\_Utility\ETC_Util\JAS_Tech_Logo.png
# End Source File
# Begin Source File

SOURCE=.\res\JAS_Tech_Title.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LedOff.ico
# End Source File
# Begin Source File

SOURCE=.\res\LedOn.ico
# End Source File
# Begin Source File

SOURCE=.\res\MPlatformOlb.ico
# End Source File
# Begin Source File

SOURCE=.\res\quesicon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\stopicon.bmp
# End Source File
# Begin Source File

SOURCE=".\res\X변.jpg"
# End Source File
# Begin Source File

SOURCE=".\res\Y변.jpg"
# End Source File
# Begin Source File

SOURCE=".\res\전부선택.ico"
# End Source File
# End Group
# Begin Group "System"

# PROP Default_Filter ""
# Begin Group "Data"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Component\System\MAutoAlignData.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\System\MAutoAlignData.h
# End Source File
# Begin Source File

SOURCE=.\Component\System\MPanelData.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\System\MPanelData.h
# End Source File
# Begin Source File

SOURCE=.\Component\System\MPreBonderData.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\System\MPreBonderData.h
# End Source File
# Begin Source File

SOURCE=.\Component\System\MProcessData.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\System\MProcessData.h
# End Source File
# Begin Source File

SOURCE=.\Component\System\MProductInfoData.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\System\MProductInfoData.h
# End Source File
# Begin Source File

SOURCE=.\Component\System\MSystemData.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\System\MSystemData.h
# End Source File
# Begin Source File

SOURCE=.\Component\System\MSystemInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\System\MSystemInfo.h
# End Source File
# Begin Source File

SOURCE=.\Component\System\MTabData.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\System\MTabData.h
# End Source File
# Begin Source File

SOURCE=.\Component\System\MTabOffsetData.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\System\MTabOffsetData.h
# End Source File
# Begin Source File

SOURCE=.\Component\System\MTeachingInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\System\MTeachingInfo.h
# End Source File
# Begin Source File

SOURCE=.\Component\System\MTeachingPosData.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\System\MTeachingPosData.h
# End Source File
# Begin Source File

SOURCE=.\Component\System\MTowerLampData.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\System\MTowerLampData.h
# End Source File
# End Group
# Begin Group "Inf"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Info\IO_Gate_1st.dat
# End Source File
# Begin Source File

SOURCE=.\Info\IO_Gate_2nd.dat
# End Source File
# Begin Source File

SOURCE=.\Info\IO_Source_1st.dat
# End Source File
# Begin Source File

SOURCE=.\Info\IO_Source_2nd.dat
# End Source File
# Begin Source File

SOURCE=.\Info\IO_Source_3rd.dat
# End Source File
# Begin Source File

SOURCE=.\Info\PosInfoPreBonder_Gate.inf
# End Source File
# Begin Source File

SOURCE=.\Info\PosInfoPreBonder_Source.inf
# End Source File
# Begin Source File

SOURCE=.\Info\VisionMark.inf
# End Source File
# End Group
# Begin Group "DataBase"

# PROP Default_Filter ""
# Begin Group "TabICLoss"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DataBase\TabICLoss\DBTabLoss.cpp
# End Source File
# Begin Source File

SOURCE=.\DataBase\TabICLoss\DBTabLoss.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\DataBase\AdoControl.cpp
# End Source File
# Begin Source File

SOURCE=.\DataBase\AdoControl.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Component\System\DefActuatorPreBonder.h
# End Source File
# Begin Source File

SOURCE=.\Component\System\DefAxisPreBonder.h
# End Source File
# Begin Source File

SOURCE=.\Component\System\DefIOAddrPreBonder.h
# End Source File
# Begin Source File

SOURCE=.\Component\System\DefSystem.h
# End Source File
# Begin Source File

SOURCE=.\Component\System\DefSystemConfig.h
# End Source File
# Begin Source File

SOURCE=.\Component\System\MPlatformOlbSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\System\MPlatformOlbSystem.h
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Group "MFileUtil"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\Common\src\_Utility\MFileUtil\MFileUtil.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common\src\_Utility\MFileUtil\MFileUtil.h"
# End Source File
# End Group
# Begin Group "MLog"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\Common\src\_Utility\MLog\MLog.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common\src\_Utility\MLog\MLog.h"
# End Source File
# End Group
# Begin Group "MPosition"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\Common\src\_Utility\MPosition\MPos_XY.h"
# End Source File
# Begin Source File

SOURCE="..\Common\src\_Utility\MPosition\MPos_XYT.h"
# End Source File
# Begin Source File

SOURCE="..\Common\src\_Utility\MPosition\MPos_XYTZ.h"
# End Source File
# End Group
# Begin Group "MTickTimer"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\Common\src\_Utility\MTickTimer\MTickTimer.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common\src\_Utility\MTickTimer\MTickTimer.h"
# End Source File
# End Group
# Begin Group "MFileManager"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\Common\src\_Utility\FileManager\MFileManager.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common\src\_Utility\FileManager\MFileManager.h"
# End Source File
# End Group
# Begin Group "MViewUtil"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\Common\src\_Utility\ViewUtil\Gradient.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common\src\_Utility\ViewUtil\Gradient.h"
# End Source File
# Begin Source File

SOURCE="..\Common\src\_Utility\ViewUtil\LedButton.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common\src\_Utility\ViewUtil\LedButton.h"
# End Source File
# Begin Source File

SOURCE="..\Common\src\_Utility\ViewUtil\MyMsgWnd.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common\src\_Utility\ViewUtil\MyMsgWnd.h"
# End Source File
# Begin Source File

SOURCE="..\Common\src\_Utility\ViewUtil\MyProgressWnd.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common\src\_Utility\ViewUtil\MyProgressWnd.h"
# End Source File
# Begin Source File

SOURCE="..\Common\src\_Utility\ViewUtil\Mywnd.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common\src\_Utility\ViewUtil\Mywnd.h"
# End Source File
# Begin Source File

SOURCE="..\Common\src\_Utility\ViewUtil\Splash.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common\src\_Utility\ViewUtil\Splash.h"
# End Source File
# Begin Source File

SOURCE="..\Common\src\_Utility\ViewUtil\TBalloon.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common\src\_Utility\ViewUtil\TBalloon.h"
# End Source File
# End Group
# Begin Group "MEtcUtil"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\Common\src\_Utility\ETC_Util\Common.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common\src\_Utility\ETC_Util\common.h"
# End Source File
# Begin Source File

SOURCE=..\Common\src\_Utility\ETC_Util\LanguageManager.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\_Utility\ETC_Util\LanguageManager.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\_Utility\ETC_Util\MDebug.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\_Utility\ETC_Util\MDebug.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\_Utility\ETC_Util\MString.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\_Utility\ETC_Util\MString.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\_Utility\ETC_Util\UtilManager.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\_Utility\ETC_Util\UtilManager.h
# End Source File
# End Group
# End Group
# Begin Group "I_COMMON"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\Common\src\_CommonIF\ICommon.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common\src\_CommonIF\ICommon.h"
# End Source File
# End Group
# Begin Group "Process Layer"

# PROP Default_Filter ""
# Begin Group "P_TrsJog"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\Common\src\P_TrsJog\MTrsJog.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common\src\P_TrsJog\MTrsJog.h"
# End Source File
# End Group
# Begin Group "P_TrsTabFeeder"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Component\P_TrsTabFeeder\ITI_TabFeeder.h
# End Source File
# Begin Source File

SOURCE=.\Component\P_TrsTabFeeder\MTrsTabFeeder.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\P_TrsTabFeeder\MTrsTabFeeder.h
# End Source File
# End Group
# Begin Group "P_TrsTabMounter"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Component\P_TrsTabMounter\ITI_TabMounter.h
# End Source File
# Begin Source File

SOURCE=.\Component\P_TrsTabMounter\MTrsTabMounter.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\P_TrsTabMounter\MTrsTabMounter.h
# End Source File
# End Group
# Begin Group "P_TrsTabCarrier"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Component\P_TrsTabCarrier\ITI_TabCarrier.h
# End Source File
# Begin Source File

SOURCE=.\Component\P_TrsTabCarrier\MTrsTabCarrier.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\P_TrsTabCarrier\MTrsTabCarrier.h
# End Source File
# End Group
# Begin Group "P_TrsPanelTransfer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\src\P_TrsPanelTransfer\ITI_PanelTransfer.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\P_TrsPanelTransfer\MTrsPanelTransfer.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\P_TrsPanelTransfer\MTrsPanelTransfer.h
# End Source File
# End Group
# Begin Group "P_TrsPanelAligner"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Component\P_TrsPanelAligner\ITI_PanelAligner.h
# End Source File
# Begin Source File

SOURCE=.\Component\P_TrsPanelAligner\MTrsPanelAligner.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\P_TrsPanelAligner\MTrsPanelAligner.h
# End Source File
# End Group
# Begin Group "P_TrsAutoManager"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Component\P_TrsAutoManager\MTrsAutoManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\P_TrsAutoManager\MTrsAutoManager.h
# End Source File
# End Group
# Begin Group "P_TrsInspectionCamera"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Component\P_TrsInspectionCamera\ITI_InspectionCamera.h
# End Source File
# Begin Source File

SOURCE=.\Component\P_TrsInspectionCamera\MTrsInspectionCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\P_TrsInspectionCamera\MTrsInspectionCamera.h
# End Source File
# End Group
# Begin Group "P_TrsPanelTransferOut"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\src\P_TrsPanelTransferOut\ITI_PanelTransferOut.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\P_TrsPanelTransferOut\MTrsPanelTransferOut.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\P_TrsPanelTransferOut\MTrsPanelTransferOut.h
# End Source File
# End Group
# Begin Group "P_TrsPanelTransferCenter"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\src\P_TrsPanelTransferCenter\ITI_PanelTransferCenter.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\P_TrsPanelTransferCenter\MTrsPanelTransferCenter.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\P_TrsPanelTransferCenter\MTrsPanelTransferCenter.h
# End Source File
# End Group
# Begin Group "P_TrsTHandler"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Component\P_TrsTHandler\ITI_THandler.h
# End Source File
# Begin Source File

SOURCE=.\Component\P_TrsTHandler\MTrsTHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\P_TrsTHandler\MTrsTHandler.h
# End Source File
# End Group
# End Group
# Begin Group "Mechanical Layer"

# PROP Default_Filter ""
# Begin Group "M_OpPanel"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\Common\src\M_OpPanel\DefOpPanel.h"
# End Source File
# Begin Source File

SOURCE="..\Common\src\M_OpPanel\IOpPanel.h"
# End Source File
# Begin Source File

SOURCE="..\Common\src\M_OpPanel\MOpPanel.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common\src\M_OpPanel\MOpPanel.h"
# End Source File
# End Group
# Begin Group "M_TabCarrier"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Component\M_TabCarrier\DefTabCarrier.h
# End Source File
# Begin Source File

SOURCE=.\Component\M_TabCarrier\IStateTabCarrier.h
# End Source File
# Begin Source File

SOURCE=.\Component\M_TabCarrier\MTabCarrier.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\M_TabCarrier\MTabCarrier.h
# End Source File
# End Group
# Begin Group "M_TabFeeder"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Component\M_TabFeeder\DefTabFeeder.h
# End Source File
# Begin Source File

SOURCE=.\Component\M_TabFeeder\IStateTabFeeder.h
# End Source File
# Begin Source File

SOURCE=.\Component\M_TabFeeder\MTabFeeder.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\M_TabFeeder\MTabFeeder.h
# End Source File
# End Group
# Begin Group "M_CameraCarrier"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Component\M_CameraCarrier\DefCameraCarrier.h
# End Source File
# Begin Source File

SOURCE=.\Component\M_CameraCarrier\IStateCameraCarrier.h
# End Source File
# Begin Source File

SOURCE=.\Component\M_CameraCarrier\MCameraCarrier.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\M_CameraCarrier\MCameraCarrier.h
# End Source File
# End Group
# Begin Group "M_RollerConveyor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\src\M_RollerConveyor\DefRollerConveyor.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\M_RollerConveyor\IStateRollerConveyor.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\M_RollerConveyor\MRollerConveyor.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\M_RollerConveyor\MRollerConveyor.h
# End Source File
# End Group
# Begin Group "M_TabMounter"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Component\M_TabMounter\DefTabMounter.h
# End Source File
# Begin Source File

SOURCE=.\Component\M_TabMounter\IStateTabMounter.h
# End Source File
# Begin Source File

SOURCE=.\Component\M_TabMounter\MTabMounter.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\M_TabMounter\MTabMounter.h
# End Source File
# End Group
# Begin Group "M_TabAligner"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Component\M_TabAligner\DefTabAligner.h
# End Source File
# Begin Source File

SOURCE=.\Component\M_TabAligner\IStateTabAligner.h
# End Source File
# Begin Source File

SOURCE=.\Component\M_TabAligner\MTabAligner.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\M_TabAligner\MTabAligner.h
# End Source File
# End Group
# Begin Group "M_PanelTransfer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\src\M_PanelTransfer\DefPanelTransfer.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\M_PanelTransfer\IStatePanelTransfer.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\M_PanelTransfer\MPanelTransfer.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\M_PanelTransfer\MPanelTransfer.h
# End Source File
# End Group
# Begin Group "M_PanelAligner"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\src\M_PanelAligner\DefPanelAligner.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\M_PanelAligner\IStatePanelAligner.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\M_PanelAligner\MPanelAligner.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\M_PanelAligner\MPanelAligner.h
# End Source File
# End Group
# Begin Group "M_ModelChange"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\src\M_ModelChange\DefModelChange.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\M_ModelChange\IStateModelChange.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\M_ModelChange\MModelChange.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\M_ModelChange\MModelChange.h
# End Source File
# End Group
# Begin Group "M_RFID"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Component\M_RFID\DefRFID.h
# End Source File
# Begin Source File

SOURCE=.\Component\M_RFID\IRFID.h
# End Source File
# Begin Source File

SOURCE=.\Component\M_RFID\MRFID.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\M_RFID\MRFID.h
# End Source File
# End Group
# Begin Group "M_Online"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\src\M_Online\MLCNet.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\M_Online\MLCNet.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\M_Online\MLCNetData.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\M_Online\MLCNetData.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\M_Online\xnet.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\M_Online\xnet.h
# End Source File
# End Group
# Begin Group "M_Handler"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Component\M_Handler\DefHandler.h
# End Source File
# Begin Source File

SOURCE=.\Component\M_Handler\DefHandlerCommon.h
# End Source File
# Begin Source File

SOURCE=.\Component\M_Handler\IStateHandler.h
# End Source File
# Begin Source File

SOURCE=.\Component\M_Handler\MHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\M_Handler\MHandler.h
# End Source File
# Begin Source File

SOURCE=.\Component\M_Handler\MHandlerCommon.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\M_Handler\MHandlerCommon.h
# End Source File
# End Group
# End Group
# Begin Group "Hardware Layer"

# PROP Default_Filter ""
# Begin Group "H_Motion"

# PROP Default_Filter ""
# Begin Group "H_SERCOS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_SERCOS\ISercosMultiAxis.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_SERCOS\ISercosSetup.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_SERCOS\MSercosAxis.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_SERCOS\MSercosAxis.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_SERCOS\MSercosMultiAxis.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_SERCOS\MSercosMultiAxis.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_SERCOS\MSercosSetup.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_SERCOS\MSercosSetup.h
# End Source File
# End Group
# Begin Group "H_MMC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_MMC\IMotionLib.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_MMC\MAxis.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_MMC\MAxis.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_MMC\MMMCLib.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_MMC\MMMCLib.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_MMC\MMotionBoard.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_MMC\MMotionBoard.h
# End Source File
# End Group
# Begin Group "H_ACS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_ACS\MACSAxis.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_ACS\MACSAxis.h
# End Source File
# End Group
# Begin Group "H_CAN"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_CAN\MCANAxis.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_CAN\MCANAxis.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_CAN\MCANSetup.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_CAN\MCANSetup.h
# End Source File
# End Group
# Begin Group "H_RS_NMC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_MRS_NMC\IRSNMCMultiAxis.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_MRS_NMC\MRS_NMC_Setup.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_MRS_NMC\MRS_NMC_Setup.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_MRS_NMC\MRS_NMCLib.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_MRS_NMC\MRS_NMCLib.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_MRS_NMC\MRSNMCAxis.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_MRS_NMC\MRSNMCAxis.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_MRS_NMC\MRSNMCMultiAxis.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\H_MRS_NMC\MRSNMCMultiAxis.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\Common\src\H_Motion\DefCommonMotion.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\DefMotionError.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\DefMotionStruct.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\DefMotionValue.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\IAxis.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\MAxisData.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Motion\MAxisData.h
# End Source File
# End Group
# Begin Group "H_Serial"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\Common\src\H_Serial\CQueue.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common\src\H_Serial\CQueue.h"
# End Source File
# Begin Source File

SOURCE="..\Common\src\H_Serial\DefSerialValue.h"
# End Source File
# Begin Source File

SOURCE="..\Common\src\H_Serial\IComThread.h"
# End Source File
# Begin Source File

SOURCE="..\Common\src\H_Serial\MComThread.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common\src\H_Serial\MComThread.h"
# End Source File
# End Group
# Begin Group "H_Actuator"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\Common\src\H_Actuator\ICylinder.h"
# End Source File
# Begin Source File

SOURCE="..\Common\src\H_Actuator\IVacuum.h"
# End Source File
# Begin Source File

SOURCE="..\Common\src\H_Actuator\MCylinder.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common\src\H_Actuator\MCylinder.h"
# End Source File
# Begin Source File

SOURCE="..\Common\src\H_Actuator\MCylinderData.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common\src\H_Actuator\MCylinderData.h"
# End Source File
# Begin Source File

SOURCE="..\Common\src\H_Actuator\MVacuum.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common\src\H_Actuator\MVacuum.h"
# End Source File
# Begin Source File

SOURCE="..\Common\src\H_Actuator\MVacuumData.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common\src\H_Actuator\MVacuumData.h"
# End Source File
# End Group
# Begin Group "H_IO"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\src\H_IO\IIO.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_IO\MHilscherDnet.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_IO\MHilscherDnet.h
# End Source File
# End Group
# Begin Group "H_Vision"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\src\H_Vision\DefVision.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Vision\DefVisionExt.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Vision\IVision.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Vision\Mdisplay.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Vision\Mdisplay.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Vision\MdisplayThread.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Vision\MdisplayThread.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Vision\MdisplayWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Vision\MdisplayWnd.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Vision\Msystem.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Vision\Msystem.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Vision\Msystem_meteor.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Vision\Msystem_meteor.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Vision\Msystem_solios.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Vision\Msystem_solios.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Vision\MVision.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Vision\MVision.h
# End Source File
# End Group
# Begin Group "H_MelsecNet"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\src\H_Melsec\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Melsec\BtnST.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Melsec\Mdfunc.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Melsec\MNetH.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Melsec\MNetH.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Melsec\MNetHData.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Melsec\MNetHData.h
# End Source File
# End Group
# Begin Group "H_Lighting"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\src\H_Lighting\ILighting.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Lighting\MLighting.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Lighting\MLighting.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Lighting\MLighting_ONI.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Lighting\MLighting_ONI.h
# End Source File
# End Group
# Begin Group "H_Inverter"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\src\H_Inverter\IInverter.h
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Inverter\MInverter.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\H_Inverter\MInverter.h
# End Source File
# End Group
# End Group
# Begin Group "Lib"

# PROP Default_Filter ""
# Begin Group "DeviceNet"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\LIB\HilscherDnet\Cif32dll.dll
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\HilscherDnet\CIFUSER.H
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\HilscherDnet\CIF32DLL.LIB
# End Source File
# End Group
# Begin Group "MIL"

# PROP Default_Filter ""
# Begin Group "include"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=..\Common\LIB\MIL\include\Mil.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MIL\include\Mildec.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MIL\include\MilDyn.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MIL\include\Milerr.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MIL\include\MilFunc.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MIL\include\milfuncll.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MIL\include\Milos.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MIL\include\Milproto.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MIL\include\Milsetup.h
# End Source File
# End Group
# Begin Group "library"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\LIB\MIL\lib\mil.lib
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MIL\lib\milblob.lib
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MIL\lib\milcal.lib
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MIL\lib\milcode.lib
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MIL\lib\miledge.lib
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MIL\lib\milim.lib
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MIL\lib\milmeas.lib
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MIL\lib\milmod.lib
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MIL\lib\milmon.lib
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MIL\lib\milocr.lib
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MIL\lib\milpat.lib
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MIL\lib\MilStr.lib
# End Source File
# End Group
# End Group
# Begin Group "Help"

# PROP Default_Filter ""
# End Group
# Begin Group "NMC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\LIB\NMC\smmcdef.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\NMC\SERCOSLib.lib
# End Source File
# End Group
# Begin Group "MMC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\LIB\MMC\MMCDEF.H
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MMC\MMCWFP40.H
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MMC\MMCWHP10.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MMC\MMCWHP10.lib
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MMC\VC60FP40.LIB
# End Source File
# End Group
# Begin Group "ACS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\LIB\ACS\ACSC.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\Acs\ACSCL_x86.LIB
# End Source File
# End Group
# Begin Group "CAN"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\LIB\CAN\Amp.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\AmpFile.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\AmpFW.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\AmpParam.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\AmpPDO.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\AmpPVT.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\AmpStruct.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\AmpUnits.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\Can.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\can_copley.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\can_copley.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\can_kvaser.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\can_kvaser.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\INC\canlib.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CanOpen.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML_Amp.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML_AmpDef.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML_AmpStruct.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML_Can.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML_CanOpen.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML_Copley.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML_Error.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML_ErrorCodes.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML_EventMap.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML_Filter.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML_Firmware.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML_Geometry.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML_IO.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML_Linkage.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML_Node.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML_Path.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML_PDO.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML_SDO.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML_Settings.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML_Threads.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML_Trajectory.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML_TrjScurve.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CML_Utils.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\copley_internal.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\CopleyNode.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\Error.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\EventMap.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\Filter.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\Firmware.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\Geometry.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\IOmodule.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\Linkage.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\LSS.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\Node.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\Path.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\PDO.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\SDO.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\Threads.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\Threads_w32.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\TrjScurve.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\CAN\Utils.cpp
# End Source File
# End Group
# Begin Group "MRS_NMC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\LIB\MRS_NMC\CIPobjEcatMaster.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MRS_NMC\CIPobjEcatSlave.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MRS_NMC\CIPobjEeprom.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MRS_NMC\NMC_Motion.h
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MRS_NMC\NMC_Motion.lib
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MRS_NMC\NMC_Comm.lib
# End Source File
# Begin Source File

SOURCE=..\Common\LIB\MRS_NMC\NMC_Driver.lib
# End Source File
# End Group
# Begin Source File

SOURCE=..\Common\src\H_Melsec\MdFunc32.lib
# End Source File
# End Group
# Begin Group "Message"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Info\ErrorMessage_Chinese.dat
# End Source File
# Begin Source File

SOURCE=.\Info\ErrorMessage_English.dat
# End Source File
# Begin Source File

SOURCE=.\Info\ErrorMessage_Korean.dat
# End Source File
# Begin Source File

SOURCE=.\Info\ErrorMessage_Slovak.dat
# End Source File
# Begin Source File

SOURCE=.\Info\Language_Chinese.dat
# End Source File
# Begin Source File

SOURCE=.\Info\Language_English.dat
# End Source File
# Begin Source File

SOURCE=.\Info\Language_Korean.dat
# End Source File
# Begin Source File

SOURCE=.\Info\Language_Slovak.dat
# End Source File
# End Group
# Begin Group "Control Layer"

# PROP Default_Filter ""
# Begin Group "C_ManageOpPanel"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\Common\src\C_ManageOpPanel\DefManageOpPanel.h"
# End Source File
# Begin Source File

SOURCE="..\Common\src\C_ManageOpPanel\MManageOpPanel.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common\src\C_ManageOpPanel\MManageOpPanel.h"
# End Source File
# End Group
# Begin Group "C_VisionCalibration"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\Common\src\C_VisionCalibration\MVisionCalibration.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common\src\C_VisionCalibration\MVisionCalibration.h"
# End Source File
# Begin Source File

SOURCE="..\Common\src\C_VisionCalibration\MVisionCalibrationData.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common\src\C_VisionCalibration\MVisionCalibrationData.h"
# End Source File
# End Group
# Begin Group "C_CtrlTabFeeder"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Component\C_CtrlTabFeeder\MCtrlTabFeeder.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\C_CtrlTabFeeder\MCtrlTabFeeder.h
# End Source File
# End Group
# Begin Group "C_CtrlTabCarrier"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Component\C_CtrlTabCarrier\MCtrlTabCarrier.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\C_CtrlTabCarrier\MCtrlTabCarrier.h
# End Source File
# End Group
# Begin Group "C_CtrlTabMounter"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Component\C_CtrlTabMounter\MCtrlTabMounter.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\C_CtrlTabMounter\MCtrlTabMounter.h
# End Source File
# End Group
# Begin Group "C_CtrlInspectionCamera"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Component\C_CtrlInspectionCamera\MCtrlInspectionCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\C_CtrlInspectionCamera\MCtrlInspectionCamera.h
# End Source File
# End Group
# Begin Group "C_CtrlPanelAligner"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\src\C_CtrlPanelAligner\MCtrlPanelAligner.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\src\C_CtrlPanelAligner\MCtrlPanelAligner.h
# End Source File
# End Group
# Begin Group "C_Teaching"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\src\C_Teaching\MHW_Teach.h
# End Source File
# End Group
# Begin Group "C_CtrlHandler"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Component\C_CtrlHandler\MCtrlHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Component\C_CtrlHandler\MCtrlHandler.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\History.txt
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=".\작업일지(KKY).txt"
# End Source File
# End Target
# End Project
# Section MPlatformOlb : {A3297BF4-7819-47AC-9B10-B243523C29C7}
# 	2:21:DefaultSinkHeaderFile:xnet.h
# 	2:16:DefaultSinkClass:CXNet
# End Section
# Section MPlatformOlb : {08D6995A-8307-4AD1-BC11-5D3AD044D372}
# 	1:17:IDD_DATA_DISTANCE:127
# 	2:16:Resource Include:resource.h
# 	2:19:DataPanelAlignerDlg.cpp:DataPanelAlignerDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:17:IDD_DATA_DISTANCE:IDD_DATA_DISTANCE
# 	2:17:DataPanelAlignerDlg.h:DataPanelAlignerDlg.h
# 	2:19:Application Include:MPlatformOlb.h
# 	2:23:CLASS: CDataPanelAlignerDlg:CDataPanelAlignerDlg
# End Section
# Section MPlatformOlb : {9C6C9751-E5D4-41B9-8937-00CB9CE31436}
# 	1:22:IDD_VISION_CALIBRATION:117
# 	2:16:Resource Include:resource.h
# 	2:22:IDD_VISION_CALIBRATION:IDD_VISION_CALIBRATION
# 	2:10:ENUM: enum:enum
# 	2:22:CLASS: CVisionCalibDlg:CVisionCalibDlg
# 	2:18:VisionCalibDlg.cpp:VisionCalibDlg.cpp
# 	2:19:Application Include:MPlatformOlb.h
# 	2:16:VisionCalibDlg.h:VisionCalibDlg.h
# End Section
# Section MPlatformOlb : {A880B0EB-C04C-4B81-A04B-E275D48D3E5E}
# 	1:23:IDD_COMMON_ESTOP_NOTIFY:140
# 	2:16:Resource Include:resource.h
# 	2:22:CommonEStopNotifyDlg.h:CommonEStopNotifyDlg.h
# 	2:23:IDD_COMMON_ESTOP_NOTIFY:IDD_COMMON_ESTOP_NOTIFY
# 	2:10:ENUM: enum:enum
# 	2:28:CLASS: CCommonEStopNotifyDlg:CCommonEStopNotifyDlg
# 	2:24:CommonEStopNotifyDlg.cpp:CommonEStopNotifyDlg.cpp
# 	2:19:Application Include:MPlatformOlb.h
# End Section
# Section MPlatformOlb : {400356E4-EE48-4267-8CC7-743E0C30CE41}
# 	1:10:IDD_MLCNET:106
# 	2:16:Resource Include:resource.h
# 	2:16:CLASS: MLCNetDlg:MLCNetDlg
# 	2:10:IDD_MLCNET:IDD_MLCNET
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:MPlatformOlb.h
# 	2:13:MLCNetDlg.cpp:MLCNetDlg.cpp
# 	2:11:MLCNetDlg.h:MLCNetDlg.h
# End Section
# Section MPlatformOlb : {9F6AA700-D188-11CD-AD48-00AA003C9CB6}
# 	2:5:Class:CRowCursor
# 	2:10:HeaderFile:rowcursor.h
# 	2:8:ImplFile:rowcursor.cpp
# End Section
# Section MPlatformOlb : {FE56B492-7769-4F0E-940A-EE1E59385B78}
# 	1:14:IDD_DATA_MODEL:255
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:14:IDD_DATA_MODEL:IDD_DATA_MODEL
# 	2:14:DataModelDlg.h:DataModelDlg.h
# 	2:20:CLASS: CDataModelDlg:CDataModelDlg
# 	2:19:Application Include:mplatformolb.h
# 	2:16:DataModelDlg.cpp:DataModelDlg.cpp
# End Section
# Section MPlatformOlb : {11392D14-CAD7-46DF-B10D-47DDBE6A3BCC}
# 	1:22:IDD_VISION_CALIBRATION:138
# 	2:16:Resource Include:resource.h
# 	2:22:IDD_VISION_CALIBRATION:IDD_VISION_CALIBRATION
# 	2:10:ENUM: enum:enum
# 	2:22:CLASS: CVisionCalibDlg:CVisionCalibDlg
# 	2:18:VisionCalibDlg.cpp:VisionCalibDlg.cpp
# 	2:19:Application Include:MPlatformOlb.h
# 	2:16:VisionCalibDlg.h:VisionCalibDlg.h
# End Section
# Section MPlatformOlb : {52D02F41-6B6D-49E1-A311-9AF658F6ED9C}
# 	1:19:IDD_DATA_PANEL_DATA:114
# 	2:16:Resource Include:resource.h
# 	2:14:PanelDataDlg.h:PanelDataDlg.h
# 	2:10:ENUM: enum:enum
# 	2:20:CLASS: CPanelDataDlg:CPanelDataDlg
# 	2:16:PanelDataDlg.cpp:PanelDataDlg.cpp
# 	2:19:Application Include:MPlatformOlb.h
# 	2:19:IDD_DATA_PANEL_DATA:IDD_DATA_PANEL_DATA
# End Section
# Section MPlatformOlb : {95153706-AE99-479E-8333-CB1EC113B6AA}
# 	1:16:IDD_TEACH_PRESS1:261
# 	2:16:Resource Include:resource.h
# 	2:15:TeachPressDlg.h:TeachPressDlg1.h
# 	2:10:ENUM: enum:enum
# 	2:17:TeachPressDlg.cpp:TeachPressDlg1.cpp
# 	2:21:CLASS: CTeachPressDlg:CTeachPressDlg
# 	2:15:IDD_TEACH_PRESS:IDD_TEACH_PRESS1
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {486EAD3D-CBA0-4641-998C-62A4CB9DD103}
# 	1:15:IDD_DATA_MODEL1:222
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:14:IDD_DATA_MODEL:IDD_DATA_MODEL1
# 	2:14:DataModelDlg.h:DataModelDlg1.h
# 	2:20:CLASS: CDataModelDlg:CDataModelDlg
# 	2:19:Application Include:MPlatformOlb.h
# 	2:16:DataModelDlg.cpp:DataModelDlg1.cpp
# End Section
# Section MPlatformOlb : {575BD83B-BFF5-457E-A3DA-F8878DC9CB1B}
# 	1:19:IDD_AUTO_TABIC_INFO:208
# 	2:16:Resource Include:resource.h
# 	2:19:IDD_AUTO_TABIC_INFO:IDD_AUTO_TABIC_INFO
# 	2:18:AutoTabICInfoDlg.h:AutoTabICInfoDlg.h
# 	2:10:ENUM: enum:enum
# 	2:20:AutoTabICInfoDlg.cpp:AutoTabICInfoDlg.cpp
# 	2:24:CLASS: CAutoTabICInfoDlg:CAutoTabICInfoDlg
# 	2:19:Application Include:MPlatformOlb.h
# End Section
# Section MPlatformOlb : {E594BB32-1C77-4213-A45B-B27514787D74}
# 	1:12:IDD_EQ_STOP1:232
# 	2:16:Resource Include:resource.h
# 	2:14:EqStopDlg1.cpp:EqStopDlg1.cpp
# 	2:12:IDD_EQ_STOP1:IDD_EQ_STOP1
# 	2:17:CLASS: CEqStopDlg:CEqStopDlg
# 	2:10:ENUM: enum:enum
# 	2:12:EqStopDlg1.h:EqStopDlg1.h
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {81629A89-7FD0-48CC-9C20-9B6D1211A2A7}
# 	1:10:IDD_MLCNET:216
# 	2:16:Resource Include:resource.h
# 	2:16:CLASS: MLCNetDlg:MLCNetDlg
# 	2:10:IDD_MLCNET:IDD_MLCNET
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:mplatformolb.h
# 	2:13:MLCNetDlg.cpp:MLCNetDlg.cpp
# 	2:11:MLCNetDlg.h:MLCNetDlg.h
# End Section
# Section MPlatformOlb : {C00969C4-8B90-4672-916A-51BAC32DBB6C}
# 	1:19:IDD_DATA_MODEL_NEW1:278
# 	2:16:Resource Include:resource.h
# 	2:21:CLASS: SDeviceIDModel:SDeviceIDModel
# 	2:19:DataModelDlgNew.cpp:DataModelDlgNew1.cpp
# 	2:10:ENUM: enum:enum
# 	2:18:IDD_DATA_MODEL_NEW:IDD_DATA_MODEL_NEW1
# 	2:23:CLASS: CDataModelDlgNew:CDataModelDlgNew
# 	2:17:DataModelDlgNew.h:DataModelDlgNew1.h
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {40B5CE82-C5A8-11D2-8183-00002440DFD8}
# 	2:5:Class:CBtnEnh
# 	2:10:HeaderFile:btnenh.h
# 	2:8:ImplFile:btnenh.cpp
# End Section
# Section MPlatformOlb : {9E6120E2-A24C-4EFA-9A5B-E6DBCA4D156A}
# 	1:17:IDD_VISION_OPTION:121
# 	2:16:Resource Include:resource.h
# 	2:17:VisionOptionDlg.h:VisionOptionDlg.h
# 	2:19:VisionOptionDlg.cpp:VisionOptionDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:17:IDD_VISION_OPTION:IDD_VISION_OPTION
# 	2:23:CLASS: CVisionOptionDlg:CVisionOptionDlg
# 	2:19:Application Include:MPlatformOlb.h
# End Section
# Section MPlatformOlb : {704C503A-B923-488A-8C74-5042A1D50A9E}
# 	1:25:IDD_PRODUCT_CREATE_DIALOG:274
# 	2:16:Resource Include:resource.h
# 	2:18:ProductCreateDlg.h:ProductCreateDlg.h
# 	2:24:CLASS: CProductCreateDlg:CProductCreateDlg
# 	2:25:IDD_PRODUCT_CREATE_DIALOG:IDD_PRODUCT_CREATE_DIALOG
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:mplatformolb.h
# 	2:20:ProductCreateDlg.cpp:ProductCreateDlg.cpp
# End Section
# Section MPlatformOlb : {48344309-1D0B-4A1A-89F3-B7538706557B}
# 	1:11:IDD_EQ_STOP:231
# 	2:16:Resource Include:resource.h
# 	2:11:IDD_EQ_STOP:IDD_EQ_STOP
# 	2:13:EqStopDlg.cpp:EqStopDlg.cpp
# 	2:17:CLASS: CEqStopDlg:CEqStopDlg
# 	2:11:EqStopDlg.h:EqStopDlg.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {6FD131B6-A1C6-4371-9686-E4FF6E75B558}
# 	1:19:IDD_DATA_FUNC_ONOFF:125
# 	2:16:Resource Include:resource.h
# 	2:20:DataFuncOnOffDlg.cpp:DataFuncOnOffDlg.cpp
# 	2:19:IDD_DATA_FUNC_ONOFF:IDD_DATA_FUNC_ONOFF
# 	2:10:ENUM: enum:enum
# 	2:18:DataFuncOnOffDlg.h:DataFuncOnOffDlg.h
# 	2:19:Application Include:MPlatformOlb.h
# 	2:24:CLASS: CDataFuncOnOffDlg:CDataFuncOnOffDlg
# End Section
# Section MPlatformOlb : {62C364B5-B0C4-44E2-956E-79D52F81A250}
# 	1:19:IDD_DATA_TOWER_LAMP:257
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:18:DataTowerLampDlg.h:DataTowerLampDlg.h
# 	2:19:Application Include:mplatformolb.h
# 	2:20:DataTowerLampDlg.cpp:DataTowerLampDlg.cpp
# 	2:24:CLASS: CDataTowerLampDlg:CDataTowerLampDlg
# 	2:19:IDD_DATA_TOWER_LAMP:IDD_DATA_TOWER_LAMP
# End Section
# Section MPlatformOlb : {C88A7F68-F911-401E-BA14-CF7ACC77BDF7}
# 	1:17:IDD_TEACH_HANDLER:293
# 	2:16:Resource Include:resource.h
# 	2:17:IDD_TEACH_HANDLER:IDD_TEACH_HANDLER
# 	2:19:TeachHandlerDlg.cpp:TeachHandlerDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:mplatformolb.h
# 	2:23:CLASS: CTeachHandlerDlg:CTeachHandlerDlg
# 	2:17:TeachHandlerDlg.h:TeachHandlerDlg.h
# End Section
# Section MPlatformOlb : {AEA2F591-EC67-4A68-B0A5-0B9FACC4657D}
# 	1:18:IDD_AUTO_NSMC_INFO:246
# 	2:16:Resource Include:resource.h
# 	2:19:AutoNSMCInfoDlg.cpp:AutoNSMCInfoDlg.cpp
# 	2:18:IDD_AUTO_NSMC_INFO:IDD_AUTO_NSMC_INFO
# 	2:23:CLASS: CAutoNSMCInfoDlg:CAutoNSMCInfoDlg
# 	2:17:AutoNSMCInfoDlg.h:AutoNSMCInfoDlg.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {34AA2F5C-C9B6-4AAA-9375-71A62AAABFD7}
# 	1:18:IDD_MANUAL_TILTING:251
# 	2:16:Resource Include:resource.h
# 	2:18:IDD_MANUAL_TILTING:IDD_MANUAL_TILTING
# 	2:17:ManualTiltDlg.cpp:ManualTiltDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:15:ManualTiltDlg.h:ManualTiltDlg.h
# 	2:21:CLASS: CManualTiltDlg:CManualTiltDlg
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {5958356F-2D1C-4BDC-8694-70638CEFAABA}
# 	1:25:IDD_VISION_MONITOR_FOR_HD:179
# 	2:16:Resource Include:resource.h
# 	2:22:VisionMonitorForHD.cpp:VisionMonitorForHD.cpp
# 	2:25:IDD_VISION_MONITOR_FOR_HD:IDD_VISION_MONITOR_FOR_HD
# 	2:26:CLASS: CVisionMonitorForHD:CVisionMonitorForHD
# 	2:10:ENUM: enum:enum
# 	2:20:VisionMonitorForHD.h:VisionMonitorForHD.h
# 	2:19:Application Include:MPlatformOlb.h
# End Section
# Section MPlatformOlb : {37EB566A-998C-43F9-971F-A0A0F7F4F802}
# 	1:18:IDD_DATA_MAIN_FORM:250
# 	2:16:Resource Include:resource.h
# 	2:20:CLASS: CDataMainForm:CDataMainForm
# 	2:14:DataMainForm.h:DataMainForm1.h
# 	2:16:DataMainForm.cpp:DataMainForm1.cpp
# 	2:10:ENUM: enum:enum
# 	2:18:IDD_DATA_MAIN_FORM:IDD_DATA_MAIN_FORM
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {E993B65C-5E3D-4353-8B42-8E44A02D63BB}
# 	1:29:IDD_TEACH_CARRIERCAMERAEXPAND:289
# 	2:16:Resource Include:resource.h
# 	2:31:TeachCarrierCameraExpandDlg.cpp:TeachCarrierCameraExpandDlg.cpp
# 	2:29:IDD_TEACH_CARRIERCAMERAEXPAND:IDD_TEACH_CARRIERCAMERAEXPAND
# 	2:29:TeachCarrierCameraExpandDlg.h:TeachCarrierCameraExpandDlg.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:mplatformolb.h
# 	2:35:CLASS: CTeachCarrierCameraExpandDlg:CTeachCarrierCameraExpandDlg
# End Section
# Section MPlatformOlb : {285A770D-C42D-4AEF-A2F2-2F1B5D05F505}
# 	1:17:IDD_AUTO_ZIG_INFO:209
# 	2:16:Resource Include:resource.h
# 	2:18:AutoZigInfoDlg.cpp:AutoZigInfoDlg.cpp
# 	2:17:IDD_AUTO_ZIG_INFO:IDD_AUTO_ZIG_INFO
# 	2:10:ENUM: enum:enum
# 	2:22:CLASS: CAutoZigInfoDlg:CAutoZigInfoDlg
# 	2:19:Application Include:MPlatformOlb.h
# 	2:16:AutoZigInfoDlg.h:AutoZigInfoDlg.h
# End Section
# Section MPlatformOlb : {A23C7503-00A7-4047-A65A-62EF9807C4C8}
# 	1:19:IDD_TEACH_TABDETECT:161
# 	2:16:Resource Include:resource.h
# 	2:21:TeachTabDetectorDlg.h:TeachTabDetectorDlg.h
# 	2:10:ENUM: enum:enum
# 	2:27:CLASS: CTeachTabDetectorDlg:CTeachTabDetectorDlg
# 	2:19:IDD_TEACH_TABDETECT:IDD_TEACH_TABDETECT
# 	2:19:Application Include:MPlatformOlb.h
# 	2:23:TeachTabDetectorDlg.cpp:TeachTabDetectorDlg.cpp
# End Section
# Section MPlatformOlb : {40B5CE81-C5A8-11D2-8183-00002440DFD8}
# 	2:21:DefaultSinkHeaderFile:btnenh.h
# 	2:16:DefaultSinkClass:CBtnEnh
# End Section
# Section MPlatformOlb : {E9CE2543-BA6D-4690-8761-B1D8C677BD46}
# 	1:18:IDD_VISION_DISPLAY:170
# 	2:16:Resource Include:resource.h
# 	2:18:VisionDisplayDlg.h:VisionDisplayDlg.h
# 	2:18:IDD_VISION_DISPLAY:IDD_VISION_DISPLAY
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:MPlatformOlb.h
# 	2:20:VisionDisplayDlg.cpp:VisionDisplayDlg.cpp
# 	2:24:CLASS: CVisionDisplayDlg:CVisionDisplayDlg
# End Section
# Section MPlatformOlb : {492BD167-9DA0-4C0B-889A-0E89F98F6A9F}
# 	1:17:IDD_AUTO_ZIG_INFO:171
# 	2:16:Resource Include:resource.h
# 	2:18:AutoZigInfoDlg.cpp:AutoZigInfoDlg.cpp
# 	2:17:IDD_AUTO_ZIG_INFO:IDD_AUTO_ZIG_INFO
# 	2:10:ENUM: enum:enum
# 	2:22:CLASS: CAutoZigInfoDlg:CAutoZigInfoDlg
# 	2:19:Application Include:MPlatformOlb.h
# 	2:16:AutoZigInfoDlg.h:AutoZigInfoDlg.h
# End Section
# Section MPlatformOlb : {A62CAD0E-14BB-44A6-941B-888DC2494ED8}
# 	1:18:IDD_DATA_ECDATA_DB:244
# 	2:16:Resource Include:resource.h
# 	2:18:IDD_DATA_ECDATA_DB:IDD_DATA_ECDATA_DB
# 	2:19:CLASS: CDataECDBDlg:CDataECDBDlg
# 	2:10:ENUM: enum:enum
# 	2:15:DataECDBDlg.cpp:DataECDBDlg.cpp
# 	2:19:Application Include:mplatformolb.h
# 	2:13:DataECDBDlg.h:DataECDBDlg.h
# End Section
# Section MPlatformOlb : {D77E5E11-84A8-49BE-B364-C73B32F703A9}
# 	1:22:IDD_VISION_SELECT_MARK:197
# 	2:16:Resource Include:resource.h
# 	2:27:CLASS: CVisionSelectMarkDlg:CVisionSelectMarkDlg
# 	2:22:IDD_VISION_SELECT_MARK:IDD_VISION_SELECT_MARK
# 	2:23:VisionSelectMarkDlg.cpp:VisionSelectMarkDlg.cpp
# 	2:21:VisionSelectMarkDlg.h:VisionSelectMarkDlg.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:MPlatformOlb.h
# End Section
# Section MPlatformOlb : {8D83F1DD-5122-4CA0-9B38-FDE83DC0CB40}
# 	1:22:IDD_VISION_SELECT_MARK:122
# 	2:16:Resource Include:resource.h
# 	2:27:CLASS: CVisionSelectMarkDlg:CVisionSelectMarkDlg
# 	2:22:IDD_VISION_SELECT_MARK:IDD_VISION_SELECT_MARK
# 	2:23:VisionSelectMarkDlg.cpp:VisionSelectMarkDlg.cpp
# 	2:21:VisionSelectMarkDlg.h:VisionSelectMarkDlg.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:MPlatformOlb.h
# End Section
# Section MPlatformOlb : {02D3A7AC-E46B-4291-A4B9-CC9A8287BD7B}
# 	1:25:IDD_DLGFIXEDVISION_DIALOG:295
# 	2:16:Resource Include:resource.h
# 	2:25:IDD_DLGFIXEDVISION_DIALOG:IDD_DLGFIXEDVISION_DIALOG
# 	2:10:ENUM: enum:enum
# 	2:18:DlgFixedVision.cpp:DlgFixedVision.cpp
# 	2:22:CLASS: CDlgFixedVision:CDlgFixedVision
# 	2:19:Application Include:mplatformolb.h
# 	2:16:DlgFixedVision.h:DlgFixedVision.h
# End Section
# Section MPlatformOlb : {05798E56-61FB-4714-80D8-F96CB7FB4CDF}
# 	1:17:IDD_VISION_OPTION:195
# 	2:16:Resource Include:resource.h
# 	2:17:VisionOptionDlg.h:VisionOptionDlg.h
# 	2:19:VisionOptionDlg.cpp:VisionOptionDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:17:IDD_VISION_OPTION:IDD_VISION_OPTION
# 	2:23:CLASS: CVisionOptionDlg:CVisionOptionDlg
# 	2:19:Application Include:MPlatformOlb.h
# End Section
# Section MPlatformOlb : {F4358CAA-AD19-48BD-8C64-42F3E518721B}
# 	1:22:IDD_ERROR_NOT_SEND_DLG:287
# 	2:16:Resource Include:resource.h
# 	2:23:CLASS: CErrorNotSendDlg:CErrorNotSendDlg
# 	2:17:ErrorNotSendDlg.h:ErrorNotSendDlg.h
# 	2:19:ErrorNotSendDlg.cpp:ErrorNotSendDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:22:IDD_ERROR_NOT_SEND_DLG:IDD_ERROR_NOT_SEND_DLG
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {F10E1A36-B752-4778-A2CC-B61C53F75FAE}
# 	1:23:IDD_PAGE_MCODE_INFO_2ND:269
# 	2:16:Resource Include:resource.h
# 	2:17:PageMCodeInfo.cpp:PageMCodeInfo1.cpp
# 	2:24:CLASS: CPageMCodeInfo2ND:CPageMCodeInfo2ND
# 	2:10:ENUM: enum:enum
# 	2:23:IDD_PAGE_MCODE_INFO_2ND:IDD_PAGE_MCODE_INFO_2ND
# 	2:19:Application Include:mplatformolb.h
# 	2:15:PageMCodeInfo.h:PageMCodeInfo1.h
# End Section
# Section MPlatformOlb : {70B7D7AD-6F14-4897-BB10-0B16955FEE20}
# 	1:22:IDD_AUTO_MATERIAL_INFO:207
# 	2:16:Resource Include:resource.h
# 	2:21:AutoMaterialInfoDlg.h:AutoMaterialInfoDlg1.h
# 	2:10:ENUM: enum:enum
# 	2:23:AutoMaterialInfoDlg.cpp:AutoMaterialInfoDlg1.cpp
# 	2:22:IDD_AUTO_MATERIAL_INFO:IDD_AUTO_MATERIAL_INFO
# 	2:27:CLASS: CAutoMaterialInfoDlg:CAutoMaterialInfoDlg
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {AE856F81-1AA4-435C-866A-90AD5FC48BE5}
# 	1:29:IDD_VISIONDISPALIGNCAM_DIALOG:221
# 	2:16:Resource Include:resource.h
# 	2:26:CLASS: CVisionDispAlignCam:CVisionDispAlignCam
# 	2:29:IDD_VISIONDISPALIGNCAM_DIALOG:IDD_VISIONDISPALIGNCAM_DIALOG
# 	2:22:VisionDispAlignCam.cpp:VisionDispAlignCam.cpp
# 	2:10:ENUM: enum:enum
# 	2:20:VisionDispAlignCam.h:VisionDispAlignCam.h
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {9EC78A27-118A-4ED3-BB49-D0A0055A43B0}
# 	1:17:IDD_DATA_TAB_DATA:173
# 	2:16:Resource Include:resource.h
# 	2:17:IDD_DATA_TAB_DATA:IDD_DATA_TAB_DATA
# 	2:10:ENUM: enum:enum
# 	2:20:CLASS: CDataTabICDlg:CDataTabICDlg
# 	2:19:Application Include:MPlatformOlb.h
# 	2:16:DataTabICDlg.cpp:DataTabICDlg.cpp
# 	2:14:DataTabICDlg.h:DataTabICDlg.h
# End Section
# Section MPlatformOlb : {A71C494E-1A10-4F4D-9AED-88C0AA651512}
# 	1:26:IDD_PANEL_VALIDATION_CHECK:264
# 	2:16:Resource Include:resource.h
# 	2:27:PanelValidationCheckDlg.cpp:PanelValidationCheckDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:26:IDD_PANEL_VALIDATION_CHECK:IDD_PANEL_VALIDATION_CHECK
# 	2:25:PanelValidationCheckDlg.h:PanelValidationCheckDlg.h
# 	2:31:CLASS: CPanelValidationCheckDlg:CPanelValidationCheckDlg
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {05D1A2C7-949F-4867-B96E-632327CF283B}
# 	1:21:IDD_COMMON_OPCALL_DLG:185
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:19:CommonOPCallDlg.cpp:CommonOPCallDlg.cpp
# 	2:23:CLASS: CCommonOPCallDlg:CCommonOPCallDlg
# 	2:21:IDD_COMMON_OPCALL_DLG:IDD_COMMON_OPCALL_DLG
# 	2:17:CommonOPCallDlg.h:CommonOPCallDlg.h
# 	2:19:Application Include:MPlatformOlb.h
# End Section
# Section MPlatformOlb : {34A0E701-9953-44C3-9B56-87BFC65B6702}
# 	1:9:IDD_MNETH:105
# 	2:16:Resource Include:resource.h
# 	2:15:CLASS: MNetHDlg:MNetHDlg
# 	2:10:ENUM: enum:enum
# 	2:10:MNetHDlg.h:MNetHDlg.h
# 	2:19:Application Include:MPlatformOlb.h
# 	2:12:MNetHDlg.cpp:MNetHDlg.cpp
# 	2:9:IDD_MNETH:IDD_MNETH
# End Section
# Section MPlatformOlb : {0512C31A-B5AD-4369-86D7-F9EA1514CFC0}
# 	1:18:IDD_MANUAL_TILTING:239
# 	2:16:Resource Include:resource.h
# 	2:18:IDD_MANUAL_TILTING:IDD_MANUAL_TILTING
# 	2:17:ManualTiltDlg.cpp:ManualTiltDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:15:ManualTiltDlg.h:ManualTiltDlg.h
# 	2:21:CLASS: CManualTiltDlg:CManualTiltDlg
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {EEFFEB0A-7A7E-473C-8015-358EB6658CA6}
# 	1:22:IDD_AUTO_SCRAP_UNSCRAP:213
# 	2:16:Resource Include:resource.h
# 	2:25:CLASS: CAutoScrapGlassDlg:CAutoScrapGlassDlg
# 	2:19:AutoScrapGlassDlg.h:AutoScrapGlassDlg.h
# 	2:10:ENUM: enum:enum
# 	2:21:AutoScrapGlassDlg.cpp:AutoScrapGlassDlg.cpp
# 	2:19:Application Include:mplatformolb.h
# 	2:22:IDD_AUTO_SCRAP_UNSCRAP:IDD_AUTO_SCRAP_UNSCRAP
# End Section
# Section MPlatformOlb : {BC47972D-1DFD-4895-9D57-3480C6433D40}
# 	1:19:IDD_MANUAL_CONVEYOR:188
# 	2:25:CLASS: CManualConveyorDlg:CManualConveyorDlg
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:19:ManualConveyorDlg.h:ManualConveyorDlg.h
# 	2:19:Application Include:mplatformolb.h
# 	2:19:IDD_MANUAL_CONVEYOR:IDD_MANUAL_CONVEYOR
# 	2:21:ManualConveyorDlg.cpp:ManualConveyorDlg.cpp
# End Section
# Section MPlatformOlb : {29D0A39C-6A59-43B4-A8EF-4BA90CFAE0EF}
# 	1:24:IDD_TEACH_BADTABDETECTOR:172
# 	2:16:Resource Include:resource.h
# 	2:24:IDD_TEACH_BADTABDETECTOR:IDD_TEACH_BADTABDETECTOR
# 	2:10:ENUM: enum:enum
# 	2:26:TeachBadTabDetectorDlg.cpp:TeachBadTabDetectorDlg.cpp
# 	2:24:TeachBadTabDetectorDlg.h:TeachBadTabDetectorDlg.h
# 	2:30:CLASS: CTeachBadTabDetectorDlg:CTeachBadTabDetectorDlg
# 	2:19:Application Include:MPlatformOlb.h
# End Section
# Section MPlatformOlb : {753B453D-F555-4D7F-8684-0F40E3885473}
# 	1:16:IDD_DATA_FIVEBTN:260
# 	2:16:Resource Include:resource.h
# 	2:19:CLASS: SystemBtnDlg:SystemBtnDlg
# 	2:10:ENUM: enum:enum
# 	2:14:SystemBtnDlg.h:SystemBtnDlg.h
# 	2:16:SystemBtnDlg.cpp:SystemBtnDlg.cpp
# 	2:19:Application Include:mplatformolb.h
# 	2:16:IDD_DATA_FIVEBTN:IDD_DATA_FIVEBTN
# End Section
# Section MPlatformOlb : {A9ED5F39-3664-46FA-BFD9-34181D7A21B3}
# 	1:21:IDD_AUTO_MOTOR_ORIGIN:233
# 	2:16:Resource Include:resource.h
# 	2:21:IDD_AUTO_MOTOR_ORIGIN:IDD_AUTO_MOTOR_ORIGIN
# 	2:26:CLASS: CAutoMotorOriginDlg:CAutoMotorOriginDlg
# 	2:22:AutoMotorOriginDlg.cpp:AutoMotorOriginDlg.cpp
# 	2:23:CLASS: SOriginGroupData:SOriginGroupData
# 	2:10:ENUM: enum:enum
# 	2:25:TYPEDEF: SOriginGroupData:SOriginGroupData
# 	2:19:Application Include:mplatformolb.h
# 	2:20:AutoMotorOriginDlg.h:AutoMotorOriginDlg.h
# End Section
# Section MPlatformOlb : {5F4DF280-531B-11CF-91F6-C2863C385E30}
# 	2:5:Class:CMSFlexGrid
# 	2:10:HeaderFile:msflexgrid.h
# 	2:8:ImplFile:msflexgrid.cpp
# End Section
# Section MPlatformOlb : {8B429C8C-941E-4274-A01E-5E568A56F454}
# 	1:18:IDD_MANUAL_HANDLER:294
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:24:CLASS: CManualHandlerDlg:CManualHandlerDlg
# 	2:18:IDD_MANUAL_HANDLER:IDD_MANUAL_HANDLER
# 	2:18:ManualHandlerDlg.h:ManualHandlerDlg.h
# 	2:19:Application Include:mplatformolb.h
# 	2:20:ManualHandlerDlg.cpp:ManualHandlerDlg.cpp
# End Section
# Section MPlatformOlb : {F7C0B17E-E5F9-4A91-B595-B116B828CC77}
# 	1:15:IDD_VER_DISPLAY:290
# 	2:16:Resource Include:resource.h
# 	2:19:VersionDisplayDlg.h:VersionDisplayDlg.h
# 	2:10:ENUM: enum:enum
# 	2:21:VersionDisplayDlg.cpp:VersionDisplayDlg.cpp
# 	2:25:CLASS: CVersionDisplayDlg:CVersionDisplayDlg
# 	2:15:IDD_VER_DISPLAY:IDD_VER_DISPLAY
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {8FB7464A-4A3F-4CE1-BC0C-B750AB2F29DC}
# 	1:21:IDD_MANUAL_IO_MONITOR:220
# 	2:16:Resource Include:resource.h
# 	2:21:IDD_MANUAL_IO_MONITOR:IDD_MANUAL_IO_MONITOR
# 	2:10:ENUM: enum:enum
# 	2:29:CLASS: CManualIOMonitoringDlg:CManualIOMonitoringDlg
# 	2:25:ManualIOMonitoringDlg.cpp:ManualIOMonitoringDlg.cpp
# 	2:19:Application Include:MPlatformOlb.h
# 	2:23:ManualIOMonitoringDlg.h:ManualIOMonitoringDlg.h
# End Section
# Section MPlatformOlb : {E15030E4-2D53-4C1A-82CA-891D11CC4E18}
# 	2:5:Class:CXNet
# 	2:10:HeaderFile:xnet.h
# 	2:8:ImplFile:xnet.cpp
# End Section
# Section MPlatformOlb : {4E867E46-92F7-4905-BEF1-58822351F3E1}
# 	1:25:IDD_MANUAL_PANEL_TRANSFER:238
# 	2:16:Resource Include:resource.h
# 	2:26:ManualPanelTransferDlg.cpp:ManualPanelTransferDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:30:CLASS: CManualPanelTransferDlg:CManualPanelTransferDlg
# 	2:25:IDD_MANUAL_PANEL_TRANSFER:IDD_MANUAL_PANEL_TRANSFER
# 	2:19:Application Include:mplatformolb.h
# 	2:24:ManualPanelTransferDlg.h:ManualPanelTransferDlg.h
# End Section
# Section MPlatformOlb : {B0087674-4F56-4617-919A-CAB617973587}
# 	1:18:IDD_VISION_JOG_KEY:193
# 	2:16:Resource Include:resource.h
# 	2:17:VisionJogKeyDlg.h:VisionJogKeyDlg.h
# 	2:23:CLASS: CVisionJogKeyDlg:CVisionJogKeyDlg
# 	2:18:IDD_VISION_JOG_KEY:IDD_VISION_JOG_KEY
# 	2:10:ENUM: enum:enum
# 	2:19:VisionJogKeyDlg.cpp:VisionJogKeyDlg.cpp
# 	2:19:Application Include:MPlatformOlb.h
# End Section
# Section MPlatformOlb : {A7433E52-7F3C-4CD8-8611-9796FB0BCE32}
# 	1:17:IDD_MOLD_ZIG_INFO:240
# 	2:16:Resource Include:resource.h
# 	2:13:MoldZigInfo.h:MoldZigInfo.h
# 	2:17:IDD_MOLD_ZIG_INFO:IDD_MOLD_ZIG_INFO
# 	2:15:MoldZigInfo.cpp:MoldZigInfo.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:CLASS: CMoldZigInfo:CMoldZigInfo
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section MPlatformOlb : {6262D3A0-531B-11CF-91F6-C2863C385E30}
# 	2:21:DefaultSinkHeaderFile:msflexgrid.h
# 	2:16:DefaultSinkClass:CMSFlexGrid
# End Section
# Section MPlatformOlb : {08E20BA0-66DE-4D88-9347-69C67F00606C}
# 	1:16:IDD_TEACH_PRESS:205
# 	2:16:Resource Include:resource.h
# 	2:15:TeachPressDlg.h:TeachPressDlg.h
# 	2:10:ENUM: enum:enum
# 	2:17:TeachPressDlg.cpp:TeachPressDlg.cpp
# 	2:21:CLASS: CTeachPressDlg:CTeachPressDlg
# 	2:15:IDD_TEACH_PRESS:IDD_TEACH_PRESS
# 	2:19:Application Include:MPlatformOlb.h
# End Section
# Section MPlatformOlb : {AAB9583F-78F6-4D86-A407-D0D7E8A5209F}
# 	1:23:IDD_MODEL_DEVICEID_LIST:273
# 	2:29:CLASS: CModelNDeviceIDListDlg:CModelNDeviceIDListDlg
# 	2:16:Resource Include:resource.h
# 	2:26:ModelNDeviceIDListDlg1.cpp:ModelNDeviceIDListDlg1.cpp
# 	2:10:ENUM: enum:enum
# 	2:23:IDD_MODEL_DEVICEID_LIST:IDD_MODEL_DEVICEID_LIST
# 	2:19:Application Include:mplatformolb.h
# 	2:24:ModelNDeviceIDListDlg1.h:ModelNDeviceIDListDlg1.h
# End Section
# Section MPlatformOlb : {78989AC9-60D7-48C1-829E-CEC71FCDAA23}
# 	1:27:IDD_PLATE_VACUUM_MODEL_DATA:282
# 	2:16:Resource Include:resource.h
# 	2:27:IDD_PLATE_VACUUM_MODEL_DATA:IDD_PLATE_VACUUM_MODEL_DATA
# 	2:10:ENUM: enum:enum
# 	2:22:PlateVacModelDataDlg.h:PlateVacModelDataDlg.h
# 	2:24:PlateVacModelDataDlg.cpp:PlateVacModelDataDlg.cpp
# 	2:19:Application Include:mplatformolb.h
# 	2:28:CLASS: CPlateVacModelDataDlg:CPlateVacModelDataDlg
# End Section
# Section MPlatformOlb : {DFA686C4-4F80-4E4E-9266-D98050DD40C4}
# 	1:15:IDD_DATA_COMMON:199
# 	2:16:Resource Include:resource.h
# 	2:21:CLASS: CDataCommonDlg:CDataCommonDlg
# 	2:15:IDD_DATA_COMMON:IDD_DATA_COMMON
# 	2:17:DataCommonDlg.cpp:DataCommonDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:mplatformolb.h
# 	2:15:DataCommonDlg.h:DataCommonDlg.h
# End Section
# Section MPlatformOlb : {A0ABB6DD-F92A-4DF6-A041-25FA4ACF5CFA}
# 	1:11:IDD_EQ_STOP:241
# 	2:16:Resource Include:resource.h
# 	2:11:IDD_EQ_STOP:IDD_EQ_STOP
# 	2:10:ENUM: enum:enum
# 	2:17:EqStopCodeDlg.cpp:EqStopCodeDlg.cpp
# 	2:15:EqStopCodeDlg.h:EqStopCodeDlg.h
# 	2:19:Application Include:mplatformolb.h
# 	2:21:CLASS: CEqStopCodeDlg:CEqStopCodeDlg
# End Section
# Section MPlatformOlb : {1237362D-7581-437E-ADC3-DB54E3FB59E9}
# 	1:22:IDD_MODULE_PORT_ID_DLG:270
# 	2:16:Resource Include:resource.h
# 	2:23:CLASS: CModulePortIDDlg:CModulePortIDDlg
# 	2:19:ModulePortIDDlg.cpp:ModulePortIDDlg1.cpp
# 	2:22:IDD_MODULE_PORT_ID_DLG:IDD_MODULE_PORT_ID_DLG
# 	2:10:ENUM: enum:enum
# 	2:17:ModulePortIDDlg.h:ModulePortIDDlg.h
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {A7429256-632D-4EC3-9F4B-66C4853C51A5}
# 	1:23:IDD_MODULE_PORT_ID_DLG1:273
# 	2:16:Resource Include:resource.h
# 	2:23:CLASS: CModulePortIDDlg:CModulePortIDDlg
# 	2:19:ModulePortIDDlg.cpp:ModulePortIDDlg1.cpp
# 	2:22:IDD_MODULE_PORT_ID_DLG:IDD_MODULE_PORT_ID_DLG1
# 	2:10:ENUM: enum:enum
# 	2:17:ModulePortIDDlg.h:ModulePortIDDlg1.h
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {A1EE52BE-DCFE-4A49-8D69-24818A4CAF8A}
# 	1:21:IDD_TEACH_TAB_MOUNTER:253
# 	2:16:Resource Include:resource.h
# 	2:21:IDD_TEACH_TAB_MOUNTER:IDD_TEACH_TAB_MOUNTER
# 	2:26:CLASS: CTeachTabMounterDlg:CTeachTabMounterDlg
# 	2:22:TeachTabMounterDlg.cpp:TeachTabMounterDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:20:TeachTabMounterDlg.h:TeachTabMounterDlg.h
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {DCB66404-EBE6-4428-A5DC-D0E0DF7F301D}
# 	1:19:IDD_DATA_MODEL_DATA:116
# 	2:16:Resource Include:resource.h
# 	2:20:CLASS: CModelDataDlg:CModelDataDlg
# 	2:16:ModelDataDlg.cpp:ModelDataDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:IDD_DATA_MODEL_DATA:IDD_DATA_MODEL_DATA
# 	2:19:Application Include:MPlatformOlb.h
# 	2:14:ModelDataDlg.h:ModelDataDlg.h
# End Section
# Section MPlatformOlb : {87561D9A-F510-417E-B146-7CA8C75E602B}
# 	1:18:IDD_AUTO_NSMC_INFO:228
# 	2:16:Resource Include:resource.h
# 	2:19:AutoNSMCInfoDlg.cpp:AutoNSMCInfoDlg.cpp
# 	2:18:IDD_AUTO_NSMC_INFO:IDD_AUTO_NSMC_INFO
# 	2:23:CLASS: CAutoNSMCInfoDlg:CAutoNSMCInfoDlg
# 	2:17:AutoNSMCInfoDlg.h:AutoNSMCInfoDlg.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {42A1A2F8-7393-4409-8415-97FD0552C708}
# 	1:22:IDD_VISION_CALIBRATION:234
# 	2:16:Resource Include:resource.h
# 	2:22:IDD_VISION_CALIBRATION:IDD_VISION_CALIBRATION
# 	2:29:TYPEDEF: SCalibrationTypeData:SCalibrationTypeData
# 	2:10:ENUM: enum:enum
# 	2:22:CLASS: CVisionCalibDlg:CVisionCalibDlg
# 	2:27:CLASS: SCalibrationTypeData:SCalibrationTypeData
# 	2:18:VisionCalibDlg.cpp:VisionCalibDlg.cpp
# 	2:19:Application Include:mplatformolb.h
# 	2:16:VisionCalibDlg.h:VisionCalibDlg.h
# End Section
# Section MPlatformOlb : {5715581F-5C29-4CC3-A1F5-DF38E88774EE}
# 	1:17:IDD_COMMON_NOTIFY:223
# 	2:16:Resource Include:resource.h
# 	2:17:CommonNotifyDlg.h:CommonNotifyDlg.h
# 	2:17:IDD_COMMON_NOTIFY:IDD_COMMON_NOTIFY
# 	2:19:CommonNotifyDlg.cpp:CommonNotifyDlg.cpp
# 	2:23:CLASS: CCommonNotifyDlg:CCommonNotifyDlg
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:MPlatformOlb.h
# End Section
# Section MPlatformOlb : {FE3E716C-AC90-4DB9-95F9-14BEF0B6A3C6}
# 	1:16:IDD_PRODUCT_PLAN:263
# 	2:16:Resource Include:resource.h
# 	2:16:ProductPlanDlg.h:ProductPlanDlg.h
# 	2:18:ProductPlanDlg.cpp:ProductPlanDlg.cpp
# 	2:22:CLASS: CProductPlanDlg:CProductPlanDlg
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:mplatformolb.h
# 	2:16:IDD_PRODUCT_PLAN:IDD_PRODUCT_PLAN
# End Section
# Section MPlatformOlb : {503670D2-BFB4-4780-B633-D65AA930767C}
# 	1:20:IDD_COMMON_ERROR_MSG:139
# 	2:16:Resource Include:resource.h
# 	2:20:IDD_COMMON_ERROR_MSG:IDD_COMMON_ERROR_MSG
# 	2:19:CommonErrMsgDlg.cpp:CommonErrMsgDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:23:CLASS: CCommonErrMsgDlg:CCommonErrMsgDlg
# 	2:17:CommonErrMsgDlg.h:CommonErrMsgDlg.h
# 	2:19:Application Include:MPlatformOlb.h
# End Section
# Section MPlatformOlb : {463B1097-DF0F-4F4A-AC5D-DBC02D050A56}
# 	1:18:IDD_VISION_MONITOR:194
# 	2:16:Resource Include:resource.h
# 	2:20:VisionMonitorDlg.cpp:VisionMonitorDlg.cpp
# 	2:18:VisionMonitorDlg.h:VisionMonitorDlg.h
# 	2:10:ENUM: enum:enum
# 	2:18:IDD_VISION_MONITOR:IDD_VISION_MONITOR
# 	2:19:Application Include:MPlatformOlb.h
# 	2:24:CLASS: CVisionMonitorDlg:CVisionMonitorDlg
# End Section
# Section MPlatformOlb : {84B6677D-DDAD-49C2-A4E5-20037832A72F}
# 	1:22:IDD_VISION_SEARCH_AREA:159
# 	2:16:Resource Include:resource.h
# 	2:21:VisionSearchAreaDlg.h:VisionSearchAreaDlg.h
# 	2:10:ENUM: enum:enum
# 	2:27:CLASS: CVisionSearchAreaDlg:CVisionSearchAreaDlg
# 	2:22:IDD_VISION_SEARCH_AREA:IDD_VISION_SEARCH_AREA
# 	2:19:Application Include:MPlatformOlb.h
# 	2:23:VisionSearchAreaDlg.cpp:VisionSearchAreaDlg.cpp
# End Section
# Section MPlatformOlb : {7B752C71-8C78-4492-86B0-C969CD87BD75}
# 	1:19:IDD_DATA_TABMOUNTER:292
# 	2:25:CLASS: CDataTabMounterDlg:CDataTabMounterDlg
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:19:DataTabMounterDlg.h:DataTabMounterDlg.h
# 	2:21:DataTabMounterDlg.cpp:DataTabMounterDlg.cpp
# 	2:19:IDD_DATA_TABMOUNTER:IDD_DATA_TABMOUNTER
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {C3CBE6F6-3CE4-4742-81CF-4A3519878C0C}
# 	1:18:IDD_VISION_MONITOR:119
# 	2:16:Resource Include:resource.h
# 	2:20:VisionMonitorDlg.cpp:VisionMonitorDlg.cpp
# 	2:18:VisionMonitorDlg.h:VisionMonitorDlg.h
# 	2:10:ENUM: enum:enum
# 	2:18:IDD_VISION_MONITOR:IDD_VISION_MONITOR
# 	2:19:Application Include:MPlatformOlb.h
# 	2:24:CLASS: CVisionMonitorDlg:CVisionMonitorDlg
# End Section
# Section MPlatformOlb : {0B4583BE-9DA7-463C-BD60-0E5CDB120C71}
# 	1:30:IDD_DLGVIEWWORKSCHEDULE_DIALOG:252
# 	2:16:Resource Include:resource.h
# 	2:30:IDD_DLGVIEWWORKSCHEDULE_DIALOG:IDD_DLGVIEWWORKSCHEDULE_DIALOG
# 	2:10:ENUM: enum:enum
# 	2:23:DlgViewWorkSchedule.cpp:DlgViewWorkSchedule1.cpp
# 	2:19:Application Include:mplatformolb.h
# 	2:27:CLASS: CDlgViewWorkSchedule:CDlgViewWorkSchedule
# 	2:21:DlgViewWorkSchedule.h:DlgViewWorkSchedule1.h
# End Section
# Section MPlatformOlb : {76398E06-3829-42B7-B6BE-FFADC24EB278}
# 	1:21:IDD_MANUAL_MARK_RECOG:283
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:26:CLASS: CManualMarkRecogDlg:CManualMarkRecogDlg
# 	2:21:IDD_MANUAL_MARK_RECOG:IDD_MANUAL_MARK_RECOG
# 	2:19:Application Include:mplatformolb.h
# 	2:22:ManualMarkRecogDlg.cpp:ManualMarkRecogDlg.cpp
# 	2:20:ManualMarkRecogDlg.h:ManualMarkRecogDlg.h
# End Section
# Section MPlatformOlb : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
# Section MPlatformOlb : {428F4A35-16CB-4DD8-AE18-0B1436937CE4}
# 	1:20:IDD_AUTO_ELAPSE_TIME:186
# 	2:16:Resource Include:resource.h
# 	2:24:CLASS: AutoElapseTimeDlg:AutoElapseTimeDlg
# 	2:21:AutoElapseTimeDlg.cpp:AutoElapseTimeDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:20:IDD_AUTO_ELAPSE_TIME:IDD_AUTO_ELAPSE_TIME
# 	2:19:AutoElapseTimeDlg.h:AutoElapseTimeDlg.h
# 	2:19:Application Include:MPlatformOlb.h
# End Section
# Section MPlatformOlb : {CE204B4B-EEA4-47A4-934B-8D4490E33271}
# 	1:27:IDD_SYSTEMFOURBTNDLG_DIALOG:284
# 	2:16:Resource Include:resource.h
# 	2:18:SystemFourBtnDlg.h:SystemFourBtnDlg.h
# 	2:27:IDD_SYSTEMFOURBTNDLG_DIALOG:IDD_SYSTEMFOURBTNDLG_DIALOG
# 	2:10:ENUM: enum:enum
# 	2:24:CLASS: CSystemFourBtnDlg:CSystemFourBtnDlg
# 	2:20:SystemFourBtnDlg.cpp:SystemFourBtnDlg.cpp
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {7DDFBEAB-4C08-4296-AD28-1E9E3EA84225}
# 	1:16:IDD_PRODUCT_PLAN:265
# 	2:16:Resource Include:resource.h
# 	2:16:ProductPlanDlg.h:ProductPlanDlg.h
# 	2:18:ProductPlanDlg.cpp:ProductPlanDlg.cpp
# 	2:22:CLASS: CProductPlanDlg:CProductPlanDlg
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:mplatformolb.h
# 	2:16:IDD_PRODUCT_PLAN:IDD_PRODUCT_PLAN
# End Section
# Section MPlatformOlb : {2433C321-3E2A-489C-96FE-F0F2647FB513}
# 	1:24:IDD_PM_MODE_FULL_DISPLAY:249
# 	2:16:Resource Include:resource.h
# 	2:24:IDD_PM_MODE_FULL_DISPLAY:IDD_PM_MODE_FULL_DISPLAY
# 	2:17:PMFullDisplay.cpp:PMFullDisplay.cpp
# 	2:10:ENUM: enum:enum
# 	2:21:CLASS: CPMFullDisplay:CPMFullDisplay
# 	2:19:Application Include:mplatformolb.h
# 	2:15:PMFullDisplay.h:PMFullDisplay.h
# End Section
# Section MPlatformOlb : {28BF08BF-2FB7-4D15-94FD-9E77A7908E1D}
# 	1:25:IDD_VISION_BLOB_THRESHOLD:191
# 	2:16:Resource Include:resource.h
# 	2:30:CLASS: CVisionBlobThresholdDlg:CVisionBlobThresholdDlg
# 	2:25:IDD_VISION_BLOB_THRESHOLD:IDD_VISION_BLOB_THRESHOLD
# 	2:26:VisionBlobThresholdDlg.cpp:VisionBlobThresholdDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:24:VisionBlobThresholdDlg.h:VisionBlobThresholdDlg.h
# 	2:19:Application Include:MPlatformOlb.h
# End Section
# Section MPlatformOlb : {3F994646-9281-450D-B4EB-8B6EEFED7465}
# 	1:21:IDD_VISION_SEARCHAREA:157
# 	2:16:Resource Include:resource.h
# 	2:16:CSearchAreaDlg.h:CSearchAreaDlg.h
# 	2:21:IDD_VISION_SEARCHAREA:IDD_VISION_SEARCHAREA
# 	2:22:CLASS: CCSearchAreaDlg:CCSearchAreaDlg
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:MPlatformOlb.h
# 	2:18:CSearchAreaDlg.cpp:CSearchAreaDlg.cpp
# End Section
# Section MPlatformOlb : {B1A64102-F686-411F-B715-602236F9F0CC}
# 	1:31:IDD_DATA_DEVICE_ID_MATCHING_DLG:230
# 	2:16:Resource Include:resource.h
# 	2:31:IDD_DATA_DEVICE_ID_MATCHING_DLG:IDD_DATA_DEVICE_ID_MATCHING_DLG
# 	2:24:DataMatchingTableDlg.cpp:DataMatchingTableDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:28:CLASS: CDataMatchingTableDlg:CDataMatchingTableDlg
# 	2:19:Application Include:mplatformolb.h
# 	2:22:DataMatchingTableDlg.h:DataMatchingTableDlg.h
# End Section
# Section MPlatformOlb : {1C4A7856-90F6-444A-B32F-FDE940FEF8D1}
# 	1:24:IDD_MANUAL_PANEL_ALIGNER:281
# 	2:16:Resource Include:resource.h
# 	2:25:ManualPanelAlignerDlg.cpp:ManualPanelAlignerDlg.cpp
# 	2:23:ManualPanelAlignerDlg.h:ManualPanelAlignerDlg.h
# 	2:10:ENUM: enum:enum
# 	2:29:CLASS: CManualPanelAlignerDlg:CManualPanelAlignerDlg
# 	2:19:Application Include:mplatformolb.h
# 	2:24:IDD_MANUAL_PANEL_ALIGNER:IDD_MANUAL_PANEL_ALIGNER
# End Section
# Section MPlatformOlb : {5A9F4225-FB2B-4AF5-BFBD-E534B884D611}
# 	1:19:IDD_LOG_FILE_SELECT:149
# 	2:24:CLASS: CLogFileSelectDlg:CLogFileSelectDlg
# 	2:16:Resource Include:resource.h
# 	2:20:LogFileSelectDlg.cpp:LogFileSelectDlg.cpp
# 	2:19:IDD_LOG_FILE_SELECT:IDD_LOG_FILE_SELECT
# 	2:18:LogFileSelectDlg.h:LogFileSelectDlg.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:MPlatformOlb.h
# End Section
# Section MPlatformOlb : {C6D81348-1AE6-4841-8594-8452F907017C}
# 	1:24:IDD_PAGE_ACF_ELAPSE_TIME:275
# 	2:16:Resource Include:resource.h
# 	2:24:IDD_PAGE_ACF_ELAPSE_TIME:IDD_PAGE_ACF_ELAPSE_TIME
# 	2:18:AutoElapseTime.cpp:AutoElapseTime.cpp
# 	2:10:ENUM: enum:enum
# 	2:22:CLASS: CAutoElapseTime:CAutoElapseTime
# 	2:16:AutoElapseTime.h:AutoElapseTime.h
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {C085B98D-9D17-4A52-A70F-98A091EDAECF}
# 	1:17:IDD_LOG_MAIN_FORM:146
# 	2:16:Resource Include:resource.h
# 	2:17:IDD_LOG_MAIN_FORM:IDD_LOG_MAIN_FORM
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:MPlatformOlb.h
# 	2:13:LogMainForm.h:LogMainForm.h
# 	2:19:CLASS: CLogMainForm:CLogMainForm
# 	2:15:LogMainForm.cpp:LogMainForm.cpp
# End Section
# Section MPlatformOlb : {C5800BA9-066A-4B4F-8200-29A28D176EDC}
# 	1:22:IDD_VISION_SEARCH_AREA:196
# 	2:16:Resource Include:resource.h
# 	2:21:VisionSearchAreaDlg.h:VisionSearchAreaDlg.h
# 	2:10:ENUM: enum:enum
# 	2:27:CLASS: CVisionSearchAreaDlg:CVisionSearchAreaDlg
# 	2:22:IDD_VISION_SEARCH_AREA:IDD_VISION_SEARCH_AREA
# 	2:19:Application Include:MPlatformOlb.h
# 	2:23:VisionSearchAreaDlg.cpp:VisionSearchAreaDlg.cpp
# End Section
# Section MPlatformOlb : {097C4CCE-43BB-4F7A-9379-4D1AF38FF54D}
# 	1:20:IDD_COMMON_ERROR_MSG:147
# 	2:16:Resource Include:resource.h
# 	2:20:IDD_COMMON_ERROR_MSG:IDD_COMMON_ERROR_MSG
# 	2:19:CommonErrMsgDlg.cpp:CommonErrMsgDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:23:CLASS: CCommonErrMsgDlg:CCommonErrMsgDlg
# 	2:17:CommonErrMsgDlg.h:CommonErrMsgDlg.h
# 	2:19:Application Include:MPlatformOlb.h
# End Section
# Section MPlatformOlb : {630D0AFF-D9C8-4713-ABCC-7FE81AD3AB7E}
# 	1:23:IDD_TEACH_PANEL_ALIGNER:236
# 	2:16:Resource Include:resource.h
# 	2:23:IDD_TEACH_PANEL_ALIGNER:IDD_TEACH_PANEL_ALIGNER
# 	2:22:TeachPanelAlignerDlg.h:TeachPanelAlignerDlg.h
# 	2:10:ENUM: enum:enum
# 	2:28:CLASS: CTeachPanelAlignerDlg:CTeachPanelAlignerDlg
# 	2:24:TeachPanelAlignerDlg.cpp:TeachPanelAlignerDlg.cpp
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {630C38A0-F5AA-467F-BAB2-0EB42CC4397A}
# 	1:29:IDD_TEACH_CARRIERCAMERAEXPAND:291
# 	2:16:Resource Include:resource.h
# 	2:31:TeachCarrierCameraExpandDlg.cpp:TeachCarrierCameraExpandDlg.cpp
# 	2:29:IDD_TEACH_CARRIERCAMERAEXPAND:IDD_TEACH_CARRIERCAMERAEXPAND
# 	2:29:TeachCarrierCameraExpandDlg.h:TeachCarrierCameraExpandDlg.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:mplatformolb.h
# 	2:35:CLASS: CTeachCarrierCameraExpandDlg:CTeachCarrierCameraExpandDlg
# End Section
# Section MPlatformOlb : {783CD237-52FA-4146-A085-ADE80B880998}
# 	1:18:IDD_TEACH_TRANSFER:237
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:24:CLASS: CTeachTransferDlg:CTeachTransferDlg
# 	2:18:TeachTransferDlg.h:TeachTransferDlg.h
# 	2:19:Application Include:mplatformolb.h
# 	2:18:IDD_TEACH_TRANSFER:IDD_TEACH_TRANSFER
# 	2:20:TeachTransferDlg.cpp:TeachTransferDlg.cpp
# End Section
# Section MPlatformOlb : {DE984808-2684-47AD-815F-4368C597B49B}
# 	1:22:IDD_DATA_MODEL_ID_LIST:224
# 	2:16:Resource Include:resource.h
# 	2:22:IDD_DATA_MODEL_ID_LIST:IDD_DATA_MODEL_ID_LIST
# 	2:17:DataModelIDList.h:DataModelIDList.h
# 	2:23:CLASS: CDataModelIDList:CDataModelIDList
# 	2:19:DataModelIDList.cpp:DataModelIDList.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:MPlatformOlb.h
# End Section
# Section MPlatformOlb : {73FF3892-54D8-4E05-8C1E-B2FF2AC60754}
# 	1:34:IDD_COMMON_SELECT_USERGROUP_DIALOG:153
# 	2:16:Resource Include:resource.h
# 	2:34:IDD_COMMON_SELECT_USERGROUP_DIALOG:IDD_COMMON_SELECT_USERGROUP_DIALOG
# 	2:28:CommonSelectUserGroupDlg.cpp:CommonSelectUserGroupDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:MPlatformOlb.h
# 	2:32:CLASS: CCommonSelectUserGroupDlg:CCommonSelectUserGroupDlg
# 	2:26:CommonSelectUserGroupDlg.h:CommonSelectUserGroupDlg.h
# End Section
# Section MPlatformOlb : {3A2AD7AE-62C0-40A3-B4E2-53E70FA3DAA3}
# 	1:10:IDD_MLCNET:206
# 	2:16:Resource Include:resource.h
# 	2:16:CLASS: MLCNetDlg:MLCNetDlg
# 	2:10:IDD_MLCNET:IDD_MLCNET
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:mplatformolb.h
# 	2:13:MLCNetDlg.cpp:MLCNetDlg.cpp
# 	2:11:MLCNetDlg.h:MLCNetDlg.h
# End Section
# Section MPlatformOlb : {ED12E171-BBC4-462E-BE24-46E573462639}
# 	1:23:IDD_MATERIAL_CHANGE_DLG:248
# 	2:25:CLASS: CMaterialChangeDlg:CMaterialChangeDlg
# 	2:16:Resource Include:resource.h
# 	2:23:IDD_MATERIAL_CHANGE_DLG:IDD_MATERIAL_CHANGE_DLG
# 	2:19:MaterialChangeDlg.h:MaterialChangeDlg.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:mplatformolb.h
# 	2:21:MaterialChangeDlg.cpp:MaterialChangeDlg.cpp
# End Section
# Section MPlatformOlb : {53B7AE93-DE7F-40BF-9E1E-E7FFA7084B4B}
# 	1:24:IDD_TEACH_CAMERA_CARRIER:235
# 	2:16:Resource Include:resource.h
# 	2:24:IDD_TEACH_CAMERA_CARRIER:IDD_TEACH_CAMERA_CARRIER
# 	2:23:TeachCameraCarrierDlg.h:TeachCameraCarrierDlg.h
# 	2:10:ENUM: enum:enum
# 	2:25:TeachCameraCarrierDlg.cpp:TeachCameraCarrierDlg.cpp
# 	2:29:CLASS: CTeachCameraCarrierDlg:CTeachCameraCarrierDlg
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {769CBAD4-CE2C-4D91-8495-DFE4438035D0}
# 	1:7:IDD_MSG:107
# 	2:16:Resource Include:resource.h
# 	2:7:IDD_MSG:IDD_MSG
# 	2:8:MsgDlg.h:MsgDlg.h
# 	2:10:ENUM: enum:enum
# 	2:14:CLASS: CMsgDlg:CMsgDlg
# 	2:10:MsgDlg.cpp:MsgDlg.cpp
# 	2:19:Application Include:MPlatformOlb.h
# End Section
# Section MPlatformOlb : {507926E0-E95F-47FE-8B5C-518DA1728188}
# 	1:14:IDD_LOG_CONFIG:163
# 	2:16:Resource Include:resource.h
# 	2:16:LogConfigDlg.cpp:LogConfigDlg.cpp
# 	2:14:IDD_LOG_CONFIG:IDD_LOG_CONFIG
# 	2:20:CLASS: CLogConfigDlg:CLogConfigDlg
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:MPlatformOlb.h
# 	2:14:LogConfigDlg.h:LogConfigDlg.h
# End Section
# Section MPlatformOlb : {10A1CC72-E28B-45CC-B459-A4D64AD72984}
# 	1:15:IDD_TEACH_PRESS:288
# 	2:16:Resource Include:resource.h
# 	2:15:TeachPressDlg.h:TeachPressDlg.h
# 	2:10:ENUM: enum:enum
# 	2:17:TeachPressDlg.cpp:TeachPressDlg.cpp
# 	2:21:CLASS: CTeachPressDlg:CTeachPressDlg
# 	2:15:IDD_TEACH_PRESS:IDD_TEACH_PRESS
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {7C696AD8-F22C-464D-9426-6B1D360C9A6B}
# 	1:19:IDD_DATA_PANEL_DATA:113
# 	2:16:Resource Include:resource.h
# 	2:16:DataPanelDlg.cpp:DataPanelDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:20:CLASS: CDataPanelDlg:CDataPanelDlg
# 	2:19:Application Include:MPlatformOlb.h
# 	2:14:DataPanelDlg.h:DataPanelDlg.h
# 	2:19:IDD_DATA_PANEL_DATA:IDD_DATA_PANEL_DATA
# End Section
# Section MPlatformOlb : {4B211B40-213B-4264-BE79-257C57AD1D91}
# 	1:18:IDD_VISION_MONITOR:120
# 	2:16:Resource Include:resource.h
# 	2:20:VisionMonitorDlg.cpp:VisionMonitorDlg.cpp
# 	2:18:VisionMonitorDlg.h:VisionMonitorDlg.h
# 	2:10:ENUM: enum:enum
# 	2:18:IDD_VISION_MONITOR:IDD_VISION_MONITOR
# 	2:19:Application Include:MPlatformOlb.h
# 	2:24:CLASS: CVisionMonitorDlg:CVisionMonitorDlg
# End Section
# Section MPlatformOlb : {6BD0BA3E-58C2-4995-9488-FF568028B397}
# 	1:18:IDD_DATA_MOLD_INFO:210
# 	2:16:Resource Include:resource.h
# 	2:23:CLASS: CDataMoldInfoDlg:CDataMoldInfoDlg
# 	2:17:DataMoldInfoDlg.h:DataMoldInfoDlg.h
# 	2:10:ENUM: enum:enum
# 	2:19:DataMoldInfoDlg.cpp:DataMoldInfoDlg.cpp
# 	2:19:Application Include:MPlatformOlb.h
# 	2:18:IDD_DATA_MOLD_INFO:IDD_DATA_MOLD_INFO
# End Section
# Section MPlatformOlb : {92B90293-72A5-416D-94EA-B4B1D551941C}
# 	1:14:IDD_COMMON_MSG:126
# 	2:16:Resource Include:resource.h
# 	2:20:CLASS: CCommonMsgDlg:CCommonMsgDlg
# 	2:14:CommonMsgDlg.h:CommonMsgDlg.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:MPlatformOlb.h
# 	2:14:IDD_COMMON_MSG:IDD_COMMON_MSG
# 	2:16:CommonMsgDlg.cpp:CommonMsgDlg.cpp
# End Section
# Section MPlatformOlb : {80CC1915-C663-4ACB-AA56-90506D8D2818}
# 	1:22:IDD_MODULE_PORT_ID_DLG:266
# 	2:16:Resource Include:resource.h
# 	2:23:CLASS: CModulePortIDDlg:CModulePortIDDlg
# 	2:19:ModulePortIDDlg.cpp:ModulePortIDDlg.cpp
# 	2:22:IDD_MODULE_PORT_ID_DLG:IDD_MODULE_PORT_ID_DLG
# 	2:10:ENUM: enum:enum
# 	2:17:ModulePortIDDlg.h:ModulePortIDDlg.h
# 	2:19:Application Include:mplatformolb.h
# End Section
# Section MPlatformOlb : {D13326EC-97AB-412F-A7BE-EE681AD95C46}
# 	1:15:IDD_VISION_VIEW:198
# 	2:16:Resource Include:resource.h
# 	2:21:CLASS: CVisionViewDlg:CVisionViewDlg
# 	2:17:VisionViewDlg.cpp:VisionViewDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:15:VisionViewDlg.h:VisionViewDlg.h
# 	2:19:Application Include:MPlatformOlb.h
# 	2:15:IDD_VISION_VIEW:IDD_VISION_VIEW
# End Section
# Section MPlatformOlb : {7210C4FE-74C6-4FBC-94C4-9A943D694539}
# 	1:26:IDD_DATA_SEARCH_MAJOR_SPEC:279
# 	2:16:Resource Include:resource.h
# 	2:27:CLASS: CDataSearchMajorSpec:CDataSearchMajorSpec
# 	2:21:DataSearchMajorSpec.h:DataSearchMajorSpec.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:mplatformolb.h
# 	2:26:IDD_DATA_SEARCH_MAJOR_SPEC:IDD_DATA_SEARCH_MAJOR_SPEC
# 	2:23:DataSearchMajorSpec.cpp:DataSearchMajorSpec.cpp
# End Section
# Section MPlatformOlb : {60AFFCB3-3AEF-46F9-8808-D6258B5916A4}
# 	1:19:IDD_TEACH_SUPPLIER1:212
# 	2:16:Resource Include:resource.h
# 	2:20:TeachSupplierDlg.cpp:TeachSupplierDlg1.cpp
# 	2:10:ENUM: enum:enum
# 	2:18:IDD_TEACH_SUPPLIER:IDD_TEACH_SUPPLIER1
# 	2:18:TeachSupplierDlg.h:TeachSupplierDlg1.h
# 	2:19:Application Include:MPlatformOlb.h
# 	2:24:CLASS: CTeachSupplierDlg:CTeachSupplierDlg
# End Section
