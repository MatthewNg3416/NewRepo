/***************************************************************************/
/*

    Filename:  MILBLOB.H
    Owner   :  Matrox Imaging dept.
    Revision:  9.02.0580
    Content :  This file contains the defines for the MIL blob
               analysis module. (Mblob...).

    Copyright © Matrox Electronic Systems Ltd., 1992-2010.
    All Rights Reserved

*/
/***************************************************************************/

#ifndef __MILBLOB_H__
#define __MILBLOB_H__

#define BW_COMPATIBILITY 0x092

#if (!M_MIL_LITE) // MIL FULL ONLY

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif

/* Binary only */

#define M_LABEL_VALUE              1L
#define M_AREA                     2L
#define M_PERIMETER                3L
#define M_FERET_X                 72L
#define M_FERET_Y                  5L
#define M_WORLD_FERET_X         1465L
#define M_WORLD_FERET_Y         1466L
#define M_WORLD_BOX_X_MIN       1479L
#define M_WORLD_BOX_Y_MIN       1480L
#define M_WORLD_BOX_X_MAX       1481L
#define M_WORLD_BOX_Y_MAX       1482L
#define M_BOX_X_MIN                6L
#define M_BOX_Y_MIN                7L
#define M_BOX_X_MAX                8L
#define M_BOX_Y_MAX                9L
#define M_BOX_AREA                90L
#define M_BOX_ASPECT_RATIO        91L
#define M_BOX_FILL_RATIO          92L
#define M_CONVEX_HULL_FILL_RATIO  93L
#define M_FIRST_POINT_X           75L
#define M_FIRST_POINT_Y          132L
#define M_AXIS_PRINCIPAL_LENGTH   12L
#define M_AXIS_SECONDARY_LENGTH   13L
#define M_FERET_MIN_DIAMETER      14L
#define M_FERET_MIN_ANGLE         15L
#define M_FERET_MAX_DIAMETER      16L
#define M_FERET_MAX_ANGLE         17L
#define M_FERET_MEAN_DIAMETER     18L
#define M_CONVEX_AREA             19L
#define M_CONVEX_PERIMETER        20L
#define M_CONVEX_HULL             80L
#define M_X_MIN_AT_Y_MIN          21L
#define M_X_MAX_AT_Y_MAX          22L
#define M_Y_MIN_AT_X_MAX          23L
#define M_Y_MAX_AT_X_MIN          24L
#define M_X_MIN_AT_Y_MAX          58L
#define M_X_MAX_AT_Y_MIN          59L
#define M_Y_MIN_AT_X_MIN          60L
#define M_Y_MAX_AT_X_MAX          61L
#define M_WORLD_X_AT_Y_MIN      1483L
#define M_WORLD_X_AT_Y_MAX      1484L
#define M_WORLD_Y_AT_X_MIN      1485L
#define M_WORLD_Y_AT_X_MAX      1486L
#define M_COMPACTNESS             25L
#define M_NUMBER_OF_HOLES         26L
#define M_FERET_ELONGATION        27L
#define M_ROUGHNESS               28L
#define M_EULER_NUMBER            47L
#define M_LENGTH          0x00002000L
#define M_BREADTH                 49L
#define M_ELONGATION              50L
#define M_INTERCEPT_0             51L
#define M_INTERCEPT_45            52L
#define M_INTERCEPT_90            53L
#define M_INTERCEPT_135           54L
#define M_NUMBER_OF_RUNS          55L
#define M_GENERAL_FERET        0x400L
#define M_TIMEOUT_END             10L //Already defined in milmod.h
#define M_MAX_BLOBS_END           57L
#define M_RETURN_PARTIAL_RESULTS  70L
#define M_STOP_CALCULATE         116L

/* Greyscale only (ie, trivial for binary) */

#define M_SUM_PIXEL               29L
#define M_MIN_PIXEL               30L
#define M_MAX_PIXEL               31L
#define M_MEAN_PIXEL              32L
#define M_SIGMA_PIXEL             33L
#define M_SUM_PIXEL_SQUARED       46L

/* Binary or greyscale (might want both for a greyscale image) */

#define M_CENTER_OF_GRAVITY_X     34L
#define M_CENTER_OF_GRAVITY_Y     35L
#define M_MOMENT_X0_Y1            36L
#define M_MOMENT_X1_Y0            37L
#define M_MOMENT_X1_Y1            38L
#define M_MOMENT_X0_Y2            39L
#define M_MOMENT_X2_Y0            40L
#define M_MOMENT_CENTRAL_X1_Y1    41L
#define M_MOMENT_CENTRAL_X0_Y2    42L
#define M_MOMENT_CENTRAL_X2_Y0    43L
#define M_AXIS_PRINCIPAL_ANGLE    44L
#define M_AXIS_SECONDARY_ANGLE    45L
#define M_GENERAL_MOMENT       0x800L

/* General moment type */

#define M_ORDINARY             0x400L
#define M_CENTRAL              0x800L

/* the defines are used by the active mil BlobAnalysis.Results.Get method YV */

#define M_RUN_LENGTHS            100L
#define M_RUN_X_COORDINATES      101L
#define M_RUN_Y_COORDINATES      102L

