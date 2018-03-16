////////////////////////////////////////////////////////////////////////////////
//! 
//! \file  mil3dmap.h
//! 
//! \brief Mil3dmap CAPI header (M3dmap...)
//! 
//! AUTHOR: Matrox Imaging dept.
//!
//! COPYRIGHT NOTICE:
//! Copyright © Matrox Electronic Systems Ltd., 1992-2010.
//! All Rights Reserved 
//  Revision:  9.02.0580
////////////////////////////////////////////////////////////////////////////////

#ifndef __MIL_3DMAP_H__
#define __MIL_3DMAP_H__

#define BW_COMPATIBILITY 0x092

#if (!M_MIL_LITE) /* MIL FULL ONLY */

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
   {
#endif

////////////////////////////////////////////////////////////////////////////////
// M3dmapAlloc ContextTypes

#define M_LASER                                       1L
#define M_GEOMETRY                                 1306L

// ControlFlags for M_LASER ContextType

#define M_CALIBRATED_CAMERA_LINEAR_MOTION             1L
#define M_DEPTH_CORRECTION                            2L

////////////////////////////////////////////////////////////////////////////////
// M3dmapAllocResult ResultTypes

#define M_LASER_DATA                                  1L
#define M_STAT_RESULT                              1307L

////////////////////////////////////////////////////////////////////////////////
// M3dmapControl/M3dmapInquire

// ControlTypes for 3d reconstruction contexts

#define M_SCAN_SPEED                                  2L
#define M_FILL_MODE                                   3L
#define M_FILL_SHARP_ELEVATION                        4L
#define M_FILL_SHARP_ELEVATION_DEPTH                  5L

#define M_PEAK_WIDTH                                 11L
#define M_MIN_INTENSITY                              12L

#define M_MAX_FRAMES                                 17L
#define M_CORRECTED_DEPTH                            18L

#define M_CALIBRATION_STATUS                        159L // Already defined in milcal.h

#define M_CAMERA_IMAGE_SIZE_X                      1303L
#define M_CAMERA_IMAGE_SIZE_Y                      1304L
#define M_EXTRACTION_CHILD_OFFSET_X                1308L
#define M_EXTRACTION_CHILD_OFFSET_Y                1309L
#define M_EXTRACTION_FIXED_POINT                   1310L
#define M_EXTRACTION_CUMULATIVE                    1311L
#define M_EXTRACTION_SCALE_MODE                    1312L
#define M_LASER_CONTEXT_TYPE                       1313L
#define M_CALIBRATION_DEPTHS                       1314L
#define M_NUMBER_OF_CALIBRATION_DEPTHS             1315L
#define M_NUMBER_OF_MISSING_DATA_PER_COLUMN        1316L
#define M_NUMBER_OF_COLUMNS                        1317L
#define M_NUMBER_OF_COLUMNS_WITH_MISSING_DATA      1318L
#define M_NUMBER_OF_INVERSIONS_PER_COLUMN          1319L
#define M_NUMBER_OF_COLUMNS_WITH_INVERSIONS        1320L
#define M_FIT_RMS_ERROR                            1333L
#define M_LASER_PLANE_A                            1385L
#define M_LASER_PLANE_B                            1386L
#define M_LASER_PLANE_C                            1387L
#define M_LASER_PLANE_D                            1388L
#define M_EXTRACTION_CHILD_OFFSET_X_TO_USE         1389L
#define M_EXTRACTION_CHILD_OFFSET_Y_TO_USE         1390L
#define M_CAMERA_IMAGE_SIZE_IS_KNOWN               1404L
#define M_INTERNAL_CALIBRATION                     1408L

#define M_ORIENTATION                        0x00002400L // Already defined in mil.h, milpat.h, milmeas.h

// ControlTypes for 3d reconstruction result buffers

#define M_PIXEL_SIZE_X                              139L // Already defined in mildisplay.h, milcal.h
#define M_PIXEL_SIZE_Y                              140L // Already defined in mildisplay.h, milcal.h
//      ALREADY_USED                                141L
//      ALREADY_USED                                142L
//      ALREADY_USED                                143L
//      ALREADY_USED                                144L

//      ALREADY_USED                                159L

#define M_WORLD_POS_X                               207L // Already defined in milcal.h
#define M_WORLD_POS_Y                               208L // Already defined in milcal.h
#define M_GRAY_LEVEL_SIZE_Z                        1370L // Already defined in milcal.h
#define M_WORLD_POS_Z                              1371L // Already defined in milcal.h

// ControlTypes for geometry objects

#define M_GEOMETRY_TYPE                            1321L
#define M_FIT_PARAM_AX                             1322L
#define M_FIT_PARAM_AXX                            1323L
#define M_FIT_PARAM_AXY                            1324L
#define M_FIT_PARAM_AY                             1325L
#define M_FIT_PARAM_AYY                            1326L
#define M_FIT_PARAM_RADIUS                         1327L
#define M_FIT_PARAM_X0                             1328L
#define M_FIT_PARAM_Y0                             1329L
#define M_FIT_PARAM_Z0                             1330L
#define M_FIT_PARAM_SIGN                           1331L
#define M_GEOMETRY_DEFINITION_TYPE                 1332L
#define M_FIT_RMS_ERROR                            1333L

#define M_STATUS                             0x00008002L // Already defined in milcode.h, milcolor.h, miledge.h

// ControlTypes for stat result buffers

#define M_MAX_PIXELS_FOR_STAT                      1334L

// Control Values for M_FIT_PARAM_SIGN

#define M_POSITIVE                                    2L // Already defined in milmeas.h
#define M_NEGATIVE                                   -2L // Already defined in milmeas.h

// Control Values for M_FILL_MODE

#define M_DISABLE                                 -9999L // Already defined in milblob.h, milcal.h, milmeas.h, mil.h, ...
#define M_X_THEN_Y                                    1L

// Control Values for M_FILL_SHARP_ELEVATION

#define M_DISABLE                                 -9999L // Already defined in milblob.h, milcal.h, milmeas.h, mil.h, ...
#define M_MIN                                0x02000000L // Already defined in mil.h, milim.h, ...
#define M_MAX                                0x04000000L // Already defined in mil.h, milim.h, ...

// Control Values for M_FILL_SHARP_ELEVATION_DEPTH

#define M_INFINITE                                   -1L // Already defined in mil.h, milstr.h, milreg.h, milmetrol.h

// Control Values for M_EXTRACTION_SCALE_MODE

#define M_AUTO_SCALE                                  2L // Already defined in mil.h, mildisplay.h
#define M_USER_DEFINED                               21L // Already defined in mil.h, milcal.h, milcol.h, ...
#define M_USE_DESTINATION_SCALES                   1335L

// Combination flags for M_AUTO_SCALE control value of M_EXTRACTION_SCALE_MODE

#define M_TOP_WHITE                          0x00040000L
#define M_TOP_BLACK                          0x00080000L

// Control Values for M_ORIENTATION

#define M_1D_ROWS                            0x00000010L // Already defined in milim.h
#define M_1D_COLUMNS                         0x00000020L // Already defined in milim.h

// Control Values for M_CALIBRATION_STATUS

#define M_CALIBRATED                          0x0000300L // Already defined in milcal.h, milmod.h
#define M_LASER_LINE_NOT_DETECTED                     2L
#define M_NOT_INITIALIZED                             3L // Already defined in milcal.h
#define M_NOT_ENOUGH_MEMORY                           4L
#define M_INTERNAL_ERROR                              5L
#define M_MATHEMATICAL_EXCEPTION                      8L // Already defined in milcal.h

// Control Values for M_STATUS

#define M_SUCCESS                            0x00000000L // Already defined in milreg.h, milcolor.h
#define M_NOT_INITIALIZED                             3L // Already defined in milcal.h
#define M_NOT_ENOUGH_VALID_DATA                    1336L
#define M_ALL_POINTS_COLINEAR                      1337L

// Control Values for M_GEOMETRY_DEFINITION_TYPE

#define M_FIT                                    0x0020L // Already defined in milcal.h, milmetrol.h
#define M_PARAMETRIC                             0x0021L // Already defined in milmetrol.h

////////////////////////////////////////////////////////////////////////////////
// M3dmapAddScan ControlFlags

#define M_RESET                                       9L // Already defined in mildisplay.h, milblob.h, ...
#define M_EXTRACT_LINE                             1338L
#define M_LINE_ALREADY_EXTRACTED                   1339L
#define M_CLEAR_DATA                               1340L
#define M_REMOVE_LAST_SCAN                         1493L

////////////////////////////////////////////////////////////////////////////////
// M3dmapExtract Operations

#define M_CORRECTED_DEPTH_MAP                         1L
#define M_UNCORRECTED_DEPTH_MAP                    1341L
#define M_FILL_MISSING_DATA_ONLY                   1342L

////////////////////////////////////////////////////////////////////////////////
// M3dmapGetResult ResultTypes

#define M_3D_POINTS_X                                 1L
#define M_3D_POINTS_Y                                 2L
#define M_3D_POINTS_Z                                 3L
#define M_3D_POINTS_I                                 4L
#define M_NUMBER_OF_3D_POINTS                         5L
#define M_CORRECTED_DEPTH_MAP_SIZE_X                  6L
#define M_CORRECTED_DEPTH_MAP_SIZE_Y                  7L
#define M_CORRECTED_DEPTH_MAP_BUFFER_TYPE             8L
#define M_INTENSITY_MAP_BUFFER_TYPE                   9L
#define M_CAMERA_IMAGE_SIZE_X                      1303L
#define M_CAMERA_IMAGE_SIZE_Y                      1304L
#define M_UNCORRECTED_DEPTH_MAP_FIXED_POINT        1343L
#define M_UNCORRECTED_DEPTH_MAP_SIZE_X             1344L
#define M_UNCORRECTED_DEPTH_MAP_SIZE_Y             1345L
#define M_UNCORRECTED_DEPTH_MAP_BUFFER_TYPE        1346L
#define M_DEPTH_CORRECTION_STATE                   1347L
#define M_NUMBER_OF_MISSING_DATA_LAST_SCAN         1348L

// Control Values for M_DEPTH_CORRECTION_STATE

#define M_UNCORRECTED                              1349L
#define M_PARTIALLY_CORRECTED                      1350L
#define M_FULLY_CORRECTED                          1351L

////////////////////////////////////////////////////////////////////////////////
// M3dmapTriangulate ControlFlags

#define M_NO_INVALID_POINT                            1L

////////////////////////////////////////////////////////////////////////////////
// M3dmapSetGeometry

// GeometryTypes
#define M_HORIZONTAL_PLANE                         1352L
#define M_PLANE                                    1353L
#define M_HALFSPHERE                               1354L

// OperationTypes
#define M_FIT                                    0x0020L // Already defined in milcal.h, milmetrol.h
#define M_PARAMETRIC                             0x0021L // Already defined in milmetrol.h

// Fit directions
#define M_GREATER_Z                                1355L
#define M_LOWER_Z                                  1356L

////////////////////////////////////////////////////////////////////////////////
// M3dmapDraw Operations

#define M_DRAW_PEAKS_LAST                          1305L
#define M_DRAW_GEOMETRY                            1357L
#define M_DRAW_REGION_VALID                        1380L
#define M_DRAW_REGION_INTERPOLATED                 1381L
#define M_DRAW_REGION_UNCALIBRATED                 1382L
#define M_DRAW_REGION_MISSING_DATA                 1383L
#define M_DRAW_REGION_INVERTED                     1384L
#define M_DRAW_CALIBRATION_LINES                   1391L

////////////////////////////////////////////////////////////////////////////////
// M3dmapStat/M3dmapGetResult

// StatTypes
#define M_DEVIATION_MEAN                           1358L
#define M_DEVIATION_MAX                            1359L
#define M_VOLUME                                   1360L
#define M_CROSS_SECTION_AREA                       1361L
#define M_NUMBER_OF_PIXELS_VALID                   1362L
#define M_NUMBER_OF_PIXELS_OUTLIER                 1363L
#define M_NUMBER_OF_PIXELS_MISSING_DATA            1364L
#define M_NUMBER_OF_PIXELS_TOTAL                   1365L

// StatType combination flags
#define M_STAT_POSITIVE                          0x1000L
#define M_STAT_NEGATIVE                          0x2000L
#define M_STAT_ABS                               0x4000L
#define M_STAT_ALL                               0x8000L

////////////////////////////////////////////////////////////////////////////////
// M3dmapArith

// Operations
#define M_ADD                                    0x0000L // Already defined in milim.h
#define M_SUB                                    0x0001L // Already defined in milim.h
#define M_NEG_SUB                                0x000AL // Already defined in milim.h
#define M_SUB_ABS                                0x0011L // Already defined in milim.h
#define M_MIN                                0x02000000L // Already defined in milim.h
#define M_MAX                                0x04000000L // Already defined in milim.h

// ControlFlags
#define M_USE_DESTINATION_SCALES                   1335L
#define M_FIT_SCALES                               1366L
#define M_SET_WORLD_OFFSET_Z                       1367L

////////////////////////////////////////////////////////////////////////////////
// Other defines

// Possible value for OutlierDistanceWorld, MaxRadius
#if (BW_COMPATIBILITY == 0x080)
#define M_INFINITE                           0xFFFFFFFFL
#else
#define M_INFINITE                                   -1L // Already defined in mil.h
#endif

////////////////////////////////////////////////////////////////////////////////
// Deprecated

// Deprecated names, kept for code backward compatibility
#define M_FILL_GAP_MODE                    M_FILL_SHARP_ELEVATION
#define M_FILL_GAP_DEPTH                   M_FILL_SHARP_ELEVATION_DEPTH

// Deprecated values, kept for binary backward compatibility
#define M_DEPRECATED_GRAY_LEVEL_SIZE_Z              141L // Deprecated, use M_GRAY_LEVEL_SIZE_Z
#define M_WORLD_OFFSET_X                            142L // Deprecated, use M_WORLD_POS_X
#define M_WORLD_OFFSET_Y                            143L // Deprecated, use M_WORLD_POS_Y
#define M_WORLD_OFFSET_Z                            144L // Deprecated, use M_WORLD_POS_Z

////////////////////////////////////////////////////////////////////////////////
// CAPI function prototypes

#ifndef __midl // MIDL compiler used by ActiveMIL

MFTYPE32 MIL_ID MFTYPE M3dmapAlloc(MIL_ID  SystemId, 
                                   MIL_INT ContextType, 
                                   MIL_INT ControlFlag, 
                                   MIL_ID* ContextIdPtr);

MFTYPE32 MIL_ID MFTYPE M3dmapAllocResult(MIL_ID  SystemId,
                                         MIL_INT ResultType, 
                                         MIL_INT ControlFlag, 
                                         MIL_ID* ResultIdPtr);

MFTYPE32 void MFTYPE M3dmapFree(MIL_ID ContextOrResultId);

#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs
MFTYPE32 void MFTYPE M3dmapControlInt64(MIL_ID    ContextOrResultId,
                                        MIL_INT   Index,
                                        MIL_INT   ControlType,
                                        MIL_INT64 ControlValue);
MFTYPE32 void MFTYPE M3dmapControlDouble(MIL_ID     ContextOrResultId,
                                         MIL_INT    Index,
                                         MIL_INT    ControlType,
                                         MIL_DOUBLE ControlValue);
#else
// Prototypes for 32 bits OSs
#define M3dmapControlInt64  M3dmapControl
#define M3dmapControlDouble M3dmapControl
MFTYPE32 void MFTYPE M3dmapControl(MIL_ID     ContextOrResultId,
                                   MIL_INT    Index,
                                   MIL_INT    ControlType,
                                   MIL_DOUBLE ControlValue);
#endif

MFTYPE32 MIL_INT MFTYPE M3dmapInquire(MIL_ID  ContextOrResultId,
                                      MIL_INT Index,
                                      MIL_INT InquireType,
                                      void*   UserVarPtr);

MFTYPE32 void MFTYPE M3dmapAddScan(MIL_ID  ContextId,
                                   MIL_ID  ResultId,
                                   MIL_ID  LaserOrDepthMapImageId, 
                                   MIL_ID  IntensityImageId,
                                   MIL_ID  ExtraInfoArrayId,
                                   MIL_INT Index,
                                   MIL_INT ControlFlag);

MFTYPE32 void MFTYPE M3dmapCalibrate(MIL_ID  ContextId,
                                     MIL_ID  ResultId,
                                     MIL_ID  CalibrationId,
                                     MIL_INT ControlFlag);

MFTYPE32 void MFTYPE M3dmapExtract(MIL_ID  ResultId,
                                   MIL_ID  DepthMapId,
                                   MIL_ID  IntensityMapId,
                                   MIL_INT Operation,
                                   MIL_INT Index,
                                   MIL_INT ControlFlag);

MFTYPE32 void MFTYPE M3dmapGetResult(MIL_ID  ResultId,
                                     MIL_INT Index,
                                     MIL_INT ResultType,
                                     void*   ResultArrayPtr);

MFTYPE32 void MFTYPE M3dmapTriangulate(const MIL_ID*     CalibrationOrImageIdArrayPtr,
                                       const MIL_DOUBLE* XPixelArrayPtr,
                                       const MIL_DOUBLE* YPixelArrayPtr,
                                       MIL_DOUBLE*       XWorldArrayPtr,
                                       MIL_DOUBLE*       YWorldArrayPtr,
                                       MIL_DOUBLE*       ZWorldArrayPtr,
                                       MIL_DOUBLE*       RMSErrorArrayPtr,
                                       MIL_INT           NbCalibrations,
                                       MIL_INT           NbPoints,
                                       MIL_INT           CoordinateSystem,
                                       MIL_INT           ControlFlag);

MFTYPE32 void MFTYPE M3dmapSetGeometry(MIL_ID     GeometryId,
                                       MIL_INT    GeometryType,
                                       MIL_INT    OperationType,
                                       MIL_DOUBLE Param1,
                                       MIL_DOUBLE Param2,
                                       MIL_DOUBLE Param3,
                                       MIL_DOUBLE Param4,
                                       MIL_DOUBLE Param5);

MFTYPE32 void MFTYPE M3dmapDraw(MIL_ID  GraphContId,
                                MIL_ID  ContextOrResultId,
                                MIL_ID  DestImageBufId,
                                MIL_INT Operation,
                                MIL_INT Index,
                                MIL_INT ControlFlag);

MFTYPE32 void MFTYPE M3dmapStat(MIL_ID      ImageBufId,
                                MIL_ID      ImageBufOrGeometryId,
                                MIL_ID      MaskBufId,
                                MIL_ID      ResultId,
                                MIL_INT     StatType,
                                MIL_DOUBLE  OutlierDistanceWorld,
                                MIL_INT     ControlFlag,
                                MIL_DOUBLE* StatValuePtr);

MFTYPE32 void MFTYPE M3dmapArith(MIL_ID  Src1ImageBufOrGeometryId,
                                 MIL_ID  Src2ImageBufOrGeometryId,
                                 MIL_ID  DestImageBufId,
                                 MIL_ID  MaskBufId,
                                 MIL_INT Operation,
                                 MIL_INT ControlFlag);

#if M_MIL_USE_UNICODE
MFTYPE32 void MFTYPE M3dmapSaveA(const char* FileName,
                                 MIL_ID      ContextId,
                                 MIL_INT     ControlFlag);

MFTYPE32 MIL_ID MFTYPE M3dmapRestoreA(const char* FileName,
                                      MIL_ID      SystemId,
                                      MIL_INT     ControlFlag,
                                      MIL_ID*     ContextIdPtr);

MFTYPE32 void MFTYPE M3dmapStreamA(char*      MemPtrOrFileName,
                                   MIL_ID     SystemId,
                                   MIL_INT    Operation,
                                   MIL_INT    StreamType,
                                   MIL_DOUBLE Version,
                                   MIL_INT    ControlFlag,
                                   MIL_ID*    ContextIdPtr,
                                   MIL_INT*   SizeByteVarPtr);

MFTYPE32 void MFTYPE M3dmapSaveW(MIL_CONST_TEXT_PTR FileName,
                                 MIL_ID             ContextId,
                                 MIL_INT            ControlFlag);

MFTYPE32 MIL_ID MFTYPE M3dmapRestoreW(MIL_CONST_TEXT_PTR FileName,
                                      MIL_ID             SystemId,
                                      MIL_INT            ControlFlag,
                                      MIL_ID*            ContextIdPtr);

MFTYPE32 void MFTYPE M3dmapStreamW(MIL_TEXT_PTR MemPtrOrFileName,
                                   MIL_ID       SystemId,
                                   MIL_INT      Operation,
                                   MIL_INT      StreamType,
                                   MIL_DOUBLE   Version,
                                   MIL_INT      ControlFlag,
                                   MIL_ID*      ContextIdPtr,
                                   MIL_INT*     SizeByteVarPtr);

#if M_MIL_UNICODE_API
#define M3dmapSave           M3dmapSaveW
#define M3dmapRestore        M3dmapRestoreW
#define M3dmapStream         M3dmapStreamW
#else
#define M3dmapSave           M3dmapSaveA
#define M3dmapRestore        M3dmapRestoreA
#define M3dmapStream         M3dmapStreamA
#endif

#else
MFTYPE32 void MFTYPE M3dmapSave(MIL_CONST_TEXT_PTR FileName,
                                MIL_ID             ContextId,
                                MIL_INT            ControlFlag);

MFTYPE32 MIL_ID MFTYPE M3dmapRestore(MIL_CONST_TEXT_PTR FileName,
                                     MIL_ID             SystemId,
                                     MIL_INT            ControlFlag,
                                     MIL_ID*            ContextIdPtr);

MFTYPE32 void MFTYPE M3dmapStream(MIL_TEXT_PTR MemPtrOrFileName,
                                  MIL_ID       SystemId,
                                  MIL_INT      Operation,
                                  MIL_INT      StreamType,
                                  MIL_DOUBLE   Version,
                                  MIL_INT      ControlFlag,
                                  MIL_ID*      ContextIdPtr,
                                  MIL_INT*     SizeByteVarPtr);
#endif

#endif /* #ifdef __midl */

/* C++ directive if needed */
#ifdef __cplusplus
}
#endif
////////////////////////////////////////////////////////////////////////////////

