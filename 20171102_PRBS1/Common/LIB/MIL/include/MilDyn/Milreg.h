/***************************************************************************/
/*

    Filename:  MilReg.h
    Owner   :  Matrox Imaging dept.
    Revision:  9.02.0580
    Content :  This file contains the defines and the prototypes for the
               MIL Registration module. (Mreg...).

    Copyright © Matrox Electronic Systems Ltd., 1992-2010.
    All Rights Reserved

*/
/***************************************************************************/

#ifndef __MILREG_H__
#define __MILREG_H__

#define BW_COMPATIBILITY 0x092

#if (!M_MIL_LITE) // MIL FULL ONLY

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif

/***************************************************************************/
/*                      MilReg CAPI defines                                */
/***************************************************************************/

/***************************************************************************/
/* Index values                                                            */
/***************************************************************************/
#define M_DEFAULT                               0x10000000L // Also defined in mil.h
#define M_CONTEXT                               0x08000000L
#define M_ALL                                   0x40000000L // Also defined in Milmod.h
#define M_GENERAL                               0x20000000L // Also defined in Milmod.h

/***************************************************************************/
/* MregAlloc()                                                             */
/***************************************************************************/

/* Registration types */
#define M_CORRELATION                                    1L

/***************************************************************************/
/* MregControl() and MregInquire()                                         */
/***************************************************************************/

/* ControlType and InquireType for context */
#define M_REGISTRATION_TYPE                 1L
#define M_OWNER_SYSTEM                      1101L      /* already defined in mil.h */
#define M_NUMBER_OF_ELEMENTS                2L
#define M_TRANSFORMATION_TYPE               3L
#define M_MIN_OVERLAP                       4L
#define M_MOSAIC_STATIC_INDEX               5L
#define M_MOSAIC_OFFSET_X                   6L
#define M_MOSAIC_OFFSET_Y                   7L
#define M_MOSAIC_COMPOSITION                8L
#define M_SR_PSF_TYPE                      10L
#define M_SR_PSF_RADIUS                    11L
#define M_SR_CONDITIONING_TYPE             12L
#define M_SR_SMOOTHNESS                    13L
#define M_SR_FLOAT_MODE                    14L
#define M_SR_FIXED_POINT                   15L
#define M_DELETE_ELEMENT                   16L
#define M_PAIRING_MODE                     17L
#define M_MAX_INTERMEDIATES                18L
#define M_MOSAIC_SCALE                     19L
#define M_SR_MAX_ITER                      20L
// skip 21L, reserved for M_FAILURE_MESSAGE
#define M_SR_COMMUTE_BLUR_AND_WARP         22L
#define M_SCORE_TYPE                       37L  // already defined in milpat.h

/* ControlType and InquireType for context element */
#define M_OPTIMIZE_LOCATION              1002L
#define M_LOCATION_DELTA                 1003L
#define M_ACCURACY                        106L  // already defined in    milmod.h, miledge.h, milpat.h
#define M_REFERENCE_X                     100L  // already defined in    milmod.h, milpat.h
#define M_REFERENCE_Y                     101L  // already defined in    milmod.h, milpat.h
#define M_OPTICAL_CENTER_X               1007L
#define M_OPTICAL_CENTER_Y               1008L
#define M_FOCAL_LENGTH                   1009L
#define M_MASK_IMAGE                     1020L
#define M_EXTRACT_MODELS                 1021L
#define M_ACCEPTANCE_LEVEL               1022L

#define M_SET_LOCATION_TARGET            1010L
#define M_SET_LOCATION_PARAM_TYPE        1011L
#define M_SET_LOCATION_PARAM_1           1012L
#define M_SET_LOCATION_PARAM_2           1013L
#define M_SET_LOCATION_PARAM_3           1014L
#define M_SET_LOCATION_PARAM_4           1015L

#define M_SET_LOCATION_PARAM_DATA_1      1016L

