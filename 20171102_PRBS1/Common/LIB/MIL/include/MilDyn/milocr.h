/***************************************************************************/
/*

    Filename:  MILOCR.H
    Owner   :  Matrox Imaging dept.
    Revision:  9.02.0580
    Content :  This file contains the prototypes and #defines for the Matrox
               Imaging Library (MIL) C OCR module user's functions.

    Copyright © Matrox Electronic Systems Ltd., 1992-2010.
    All Rights Reserved

*/
/***************************************************************************/

#ifndef __MILOCR_H__
#define __MILOCR_H__

#define BW_COMPATIBILITY 0x092

#if (!M_MIL_LITE) // MIL FULL ONLY

#ifndef __midl // MIDL compiler used by ActiveMIL

/* Prototypes              */
/* C++ directive if needed */

#ifdef __cplusplus
extern "C"
{
#endif


typedef MFTYPE32 MIL_INT (MFTYPE MPTYPE *MIL_OCR_HOOK_FUNCTION_PTR)( MIL_INT HookType,
                                                                  char *String,
                                                                  void MPTYPE *FExpansionFlagPtr);

MFTYPE32 MIL_ID  MFTYPE MocrAllocFont(    MIL_ID SystemId,
                                          MIL_INT FontType,
                                          MIL_INT CharNumber,
                                          MIL_INT CharBoxSizeX,
                                          MIL_INT CharBoxSizeY,
                                          MIL_INT CharOffsetX,
                                          MIL_INT CharOffsetY,
                                          MIL_INT CharSizeX,
                                          MIL_INT CharSizeY,
                                          MIL_INT CharThickness,
                                          MIL_INT StringLength,
                                          MIL_INT InitFlag,
                                          MIL_ID *FontIdPtr);

MFTYPE32 MIL_ID  MFTYPE MocrAllocResult(  MIL_ID SystemId,
                                          MIL_INT InitFlag,
                                          MIL_ID *ResultIdPtr);

MFTYPE32 void    MFTYPE MocrFree(         MIL_ID FontOrResultId);

MFTYPE32 void    MFTYPE MocrGetResult(    MIL_ID ResultId,
                                          MIL_INT ResultToGet,
                                          void *ResultPtr);

MFTYPE32 MIL_INT    MFTYPE MocrInquire(      MIL_ID FontId,
                                          MIL_INT InquireItem,
                                          void *ResultPtr);

MFTYPE32 void    MFTYPE MocrReadString(   MIL_ID ImageId,
                                          MIL_ID FontId,
                                          MIL_ID ResultId);

MFTYPE32 void    MFTYPE MocrModifyFont(   MIL_ID FontId,
                                          MIL_INT   Operation,
                                          MIL_INT   ControlFlag);

#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs
MFTYPE32 void    MFTYPE MocrControlInt64 (MIL_ID FontId,
                                          MIL_INT ControlToSet,
                                          MIL_INT64 Value);

MFTYPE32 void    MFTYPE MocrControlDouble(MIL_ID FontId,
                                          MIL_INT ControlToSet,
                                          double Value);

#else
// Prototypes for 32 bits OSs
#define MocrControlInt64  MocrControl
#define MocrControlDouble MocrControl

MFTYPE32 void    MFTYPE MocrControl( MIL_ID FontId,
                                     MIL_INT ControlToSet,
                                     double Value);
#endif

MFTYPE32 void    MFTYPE MocrLineControl(  MIL_ID FontId,
                                          MIL_INT LineIndex,
                                          MIL_INT ControlToSet,
                                          double Value);


MFTYPE32 MIL_OCR_HOOK_FUNCTION_PTR MFTYPE MocrHookFunction( MIL_ID FontId,
                                                            MIL_INT HookType,
                                                            MIL_OCR_HOOK_FUNCTION_PTR FctPtr,
                                                            void   *UserData);

MFTYPE32 MIL_INT MFTYPE OcrSemiM1292CheckValid(MIL_INT HookType,
                                            char *String,
                                            void MPTYPE *FExpansionFlagPtr);
MFTYPE32 MIL_INT MFTYPE OcrSemiM1388CheckValid(MIL_INT HookType,
                                            char *String,
                                            void MPTYPE *FExpansionFlagPtr);
MFTYPE32 MIL_INT MFTYPE OcrSemiCheckValid(   MIL_INT HookType,
                                          char *String,
                                          void MPTYPE *FExpansionFlagPtr);
MFTYPE32 MIL_INT MFTYPE OcrDefaultCheckValid(MIL_INT HookType,
                                          char *String,
                                          void MPTYPE *FExpansionFlagPtr);

MFTYPE32 MIL_ID MFTYPE MocrAutoAllocFont( MIL_ID SystemId,
                                          MIL_ID ImageId,
                                          MIL_INT   CharNumber,
                                          MIL_INT   StringLength,
                                          MIL_INT   InitFlag,
                                          MIL_ID *FontIdPtr);

MFTYPE32 void MFTYPE MocrPreprocess(MIL_ID FontId, MIL_INT Mode);

#if M_MIL_USE_UNICODE

MFTYPE32 void     MFTYPE MocrCalibrateFontW(MIL_ID CalibrationImageId,
                                            MIL_ID FontId,
                                            MIL_CONST_TEXT_PTR String,
                                            double TargetCharSizeXMin,
                                            double TargetCharSizeXMax,
                                            double TargetCharSizeXStep,
                                            double TargetCharSizeYMin,
                                            double TargetCharSizeYmax,
                                            double TargetCharSizeYStep,
                                            MIL_INT Operation);

MFTYPE32 void     MFTYPE MocrCopyFontW(MIL_ID ImageId,
                                       MIL_ID FontId,
                                       MIL_INT Operation,
                                       MIL_CONST_TEXT_PTR CharListString);

MFTYPE32 void     MFTYPE MocrImportFontW(MIL_CONST_TEXT_PTR FileName,
                                         MIL_INT FileFormat,
                                         MIL_INT Operation,
                                         MIL_CONST_TEXT_PTR CharListString,
                                         MIL_ID FontId);

MFTYPE32 MIL_ID   MFTYPE MocrRestoreFontW(MIL_CONST_TEXT_PTR FileName,
                                          MIL_INT Operation,
                                          MIL_ID SystemId,
                                          MIL_ID *FontId);

MFTYPE32 void     MFTYPE MocrSaveFontW(MIL_CONST_TEXT_PTR FileName,
                                       MIL_INT Operation,
                                       MIL_ID FontId);

MFTYPE32 void     MFTYPE MocrSetConstraintW(MIL_ID FontId,
                                            MIL_INT CharPos,
                                            MIL_INT CharPosType,
                                            MIL_CONST_TEXT_PTR CharValidString);

MFTYPE32 void     MFTYPE MocrLineSetConstraintW(MIL_ID FontId,
                                                MIL_INT LineIndex,
                                                MIL_INT CharPos,
                                                MIL_INT CharPosType,
                                                MIL_CONST_TEXT_PTR CharValid);

MFTYPE32 void     MFTYPE MocrStreamW(MIL_TEXT_PTR MemPtrOrFileName ,
                                     MIL_ID       SystemId         ,
                                     MIL_INT      Operation        , 
                                     MIL_INT      StreamType       ,
                                     double       Version          ,
                                     MIL_INT      ControlFlag      ,
                                     MIL_ID       *ObjectIdPtr     ,
                                     MIL_INT      *SizeByteVarPtr  );

MFTYPE32 void     MFTYPE MocrVerifyStringW(MIL_ID ImageId,
                                           MIL_ID FontId,
                                           MIL_CONST_TEXT_PTR String,
                                           MIL_ID ResultId);

MFTYPE32 MIL_INT  MFTYPE MocrValidateStringW(MIL_ID FontId,
                                             MIL_INT Mode,
                                             MIL_CONST_TEXT_PTR String);

MFTYPE32 void     MFTYPE MocrDrawW(MIL_ID         GraphContId,
                                   MIL_ID         ContextOrResultId,
                                   MIL_ID         DestImageBufId,
                                   MIL_INT        Operation,
                                   MIL_INT        Index,
                                   MIL_CONST_TEXT_PTR   CharList,
                                   MIL_INT        ControlFlag);

MFTYPE32 void     MFTYPE MocrCalibrateFontA(MIL_ID CalibrationImageId,
                                            MIL_ID FontId,
                                            const char *String,
                                            double TargetCharSizeXMin,
                                            double TargetCharSizeXMax,
                                            double TargetCharSizeXStep,
                                            double TargetCharSizeYMin,
                                            double TargetCharSizeYmax,
                                            double TargetCharSizeYStep,
                                            MIL_INT Operation);

MFTYPE32 void     MFTYPE MocrCopyFontA(MIL_ID ImageId,
                                       MIL_ID FontId,
                                       MIL_INT Operation,
                                       const char *CharListString);

MFTYPE32 void     MFTYPE MocrImportFontA(const char* FileName,
                                         MIL_INT FileFormat,
                                         MIL_INT Operation,
                                         const char *CharListString,
                                         MIL_ID FontId);

MFTYPE32 MIL_ID   MFTYPE MocrRestoreFontA(const char* FileName,
                                          MIL_INT Operation,
                                          MIL_ID SystemId,
                                          MIL_ID *FontId);

MFTYPE32 void     MFTYPE MocrSaveFontA(const char* FileName,
                                       MIL_INT Operation,
                                       MIL_ID FontId);

MFTYPE32 void     MFTYPE MocrSetConstraintA(MIL_ID FontId,
                                            MIL_INT CharPos,
                                            MIL_INT CharPosType,
                                            const char *CharValidString);

MFTYPE32 void     MFTYPE MocrLineSetConstraintA(MIL_ID FontId,
                                                MIL_INT LineIndex,
                                                MIL_INT CharPos,
                                                MIL_INT CharPosType,
                                                const char* CharValid);

MFTYPE32 void     MFTYPE MocrStreamA(char*        MemPtrOrFileName ,
                                     MIL_ID       SystemId         ,
                                     MIL_INT      Operation        , 
                                     MIL_INT      StreamType       ,
                                     double       Version          ,
                                     MIL_INT      ControlFlag      ,
                                     MIL_ID       *ObjectIdPtr     ,
                                     MIL_INT      *SizeByteVarPtr  );

MFTYPE32 void     MFTYPE MocrVerifyStringA(MIL_ID ImageId,
                                           MIL_ID FontId,
                                           const char *String,
                                           MIL_ID ResultId);

MFTYPE32 MIL_INT  MFTYPE MocrValidateStringA(MIL_ID FontId,
                                             MIL_INT Mode,
                                             const char *String);

MFTYPE32 void     MFTYPE MocrDrawA(MIL_ID         GraphContId,
                                   MIL_ID         ContextOrResultId,
                                   MIL_ID         DestImageBufId,
                                   MIL_INT        Operation,
                                   MIL_INT        Index,
                                   const char*          CharList,
                                   MIL_INT        ControlFlag);

#if M_MIL_UNICODE_API

#define MocrCalibrateFont     MocrCalibrateFontW
#define MocrCopyFont          MocrCopyFontW
#define MocrImportFont        MocrImportFontW
#define MocrSaveFont          MocrSaveFontW
#define MocrSetConstraint     MocrSetConstraintW
#define MocrLineSetConstraint MocrLineSetConstraintW
#define MocrRestoreFont       MocrRestoreFontW
#define MocrStream            MocrStreamW
#define MocrValidateString    MocrValidateStringW
#define MocrVerifyString      MocrVerifyStringW
#define MocrDraw              MocrDrawW

#else

#define MocrCalibrateFont     MocrCalibrateFontA
#define MocrCopyFont          MocrCopyFontA
#define MocrImportFont        MocrImportFontA
#define MocrSaveFont          MocrSaveFontA
#define MocrSetConstraint     MocrSetConstraintA
#define MocrLineSetConstraint MocrLineSetConstraintA
#define MocrRestoreFont       MocrRestoreFontA
#define MocrStream            MocrStreamA
#define MocrValidateString    MocrValidateStringA
#define MocrVerifyString      MocrVerifyStringA
#define MocrDraw              MocrDrawA

#endif

#else

MFTYPE32 void    MFTYPE MocrCalibrateFont(MIL_ID CalibrationImageId,
                                          MIL_ID FontId,
                                          MIL_CONST_TEXT_PTR String,
                                          double TargetCharSizeXMin,
                                          double TargetCharSizeXMax,
                                          double TargetCharSizeXStep,
                                          double TargetCharSizeYMin,
                                          double TargetCharSizeYmax,
                                          double TargetCharSizeYStep,
                                          MIL_INT Operation);

MFTYPE32 void    MFTYPE MocrCopyFont(MIL_ID ImageId,
                                     MIL_ID FontId,
                                     MIL_INT Operation,
                                     MIL_CONST_TEXT_PTR CharListString);

MFTYPE32 void    MFTYPE MocrImportFont(MIL_CONST_TEXT_PTR FileName,
                                       MIL_INT FileFormat,
                                       MIL_INT Operation,
                                       MIL_CONST_TEXT_PTR CharListString,
                                       MIL_ID FontId);

MFTYPE32 MIL_ID  MFTYPE MocrRestoreFont(MIL_CONST_TEXT_PTR FileName,
                                        MIL_INT Operation,
                                        MIL_ID SystemId,
                                        MIL_ID *FontId);

MFTYPE32 void    MFTYPE MocrSaveFont(MIL_CONST_TEXT_PTR FileName,
                                     MIL_INT Operation,
                                     MIL_ID FontId);

MFTYPE32 void    MFTYPE MocrSetConstraint(MIL_ID FontId,
                                          MIL_INT CharPos,
                                          MIL_INT CharPosType,
                                          MIL_CONST_TEXT_PTR CharValidString);

MFTYPE32 void    MFTYPE MocrLineSetConstraint(MIL_ID FontId,
                                              MIL_INT LineIndex,
                                              MIL_INT CharPos,
                                              MIL_INT CharPosType,
                                              MIL_CONST_TEXT_PTR CharValid);

MFTYPE32 void MFTYPE MocrStream(MIL_TEXT_PTR MemPtrOrFileName ,
                                MIL_ID       SystemId         ,
                                MIL_INT      Operation        , 
                                MIL_INT      StreamType       ,
                                double       Version          ,   
                                MIL_INT      ControlFlag      ,
                                MIL_ID       *ObjectIdPtr     ,
                                MIL_INT      *SizeByteVarPtr  );

MFTYPE32 void    MFTYPE MocrVerifyString( MIL_ID ImageId,
                                         MIL_ID FontId,
                                         MIL_CONST_TEXT_PTR String,
                                         MIL_ID ResultId);

MFTYPE32 MIL_INT    MFTYPE MocrValidateString(MIL_ID FontId,
                                              MIL_INT Mode,
                                              MIL_CONST_TEXT_PTR String);

MFTYPE32 void     MFTYPE MocrDraw(MIL_ID         GraphContId,
                                  MIL_ID         ContextOrResultId,
                                  MIL_ID         DestImageBufId,
                                  MIL_INT        Operation,
                                  MIL_INT        Index,
                                  MIL_CONST_TEXT_PTR   CharList,
                                  MIL_INT        ControlFlag);
#endif

/* C++ directive if needed */
#ifdef __cplusplus
}
#endif


