/***************************************************************************/
/*

    Filename:  MILPAT.H
    Owner   :  Matrox Imaging dept.
    Revision:  9.02.0580
    Content :  This file contains the defines for the MIL pattern
               recognition module. (Mpat...).

    Copyright © Matrox Electronic Systems Ltd., 1992-2010.
    All Rights Reserved

*/
/***************************************************************************/

#ifndef __MILPAT_H__
#define __MILPAT_H__

#define BW_COMPATIBILITY 0x092

#if (!M_MIL_LITE) // MIL FULL ONLY

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif

/* Bit encoded model types */
#define M_NORMALIZED             0x00000002L
#define M_NO_PYRAMIDS            0x00000008L
#define M_6_BITS_MODEL           0x00000010L
#define M_ORIENTATION            0x00002400L
#define M_NOISY                  0x00000800L
#define M_CIRCULAR_OVERSCAN      0x00010000L


/* Levels of speed and/or accuracy */
#define M_VERY_LOW                        0L
#define M_LOW                             1L
#define M_MEDIUM                          2L
#define M_HIGH                            3L
#define M_VERY_HIGH                       4L
#define M_FULL_SEARCH                  0x80L


/* Bit encoded flags for MpatPreprocModel() */
#define M_DELETE_LOW                   0x10L
#define M_DELETE_MEDIUM                0x20L
#define M_DELETE_HIGH                  0x40L

#define M_ALL                    0x40000000L // Already defined in Mil.h
#define M_ALL_OLD                         0L
#define M_DISABLE                     -9999L
#define M_UNKNOWN                     -9999L
#define M_NO_CHANGE                   -9998L
#define M_ABSOLUTE                        1L
#define M_OFFSET                          2L
#define M_GAUSSNOISE                      4L

/* 'type' parameter of MpatAlloc() */
#define M_REGULAR                0x00020000L
#define M_FAST                   0x00002000L
#define M_NO_ROTATION            0x00080000L
#define M_BEST                   0x00100000L
#define M_PAT_DEBUG              0x00200000L
#define M_MULTIPLE               0x00400000L

/* 'Flag' parameter of MpatCopy() */
#define M_CLEAR_BACKGROUND           0x2000L
#define M_INTERNAL_BUF               0x4000L


/* Used by MpatDraw() */
#define M_DRAW_IMAGE             0x00000002L   // Already define in MilMod.h
#define M_DRAW_DONT_CARES        0x00000008L   // Already define in MilMod.h
#define M_DRAW_BOX               0x00000020L   // Already define in MilMod.h
#define M_DRAW_POSITION          0x00000040L   // Already define in MilMod.h
#define M_ORIGINAL               0x00000199L   // Already define in MilMod.h


/* Used by MpatGetResult() */
#define M_FOUND_FLAG                      1L
#define M_SCORE                  0x00001400L
#define M_POSITION_X             0x00003400L
#define M_POSITION_Y             0x00004400L
#define M_MOD_INDEX              0x00005400L
#define M_SCALE                  0x00008010L   // Already define in MilMod.h
#define M_NOISE_CORRECTED_SCORE  0x00008400L
#define M_FIRST_LEVEL_USED       0x00009400L
#define M_LAST_LEVEL_USED        0x00009500L
#define M_SUM_I                  0x00010500L
#define M_SUM_II                 0x00010700L
#define M_SUM_IM                 0x00010900L
#define M_SUM_M                  0x00011100L
#define M_SUM_MM                 0x00011300L
#define M_NUMBER_OF_PIXELS       0x00011500L