/* ControlType and InquireType for result */
#define M_DRAW_CROSS_SIZE                              115L // also defined in miledge.h
#define M_DRAW_RELATIVE_ORIGIN_X                       319L // also defined in milmod.h, miledge.h
#define M_DRAW_RELATIVE_ORIGIN_Y                       320L // also defined in milmod.h, miledge.h
#define M_DRAW_SCALE_X                                3203L // also defined in milmod.h, miledge.h
#define M_DRAW_SCALE_Y                                3204L // also defined in milmod.h, miledge.h

/* InquireType modifiers */
#define M_DEFAULT                               0x10000000L // already defined in mil.h

/* Index */
#define M_ALL                                   0x40000000L // already defined in mil.h

/* Basic ControlValue  */
#define M_DEFAULT                               0x10000000L // already defined in mil.h
#define M_ENABLE                                     -9997L // already defined in mil.h
#define M_DISABLE                                    -9999L // already defined in mil.h
#if (BW_COMPATIBILITY == 0x080)
#define M_INFINITE                              0xFFFFFFFFL
#else
#define M_INFINITE                              -1L // Also defined in Mil.h
#endif

/* ControlValue for M_TRANSFORMATION_TYPE */
#define M_TRANSLATION                                    1L // also in milcal.h
#define M_TRANSLATION_ROTATION                           2L // also in milcal.h
#define M_TRANSLATION_ROTATION_SCALE                     3L // also in milcal.h
#define M_AZIM_ELEV_ROLL                                 4L
#define M_PERSPECTIVE                                    5L

/* ControlValue for M_ACCURACY */
#define M_LOW                                            1L          // Already defined in    miledge.h, milcode.h, milmod.h, milocr, milpat.h, mil.h
#define M_HIGH                                           3L          // Already defined in    miledge.h, milcode.h, milmod.h, milocr, milpat.h, mil.h
#define M_VERY_HIGH                                      4L          // Already defined in    miledge.h, milcode.h, milmod.h, milocr, milpat.h

/* ControlValue for M_REFERENCE_X, M_REFERENCE_Y, M_OPTICAL_CENTER_X, M_OPTICAL_CENTER_Y */
#define M_CENTER_ELEMENT                        0x40000000L

/* ControlValue for M_OPTICAL_CENTER_X, M_OPTICAL_CENTER_Y */
#define M_CENTER_OBJECT                         0x10000000L 

/* ControlValue for M_MOSAIC_STATIC_INDEX */
#define M_REGISTRATION_GLOBAL                   0x02000000L
#define M_ALL                                   0x40000000L // already defined in mil.h

/* ControlValue for M_MOSAIC_OFFSET_X */
#define M_ALIGN_LEFT                            0x40000010L

/* ControlValue for M_MOSAIC_OFFSET_Y */
#define M_ALIGN_TOP                             0x40000011L


/* ControlValue for M_MOSAIC_COMPOSITION */
#define M_SUPER_RESOLUTION                               1L
#define M_FIRST_IMAGE                                    2L
#define M_LAST_IMAGE                                     3L
#define M_AVERAGE_IMAGE                                  4L
#define M_FUSION_IMAGE                                   5L

/* ControlValue for M_SR_PSF_TYPE */
#define M_CIRCULAR                                       1L
#define M_GAUSSIAN                                   0x805L // also defined in miledge.h
/*#define M_SMOOTH */                                         // also defined in miledge.h

/* ControlValue for M_SR_CONDITIONING_TYPE */
/*#define M_SMOOTH */                                         // also defined in miledge.h

/* ControlValue for M_PAIRING_MODE */
#define M_USE_SET_LOCATION_TARGET                        1L
#define M_USE_MAX_INTERMEDIATES                          2L


/***************************************************************************/
/* MregSetLocation()                                                       */
/***************************************************************************/

/* Index */
#define M_ALL                                   0x40000000L // already defined in mil.h

/* Target */
#define M_REGISTRATION_GLOBAL                   0x02000000L
#define M_PREVIOUS                              0x40000003L
#define M_NEXT                                  0x40000004L
#define M_UNCHANGED                             0x40000005L
#define M_COPY                                  0x00020000L // already defined in mil.h