#endif /* #ifdef __midl */

// OCR control defaults
#define M_DEF_STRING_LOC_MAX_NB_ITER           2L
#define M_DEF_STRING_LOC_STOP_ITER           0.50
#define M_DEF_STRING_LOC_GOOD_NB_CHAR          4L
#define M_DEF_STRING_READ_BAD_SIZE_X         0.40
#define M_DEF_STRING_READ_BAD_SIZE_Y         0.40
#define M_DEF_STRING_READ_GOOD_SIZE_X        0.25
#define M_DEF_STRING_READ_GOOD_SIZE_Y        0.25
#define M_DEF_STRING_READ_BAD_ADD_CHAR         4L
#define M_DEF_STRING_LOC_MIN_CHAR_SIZE       0.66
#define M_DEF_STRING_LOC_MAX_CHAR_SIZE       1.50
#define M_DEF_STRING_LOC_MIN_CHAR_SPACE      0.66
#define M_DEF_STRING_LOC_MAX_CHAR_DISTANCE   0.50
#define M_DEF_STRING_LOC_GOOD_CHAR_SIZE      0.90
#define M_DEF_STRING_MAX_SLOPE               0.1763269
#define M_DEF_STRING_READ_SIZE_X             0.33
#define M_DEF_STRING_READ_SIZE_Y             0.25
#define M_DEF_SKIP_SEARCH               M_DISABLE
#define M_DEF_SKIP_STRING_LOCATION      M_DISABLE
#define M_DEF_SKIP_CONTRAST_ENHANCE     M_DISABLE
#define M_DEF_STRING_ACCEPTANCE              1.00
#define M_DEF_CHAR_ACCEPTANCE                1.00
#define M_DEF_CHAR_INVALID                      0
#define M_DEF_DEBUG                             0
#define M_DEF_ENHANCE_MORPHO                   2L
#define M_DEF_KILL_BORDER                M_ENABLE
#define M_DEF_READ_SPEED                 M_MEDIUM
#define M_DEF_READ_ACCURACY              M_MEDIUM
#define M_DEF_READ_FIRST_LEVEL          M_DEFAULT
#define M_DEF_READ_LAST_LEVEL           M_DEFAULT
#define M_DEF_READ_MODEL_STEP           M_DEFAULT
#define M_DEF_READ_FAST_FIND            M_DEFAULT
#define M_DEF_READ_ROBUSTNESS            M_MEDIUM
#define M_DEF_STRING_LOC_NB_MODELS             2L
#define M_DEF_PAT_ON_ACCELERATED        M_DISABLE
#define M_DEF_BLOB_ON_ACCELERATED       M_DISABLE
#define M_DEF_PROC_ON_ACCELERATED        M_ENABLE
#define M_DEF_ENHANCE_DOT_CHARS                0L  //default 0 disabled
#define M_DEF_ADD_SEARCH_AREA_X               0.0
#define M_DEF_ADD_SEARCH_AREA_Y               0.0
#define M_DEF_EXTRA_CHARACTERS          M_DISABLE