/* Used by MpatInquire() */
#define M_ALLOC_TYPE                    322L
#define M_ALLOC_SIZE_X                    2L
#define M_ALLOC_SIZE_Y                    3L
#define M_CENTER_X                        4L
#define M_CENTER_Y                        5L
#define M_ORIGINAL_X                      6L
#define M_ORIGINAL_Y                      7L
#define M_SPEED_FACTOR                    M_SPEED
#define M_SPEED                           8L
#define M_POSITION_START_X                9L
#define M_POSITION_START_Y               10L
#define M_POSITION_UNCERTAINTY_X         11L
#define M_POSITION_UNCERTAINTY_Y         12L
#define M_POSITION_ACCURACY              13L
#define M_PREPROCESSED                   14L
#define M_ALLOC_OFFSET_X                 15L
#define M_ALLOC_OFFSET_Y                 16L
#define M_ACCEPTANCE_THRESHOLD           17L 
#define M_NUMBER_OF_OCCURENCES           18L
#define M_NUMBER_OF_OCCURRENCES          M_NUMBER_OF_OCCURENCES
#define M_NUMBER_OF_ENTRIES              24L
#define M_CERTAINTY_THRESHOLD            25L
#define M_ALLOC_SIZE_BIT                 26L
#define M_INTERNAL_SIZE_X                27L
#define M_INTERNAL_SIZE_Y                28L
#define M_MODEL_MAX_LEVEL              1419L  

/* Search parameters */
#define M_FIRST_LEVEL                    31L
#define M_LAST_LEVEL                     32L
#define M_MODEL_STEP                     33L
#define M_FAST_FIND                      34L
#define M_MIN_SPACING_X                  35L
#define M_MIN_SPACING_Y                  36L
#define M_SCORE_TYPE                     37L
#define M_TARGET_CACHING                 39L
#define M_REJECTION_THRESHOLD            41L
#define M_HIGHEST_REJECTED_SCORE         42L
#define M_HIGHEST_REJECTED_LEVEL         43L
#define M_HIGHEST_REJECTED_POS_X         44L
#define M_HIGHEST_REJECTED_POS_Y         45L
#define M_EXTRA_PEAKS                    46L
#define M_EXTRA_CANDIDATES               M_EXTRA_PEAKS
#define M_MODEL_NOISE_VAR                47L
#define M_TARGET_NOISE_VAR               48L
#define M_PROC_FIRST_LEVEL               50L
#define M_PROC_LAST_LEVEL                51L
#define M_AUTO_RESET             0x00002000L // Already defined in mil.h
#define M_SAVE_SUMS                      55L
#define M_PAT_OVERSCAN                   56L
#define M_RESULT_OUTPUT_UNITS          1300L // Also in milim.h
#define M_AUTO_CONTENT_BASED           1403L
#define M_FIRST_LEVEL_SCORE_THRESHOLD  1407L
#define M_AUTO_SIZE_BASED              1409L
#define M_MAX_INITIAL_PEAKS            1418L

/* Evaluate more candidates mode */
#define M_PAT_TARGET_FILTER              49L
#define M_PAT_FILTER_NONE                 1L
#define M_PAT_FILTER_2X2                  2L
#define M_PAT_FILTER_3X3                  3L
#define M_PAT_EVAL_MORE_CANDIDATES       52L
#define M_MAX_CANDIDATES_NUMBER          53L
#define M_CONTIGUOUS_MODELS              54L

/* Search parameter values */
#define M_ENABLE                      -9997L // Already defined in mil.h
#define M_DISABLE                     -9999L // Already defined in mil.h

/* MpatControl */
#define M_NUMBER                       1009L // Already defined in mil.h
#define M_MIN_SEPARATION_X             M_MIN_SPACING_X  // already defined in MilPat.h 
#define M_MIN_SEPARATION_Y             M_MIN_SPACING_Y  // already defined in MilPat.h 
#define M_REFERENCE_X                  100L // also defined in milmod.h
#define M_REFERENCE_Y                  101L // also defined in milmod.h
#define M_SEARCH_OFFSET_X              102L // also defined in milmod.h
#define M_SEARCH_OFFSET_Y              103L // also defined in milmod.h
#define M_SEARCH_SIZE_X                104L // also defined in milmod.h
#define M_SEARCH_SIZE_Y                105L // also defined in milmod.h
#define M_ACCURACY                     106L // also defined in milmod.h
#define M_ACCEPTANCE                   200L // Already defined milmod.h
#define M_CERTAINTY                    202L // Already defined milmod.h 
#define M_SEARCH_ANGLE_RANGE           113L // Already defined milmod.h
#define M_ANGLE                        0x00000800L // already defined in Mil.h