/* ParamType */
#define M_POSITION_XY                                    1L
#define M_POSITION_XY_ANGLE                              2L
#define M_POSITION_XY_ANGLE_SCALE                        3L
#define M_WARP_POLYNOMIAL                       0x00200000L // already defined in milim.h
#define M_WARP_4_CORNER                         0x00800000L // already defined in mil.h
#define M_WARP_4_CORNER_REVERSE                 0x01000000L // already defined in mil.h
#define M_TIE_POINT_LIST                                 4L
#define M_COPY_REG_CONTEXT                      0x40000001L
#define M_COPY_REG_RESULT                       0x40000002L



/***************************************************************************/
/* MregGetResult()                                                         */
/***************************************************************************/

/* ResultIndex */
#define M_ALL                                   0x40000000L // already defined in mil.h
#define M_GENERAL                               0x20000000L // also defined in milmod.h

/* ResultType */
#define M_RESULT                                0x00001000L // also used by milmeas.h and milmod.h
#define M_SCORE                                 0x00001400L // also defined in MilPat.h and milmod.h
#define M_FAILURE_CODE                                  20L
#define M_FAILURE_MESSAGE                               21L
#define M_FAILURE_MESSAGE_LENGTH                        22L
#define M_MOSAIC_SIZE_X                                  9L
#define M_MOSAIC_SIZE_Y                                 10L
#define M_MOSAIC_OFFSET_X                                6L // also a ControlType
#define M_MOSAIC_OFFSET_Y                                7L // also a ControlType

                                                            // These results should not clash when added with M_GLOBAL
#define M_ANGLE                                 0x00000800L // already defined in mil.h
#define M_POSITION_X                            0x00003400L // also defined in milpat.h
#define M_POSITION_Y                            0x00004400L // also defined in milpat.h
#define M_SCALE                                 0x00008010L // also defined in milpat.h
#define M_TRANSFORMED_UL_X                              23L
#define M_TRANSFORMED_UL_Y                              24L
#define M_TRANSFORMED_UR_X                              25L
#define M_TRANSFORMED_UR_Y                              26L
#define M_TRANSFORMED_BL_X                              27L
#define M_TRANSFORMED_BL_Y                              28L
#define M_TRANSFORMED_BR_X                              29L
#define M_TRANSFORMED_BR_Y                              30L
#define M_ELEVATION                                     49L
#define M_AZIMUTH                                       50L
#define M_ROLL                                          51L
#define M_TRANSFORMATION_MATRIX_ID                      52L
#define M_REVERSE_TRANSFORMATION_MATRIX_ID              53L
#define M_EXPOSURE_GAIN                                 70L
#define M_EXPOSURE_OFFSET                               71L
#define M_TRANSFORMATION_MATRIX                         72L
#define M_REVERSE_TRANSFORMATION_MATRIX                 73L
#define M_PAIR_PARTNER_NUMBER                           74L
#define M_PAIR_PARTNER                                  75L

#ifndef M_MIL_75_COMPATIBLE
#define M_GLOBAL                                0x00000008L // already defined in mil.h
#else
#define M_GLOBAL                                         8L
#endif //M_MIL_75_COMPATIBLE

/* ResultType modifiers */
#define M_DEFAULT                               0x10000000L // already defined in mil.h
#define M_REGISTRATION_GLOBAL                   0x02000000L
#define M_REFERENCE                             0x04000000L

#define M_REG_OBJECT                            (M_USER_ATTRIBUTE       | MAKE_INT64(0x0000000800000000))
#define M_REG_CONTEXT                           (M_REG_OBJECT           | 0x00000001L )
#define M_REG_RESULT                            (M_REG_OBJECT           | 0x00000002L )

/* Result values */
#define M_SUCCESS                               0x00000000L
#define M_FAILURE                               0x00000001L

#define M_FAILURE_BAD_TRANSFORM_PARAM           1L
#define M_FAILURE_TOO_MANY_ELEMENTS             2L
#define M_FAILURE_NOT_FOUND                     3L
#define M_FAILURE_HINT_MATH_EXCEPTION           4L
#define M_FAILURE_CALCULATE_MATH_EXCEPTION      5L
#define M_FAILURE_IMAGE_TOO_SMALL               6L
#define M_FAILURE_IMAGE_NO_INFO                 7L
#define M_FAILURE_NO_CONVERGENCE                8L