// Control associated InfoType defines
#define M_STRING_LOC_CHAR_SIZE_X               1L
#define M_STRING_LOC_CHAR_SIZE_Y               2L
#define M_STRING_LOC_MAX_NB_ITER               3L
#define M_FOREGROUND_VALUE                     4L
#define M_STRING_LOC_STOP_ITER                 5L
#define M_STRING_LOC_GOOD_NB_CHAR              6L
#define M_STRING_READ_BAD_SIZE_X               7L
#define M_SPEED					                 8L  // Already define in milpat.h
#define M_STRING_READ_GOOD_SIZE_X              9L
#define M_STRING_READ_GOOD_SIZE_Y             10L
#define M_STRING_READ_BAD_ADD_CHAR            11L
#define M_STRING_LOC_MIN_CHAR_SIZE            12L
#define M_STRING_LOC_MAX_CHAR_SIZE            13L
#define M_STRING_LOC_MIN_CHAR_SPACE           64L
#define M_STRING_LOC_MAX_CHAR_DISTANCE        15L
#define M_STRING_LOC_GOOD_CHAR_SIZE           16L
#define M_STRING_MAX_SLOPE                    17L
#define M_STRING_READ_SIZE_X                  18L
#define M_STRING_READ_SIZE_Y                  19L
#define M_SKIP_SEARCH                         21L
#define M_SKIP_STRING_LOCATION                22L
#define M_SKIP_CONTRAST_ENHANCE               23L
#define M_SKIP_DEFAULT_CONTRAST_ENHANCE       23L 
#define M_STRING_ACCEPTANCE                   24L
#define M_CHAR_ACCEPTANCE                     25L
#define M_CHAR_INVALID                        26L
#define M_TARGET_CHAR_SIZE_X                  27L
#define M_TARGET_CHAR_SIZE_Y                  28L
#define M_TARGET_CHAR_SPACING                 29L
#define M_DEBUG                               30L
#define M_FONT_TYPE                           31L
#define M_CHAR_NUMBER                         32L
#define M_CHAR_BOX_SIZE_X                     33L
#define M_CHAR_CELL_SIZE_X                    33L  
#define M_CHAR_BOX_SIZE_Y                     34L                        
#define M_CHAR_CELL_SIZE_Y                    34L  
#define M_CHAR_OFFSET_X                       35L
#define M_CHAR_OFFSET_Y                       36L
#define M_CHAR_SIZE_X                         37L
#define M_CHAR_SIZE_Y                         38L
#define M_CHAR_THICKNESS                      39L
#define M_STRING_LENGTH               0x00000028L // Bitwise-Combined with M_ESCAPE_SEQUENCE in Milcode 
#define M_STRING_SIZE                         M_STRING_LENGTH
#define M_FONT_INIT_FLAG                      41L
#define M_CHAR_IN_FONT                        42L
#define M_KILL_BORDER                         44L
#define M_CHAR_ERASE                          45L
#define M_MODEL_LIST                          46L
#define M_CHAR_NUMBER_IN_FONT                 47L
#define M_STRING_VALIDATION                   48L
#define M_STRING_VALIDATION_HANDLER_PTR       M_STRING_VALIDATION
#define M_STRING_VALIDATION_HANDLER_USER_PTR  49L
#define M_READ_SPEED                          50L
#define M_READ_ACCURACY                       51L
#define M_READ_FIRST_LEVEL                    52L
#define M_READ_LAST_LEVEL                     53L
#define M_READ_FAST_FIND                      55L
#define M_READ_ROBUSTNESS                     56L
#define M_STRING_LOC_NB_MODELS                57L
#define M_READ_MODEL_STEP                     58L
#define M_PAT_ON_ACCELERATED                  59L
#define M_BLOB_ON_ACCELERATED                 60L
#define M_PROC_ON_ACCELERATED                 61L
#define M_STRING_LENGTH_MAX                   M_STRING_SIZE_MAX   
#define M_STRING_SIZE_MAX                     62L
#define M_STRING_READ_BAD_SIZE_Y              63L
//64 to 72 reserved
#define M_BLANK_CHARACTERS                    73L   
#define M_NBR_OF_LINES                        74L
#define M_AUTO                                444L  
#define M_OCR_THRESHOLD                       82    
#define M_THRESHOLD                           82    
#define M_ENHANCE_MORPHO                      43L 
#define M_MORPHOLOGIC_FILTERING               43L 
#define M_MORPHOLOGIC_ENHANCEMENT             43L
#define M_ENHANCE_DOT_CHARS                   84L 
#define M_ENLARGE_CHAR_THICKNESS              84L 
#define M_THICKEN_CHAR                        84L  
#define M_NBR_OF_LINES_READ                   M_STRING_NUMBER
#define M_NB_STRING                           M_STRING_NUMBER
#define M_STRING_NUMBER                       74L
#define M_NBR_OF_LINES_FOUND                  M_STRING_NUMBER_FOUND
#define M_NB_STRING_FOUND                     M_STRING_NUMBER_FOUND
#define M_STRING_NUMBER_FOUND                 86L 
#define M_STRING_ANGLE                        87L
#define M_SKEW_INTERPOLATION_MODE             88L
#define M_STRING_ANGLE_INTERPOLATION_MODE     88L  
#define M_CHAR_POSITION_VARIATION_X           91L
#define M_ADD_SEARCH_AREA_X                   M_CHAR_POSITION_VARIATION_X // obsolete
#define M_CHAR_POSITION_TOLERANCE_X           M_CHAR_POSITION_VARIATION_X // obsolete
#define M_TARGET_CHAR_POSITION_VARIATION_X    M_CHAR_POSITION_VARIATION_X // obsolete
#define M_CHAR_POSITION_VARIATION_Y           92L  
#define M_ADD_SEARCH_AREA_Y                   M_CHAR_POSITION_VARIATION_Y // obsolete
#define M_CHAR_POSITION_TOLERANCE_Y           M_CHAR_POSITION_VARIATION_Y // obsolete
#define M_TARGET_CHAR_POSITION_VARIATION_Y    M_CHAR_POSITION_VARIATION_Y // obsolete  
#define M_READ_CERTAINTY                      98L    
#define M_TARGET_CHAR_SIZE_X_MIN              99L
#define M_TARGET_CHAR_SIZE_X_MAX              100L
#define M_TARGET_CHAR_SIZE_Y_MIN              101L
#define M_TARGET_CHAR_SIZE_Y_MAX              102L
#define M_CONTEXT                      0x08000000L //already defined in Milmod.h
#define M_METHOD                              103L //To be used instead of M_CONTEXT for 7.5
#define M_READ_BROKEN_CHAR                    104L
#define M_BROKEN_CHAR                         104L
#define M_READ_TOUCHING_CHAR                  105L
#define M_TOUCHING_CHAR                       105L
#define M_STRING_ANGLE_DELTA_NEG              106L
#define M_STRING_ANGLE_DELTA_POS              107L
#define M_LINE_CHAR_SEPARATOR                 109L
#define M_TEXT_STRING_SEPARATOR               109L
#define M_CHAR_NORMAL_SIZE_X                  110L
#define M_CHAR_NORMAL_SIZE_Y                  111L
#define M_CERTAINTY                           202L //already defined in Milmod.h
#define M_CONTEXT_CONVERT                     535L
#define M_METHOD_CONVERT                      M_CONTEXT_CONVERT
#define M_METHOD_CONVERT_OLD                  108L