#if M_MIL_USE_64BIT
#ifdef __cplusplus
//////////////////////////////////////////////////////////////
// M3dmapControl function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_MIL_USE_LINUX
inline void M3dmapControl(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT ControlType, int ControlValue)
   {
   M3dmapControlInt64(ContextOrResultId, Index, ControlType, ControlValue);
   };
#endif
inline void M3dmapControl(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT ControlType, MIL_INT32 ControlValue)
   {
   M3dmapControlInt64(ContextOrResultId, Index, ControlType, ControlValue);
   };

inline void M3dmapControl(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT ControlType, MIL_INT64 ControlValue)
   {
   M3dmapControlInt64(ContextOrResultId, Index, ControlType, ControlValue);
   };

inline void M3dmapControl(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT ControlType, MIL_DOUBLE ControlValue)
   {
   M3dmapControlDouble(ContextOrResultId, Index, ControlType, ControlValue);
   };
#else
//////////////////////////////////////////////////////////////
// For C file, call the default function, i.e. Double one
//////////////////////////////////////////////////////////////
#define M3dmapControl  M3dmapControlDouble

#endif // __cplusplus
#endif // M_MIL_USE_64BIT

#if M_MIL_USE_SAFE_TYPE

//////////////////////////////////////////////////////////////
// See milos.h for explanation about these functions.
//////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------
//  M3dmapGetResult