/***************************************************************************/
/* MregDraw()                                                              */
/***************************************************************************/

/* Operation */
#define M_DRAW_POST_REGISTRATION_IMAGE                   1L
#define M_DRAW_BOX                              0x00000020L // already defined in milpat.h
#define M_DRAW_TIE_POINTS                               10L

/***************************************************************************/
/* MregTransformCoordinate() and MregTransformCoordinateList()             */
/***************************************************************************/

/* Source and destination */
#define M_REGISTRATION_GLOBAL                   0x02000000L
#define M_MOSAIC                                0x40000002L

/***************************************************************************/
/*                 MilReg CAPI function prototypes                         */
/***************************************************************************/

#ifndef __midl // MIDL compiler used by ActiveMIL

MFTYPE32 MIL_ID MFTYPE MregAlloc(MIL_ID SystemId, 
                                 MIL_INT RegistrationType, 
                                 MIL_INT ControlFlag, 
                                 MIL_ID *ContextIdPtr);

MFTYPE32 MIL_ID MFTYPE MregAllocResult(MIL_ID SystemId, 
                                       MIL_INT ControlFlag, 
                                       MIL_ID *RegResultIdPtr);

MFTYPE32 void MFTYPE MregCalculate(MIL_ID ContextId, 
                                   const MIL_ID *ElementArrayPtr,
                                   MIL_ID RegResultId, 
                                   MIL_INT NumElements,
                                   MIL_INT ControlFlag);

#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs
MFTYPE32 void MFTYPE MregControlInt64 (MIL_ID ContextOrResultId, 
                                       MIL_INT Index, 
                                       MIL_INT ControlType, 
                                       MIL_INT64 ControlValue);

MFTYPE32 void MFTYPE MregControlDouble(MIL_ID ContextOrResultId, 
                                       MIL_INT Index, 
                                       MIL_INT ControlType, 
                                       double ControlValue);

#else
// Prototypes for 32 bits OSs
#define MregControlInt64  MregControl
#define MregControlDouble MregControl
MFTYPE32 void MFTYPE MregControl      (MIL_ID ContextOrResultId, 
                                       MIL_INT Index, 
                                       MIL_INT ControlType, 
                                       double ControlValue);
#endif

MFTYPE32 void MFTYPE MregDraw(MIL_ID GraphContextId, 
                              MIL_ID RegResultId, 
                              MIL_ID DestImageId, 
                              MIL_INT Operation, 
                              MIL_INT Index, 
                              MIL_INT ControlFlag);

MFTYPE32 void MFTYPE MregFree(MIL_ID ObjectId);

MFTYPE32 void MFTYPE MregGetResult(MIL_ID RegResultId, 
                                   MIL_INT ResultIndex, 
                                   MIL_INT ResultType, 
                                   void *ResultPtr);

MFTYPE32 MIL_INT MFTYPE MregInquire(MIL_ID ContextOrResultId, 
                                 MIL_INT Index, 
                                 MIL_INT InquireType, 
                                 void *UserVarPtr);

#if M_MIL_USE_UNICODE

MFTYPE32 MIL_ID MFTYPE MregRestoreW(MIL_CONST_TEXT_PTR FileName, 
                                    MIL_ID SystemId, 
                                    MIL_INT ControlFlag, 
                                    MIL_ID *ContextIdPtr);

MFTYPE32 void MFTYPE MregSaveW(MIL_CONST_TEXT_PTR FileName, 
                               MIL_ID ContextId, 
                               MIL_INT ControlFlag);

MFTYPE32 void MFTYPE MregStreamW(MIL_TEXT_PTR MemPtrOrFileName, 
                                 MIL_ID SystemId, 
                                 MIL_INT Operation, 
                                 MIL_INT StreamType, 
                                 double Version, 
                                 MIL_INT ControlFlag, 
                                 MIL_ID *ContextIdPtr, 
                                 MIL_INT *SizeByteVarPtr);

MFTYPE32 MIL_ID MFTYPE MregRestoreA(const char* FileName, 
                                    MIL_ID SystemId, 
                                    MIL_INT ControlFlag, 
                                    MIL_ID *ContextIdPtr);