/* MpatFindMultiple */
#define M_FIND_ALL_MODELS                M_DEFAULT
#define M_FIND_BEST_MODELS                1L
#define M_FIND_ALL_MODEL                  2L


/* Parameters for find orientation */
#define M_RESULT_RANGE_180       0x00000001L
#define M_RESULT_RANGE_90        0x00000002L
#define M_RESULT_RANGE_360       0x00000004L
#define M_RESULT_RANGE_45        0x00000008L
#define M_ORIENTATION_ACCEPTANCE       200.0


/* Search parameters for search with rotation */
#define M_SEARCH_ANGLE_MODE                   0x00000080L
#define M_SEARCH_ANGLE                        0x00000100L
#define M_SEARCH_ANGLE_DELTA_NEG              0x00000200L
#define M_SEARCH_ANGLE_DELTA_POS              0x00000400L
#define M_SEARCH_ANGLE_TOLERANCE              0x00000800L
#define M_SEARCH_ANGLE_ACCURACY               0x00001000L
#define M_SEARCH_ANGLE_FINE_REGION            0x00002000L
#define M_SEARCH_ANGLE_DEBUG                  0x00004000L
#define M_SEARCH_ANGLE_INTERPOLATION_MODE     0x00008000L
#define M_SEARCH_ANGLE_DIRTY                  0x00010000L
#define M_ROTATED_MODEL_MINIMUM_SCORE         1395L
#define M_SEARCH_ANGLE_TOLERANCE_USED         1396L         // read-only
#define M_CORRELATION_SCORE_FACTOR            1398L
#define M_SEARCH_ANGLE_ACCURACY_USED          1399L


#define M_DEF_SEARCH_ANGLE_MODE                 M_DISABLE
#define M_DEF_SEARCH_ANGLE                      0.0
#define M_DEF_SEARCH_ANGLE_DELTA_NEG            0.0
#define M_DEF_SEARCH_ANGLE_DELTA_POS            0.0
#define M_DEF_SEARCH_ANGLE_TOLERANCE            5.0
#define M_DEF_SEARCH_ANGLE_ACCURACY             M_DISABLE
#define M_DEF_SEARCH_ANGLE_FINE_REGION          14L
#define M_DEF_SEARCH_ANGLE_DEBUG                M_DISABLE
#define M_DEF_SEARCH_ANGLE_INTERPOLATION_MODE   M_NEAREST_NEIGHBOR
#define M_DEF_ROTATED_MODEL_MIN_SCORE           M_DISABLE
#define M_DEF_CORRELATION_SCORE_FACTOR          0.5

/* Parameters for output units */
#define M_PIXEL                              0x1000L
#define M_WORLD                              0x2000L
#define M_ACCORDING_TO_CALIBRATION             1301L
#define M_ACCORDING_TO_RESULT                  1302L

/* Spelling variations and synonyms*/
#define M_CENTRE_X                              M_CENTER_X
#define M_CENTRE_Y                              M_CENTER_Y
#define M_COARSE_SEARCH_ACCEPTANCE              M_REJECTION_THRESHOLD     
#define M_MODEL_INDEX                           M_MOD_INDEX
#define M_KEEP_PYRAMID                          M_TARGET_CACHING

/* MpatRestore, MpatSave*/
#define M_INTERACTIVE                           M_NULL // Already defined in mil.h, milcal.h, milcode.h, miledge.h, milmeas.h, milocr.h, milmod.h


/********************************************************************
 * Function prototypes
 ********************************************************************/

#ifndef __midl // MIDL compiler used by ActiveMIL