#define M_STRING_ANGLE_SPEED                  112L
#define M_STRING_ANGLE_SCORE                  113L
#define M_TIMEOUT                            2077L // already define in mil.h
#define M_STRING_ANGLE_SMOOTHNESS             114L // 
#define M_RESULT_OUTPUT_UNITS                1300L // Also in milim.h

#define M_EXTRA_CHARACTERS                    115L

#define M_CONSTRAINED                  0x40000000L //method
#define M_GENERAL                      0x20000000L //method
#define M_FLEXIBLE                     0x20000000L //method
#define M_FUTUR_METHODE1                        3L
#define M_FUTUR_METHODE2                        4L

#define M_ANY                          0x11000000L
#define M_SAME                                 -1L

#define M_OCR_PREPROCESSED                     14L
#define M_PREPROCESSED                         14L  // Already defined in MilPat.h

#define M_CONSTRAINT                   0x04000000L

#define M_CONSTRAINT_TYPE             0x00008000L
#define M_CONSTRAINT_TYPE_OLD          0x8000000L

#define M_PRINT_ENABLE                         1L
#define M_DISPLAY_ENABLE                       2L
#define M_BENCHMARK_ENABLE                     4L

// #MIL_INTr#
#define M_DSAVE_IMGS                   0x0001000L
#define M_DSAVE_INFO                   0x0002000L 
#define M_DFSAS_READ                   0x0003000L
#define M_DOCR_CALIB                   0x0004000L
#define M_DSKEW                        0x0005000L
#define M_DMULTIPLE_LINES              0x0006000L
#define M_DCHAR_SEG                    0x0007000L
#define M_DMATCH                       0x0008000L
#define M_DDRAW_EDGE_MODEL             0x0009000L