inline MFTYPE32 void MFTYPE M3dmapGetResultUnsafe  (MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, void          MPTYPE *ResultArrayPtr);
inline MFTYPE32 void MFTYPE M3dmapGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, int                   ResultArrayPtr);
inline MFTYPE32 void MFTYPE M3dmapGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_INT8      MPTYPE *ResultArrayPtr);
inline MFTYPE32 void MFTYPE M3dmapGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_INT16     MPTYPE *ResultArrayPtr);
inline MFTYPE32 void MFTYPE M3dmapGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_INT32     MPTYPE *ResultArrayPtr);
inline MFTYPE32 void MFTYPE M3dmapGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_INT64     MPTYPE *ResultArrayPtr);
inline MFTYPE32 void MFTYPE M3dmapGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, float         MPTYPE *ResultArrayPtr);
inline MFTYPE32 void MFTYPE M3dmapGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_DOUBLE    MPTYPE *ResultArrayPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 void MFTYPE M3dmapGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_UINT8     MPTYPE *ResultArrayPtr);
inline MFTYPE32 void MFTYPE M3dmapGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_UINT16    MPTYPE *ResultArrayPtr);
inline MFTYPE32 void MFTYPE M3dmapGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_UINT32    MPTYPE *ResultArrayPtr);
inline MFTYPE32 void MFTYPE M3dmapGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_UINT64    MPTYPE *ResultArrayPtr);
#endif