MFTYPE32 MIL_ID MFTYPE MpatAllocModel(                       MIL_ID SystemId,
                                                             MIL_ID SrcImageId,
                                                             MIL_INT OffX,
                                                             MIL_INT OffY,
                                                             MIL_INT SizeX,
                                                             MIL_INT SizeY,
                                                             MIL_INT ModelType,
                                                             MIL_ID MPTYPE *IdPtr);
MFTYPE32 MIL_ID MFTYPE MpatAllocResult(                      MIL_ID SystemId,
                                                             MIL_INT NumEntries,
                                                             MIL_ID MPTYPE *IdPtr);

MFTYPE32 MIL_ID MFTYPE MpatAllocResultNoCaching(             MIL_ID SystemId,
                                                             MIL_INT NumEntries,
                                                             MIL_ID MPTYPE *IdPtr);
MFTYPE32 void MFTYPE MpatCopy(                               MIL_ID ModelId,
                                                             MIL_ID ImageId,
                                                             MIL_INT Version);
MFTYPE32 void MFTYPE MpatFindModel(                          MIL_ID ImageId,
                                                             MIL_ID ModelId,
                                                             MIL_ID ResultId);
MFTYPE32 void MFTYPE MpatFindMultipleModel(                  MIL_ID ImageId,
                                                             MIL_API_CONST MIL_ID MPTYPE *ModelId,
                                                             MIL_API_CONST MIL_ID MPTYPE *ResultId,
                                                             MIL_INT NumModels,
                                                             MIL_INT Flag);
MFTYPE32 void MFTYPE MpatFindOrientation(                    MIL_ID ImageId,
                                                             MIL_ID ModelId,
                                                             MIL_ID ResultId,
                                                             MIL_INT ResultRange);
MFTYPE32 void MFTYPE MpatFree(                               MIL_ID PatBufferId);
MFTYPE32 MIL_INT MFTYPE MpatGetNumber(                          MIL_ID ResultId,
                                                             MIL_INT MPTYPE *CountPtr);
MFTYPE32 void MFTYPE MpatGetResult(                          MIL_ID ResultId,
                                                             MIL_INT Type,
                                                             double MPTYPE *ArrayPtr);
MFTYPE32 void MFTYPE MpatGetResultOutputUnits(               MIL_ID ResultId,
                                                             MIL_INT Type,
                                                             MIL_INT OutputUnits,
                                                             double MPTYPE *ArrayPtr);
MFTYPE32 MIL_INT MFTYPE MpatInquire(                            MIL_ID ModelId,
                                                             MIL_INT Item,
                                                             void MPTYPE *VarPtr);
MFTYPE32 void MFTYPE MpatPreprocModel(                       MIL_ID ImageId,
                                                             MIL_ID ModelId,
                                                             MIL_INT Mode);
MFTYPE32 MIL_ID MFTYPE MpatAllocRotatedModel(                MIL_ID SystemId,
                                                             MIL_ID SrcModelorImageId,
                                                             double Angle,
                                                             MIL_INT InterpolMode,
                                                             MIL_INT ModelType,
                                                             MIL_ID MPTYPE *IdPtr);
MFTYPE32 void MFTYPE MpatSetAcceptance(                      MIL_ID ModelId,
                                                             double AcceptanceThreshold);
MFTYPE32 void MFTYPE MpatSetAccuracy(                        MIL_ID ModelId,
                                                             MIL_INT Accuracy);
MFTYPE32 void MFTYPE MpatSetAngle(                           MIL_ID ModelId,
                                                             MIL_INT ControlType,
                                                             double ControlValue);
MFTYPE32 void MFTYPE MpatSetCenter(                          MIL_ID ModelId,
                                                             double OffX,
                                                             double OffY);
MFTYPE32 void MFTYPE MpatSetCertainty(                       MIL_ID ModelId,
                                                             double CertaintyThreshold);