/* Sorting options. */
#define M_NO_SORT         0L
#define M_SORT1_UP        0x02000000L
#define M_SORT2_UP        0x04000000L
#define M_SORT3_UP        0x06000000L
#define M_SORT1_DOWN      0x0A000000L
#define M_SORT2_DOWN      0x0C000000L
#define M_SORT3_DOWN      0x0E000000L

/* Chain code specific features */
#define M_NUMBER_OF_CHAINED_PIXELS 56L
#define M_CHAIN_X                  65L
#define M_CHAIN_Y                 144L
#define M_CHAIN_INDEX              67L
#define M_CHAIN_INDEXXY_PACKED     62L

/* Convex Hull specific features */
#define M_CONVEX_HULL_X                      81L
#define M_CONVEX_HULL_Y                      82L
#define M_CONVEX_HULL_XY_PACKED              83L
#define M_NUMBER_OF_CONVEX_HULL_POINTS       84L
#define M_FULL_CONVEX_HULL_X                 85L
#define M_FULL_CONVEX_HULL_Y                 86L
#define M_FULL_CONVEX_HULL_XY_PACKED         87L
#define M_NUMBER_OF_FULL_CONVEX_HULL_POINTS  88L
#define M_CONVEX_HULL_AREA                   89L

/* Short cuts for enabling multiple features */

#define M_ALL_FEATURES         0x100L   /* All except general Feret */
#define M_ALL_FEATURES_EXCEPT_CHAINS 0x105L   /* All except chains */
#define M_BOX                  0x101L
#define M_CONTACT_POINTS       0x102L
#define M_CENTER_OF_GRAVITY    0x103L
#define M_NO_FEATURES          0x104L   /* Still do label and area */
#define M_CHAIN                209L     /* Also defined in miledge.h */
#define M_CHAINS               M_CHAIN  /* Also defined in miledge.h */
#define M_WORLD_BOX            1487L

/* MblobControl() and/or MblobInquire() values */

#define M_IMAGE_TYPE               1L
#define M_BLOB_IDENTIFICATION      2L
#define M_LATTICE                  3L
#define M_FOREGROUND_VALUE         4L
#define M_PIXEL_ASPECT_RATIO       5L
#define M_NUMBER_OF_FERETS        63L
#define M_IMAGE_START_X            7L
#define M_IMAGE_START_Y            8L
#define M_RESET                    9L
#define M_IDENTIFIER_IMAGE        10L
#define M_GREY_IMAGE              11L
#define M_IDENTIFIER_START_BIT    12L
#define M_GREY_START_BIT          13L
#define M_SAVE_RUNS               14L
#define M_IDENTIFIER_TYPE         15L
#define M_MAX_LABEL               16L
#define M_SELECT_FEATURE          17L
#define M_AUTO_RESET      0x00002000L  // Already defined in mil.h
#define M_TIMEOUT               2077L  // Already defined in mil.h
#define M_MAX_BLOBS               18L
#define M_CLEAR_RESULT            19L
#define M_INPUT_SELECT_UNITS      20L
#define M_ALLOCATION_FAILURE_REAL_TEST_FLAG            971L // Also in milstr.h
#define M_RESULT_OUTPUT_UNITS   1300L // Also in milim.h


/* Blob identification values */

#define M_WHOLE_IMAGE              1L
#define M_INDIVIDUAL               2L
#define M_LABELLED                 4L
#define M_LABELED          M_LABELLED  // One "L" is american english while
                                       // two "LL" is British (ref. Thalia tech writ.).
#define M_LABELED_TOUCHING         5L

/* Enabling value. */

#define M_ENABLE               -9997L
#define M_DISABLE              -9999L

/* Lattice values */

#define M_4_CONNECTED     0x00000010L
#define M_8_CONNECTED     0x00000020L


/* Foreground values */

#define M_NONZERO               0x80L
#define M_ZERO                 0x100L
#define M_NON_ZERO          M_NONZERO
#define M_FOREGROUND_WHITE      0x80L
#define M_FOREGROUND_BLACK     0x100L

/* Parameters for output units */
#define M_PIXEL                              0x1000L
#define M_WORLD                              0x2000L
#define M_ACCORDING_TO_CALIBRATION             1301L
#define M_ACCORDING_TO_RESULT                  1302L


/* Data types for results are no longer redefined... to avoid all the conditions and defines around MIL_INT variables (x64)*/


/* Conditional test */

#define M_ALWAYS              0L
#define M_IN_RANGE            1L
#define M_OUT_RANGE           2L
#define M_EQUAL               3L
#define M_NOT_EQUAL           4L
#define M_GREATER             5L
#define M_LESS                6L
#define M_GREATER_OR_EQUAL    7L
#define M_LESS_OR_EQUAL       8L


/* Image type values (and flags for blob_enable_feature()) */

#define M_BINARY          0x00001000L
#define M_GREYSCALE           0x0200L
#define M_GRAYSCALE       M_GREYSCALE


/* MblobReconstruct() defines */