MFTYPE32 void MFTYPE MregSaveA(const char* FileName, 
                               MIL_ID ContextId, 
                               MIL_INT ControlFlag);

MFTYPE32 void MFTYPE MregStreamA(char* MemPtrOrFileName, 
                                 MIL_ID SystemId, 
                                 MIL_INT Operation, 
                                 MIL_INT StreamType, 
                                 double Version, 
                                 MIL_INT ControlFlag, 
                                 MIL_ID *ContextIdPtr, 
                                 MIL_INT *SizeByteVarPtr);

#if M_MIL_UNICODE_API
#define MregSave           MregSaveW
#define MregRestore        MregRestoreW
#define MregStream         MregStreamW
#else
#define MregSave           MregSaveA
#define MregRestore        MregRestoreA
#define MregStream         MregStreamA
#endif

#else

MFTYPE32 MIL_ID MFTYPE MregRestore(MIL_CONST_TEXT_PTR FileName, 
                                   MIL_ID SystemId, 
                                   MIL_INT ControlFlag, 
                                   MIL_ID *ContextIdPtr);

MFTYPE32 void MFTYPE MregSave(MIL_CONST_TEXT_PTR FileName, 
                              MIL_ID ContextId, 
                              MIL_INT ControlFlag);

MFTYPE32 void MFTYPE MregStream(MIL_TEXT_PTR MemPtrOrFileName, 
                                MIL_ID SystemId, 
                                MIL_INT Operation, 
                                MIL_INT StreamType, 
                                double Version, 
                                MIL_INT ControlFlag, 
                                MIL_ID *ContextIdPtr, 
                                MIL_INT *SizeByteVarPtr);
#endif

#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs
MFTYPE32 void MFTYPE MregSetLocationInt64(MIL_ID ContextId, 
                                          MIL_INT Index, 
                                          MIL_INT Target,
                                          MIL_INT ParamType,
                                          MIL_INT64 Param1,
                                          double Param2,
                                          double Param3,
                                          double Param4,
                                          MIL_INT ControlFlag);

MFTYPE32 void MFTYPE MregSetLocationDouble(MIL_ID ContextId, 
                                           MIL_INT Index, 
                                           MIL_INT Target,
                                           MIL_INT ParamType,
                                           double Param1,
                                           double Param2,
                                           double Param3,
                                           double Param4,
                                           MIL_INT ControlFlag);
#else
// Prototypes for 32 bits OSs
#define MregSetLocationInt64  MregSetLocation
#define MregSetLocationDouble MregSetLocation
MFTYPE32 void MFTYPE MregSetLocation(MIL_ID ContextId, 
                                     MIL_INT Index, 
                                     MIL_INT Target,
                                     MIL_INT ParamType,
                                     double Param1,
                                     double Param2,
                                     double Param3,
                                     double Param4,
                                     MIL_INT ControlFlag);
#endif

MFTYPE32 void MFTYPE MregTransformImage(MIL_ID RegResultId, 
                                        const MIL_ID *ImageArrayPtr,
                                        MIL_ID DestImageBufId, 
                                        MIL_INT NumImages,
                                        MIL_INT InterpolationMode,
                                        MIL_INT ControlFlag);

MFTYPE32 void MFTYPE MregTransformCoordinate(MIL_ID RegResultId, 
                                             MIL_INT Source,
                                             MIL_INT Destination,
                                             double X,
                                             double Y,
                                             double *ResXPtr,
                                             double *ResYPtr,
                                             MIL_INT ControlFlag);

MFTYPE32 void MFTYPE MregTransformCoordinateList(MIL_ID RegResultId, 
                                                 MIL_INT Source,
                                                 MIL_INT Destination,
                                                 MIL_INT NumPoints,
                                                 const double *SrcXPtr,
                                                 const double *SrcYPtr,
                                                 double *ResXPtr,
                                                 double *ResYPtr,
                                                 MIL_INT ControlFlag);

#endif /* #ifdef __midl */