// MocrControl result mode
#define M_SELECT_LINE                      0x100L
#define M_SELECT_STRING                    M_SELECT_LINE

// MocrControl (M_SKIP_STRING_LOCATION possible values)
#define M_STRING_LOCATION_BLOB_ONLY            2L            
#define M_STRING_LOCATION_SEARCH_ONLY          3L            
#define M_STRING_LOCATION_BLOB_THAN_SEARCH     4L            
#define M_STRING_LOCATION_SEARCH_THAN_BLOB     5L            

// MocrControl (M_SKIP_STRING_LOCATION possible values)
#ifdef __midl // MIDL compiler used by ActiveMIL
#define M_VERY_LOW        0L
#define M_LOW             1L
#define M_MEDIUM          2L
#define M_HIGH            3L
#define M_VERY_HIGH       4L
#endif

// Parameters for output units
#define M_PIXEL                              0x1000L
#define M_WORLD                              0x2000L
#define M_ACCORDING_TO_CALIBRATION             1301L
#define M_ACCORDING_TO_RESULT                  1302L

// MocrAllocFont()
#define M_DEFAULT                     0x10000000L //M_USER_DEFINED+M_CONSTRAINED
#define M_SEMI_M12_92                        0x1L
#define M_SEMI_M13_88                        0x2L
#define M_SEMI                               0x3L
#define M_USER_DEFINED                        21L //already defined in Mil.h
#define M_FOREGROUND_WHITE                  0x80L  
#define M_FOREGROUND_BLACK                 0x100L  
#define M_CONSTRAINED                 0x40000000L //method
#define M_GENERAL                     0x20000000L //method
#define M_FLEXIBLE                    0x20000000L //method


// MocrCalibrateFont()
#define M_DEFAULT                     0x10000000L
#ifdef  M_MIL_75_COMPATIBLE               //TBR MIL 8.0
#define M_FAST                        0x00040000L
#else
#define M_FAST                        0x00002000L
#endif

// MocrImportFont(), MocrRestoreFont(), MocrSaveFont()
#ifdef M_MIL_75_COMPATIBLE
#define M_RESTORE                              0L
#else
#define M_RESTORE                           0x10L
#endif // M_MIL_75_COMPATIBLE

#define M_LOAD                                 1L
#define M_LOAD_CONSTRAINT                    0x2L
#define M_LOAD_CONTROL                       0x4L
#define M_LOAD_CHARACTER                     0x8L
#define M_SAVE                             0x100L
#define M_SAVE_CONSTRAINT                  0x200L
#define M_SAVE_CONTROL                     0x400L
#define M_SAVE_CHARACTER                   0x800L
#define M_REPLACE                     0x01000060L
#define M_MIL                                  1L
#define M_RAW                                  2L
#define M_TIFF                                 4L
#define M_FONT_MIL                    0x00008000L
#define M_FONT_ASCII                  0x00004000L
#define M_MIL_EXTENSIONS              MT("*.mim") // Already defined in Mil.h
#define M_TIFF_EXTENSIONS      MT("*.tif;*.tiff") // Already defined in Mil.h
#define M_INTERACTIVE                      M_NULL // Already defined in mil.h, milcal.h, milcode.h, miledge.h, milmeas.h, milpat.h, milmod.h