#define M_RECONSTRUCT_FROM_SEED     1L
#define M_ERASE_BORDER_BLOBS        2L
#define M_FILL_HOLES                3L
#define M_EXTRACT_HOLES             4L
#define M_SEED_PIXELS_ALL_IN_BLOBS  1L
#define M_FOREGROUND_ZERO           2L

/* MblobDraw() defines */
#define M_DRAW_BLOBS                0x00000400L
#define M_DRAW_BLOBS_CONTOUR        0x00000100L
#define M_DRAW_HOLES                0x00002000L
#define M_DRAW_HOLES_CONTOUR        0x00004000L
#define M_DRAW_CONVEX_HULL          0x00008000L
#define M_DRAW_CONVEX_HULL_CONTOUR  0x00010000L
#define M_DRAW_BOX                  0x00000020L     // also used in m*draw
#define M_DRAW_WORLD_BOX            0x00020000L
#define M_DRAW_POSITION             0x00000040L     // also used in m*draw
#define M_DRAW_CENTER_OF_GRAVITY    0x00000080L
#define M_DRAW_FERET_MIN            0x00001000L
#define M_DRAW_FERET_MAX            0x00000008L
#define M_DRAW_FERET_BOX            0x00000010L
#define M_DRAW_AXIS                 0x00000200L     // also used in mmoddraw
#define M_DRAW_WORLD_BOX_CENTER     0x00040000L
#define M_DRAW_BOX_CENTER           0x00000001L     // also defined in milmeas.h
#define M_DRAW_WORLD_FERET_X        0x00080000L
#define M_DRAW_WORLD_FERET_Y        0x00100000L
#define M_DRAW_RELATIVE_ORIGIN_X           319L     // Already defined in miledge.h
#define M_DRAW_RELATIVE_ORIGIN_Y           320L     // Already defined in miledge.h
#define M_DRAW_SCALE_X                    3203L       // Already defined in    milmod.h
#define M_DRAW_SCALE_Y                    3204L       // Already defined in    milmod.h

/* MblobMerge() control flags */
#define M_TOP_BOTTOM                0x00001000L
#define M_LEFT_RIGHT                0x00002000L
#define M_MOVE                      0x00010000L     // also in milcal.h
#define M_COPY                      0x00020000L     // already defined in mil.h
#define M_CLONE                     0x00040000L

/* Miscellaneous */
#define M_ALL_BLOBS           0x40000000L
#define M_INCLUDED_BLOBS      0x08000000L
#define M_EXCLUDED_BLOBS      0x20000000L
#define M_ALL_BLOBS_OLD       0x105L
#define M_INCLUDED_BLOBS_OLD  0x106L
#define M_EXCLUDED_BLOBS_OLD  0x107L

#define M_INCLUDE                  1L
#define M_EXCLUDE                  2L
#define M_DELETE                   3L
#define M_MERGE           0x00000040L
#define M_MIN_FERETS               2L
#define M_MAX_FERETS              64L
#define M_INCLUDE_ONLY         0x101L
#define M_EXCLUDE_ONLY         0x102L
#define M_CONTOUR              0x800L
#define M_DUMMY               0x4000L
#define M_CLEAR           0x00000001L
#define M_NO_CLEAR        0x00000002L

/* Other defines are in MIL.H */

/********************************************************************
 * Function prototypes
 ********************************************************************/
#ifndef __midl

MFTYPE32 MIL_ID MFTYPE MblobAllocFeatureList(MIL_ID SystemId,MIL_ID MPTYPE *FeatureListPtr);
MFTYPE32 MIL_ID MFTYPE MblobAllocResult(MIL_ID SystemId, MIL_ID MPTYPE *BlobResIdPtr);
MFTYPE32 void MFTYPE MblobCalculate(MIL_ID BlobIdentImageId, MIL_ID GreyImageId,
              MIL_ID FeatureListId, MIL_ID BlobResId);

#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs
MFTYPE32 void MFTYPE MblobControlInt64(MIL_ID BlobResId, MIL_INT ProcMode, MIL_INT64 Value);
MFTYPE32 void MFTYPE MblobControlDouble(MIL_ID BlobResId, MIL_INT ProcMode, double Value);
#else
// Prototypes for 32 bits OSs
#define MblobControlInt64  MblobControl
#define MblobControlDouble MblobControl
MFTYPE32 void MFTYPE MblobControl(MIL_ID BlobResId, MIL_INT ProcMode, double Value);
#endif

MFTYPE32 void MFTYPE MblobFill(MIL_ID BlobResId, MIL_ID TargetImageId, MIL_INT Mode,
              MIL_INT Value);
MFTYPE32 void MFTYPE MblobFree(MIL_ID BlobResId);
MFTYPE32 MIL_INT MFTYPE MblobGetLabel(MIL_ID BlobResId, MIL_INT XPos, MIL_INT YPos,
              MIL_INT MPTYPE *BlobLabelPtr);
MFTYPE32 MIL_INT MFTYPE MblobGetNumber(MIL_ID BlobResId, MIL_INT MPTYPE *CountPtr);
MFTYPE32 void MFTYPE MblobGetResult(MIL_ID BlobResId, MIL_INT Feature,
              void MPTYPE *TargetArrayPtr);