/* C++ directive if needed */
#ifdef __cplusplus
}
#endif
#if M_MIL_USE_64BIT
#ifdef __cplusplus
//////////////////////////////////////////////////////////////
// MregControl function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_MIL_USE_LINUX
inline void MregControl      (MIL_ID ContextOrResultId, 
                              MIL_INT Index, 
                              MIL_INT ControlType, 
                              int ControlValue)
   {
   MregControlInt64(ContextOrResultId, Index, ControlType, ControlValue);
   };
#endif
inline void MregControl      (MIL_ID ContextOrResultId, 
                              MIL_INT Index, 
                              MIL_INT ControlType, 
                              MIL_INT32 ControlValue)
   {
   MregControlInt64(ContextOrResultId, Index, ControlType, ControlValue);
   };
inline void MregControl      (MIL_ID ContextOrResultId, 
                              MIL_INT Index, 
                              MIL_INT ControlType, 
                              MIL_INT64 ControlValue)
   {
   MregControlInt64(ContextOrResultId, Index, ControlType, ControlValue);
   };
inline void MregControl      (MIL_ID ContextOrResultId, 
                              MIL_INT Index, 
                              MIL_INT ControlType, 
                              double ControlValue)
   {
   MregControlDouble(ContextOrResultId, Index, ControlType, ControlValue);
   };

//////////////////////////////////////////////////////////////
// MregSetLocation function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_MIL_USE_LINUX
inline void MregSetLocation(MIL_ID ContextId, 
                            MIL_INT Index, 
                            MIL_INT Target,
                            MIL_INT ParamType,
                            int Param1,
                            double Param2,
                            double Param3,
                            double Param4,
                            MIL_INT ControlFlag)
   {
   MregSetLocationInt64(ContextId, Index, Target, ParamType, Param1, Param2, Param3, Param4, ControlFlag);
   }
#endif
inline void MregSetLocation(MIL_ID ContextId, 
                            MIL_INT Index, 
                            MIL_INT Target,
                            MIL_INT ParamType,
                            MIL_INT32 Param1,
                            double Param2,
                            double Param3,
                            double Param4,
                            MIL_INT ControlFlag)
   {
   MregSetLocationInt64(ContextId, Index, Target, ParamType, Param1, Param2, Param3, Param4, ControlFlag);
   }
inline void MregSetLocation(MIL_ID ContextId, 
                            MIL_INT Index, 
                            MIL_INT Target,
                            MIL_INT ParamType,
                            MIL_INT64 Param1,
                            double Param2,
                            double Param3,
                            double Param4,
                            MIL_INT ControlFlag)
   {
   MregSetLocationInt64(ContextId, Index, Target, ParamType, Param1, Param2, Param3, Param4, ControlFlag);
   }
inline void MregSetLocation(MIL_ID ContextId, 
                            MIL_INT Index, 
                            MIL_INT Target,
                            MIL_INT ParamType,
                            double Param1,
                            double Param2,
                            double Param3,
                            double Param4,
                            MIL_INT ControlFlag)
   {
   MregSetLocationDouble(ContextId, Index, Target, ParamType, Param1, Param2, Param3, Param4, ControlFlag);
   }
#else
//////////////////////////////////////////////////////////////
// For C file, call the default function, i.e. Int64 one
//////////////////////////////////////////////////////////////
#define MregControl     MregControlDouble
#define MregSetLocation MregSetLocationDouble

#endif // __cplusplus
#endif // M_MIL_USE_64BIT

#if M_MIL_USE_SAFE_TYPE

//////////////////////////////////////////////////////////////
// See milos.h for explanation about these functions.
//////////////////////////////////////////////////////////////

// ----------------------------------------------------------
// MregGetResult