MFTYPE32 void MFTYPE MpatSetDontCare(                        MIL_ID ModelId,
                                                             MIL_ID ImageId,
                                                             MIL_INT OffX,
                                                             MIL_INT OffY,
                                                             MIL_INT Value);
MFTYPE32 void MFTYPE MpatSetNumber(                          MIL_ID ModelId,
                                                             MIL_INT NumMatches);
MFTYPE32 void MFTYPE MpatSetPosition(                        MIL_ID ModelId,
                                                             MIL_INT StartX,
                                                             MIL_INT StartY,
                                                             MIL_INT SizeX,
                                                             MIL_INT SizeY);
MFTYPE32 void MFTYPE MpatSetSearchParameter(                 MIL_ID ModelId,
                                                             MIL_INT Parameter,
                                                             double Value);
MFTYPE32 void MFTYPE MpatSetSpeed(                           MIL_ID ModelId,
                                                             MIL_INT Speed);
MFTYPE32 MIL_ID  MFTYPE MpatAllocAutoModel(                  MIL_ID SystemId,
                                                             MIL_ID BufId,
                                                             MIL_INT ModelSizeX,
                                                             MIL_INT ModelSizeY,
                                                             MIL_INT PosUncertaintyX,
                                                             MIL_INT PosUncertaintyY,
                                                             MIL_INT ModelType,
                                                             MIL_INT Mode,
                                                             MIL_ID *ModelId);

MFTYPE32 void MFTYPE MpatDraw(                               MIL_ID GraphContId, 
                                                             MIL_ID ModelOrResultId, 
                                                             MIL_ID DestImageId,
                                                             MIL_INT   Operation,
                                                             MIL_INT   Index,
                                                             MIL_INT   ControlFlag);

#if M_MIL_USE_UNICODE
MFTYPE32 void MFTYPE MpatSaveA(                              MIL_API_CONST char* FileName,
                                                             MIL_ID ModelId);
MFTYPE32 MIL_ID MFTYPE MpatRestoreA(                         MIL_ID SystemId,
                                                             MIL_API_CONST char* FileName,
                                                             MIL_ID MPTYPE *IdPtr);

MFTYPE32 void MFTYPE MpatStreamA(                            char*   MemPtrOrFileName, 
                                                             MIL_ID  SystemId, 
                                                             MIL_INT Operation, 
                                                             MIL_INT StreamType, 
                                                             double  Version, 
                                                             MIL_INT ControlFlag, 
                                                             MIL_ID  *ModelIdPtr, 
                                                             MIL_INT *SizeByteVarPtr);

MFTYPE32 void MFTYPE MpatSaveW(                              MIL_API_CONST_TEXT_PTR FileName,
                                                             MIL_ID ModelId);
MFTYPE32 MIL_ID MFTYPE MpatRestoreW(                         MIL_ID SystemId,
                                                             MIL_API_CONST_TEXT_PTR FileName,
                                                             MIL_ID MPTYPE *IdPtr);

MFTYPE32 void MFTYPE MpatStreamW(                            MIL_TEXT_PTR  MemPtrOrFileName,
                                                             MIL_ID        SystemId,
                                                             MIL_INT       Operation, 
                                                             MIL_INT       StreamType,
                                                             double        Version,
                                                             MIL_INT       ControlFlag,
                                                             MIL_ID        *ModelIdPtr,
                                                             MIL_INT       *SizeByteVarPtr);
#if M_MIL_UNICODE_API
#define MpatSave           MpatSaveW
#define MpatRestore        MpatRestoreW
#define MpatStream         MpatStreamW
#else
#define MpatSave           MpatSaveA
#define MpatRestore        MpatRestoreA
#define MpatStream         MpatStreamA
#endif

#else
MFTYPE32 void MFTYPE MpatSave(                               MIL_API_CONST_TEXT_PTR FileName,
                                                             MIL_ID ModelId);