MFTYPE32 void MFTYPE MblobGetResultSingle(MIL_ID BlobResId, MIL_INT BlobLabel,
              MIL_INT Feature, void MPTYPE *ValuePtr);
MFTYPE32 void MFTYPE MblobGetRuns(MIL_ID BlobResId, MIL_INT BlobLabel, MIL_INT ArrayType,
              void MPTYPE *RunXPtr, void MPTYPE *RunYPtr, void MPTYPE *RunLengthPtr);
MFTYPE32 MIL_INT MFTYPE MblobInquire(MIL_ID BlobResId, MIL_INT ProcMode, void MPTYPE *ValuePtr);
MFTYPE32 void MFTYPE MblobLabel(MIL_ID BlobResId, MIL_ID TargetImageId, MIL_INT Mode);
MFTYPE32 void MFTYPE MblobSelect(MIL_ID BlobResId, MIL_INT Operation, MIL_INT Feature,
              MIL_INT Condition, double CondLow, double CondHigh);
MFTYPE32 void MFTYPE MblobSelectFeature(MIL_ID FeatureListId, MIL_INT Feature);
MFTYPE32 void MFTYPE MblobSelectFeret(MIL_ID FeatureListId, double Angle);
MFTYPE32 void MFTYPE MblobSelectMoment(MIL_ID FeatureListId, MIL_INT MomType,
              MIL_INT XMomOrder, MIL_INT YMomOrder);
MFTYPE32 void MFTYPE MblobReconstruct(MIL_ID srce_image_id, MIL_ID seed_image_id,
              MIL_ID dest_image_id, MIL_INT operation, MIL_INT mode);
MFTYPE32 void MFTYPE MblobMerge(MIL_ID SourceResult1, MIL_ID SourceResult2,
              MIL_ID DestResult, MIL_INT ControlFlag);

MFTYPE32 void MFTYPE MblobDraw(MIL_ID GraphContId, MIL_ID ResultId, MIL_ID DestImageId,
                     MIL_INT Operation, MIL_INT Label, MIL_INT ControlFlag);

#endif


/* C++ directive if needed */
#ifdef __cplusplus
}
#endif

#if M_MIL_USE_64BIT
#ifdef __cplusplus

#if !M_MIL_USE_LINUX
//////////////////////////////////////////////////////////////
// MblobControl function definition when compiling c++ files
//////////////////////////////////////////////////////////////
inline void MblobControl(MIL_ID BlobResId, MIL_INT ProcMode, int Value)
   {
   MblobControlInt64(BlobResId, ProcMode, Value);
   };
#endif

inline void MblobControl(MIL_ID BlobResId, MIL_INT ProcMode, MIL_INT32 Value)
   {
   MblobControlInt64(BlobResId, ProcMode, Value);
   };

inline void MblobControl(MIL_ID BlobResId, MIL_INT ProcMode, MIL_INT64 Value)
   {
   MblobControlInt64(BlobResId, ProcMode, Value);
   };

inline void MblobControl(MIL_ID BlobResId, MIL_INT ProcMode, double Value)
   {
   MblobControlDouble(BlobResId, ProcMode, Value);
   };
#else
//////////////////////////////////////////////////////////////
// For C file, call the default function, i.e. Int64 one
//////////////////////////////////////////////////////////////
#define MblobControl  MblobControlDouble

#endif // __cplusplus
#endif // M_MIL_USE_64BIT

#if M_MIL_USE_SAFE_TYPE

//////////////////////////////////////////////////////////////
// See milos.h for explanation about these functions.
//////////////////////////////////////////////////////////////

// ----------------------------------------------------------
// MblobGetResult

inline MFTYPE32 void MFTYPE MblobGetResultUnsafe  (MIL_ID BlobResId, MIL_INT Feature, void       MPTYPE *TargetArrayPtr);
inline MFTYPE32 void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT Feature, int                TargetArrayPtr);
inline MFTYPE32 void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT Feature, MIL_INT8   MPTYPE *TargetArrayPtr);
inline MFTYPE32 void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT Feature, MIL_INT16  MPTYPE *TargetArrayPtr);
inline MFTYPE32 void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT Feature, MIL_INT32  MPTYPE *TargetArrayPtr);
inline MFTYPE32 void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT Feature, MIL_INT64  MPTYPE *TargetArrayPtr);
inline MFTYPE32 void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT Feature, float      MPTYPE *TargetArrayPtr);
inline MFTYPE32 void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT Feature, MIL_DOUBLE MPTYPE *TargetArrayPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT Feature, MIL_UINT8  MPTYPE *TargetArrayPtr);
inline MFTYPE32 void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT Feature, MIL_UINT16 MPTYPE *TargetArrayPtr);
inline MFTYPE32 void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT Feature, MIL_UINT32 MPTYPE *TargetArrayPtr);
inline MFTYPE32 void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT Feature, MIL_UINT64 MPTYPE *TargetArrayPtr);
#endif

// ----------------------------------------------------------
// MblobGetResultSingle