// MocrGetResult()                            
#define M_STRING_VALID_FLAG                    1L
#define M_STRING_SCORE                         0x00001400L
#define M_STRING                               3L    
#define M_CHAR_VALID_FLAG                      4L
#define M_CHAR_SCORE                           5L
#define M_CHAR_POSITION_X                      6L
#define M_CHAR_POSITION_Y                      7L
#define M_GOOD_LOCATION_QUALITY_FLAG           8L
#define M_CHAR_SIZE_SCORE                      9L
#define M_CHAR_MIN_OFFSET_X                   10L
#define M_CHAR_MAX_OFFSET_X                   11L
#define M_CHAR_MIN_OFFSET_Y                   12L
#define M_CHAR_MAX_OFFSET_Y                   13L
#define M_CHAR_SPACING                        14L 
#define M_DOCUMENT_SCORE                      15L 
#define M_TEXT_SCORE                          15L 
#define M_TEXT                                16L 
#define M_TEXT_LENGTH                         17L 
#define M_STRING_ALLOC_SIZE                   18L

#define M_NB_STRING                           M_STRING_NUMBER
#define M_STRING_NUMBER                       74L 
#define M_NB_STRING_FOUND                     M_STRING_NUMBER_FOUND
#define M_STRING_NUMBER_FOUND                 86L

#define M_CHAR_SIZE_X                         37L //already defined in control
#define M_CHAR_SIZE_Y                         38L //already defined in control

#define M_SUM                         0x00004000L
#define M_MEAN                        0x08000000L //already defined in MILMEAS.H 
#define M_MEDIAN                      0x10000L    //already defined in MIL.H 

// MocrCopyFont()
#define M_COPY_TO_FONT                         1L
#define M_COPY_FROM_FONT                       2L
#define M_CHARACTER_PAT_MODEL            0x10000L
#define M_ALL_CHAR                        0x8000L
#define M_SKIP_SEMI_STRING_UPDATE         0x4000L
#define M_AUTO_LOCALIZE                   0x1000L

#define M_SORT                        0x00200000L

// MocrSetConstraint()
#define M_ADD                             0x0000L
#define M_CLEAR                       0x00000001L
#define M_LETTER                      0x00020000L
#define M_DIGIT                       0x00040000L
#define M_UPPERCASE                   0x00010000L
#define M_LOWERCASE                   0x00008000L
#define M_LETTER_OLD                      0x0002L
#define M_DIGIT_OLD                       0x0003L
#define M_UPPERCASE_OLD                  0x10000L
#define M_LOWERCASE_OLD                   0x8000L
#define M_BLANK                         0x200000L
#define M_DEFAULT                     0x10000000L                      
#ifdef __midl // MIDL compiler used by ActiveMIL
#define M_ANY                         0x11000000L
#endif

// MocrModifyFont()
#define M_RESIZE                               1L
#define M_INVERT                               2L
#define M_NORMALIZE_FONT                       4L

// MocrValidateString()
#define M_TRUE                                 1L
#define M_STRING_LENGTH               0x00000028L // Bitwise-Combined with M_ESCAPE_SEQUENCE in Milcode 
#define M_STRING_SIZE                         M_STRING_LENGTH
#define M_ALL                         0x40000000L // Already defined in Mil.h
#define M_ALL_OLD                              0L
#define M_PRESENT                              2L
#define M_CHECK_VALID                          3L
#define M_CHECK_VALID_FAST                     4L

// MocrDraw
#define M_DRAW_STRING_BOX             0x00000004L
#define M_DRAW_STRING_CHAR_BOX        0x00000008L
#define M_DRAW_STRING_CHAR_POSITION   0x00000010L

#define M_DRAW_CHAR                          907L //Operation for context


#if M_MIL_USE_64BIT
#ifdef __cplusplus
//////////////////////////////////////////////////////////////
// MocrControl function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_MIL_USE_LINUX
inline void MocrControl(MIL_ID FontId,
                        MIL_INT ControlToSet,
                        int Value)
   {
   MocrControlInt64(FontId, ControlToSet, Value);
   };
#endif

inline void MocrControl(MIL_ID FontId,
                        MIL_INT ControlToSet,
                        MIL_INT32 Value)
   {
   MocrControlInt64(FontId, ControlToSet, Value);
   };

inline void MocrControl(MIL_ID FontId,
                        MIL_INT ControlToSet,
                        MIL_INT64 Value)
   {
   MocrControlInt64(FontId, ControlToSet, Value);
   };

inline void MocrControl(MIL_ID FontId,
                        MIL_INT ControlToSet,
                        double Value)
   {
   MocrControlDouble(FontId, ControlToSet, Value);
   };
#else
//////////////////////////////////////////////////////////////
// For C file, call the default function, i.e. Int64 one
//////////////////////////////////////////////////////////////
#define MocrControl  MocrControlDouble

#endif // __cplusplus
#endif // M_MIL_USE_64BIT

#if M_MIL_USE_SAFE_TYPE

//////////////////////////////////////////////////////////////
// See milos.h for explanation about these functions.
//////////////////////////////////////////////////////////////

// ----------------------------------------------------------
// MocrInquire

inline MFTYPE32 MIL_INT MFTYPE MocrInquireUnsafe  (MIL_ID ContextId, MIL_INT InquireType, void          MPTYPE *UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE MocrInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, int                   UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE MocrInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_INT8      MPTYPE *UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE MocrInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_INT16     MPTYPE *UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE MocrInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_INT32     MPTYPE *UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE MocrInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_INT64     MPTYPE *UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE MocrInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, float         MPTYPE *UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE MocrInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_DOUBLE    MPTYPE *UserVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 MIL_INT MFTYPE MocrInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_UINT8     MPTYPE *UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE MocrInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_UINT16    MPTYPE *UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE MocrInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_UINT32    MPTYPE *UserVarPtr);
inline MFTYPE32 MIL_INT MFTYPE MocrInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_UINT64    MPTYPE *UserVarPtr);
#endif
#if M_MIL_SAFE_TYPE_ADD_WCHAR_T
inline MFTYPE32 MIL_INT MFTYPE MocrInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, wchar_t       MPTYPE *UserVarPtr);
#endif