inline MFTYPE32 void MFTYPE MregGetResultUnsafe  (MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, void       MPTYPE *ResultPtr);
inline MFTYPE32 void MFTYPE MregGetResultSafeType(MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, int                ResultPtr);
inline MFTYPE32 void MFTYPE MregGetResultSafeType(MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, MIL_INT8   MPTYPE *ResultPtr);
inline MFTYPE32 void MFTYPE MregGetResultSafeType(MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, MIL_INT16  MPTYPE *ResultPtr);
inline MFTYPE32 void MFTYPE MregGetResultSafeType(MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, MIL_INT32  MPTYPE *ResultPtr);
inline MFTYPE32 void MFTYPE MregGetResultSafeType(MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, MIL_INT64  MPTYPE *ResultPtr);
inline MFTYPE32 void MFTYPE MregGetResultSafeType(MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, MIL_DOUBLE MPTYPE *ResultPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 void MFTYPE MregGetResultSafeType(MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, MIL_UINT8  MPTYPE *ResultPtr);
inline MFTYPE32 void MFTYPE MregGetResultSafeType(MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, MIL_UINT16 MPTYPE *ResultPtr);
inline MFTYPE32 void MFTYPE MregGetResultSafeType(MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, MIL_UINT32 MPTYPE *ResultPtr);
inline MFTYPE32 void MFTYPE MregGetResultSafeType(MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, MIL_UINT64 MPTYPE *ResultPtr);
#endif
#if M_MIL_SAFE_TYPE_ADD_WCHAR_T
inline MFTYPE32 void MFTYPE MregGetResultSafeType(MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, wchar_t    MPTYPE *ResultPtr);
#endif

// ----------------------------------------------------------
// MregInquire

inline MFTYPE32 MIL_INT MFTYPE MregInquireUnsafe  (MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, void       MPTYPE *UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE MregInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, int                UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE MregInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_INT32  MPTYPE *UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE MregInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_INT64  MPTYPE *UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE MregInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_DOUBLE MPTYPE *UserVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 MIL_INT MFTYPE MregInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_UINT32 MPTYPE *UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE MregInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_UINT64 MPTYPE *UserVarPtr);
#endif

// ----------------------------------------------------------
// MregGetResult

inline MIL_INT MregGetResultRequiredType(MIL_INT ResultType)
   {
   switch (ResultType)
      {
      case M_FAILURE_MESSAGE:
         return M_TYPE_TEXT_CHAR;
         break;

      default:
         return M_TYPE_DOUBLE;
         break;
      }
   }

inline MFTYPE32 void MFTYPE MregGetResultSafeType(MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, int         ResultPtr)
   {
   if (ResultPtr)
      SafeTypeError(MT("MregGetResult"));

   MregGetResult(RegResultId, ResultIndex, ResultType, NULL);
   }

inline void MregGetResultSafeTypeExecute (MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, void MPTYPE *ResultPtr, MIL_INT GivenType)
   {
   MIL_INT RequiredType = (ResultType & (M_TYPE_CHAR | M_TYPE_SHORT | M_TYPE_MIL_INT32 | M_TYPE_MIL_INT64 | M_TYPE_DOUBLE |M_TYPE_MIL_ID));
   if (RequiredType == 0)
      RequiredType = MregGetResultRequiredType(ResultType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MT("MregGetResult"));

   MregGetResult(RegResultId, ResultIndex, ResultType, ResultPtr);
   }

inline MFTYPE32 void MFTYPE MregGetResultUnsafe  (MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, void       MPTYPE *ResultPtr) {MregGetResult               (RegResultId, ResultIndex, ResultType, ResultPtr                  );}
inline MFTYPE32 void MFTYPE MregGetResultSafeType(MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, MIL_INT8   MPTYPE *ResultPtr) {MregGetResultSafeTypeExecute(RegResultId, ResultIndex, ResultType, ResultPtr, M_TYPE_CHAR     );}
inline MFTYPE32 void MFTYPE MregGetResultSafeType(MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, MIL_INT16  MPTYPE *ResultPtr) {MregGetResultSafeTypeExecute(RegResultId, ResultIndex, ResultType, ResultPtr, M_TYPE_SHORT    );}
inline MFTYPE32 void MFTYPE MregGetResultSafeType(MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, MIL_INT32  MPTYPE *ResultPtr) {MregGetResultSafeTypeExecute(RegResultId, ResultIndex, ResultType, ResultPtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 void MFTYPE MregGetResultSafeType(MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, MIL_INT64  MPTYPE *ResultPtr) {MregGetResultSafeTypeExecute(RegResultId, ResultIndex, ResultType, ResultPtr, M_TYPE_MIL_INT64);}
inline MFTYPE32 void MFTYPE MregGetResultSafeType(MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, MIL_DOUBLE MPTYPE *ResultPtr) {MregGetResultSafeTypeExecute(RegResultId, ResultIndex, ResultType, ResultPtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 void MFTYPE MregGetResultSafeType(MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, MIL_UINT8  MPTYPE *ResultPtr) {MregGetResultSafeTypeExecute(RegResultId, ResultIndex, ResultType, ResultPtr, M_TYPE_CHAR     );}
inline MFTYPE32 void MFTYPE MregGetResultSafeType(MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, MIL_UINT16 MPTYPE *ResultPtr) {MregGetResultSafeTypeExecute(RegResultId, ResultIndex, ResultType, ResultPtr, M_TYPE_SHORT    );}
inline MFTYPE32 void MFTYPE MregGetResultSafeType(MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, MIL_UINT32 MPTYPE *ResultPtr) {MregGetResultSafeTypeExecute(RegResultId, ResultIndex, ResultType, ResultPtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 void MFTYPE MregGetResultSafeType(MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, MIL_UINT64 MPTYPE *ResultPtr) {MregGetResultSafeTypeExecute(RegResultId, ResultIndex, ResultType, ResultPtr, M_TYPE_MIL_INT64);}
#endif
#if M_MIL_SAFE_TYPE_ADD_WCHAR_T
inline MFTYPE32 void MFTYPE MregGetResultSafeType(MIL_ID RegResultId, MIL_INT ResultIndex, MIL_INT ResultType, wchar_t    MPTYPE *ResultPtr) {MregGetResultSafeTypeExecute(RegResultId, ResultIndex, ResultType, ResultPtr, M_TYPE_SHORT    );}
#endif

// ----------------------------------------------------------
// MregInquire

inline MFTYPE32 MIL_INT MFTYPE MregInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, int UserVarPtr)
   {
   if (UserVarPtr)
      SafeTypeError(MT("MregInquire"));

   return MregInquire(ContextOrResultId, Index, InquireType, NULL);
   }

inline MFTYPE32 MIL_INT MFTYPE MregInquireSafeTypeExecute (MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, void MPTYPE *UserVarPtr, MIL_INT GivenType)
   {
   MIL_INT RequiredType = (InquireType & (M_TYPE_MIL_INT32 | M_TYPE_MIL_INT64 | M_TYPE_DOUBLE |M_TYPE_MIL_ID));
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MT("MregInquire"));

   return MregInquire(ContextOrResultId, Index, InquireType, UserVarPtr);
   }

inline MFTYPE32 MIL_INT MFTYPE MregInquireUnsafe  (MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, void       MPTYPE *UserVarPtr) {return MregInquire               (ContextOrResultId, Index, InquireType, UserVarPtr                  );}
inline MFTYPE32 MIL_INT MFTYPE MregInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_INT32  MPTYPE *UserVarPtr) {return MregInquireSafeTypeExecute(ContextOrResultId, Index, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 MIL_INT MFTYPE MregInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_INT64  MPTYPE *UserVarPtr) {return MregInquireSafeTypeExecute(ContextOrResultId, Index, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
inline MFTYPE32 MIL_INT MFTYPE MregInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_DOUBLE MPTYPE *UserVarPtr) {return MregInquireSafeTypeExecute(ContextOrResultId, Index, InquireType, UserVarPtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 MIL_INT MFTYPE MregInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_UINT32 MPTYPE *UserVarPtr) {return MregInquireSafeTypeExecute(ContextOrResultId, Index, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 MIL_INT MFTYPE MregInquireSafeType(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT InquireType, MIL_UINT64 MPTYPE *UserVarPtr) {return MregInquireSafeTypeExecute(ContextOrResultId, Index, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
#endif

#define MregGetResult        MregGetResultSafeType
#define MregInquire          MregInquireSafeType

#else // #if M_MIL_USE_SAFE_TYPE

#define MregGetResultUnsafe        MregGetResult
#define MregInquireUnsafe          MregInquire

#endif // #if M_MIL_USE_SAFE_TYPE

#endif // !M_MIL_LITE

#endif /* __MILSTR_H__ */