inline MFTYPE32 void MFTYPE MblobGetResultSingleUnsafe  (MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, void       MPTYPE *TargetArrayPtr);
inline MFTYPE32 void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, int                TargetArrayPtr);
inline MFTYPE32 void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, MIL_INT8   MPTYPE *TargetArrayPtr);
inline MFTYPE32 void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, MIL_INT16  MPTYPE *TargetArrayPtr);
inline MFTYPE32 void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, MIL_INT32  MPTYPE *TargetArrayPtr);
inline MFTYPE32 void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, MIL_INT64  MPTYPE *TargetArrayPtr);
inline MFTYPE32 void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, float      MPTYPE *TargetArrayPtr);
inline MFTYPE32 void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, MIL_DOUBLE MPTYPE *TargetArrayPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, MIL_UINT8  MPTYPE *TargetArrayPtr);
inline MFTYPE32 void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, MIL_UINT16 MPTYPE *TargetArrayPtr);
inline MFTYPE32 void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, MIL_UINT32 MPTYPE *TargetArrayPtr);
inline MFTYPE32 void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, MIL_UINT64 MPTYPE *TargetArrayPtr);
#endif

// ----------------------------------------------------------
// MblobGetRuns

inline MFTYPE32 void MFTYPE MblobGetRunsUnsafe  (MIL_ID BlobResId, MIL_INT BlobLabel, MIL_INT ArrayType, void       MPTYPE *RunXPtr, void       MPTYPE *RunYPtr, void       MPTYPE *RunLengthPtr);
inline MFTYPE32 void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT BlobLabel, MIL_INT ArrayType, int               RunXPtr, int               RunYPtr, int               RunLengthPtr);
inline MFTYPE32 void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT BlobLabel, MIL_INT ArrayType, MIL_INT8   MPTYPE *RunXPtr, MIL_INT8   MPTYPE *RunYPtr, MIL_INT8   MPTYPE *RunLengthPtr);
inline MFTYPE32 void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT BlobLabel, MIL_INT ArrayType, MIL_INT16  MPTYPE *RunXPtr, MIL_INT16  MPTYPE *RunYPtr, MIL_INT16  MPTYPE *RunLengthPtr);
inline MFTYPE32 void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT BlobLabel, MIL_INT ArrayType, MIL_INT32  MPTYPE *RunXPtr, MIL_INT32  MPTYPE *RunYPtr, MIL_INT32  MPTYPE *RunLengthPtr);
inline MFTYPE32 void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT BlobLabel, MIL_INT ArrayType, MIL_INT64  MPTYPE *RunXPtr, MIL_INT64  MPTYPE *RunYPtr, MIL_INT64  MPTYPE *RunLengthPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT BlobLabel, MIL_INT ArrayType, MIL_UINT8  MPTYPE *RunXPtr, MIL_UINT8  MPTYPE *RunYPtr, MIL_UINT8  MPTYPE *RunLengthPtr);
inline MFTYPE32 void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT BlobLabel, MIL_INT ArrayType, MIL_UINT16 MPTYPE *RunXPtr, MIL_UINT16 MPTYPE *RunYPtr, MIL_UINT16 MPTYPE *RunLengthPtr);
inline MFTYPE32 void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT BlobLabel, MIL_INT ArrayType, MIL_UINT32 MPTYPE *RunXPtr, MIL_UINT32 MPTYPE *RunYPtr, MIL_UINT32 MPTYPE *RunLengthPtr);
inline MFTYPE32 void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT BlobLabel, MIL_INT ArrayType, MIL_UINT64 MPTYPE *RunXPtr, MIL_UINT64 MPTYPE *RunYPtr, MIL_UINT64 MPTYPE *RunLengthPtr);
#endif

// ----------------------------------------------------------
// MblobInquire

inline MFTYPE32 MIL_INT MFTYPE MblobInquireUnsafe  (MIL_ID BlobResId, MIL_INT ProcMode, void       MPTYPE *ValuePtr);
inline MFTYPE32 MIL_INT MFTYPE MblobInquireSafeType(MIL_ID BlobResId, MIL_INT ProcMode, int                ValuePtr);
inline MFTYPE32 MIL_INT MFTYPE MblobInquireSafeType(MIL_ID BlobResId, MIL_INT ProcMode, MIL_INT32  MPTYPE *ValuePtr);
inline MFTYPE32 MIL_INT MFTYPE MblobInquireSafeType(MIL_ID BlobResId, MIL_INT ProcMode, MIL_INT64  MPTYPE *ValuePtr);
inline MFTYPE32 MIL_INT MFTYPE MblobInquireSafeType(MIL_ID BlobResId, MIL_INT ProcMode, MIL_DOUBLE MPTYPE *ValuePtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 MIL_INT MFTYPE MblobInquireSafeType(MIL_ID BlobResId, MIL_INT ProcMode, MIL_UINT32 MPTYPE *ValuePtr);
inline MFTYPE32 MIL_INT MFTYPE MblobInquireSafeType(MIL_ID BlobResId, MIL_INT ProcMode, MIL_UINT64 MPTYPE *ValuePtr);
#endif

// -------------------------------------------------------------------------
// MblobGetResult

inline MFTYPE32 void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT Feature, int TargetArrayPtr)
   {
   if (TargetArrayPtr)
      SafeTypeError(MT("MblobGetResult"));

   MblobGetResult(BlobResId, Feature, NULL);
   }