// ----------------------------------------------------------
// M3dmapInquire

inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireUnsafe  (MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, void       MPTYPE * UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, int                 UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_INT8   MPTYPE * UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_INT16  MPTYPE * UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_INT32  MPTYPE * UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_INT64  MPTYPE * UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, float      MPTYPE * UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_DOUBLE MPTYPE * UserVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_UINT8  MPTYPE * UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_UINT16 MPTYPE * UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_UINT32 MPTYPE * UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_UINT64 MPTYPE * UserVarPtr);
#endif
#if M_MIL_SAFE_TYPE_ADD_WCHAR_T
inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, wchar_t    MPTYPE * UserVarPtr);
#endif

// -------------------------------------------------------------------------
// M3dmapGetResult

inline MFTYPE32 void MFTYPE M3dmapGetResultSafeType (MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, int ResultPtr)
   {
   if (ResultPtr != 0)
      SafeTypeError(MT("M3dmapGetResult"));

   M3dmapGetResult(ResultId, Index, ResultType, NULL);
   }

inline void M3dmapGetResultSafeTypeExecute (MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, void MPTYPE *ResultArrayPtr, MIL_INT GivenType)
   {
   MIL_INT RequiredType = (ResultType & (M_TYPE_DOUBLE | M_TYPE_FLOAT | M_TYPE_SHORT | M_TYPE_CHAR | M_TYPE_MIL_INT32 | M_TYPE_MIL_INT64 | M_TYPE_MIL_ID ));
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MT("M3dmapGetResult"));

   M3dmapGetResult(ResultId, Index, ResultType, ResultArrayPtr);
   }