// ----------------------------------------------------------
// MocrGetResult

inline MFTYPE32 void MFTYPE MocrGetResultUnsafe  (MIL_ID ResultId, MIL_INT ResultType, void          MPTYPE *ResultArrayPtr);
inline MFTYPE32 void MFTYPE MocrGetResultSafeType(MIL_ID ResultId, MIL_INT ResultType, int                   ResultArrayPtr);
inline MFTYPE32 void MFTYPE MocrGetResultSafeType(MIL_ID ResultId, MIL_INT ResultType, MIL_INT8      MPTYPE *ResultArrayPtr);
inline MFTYPE32 void MFTYPE MocrGetResultSafeType(MIL_ID ResultId, MIL_INT ResultType, MIL_INT16     MPTYPE *ResultArrayPtr);
inline MFTYPE32 void MFTYPE MocrGetResultSafeType(MIL_ID ResultId, MIL_INT ResultType, MIL_INT32     MPTYPE *ResultArrayPtr);
inline MFTYPE32 void MFTYPE MocrGetResultSafeType(MIL_ID ResultId, MIL_INT ResultType, MIL_INT64     MPTYPE *ResultArrayPtr);
inline MFTYPE32 void MFTYPE MocrGetResultSafeType(MIL_ID ResultId, MIL_INT ResultType, float         MPTYPE *ResultArrayPtr);
inline MFTYPE32 void MFTYPE MocrGetResultSafeType(MIL_ID ResultId, MIL_INT ResultType, MIL_DOUBLE    MPTYPE *ResultArrayPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 void MFTYPE MocrGetResultSafeType(MIL_ID ResultId, MIL_INT ResultType, MIL_UINT8     MPTYPE *ResultArrayPtr);
inline MFTYPE32 void MFTYPE MocrGetResultSafeType(MIL_ID ResultId, MIL_INT ResultType, MIL_UINT16    MPTYPE *ResultArrayPtr);
inline MFTYPE32 void MFTYPE MocrGetResultSafeType(MIL_ID ResultId, MIL_INT ResultType, MIL_UINT32    MPTYPE *ResultArrayPtr);
inline MFTYPE32 void MFTYPE MocrGetResultSafeType(MIL_ID ResultId, MIL_INT ResultType, MIL_UINT64    MPTYPE *ResultArrayPtr);
#endif
#if M_MIL_SAFE_TYPE_ADD_WCHAR_T
inline MFTYPE32 void MFTYPE MocrGetResultSafeType(MIL_ID ResultId, MIL_INT ResultType, wchar_t       MPTYPE *ResultArrayPtr);
#endif

// ----------------------------------------------------------
// MocrInquire

inline MIL_INT MocrInquireRequiredType(MIL_INT InquireType)
   {
   if ((InquireType & M_CONSTRAINT) == M_CONSTRAINT)
      return M_TYPE_TEXT_CHAR;

   switch (InquireType)
      {
      case M_CHAR_IN_FONT:
         return M_TYPE_TEXT_CHAR;
         break;

      default:
         return M_TYPE_DOUBLE;
         break;
      }
   }

inline MFTYPE32 MIL_INT MFTYPE MocrInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, int UserVarPtr)
   {
   if (UserVarPtr)
      SafeTypeError(MT("MocrInquire"));

   return MocrInquire(ContextId, InquireType, NULL);
   }

inline MFTYPE32 MIL_INT MFTYPE MocrInquireExecute (MIL_ID ContextId, MIL_INT InquireType, void       MPTYPE *ValuePtr, MIL_INT GivenType)
   {
   MIL_INT RequiredType = (InquireType & (M_TYPE_MIL_INT32 | M_TYPE_MIL_INT64 | M_TYPE_DOUBLE |M_TYPE_CHAR | M_TYPE_SHORT | M_TYPE_MIL_ID | M_TYPE_FLOAT));
   if (RequiredType == 0)
      RequiredType = MocrInquireRequiredType(InquireType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MT("MocrInquire"));

   return MocrInquire(ContextId, InquireType, ValuePtr);
   }