inline void MblobGetResultSafeTypeExecute(MIL_ID BlobResId, MIL_INT Feature, void MPTYPE *TargetArrayPtr, MIL_INT GivenType)
   {
   MIL_INT RequiredType = (Feature & (M_TYPE_CHAR | M_TYPE_DOUBLE | M_TYPE_FLOAT | M_TYPE_SHORT | M_TYPE_MIL_INT32 | M_TYPE_MIL_INT64));
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;

   if (RequiredType != GivenType)
      SafeTypeError(MT("MblobGetResult"));

   MblobGetResult(BlobResId, Feature, TargetArrayPtr);
   }


inline MFTYPE32 void MFTYPE MblobGetResultUnsafe  (MIL_ID BlobResId, MIL_INT Feature, void       MPTYPE *TargetArrayPtr) {MblobGetResult               (BlobResId, Feature, TargetArrayPtr                  );}
inline MFTYPE32 void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT Feature, MIL_INT8   MPTYPE *TargetArrayPtr) {MblobGetResultSafeTypeExecute(BlobResId, Feature, TargetArrayPtr, M_TYPE_CHAR     );}
inline MFTYPE32 void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT Feature, MIL_INT16  MPTYPE *TargetArrayPtr) {MblobGetResultSafeTypeExecute(BlobResId, Feature, TargetArrayPtr, M_TYPE_SHORT    );}
inline MFTYPE32 void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT Feature, MIL_INT32  MPTYPE *TargetArrayPtr) {MblobGetResultSafeTypeExecute(BlobResId, Feature, TargetArrayPtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT Feature, MIL_INT64  MPTYPE *TargetArrayPtr) {MblobGetResultSafeTypeExecute(BlobResId, Feature, TargetArrayPtr, M_TYPE_MIL_INT64);}
inline MFTYPE32 void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT Feature, float      MPTYPE *TargetArrayPtr) {MblobGetResultSafeTypeExecute(BlobResId, Feature, TargetArrayPtr, M_TYPE_FLOAT    );}
inline MFTYPE32 void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT Feature, MIL_DOUBLE MPTYPE *TargetArrayPtr) {MblobGetResultSafeTypeExecute(BlobResId, Feature, TargetArrayPtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT Feature, MIL_UINT8  MPTYPE *TargetArrayPtr) {MblobGetResultSafeTypeExecute(BlobResId, Feature, TargetArrayPtr, M_TYPE_CHAR     );}
inline MFTYPE32 void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT Feature, MIL_UINT16 MPTYPE *TargetArrayPtr) {MblobGetResultSafeTypeExecute(BlobResId, Feature, TargetArrayPtr, M_TYPE_SHORT    );}
inline MFTYPE32 void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT Feature, MIL_UINT32 MPTYPE *TargetArrayPtr) {MblobGetResultSafeTypeExecute(BlobResId, Feature, TargetArrayPtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT Feature, MIL_UINT64 MPTYPE *TargetArrayPtr) {MblobGetResultSafeTypeExecute(BlobResId, Feature, TargetArrayPtr, M_TYPE_MIL_INT64);}
#endif

// -------------------------------------------------------------------------
// MblobGetResultSingle

inline MFTYPE32 void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, int TargetArrayPtr)
   {
   if (TargetArrayPtr)
      SafeTypeError(MT("MblobGetResultSingle"));

   MblobGetResultSingle(BlobResId, LabelVal, Feature, NULL);
   }

inline void MblobGetResultSingleSafeTypeExecute(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, void MPTYPE *TargetArrayPtr, MIL_INT GivenType)
   {
   MIL_INT RequiredType = (Feature & (M_TYPE_CHAR | M_TYPE_DOUBLE | M_TYPE_FLOAT | M_TYPE_SHORT | M_TYPE_MIL_INT32 | M_TYPE_MIL_INT64));
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;

   if (RequiredType != GivenType)
      SafeTypeError(MT("MblobGetResultSingle"));

   MblobGetResultSingle(BlobResId, LabelVal, Feature, TargetArrayPtr);
   }