inline MFTYPE32 void MFTYPE M3dmapGetResultUnsafe  (MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, void       MPTYPE *ResultArrayPtr){M3dmapGetResult               (ResultId, Index, ResultType, ResultArrayPtr);}
inline MFTYPE32 void MFTYPE M3dmapGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_INT8   MPTYPE *ResultArrayPtr){M3dmapGetResultSafeTypeExecute(ResultId, Index, ResultType, ResultArrayPtr, M_TYPE_CHAR     );}
inline MFTYPE32 void MFTYPE M3dmapGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_INT16  MPTYPE *ResultArrayPtr){M3dmapGetResultSafeTypeExecute(ResultId, Index, ResultType, ResultArrayPtr, M_TYPE_SHORT    );}
inline MFTYPE32 void MFTYPE M3dmapGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_INT32  MPTYPE *ResultArrayPtr){M3dmapGetResultSafeTypeExecute(ResultId, Index, ResultType, ResultArrayPtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 void MFTYPE M3dmapGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_INT64  MPTYPE *ResultArrayPtr){M3dmapGetResultSafeTypeExecute(ResultId, Index, ResultType, ResultArrayPtr, M_TYPE_MIL_INT64);}
inline MFTYPE32 void MFTYPE M3dmapGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, float      MPTYPE *ResultArrayPtr){M3dmapGetResultSafeTypeExecute(ResultId, Index, ResultType, ResultArrayPtr, M_TYPE_FLOAT);}
inline MFTYPE32 void MFTYPE M3dmapGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_DOUBLE MPTYPE *ResultArrayPtr){M3dmapGetResultSafeTypeExecute(ResultId, Index, ResultType, ResultArrayPtr, M_TYPE_DOUBLE);}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 void MFTYPE M3dmapGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_UINT8  MPTYPE *ResultArrayPtr){M3dmapGetResultSafeTypeExecute(ResultId, Index, ResultType, ResultArrayPtr, M_TYPE_CHAR);}
inline MFTYPE32 void MFTYPE M3dmapGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_UINT16 MPTYPE *ResultArrayPtr){M3dmapGetResultSafeTypeExecute(ResultId, Index, ResultType, ResultArrayPtr, M_TYPE_SHORT);}
inline MFTYPE32 void MFTYPE M3dmapGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_UINT32 MPTYPE *ResultArrayPtr){M3dmapGetResultSafeTypeExecute(ResultId, Index, ResultType, ResultArrayPtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 void MFTYPE M3dmapGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_UINT64 MPTYPE *ResultArrayPtr){M3dmapGetResultSafeTypeExecute(ResultId, Index, ResultType, ResultArrayPtr, M_TYPE_MIL_INT64);}
#endif

// ----------------------------------------------------------
// M3dmapInquire

inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, int UserVarPtr)
   {
   if (UserVarPtr != 0)
      SafeTypeError(MT("M3dmapInquire"));

   return M3dmapInquire(ContextOrResultId, Index, InquireType, NULL);
   }

inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeTypeExecute(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, void MPTYPE * UserVarPtr, MIL_INT GivenType)
   {
   MIL_INT RequiredType = (InquireType & (M_TYPE_MIL_INT32 | M_TYPE_MIL_INT64 | M_TYPE_DOUBLE | M_TYPE_FLOAT | M_TYPE_SHORT | M_TYPE_CHAR | M_TYPE_MIL_ID));
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MT("M3dmapInquire"));

   return M3dmapInquire(ContextOrResultId, Index, InquireType, UserVarPtr);
   }

inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireUnsafe  (MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, void       MPTYPE * UserVarPtr) {return M3dmapInquire       (ContextOrResultId, Index, InquireType, UserVarPtr                  );}
inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_INT8   MPTYPE * UserVarPtr) {return M3dmapInquireSafeTypeExecute(ContextOrResultId, Index, InquireType, UserVarPtr, M_TYPE_CHAR);}
inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_INT16  MPTYPE * UserVarPtr) {return M3dmapInquireSafeTypeExecute(ContextOrResultId, Index, InquireType, UserVarPtr, M_TYPE_SHORT);}
inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_INT32  MPTYPE * UserVarPtr) {return M3dmapInquireSafeTypeExecute(ContextOrResultId, Index, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_INT64  MPTYPE * UserVarPtr) {return M3dmapInquireSafeTypeExecute(ContextOrResultId, Index, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, float      MPTYPE * UserVarPtr) {return M3dmapInquireSafeTypeExecute(ContextOrResultId, Index, InquireType, UserVarPtr, M_TYPE_FLOAT   );}
inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_DOUBLE MPTYPE * UserVarPtr) {return M3dmapInquireSafeTypeExecute(ContextOrResultId, Index, InquireType, UserVarPtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_UINT8  MPTYPE * UserVarPtr) {return M3dmapInquireSafeTypeExecute(ContextOrResultId, Index, InquireType, UserVarPtr, M_TYPE_CHAR);}
inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_UINT16 MPTYPE * UserVarPtr) {return M3dmapInquireSafeTypeExecute(ContextOrResultId, Index, InquireType, UserVarPtr, M_TYPE_SHORT);}
inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_UINT32 MPTYPE * UserVarPtr) {return M3dmapInquireSafeTypeExecute(ContextOrResultId, Index, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_UINT64 MPTYPE * UserVarPtr) {return M3dmapInquireSafeTypeExecute(ContextOrResultId, Index, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
#endif
#if M_MIL_SAFE_TYPE_ADD_WCHAR_T
inline MFTYPE32 MIL_INT MFTYPE M3dmapInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, wchar_t    MPTYPE * UserVarPtr) {return M3dmapInquireSafeTypeExecute(ContextOrResultId, Index, InquireType, UserVarPtr, M_TYPE_SHORT);}
#endif

#define M3dmapGetResult        M3dmapGetResultSafeType
#define M3dmapInquire          M3dmapInquireSafeType

#else // #if M_MIL_USE_SAFE_TYPE

#define M3dmapGetResultUnsafe  M3dmapGetResult
#define M3dmapInquireUnsafe    M3dmapInquire

#endif // #if M_MIL_USE_SAFE_TYPE

#endif /* !M_MIL_LITE */

#endif