inline MFTYPE32 MIL_INT MFTYPE MocrInquireUnsafe  (MIL_ID ContextId, MIL_INT InquireType, void       MPTYPE *ValuePtr) {return MocrInquire       (ContextId, InquireType, ValuePtr                  );}
inline MFTYPE32 MIL_INT MFTYPE MocrInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_INT8   MPTYPE *ValuePtr) {return MocrInquireExecute(ContextId, InquireType, ValuePtr, M_TYPE_CHAR);}
inline MFTYPE32 MIL_INT MFTYPE MocrInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_INT16  MPTYPE *ValuePtr) {return MocrInquireExecute(ContextId, InquireType, ValuePtr, M_TYPE_SHORT);}
inline MFTYPE32 MIL_INT MFTYPE MocrInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_INT32  MPTYPE *ValuePtr) {return MocrInquireExecute(ContextId, InquireType, ValuePtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 MIL_INT MFTYPE MocrInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_INT64  MPTYPE *ValuePtr) {return MocrInquireExecute(ContextId, InquireType, ValuePtr, M_TYPE_MIL_INT64);}
inline MFTYPE32 MIL_INT MFTYPE MocrInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, float      MPTYPE *ValuePtr) {return MocrInquireExecute(ContextId, InquireType, ValuePtr, M_TYPE_FLOAT);}
inline MFTYPE32 MIL_INT MFTYPE MocrInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_DOUBLE MPTYPE *ValuePtr) {return MocrInquireExecute(ContextId, InquireType, ValuePtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 MIL_INT MFTYPE MocrInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_UINT8  MPTYPE *ValuePtr) {return MocrInquireExecute(ContextId, InquireType, ValuePtr, M_TYPE_CHAR);}
inline MFTYPE32 MIL_INT MFTYPE MocrInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_UINT16 MPTYPE *ValuePtr) {return MocrInquireExecute(ContextId, InquireType, ValuePtr, M_TYPE_SHORT);}
inline MFTYPE32 MIL_INT MFTYPE MocrInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_UINT32 MPTYPE *ValuePtr) {return MocrInquireExecute(ContextId, InquireType, ValuePtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 MIL_INT MFTYPE MocrInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, MIL_UINT64 MPTYPE *ValuePtr) {return MocrInquireExecute(ContextId, InquireType, ValuePtr, M_TYPE_MIL_INT64);}
#endif
#if M_MIL_SAFE_TYPE_ADD_WCHAR_T
inline MFTYPE32 MIL_INT MFTYPE MocrInquireSafeType(MIL_ID ContextId, MIL_INT InquireType, wchar_t    MPTYPE *ValuePtr) {return MocrInquireExecute(ContextId, InquireType, ValuePtr, M_TYPE_SHORT);}
#endif

// ----------------------------------------------------------
// MocrGetResult

inline MIL_INT MocrGetResultRequiredType(MIL_INT ResultType)
   {
   switch (ResultType)
      {
      case M_TEXT  :
      case M_STRING:
         return M_TYPE_TEXT_CHAR;
         break;

      default:
         return M_TYPE_DOUBLE;
         break;
      }
   }

inline MFTYPE32 void MFTYPE MocrGetResultSafeType (MIL_ID ResultId, MIL_INT ResultType, int ValuePtr)
   {
   if (ValuePtr)
      SafeTypeError(MT("MocrGetResult"));

   MocrGetResult(ResultId, ResultType, NULL);
   }

inline void MocrGetResultExecute (MIL_ID ResultId, MIL_INT ResultType, void       MPTYPE *ValuePtr, MIL_INT GivenType)
   {
   MIL_INT RequiredType = (ResultType & (M_TYPE_MIL_INT32 | M_TYPE_MIL_INT64 | M_TYPE_DOUBLE |M_TYPE_CHAR | M_TYPE_SHORT | M_TYPE_FLOAT | M_TYPE_MIL_ID));
   if (RequiredType == 0)
      RequiredType = MocrGetResultRequiredType(ResultType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MT("MocrGetResult"));

   MocrGetResult(ResultId, ResultType, ValuePtr);
   }

inline MFTYPE32 void MFTYPE MocrGetResultUnsafe  (MIL_ID ResultId, MIL_INT ResultType, void       MPTYPE *ValuePtr) {MocrGetResult       (ResultId, ResultType, ValuePtr                  );}
inline MFTYPE32 void MFTYPE MocrGetResultSafeType(MIL_ID ResultId, MIL_INT ResultType, MIL_INT8   MPTYPE *ValuePtr) {MocrGetResultExecute(ResultId, ResultType, ValuePtr, M_TYPE_CHAR);}
inline MFTYPE32 void MFTYPE MocrGetResultSafeType(MIL_ID ResultId, MIL_INT ResultType, MIL_INT16  MPTYPE *ValuePtr) {MocrGetResultExecute(ResultId, ResultType, ValuePtr, M_TYPE_SHORT);}
inline MFTYPE32 void MFTYPE MocrGetResultSafeType(MIL_ID ResultId, MIL_INT ResultType, MIL_INT32  MPTYPE *ValuePtr) {MocrGetResultExecute(ResultId, ResultType, ValuePtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 void MFTYPE MocrGetResultSafeType(MIL_ID ResultId, MIL_INT ResultType, MIL_INT64  MPTYPE *ValuePtr) {MocrGetResultExecute(ResultId, ResultType, ValuePtr, M_TYPE_MIL_INT64);}
inline MFTYPE32 void MFTYPE MocrGetResultSafeType(MIL_ID ResultId, MIL_INT ResultType, float      MPTYPE *ValuePtr) {MocrGetResultExecute(ResultId, ResultType, ValuePtr, M_TYPE_FLOAT);}
inline MFTYPE32 void MFTYPE MocrGetResultSafeType(MIL_ID ResultId, MIL_INT ResultType, MIL_DOUBLE MPTYPE *ValuePtr) {MocrGetResultExecute(ResultId, ResultType, ValuePtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 void MFTYPE MocrGetResultSafeType(MIL_ID ResultId, MIL_INT ResultType, MIL_UINT8  MPTYPE *ValuePtr) {MocrGetResultExecute(ResultId, ResultType, ValuePtr, M_TYPE_CHAR);}
inline MFTYPE32 void MFTYPE MocrGetResultSafeType(MIL_ID ResultId, MIL_INT ResultType, MIL_UINT16 MPTYPE *ValuePtr) {MocrGetResultExecute(ResultId, ResultType, ValuePtr, M_TYPE_SHORT);}
inline MFTYPE32 void MFTYPE MocrGetResultSafeType(MIL_ID ResultId, MIL_INT ResultType, MIL_UINT32 MPTYPE *ValuePtr) {MocrGetResultExecute(ResultId, ResultType, ValuePtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 void MFTYPE MocrGetResultSafeType(MIL_ID ResultId, MIL_INT ResultType, MIL_UINT64 MPTYPE *ValuePtr) {MocrGetResultExecute(ResultId, ResultType, ValuePtr, M_TYPE_MIL_INT64);}
#endif
#if M_MIL_SAFE_TYPE_ADD_WCHAR_T
inline MFTYPE32 void MFTYPE MocrGetResultSafeType(MIL_ID ResultId, MIL_INT ResultType, wchar_t    MPTYPE *ValuePtr) {MocrGetResultExecute(ResultId, ResultType, ValuePtr, M_TYPE_SHORT);}
#endif

#define MocrGetResult        MocrGetResultSafeType
#define MocrInquire          MocrInquireSafeType

#else // #if M_MIL_USE_SAFE_TYPE

#define MocrGetResultUnsafe        MocrGetResult
#define MocrInquireUnsafe          MocrInquire

#endif // #if M_MIL_USE_SAFE_TYPE

#endif // !M_MIL_LITE
#endif // __MILOCR_H__