inline MFTYPE32 void MFTYPE MblobGetResultSingleUnsafe  (MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, void       MPTYPE *TargetArrayPtr) {MblobGetResultSingle               (BlobResId, LabelVal, Feature, TargetArrayPtr                  );}
inline MFTYPE32 void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, MIL_INT8   MPTYPE *TargetArrayPtr) {MblobGetResultSingleSafeTypeExecute(BlobResId, LabelVal, Feature, TargetArrayPtr, M_TYPE_CHAR     );}
inline MFTYPE32 void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, MIL_INT16  MPTYPE *TargetArrayPtr) {MblobGetResultSingleSafeTypeExecute(BlobResId, LabelVal, Feature, TargetArrayPtr, M_TYPE_SHORT    );}
inline MFTYPE32 void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, MIL_INT32  MPTYPE *TargetArrayPtr) {MblobGetResultSingleSafeTypeExecute(BlobResId, LabelVal, Feature, TargetArrayPtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, MIL_INT64  MPTYPE *TargetArrayPtr) {MblobGetResultSingleSafeTypeExecute(BlobResId, LabelVal, Feature, TargetArrayPtr, M_TYPE_MIL_INT64);}
inline MFTYPE32 void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, float      MPTYPE *TargetArrayPtr) {MblobGetResultSingleSafeTypeExecute(BlobResId, LabelVal, Feature, TargetArrayPtr, M_TYPE_FLOAT    );}
inline MFTYPE32 void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, MIL_DOUBLE MPTYPE *TargetArrayPtr) {MblobGetResultSingleSafeTypeExecute(BlobResId, LabelVal, Feature, TargetArrayPtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, MIL_UINT8  MPTYPE *TargetArrayPtr) {MblobGetResultSingleSafeTypeExecute(BlobResId, LabelVal, Feature, TargetArrayPtr, M_TYPE_CHAR     );}
inline MFTYPE32 void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, MIL_UINT16 MPTYPE *TargetArrayPtr) {MblobGetResultSingleSafeTypeExecute(BlobResId, LabelVal, Feature, TargetArrayPtr, M_TYPE_SHORT    );}
inline MFTYPE32 void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, MIL_UINT32 MPTYPE *TargetArrayPtr) {MblobGetResultSingleSafeTypeExecute(BlobResId, LabelVal, Feature, TargetArrayPtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT Feature, MIL_UINT64 MPTYPE *TargetArrayPtr) {MblobGetResultSingleSafeTypeExecute(BlobResId, LabelVal, Feature, TargetArrayPtr, M_TYPE_MIL_INT64);}
#endif

// ----------------------------------------------------------
// MblobGetRuns

inline MFTYPE32 void MFTYPE MblobGetRunsSafeTypeExecute(MIL_ID BlobResId, MIL_INT BlobLabel, MIL_INT ArrayType, void  MPTYPE *RunXPtr, void  MPTYPE *RunYPtr, void  MPTYPE *RunLengthPtr, MIL_INT ExpectedType)
   {
   MIL_INT RequiredType = (ArrayType  & (M_TYPE_MIL_INT32 | M_TYPE_MIL_INT64 | M_TYPE_CHAR | M_TYPE_SHORT | M_TYPE_FLOAT | M_TYPE_DOUBLE));
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;

   if (RequiredType != ExpectedType)
      SafeTypeError(MT("MblobGetRuns"));

   MblobGetRuns(BlobResId, BlobLabel, ArrayType, RunXPtr, RunYPtr, RunLengthPtr);
   }
inline MFTYPE32 void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT BlobLabel, MIL_INT ArrayType, int RunXPtr, int RunYPtr, int RunLengthPtr)
   {
   if (RunXPtr || RunYPtr || RunLengthPtr)
      SafeTypeError(MT("MblobGetRuns"));

   MblobGetRuns(BlobResId, BlobLabel, ArrayType, NULL, NULL, NULL);
   }