MFTYPE32 MIL_ID MFTYPE MpatRestore(                          MIL_ID SystemId,
                                                             MIL_API_CONST_TEXT_PTR FileName,
                                                             MIL_ID MPTYPE *IdPtr);
MFTYPE32 void MFTYPE MpatStream(                             MIL_TEXT_PTR  MemPtrOrFileName,
                                                             MIL_ID        SystemId,
                                                             MIL_INT       Operation, 
                                                             MIL_INT       StreamType,
                                                             double        Version,
                                                             MIL_INT       ControlFlag,
                                                             MIL_ID        *ModelIdPtr,
                                                             MIL_INT       *SizeByteVarPtr);
#endif

#endif /* #ifdef __midl */


/* C++ directive if needed */
#ifdef __cplusplus
}
#endif

#if M_MIL_USE_SAFE_TYPE

//////////////////////////////////////////////////////////////
// See milos.h for explanation about these functions.
//////////////////////////////////////////////////////////////

// ----------------------------------------------------------
// MpatInquire

inline MFTYPE32 MIL_INT MFTYPE MpatInquireUnsafe  (MIL_ID ContextId, MIL_INT InquireType, void          MPTYPE *UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE MpatInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, int                   UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE MpatInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_INT32     MPTYPE *UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE MpatInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_INT64     MPTYPE *UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE MpatInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_DOUBLE    MPTYPE *UserVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 MIL_INT MFTYPE MpatInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_UINT32    MPTYPE *UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE MpatInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_UINT64    MPTYPE *UserVarPtr);
#endif

//--------------------------------------------------------------------------------
// MpatInquire

inline MFTYPE32 MIL_INT MFTYPE MpatInquireSafeType (MIL_ID ContextId, MIL_INT InquireType, int ValuePtr)
   {
   if (ValuePtr)
      SafeTypeError(MT("MpatInquire"));

   return MpatInquire(ContextId, InquireType, NULL);
   }

inline MFTYPE32 MIL_INT MFTYPE MpatInquireExecute (MIL_ID ContextId, MIL_INT InquireType, void       MPTYPE *ValuePtr, MIL_INT GivenType)
   {
   MIL_INT RequiredType = (InquireType & (M_TYPE_MIL_INT32 | M_TYPE_MIL_INT64 | M_TYPE_DOUBLE | M_TYPE_MIL_ID));
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MT("MpatInquire"));

   return MpatInquire(ContextId, InquireType, ValuePtr);
   }

inline MFTYPE32 MIL_INT MFTYPE MpatInquireUnsafe  (MIL_ID ContextId, MIL_INT InquireType, void       MPTYPE *ValuePtr) {return MpatInquire       (ContextId, InquireType, ValuePtr                  );}
inline MFTYPE32 MIL_INT MFTYPE MpatInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_INT32  MPTYPE *ValuePtr) {return MpatInquireExecute(ContextId, InquireType, ValuePtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 MIL_INT MFTYPE MpatInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_INT64  MPTYPE *ValuePtr) {return MpatInquireExecute(ContextId, InquireType, ValuePtr, M_TYPE_MIL_INT64);}
inline MFTYPE32 MIL_INT MFTYPE MpatInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_DOUBLE MPTYPE *ValuePtr) {return MpatInquireExecute(ContextId, InquireType, ValuePtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 MIL_INT MFTYPE MpatInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_UINT32 MPTYPE *ValuePtr) {return MpatInquireExecute(ContextId, InquireType, ValuePtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 MIL_INT MFTYPE MpatInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_UINT64 MPTYPE *ValuePtr) {return MpatInquireExecute(ContextId, InquireType, ValuePtr, M_TYPE_MIL_INT64);}
#endif

#define MpatInquire          MpatInquireSafeType

#else // #if M_MIL_USE_SAFE_TYPE

#define MpatInquireUnsafe          MpatInquire

#endif // #if M_MIL_USE_SAFE_TYPE

#endif // !M_MIL_LITE

#endif /* __MILPAT_H__ */