inline MFTYPE32 void MFTYPE MblobGetRunsUnsafe  (MIL_ID BlobResId, MIL_INT BlobLabel, MIL_INT ArrayType, void       MPTYPE *RunXPtr, void       MPTYPE *RunYPtr, void       MPTYPE *RunLengthPtr) {MblobGetRuns               (BlobResId, BlobLabel, ArrayType, RunXPtr, RunYPtr, RunLengthPtr                  );}
inline MFTYPE32 void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT BlobLabel, MIL_INT ArrayType, MIL_INT8   MPTYPE *RunXPtr, MIL_INT8   MPTYPE *RunYPtr, MIL_INT8   MPTYPE *RunLengthPtr) {MblobGetRunsSafeTypeExecute(BlobResId, BlobLabel, ArrayType, RunXPtr, RunYPtr, RunLengthPtr, M_TYPE_CHAR     );}
inline MFTYPE32 void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT BlobLabel, MIL_INT ArrayType, MIL_INT16  MPTYPE *RunXPtr, MIL_INT16  MPTYPE *RunYPtr, MIL_INT16  MPTYPE *RunLengthPtr) {MblobGetRunsSafeTypeExecute(BlobResId, BlobLabel, ArrayType, RunXPtr, RunYPtr, RunLengthPtr, M_TYPE_SHORT    );}
inline MFTYPE32 void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT BlobLabel, MIL_INT ArrayType, MIL_INT32  MPTYPE *RunXPtr, MIL_INT32  MPTYPE *RunYPtr, MIL_INT32  MPTYPE *RunLengthPtr) {MblobGetRunsSafeTypeExecute(BlobResId, BlobLabel, ArrayType, RunXPtr, RunYPtr, RunLengthPtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT BlobLabel, MIL_INT ArrayType, MIL_INT64  MPTYPE *RunXPtr, MIL_INT64  MPTYPE *RunYPtr, MIL_INT64  MPTYPE *RunLengthPtr) {MblobGetRunsSafeTypeExecute(BlobResId, BlobLabel, ArrayType, RunXPtr, RunYPtr, RunLengthPtr, M_TYPE_MIL_INT64);}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT BlobLabel, MIL_INT ArrayType, MIL_UINT8  MPTYPE *RunXPtr, MIL_UINT8  MPTYPE *RunYPtr, MIL_UINT8  MPTYPE *RunLengthPtr) {MblobGetRunsSafeTypeExecute(BlobResId, BlobLabel, ArrayType, RunXPtr, RunYPtr, RunLengthPtr, M_TYPE_CHAR     );}
inline MFTYPE32 void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT BlobLabel, MIL_INT ArrayType, MIL_UINT16 MPTYPE *RunXPtr, MIL_UINT16 MPTYPE *RunYPtr, MIL_UINT16 MPTYPE *RunLengthPtr) {MblobGetRunsSafeTypeExecute(BlobResId, BlobLabel, ArrayType, RunXPtr, RunYPtr, RunLengthPtr, M_TYPE_SHORT    );}
inline MFTYPE32 void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT BlobLabel, MIL_INT ArrayType, MIL_UINT32 MPTYPE *RunXPtr, MIL_UINT32 MPTYPE *RunYPtr, MIL_UINT32 MPTYPE *RunLengthPtr) {MblobGetRunsSafeTypeExecute(BlobResId, BlobLabel, ArrayType, RunXPtr, RunYPtr, RunLengthPtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT BlobLabel, MIL_INT ArrayType, MIL_UINT64 MPTYPE *RunXPtr, MIL_UINT64 MPTYPE *RunYPtr, MIL_UINT64 MPTYPE *RunLengthPtr) {MblobGetRunsSafeTypeExecute(BlobResId, BlobLabel, ArrayType, RunXPtr, RunYPtr, RunLengthPtr, M_TYPE_MIL_INT64);}
#endif

// ----------------------------------------------------------
// MblobInquire

inline MFTYPE32 MIL_INT MFTYPE MblobInquireSafeType (MIL_ID BlobResId, MIL_INT ProcMode, int ValuePtr)
   {
   if (ValuePtr)
      SafeTypeError(MT("MblobInquire"));

   return MblobInquire(BlobResId, ProcMode, NULL);
   }


inline MFTYPE32 MIL_INT MFTYPE MblobInquireSafeTypeExecute (MIL_ID BlobResId, MIL_INT ProcMode, void       MPTYPE *ValuePtr, MIL_INT GivenType)
   {
   MIL_INT RequiredType = (ProcMode & (M_TYPE_MIL_INT32 | M_TYPE_MIL_INT64 | M_TYPE_DOUBLE | M_TYPE_MIL_ID));
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MT("MblobInquire"));

   return MblobInquire(BlobResId, ProcMode, ValuePtr);
   }

inline MFTYPE32 MIL_INT MFTYPE MblobInquireUnsafe  (MIL_ID BlobResId, MIL_INT ProcMode, void       MPTYPE *ValuePtr) {return MblobInquire       (BlobResId, ProcMode, ValuePtr                  );}
inline MFTYPE32 MIL_INT MFTYPE MblobInquireSafeType(MIL_ID BlobResId, MIL_INT ProcMode, MIL_INT32  MPTYPE *ValuePtr) {return MblobInquireSafeTypeExecute(BlobResId, ProcMode, ValuePtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 MIL_INT MFTYPE MblobInquireSafeType(MIL_ID BlobResId, MIL_INT ProcMode, MIL_INT64  MPTYPE *ValuePtr) {return MblobInquireSafeTypeExecute(BlobResId, ProcMode, ValuePtr, M_TYPE_MIL_INT64);}
inline MFTYPE32 MIL_INT MFTYPE MblobInquireSafeType(MIL_ID BlobResId, MIL_INT ProcMode, MIL_DOUBLE MPTYPE *ValuePtr) {return MblobInquireSafeTypeExecute(BlobResId, ProcMode, ValuePtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 MIL_INT MFTYPE MblobInquireSafeType(MIL_ID BlobResId, MIL_INT ProcMode, MIL_UINT32 MPTYPE *ValuePtr) {return MblobInquireSafeTypeExecute(BlobResId, ProcMode, ValuePtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 MIL_INT MFTYPE MblobInquireSafeType(MIL_ID BlobResId, MIL_INT ProcMode, MIL_UINT64 MPTYPE *ValuePtr) {return MblobInquireSafeTypeExecute(BlobResId, ProcMode, ValuePtr, M_TYPE_MIL_INT64);}
#endif

#define MblobGetResult       MblobGetResultSafeType
#define MblobGetResultSingle MblobGetResultSingleSafeType
#define MblobGetRuns         MblobGetRunsSafeType
#define MblobInquire         MblobInquireSafeType

#else // #if M_MIL_USE_SAFE_TYPE

#define MblobGetResultUnsafe       MblobGetResult
#define MblobGetResultSingleUnsafe MblobGetResultSingle
#define MblobGetRunsUnsafe         MblobGetRuns
#define MblobInquireUnsafe         MblobInquire

#endif // #if M_MIL_USE_SAFE_TYPE

#endif // !M_MIL_LITE

#endif /* __MILBLOB_H__ */
