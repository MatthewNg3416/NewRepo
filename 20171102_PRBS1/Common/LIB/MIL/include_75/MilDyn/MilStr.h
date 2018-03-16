/***************************************************************************/
/*

    Filename:  MilStr.h
    Owner   :  Matrox Imaging dept.
    Rev     :  $Revision:   8.0  $
    Content :  This file contains the defines and the prototypes for the
               MIL String Reader module. (Mstr...).

    COPYRIGHT (c) 2004 Matrox Electronic Systems Ltd.
    All Rights Reserved

*/
/***************************************************************************/

#ifndef __MILSTR_H__
#define __MILSTR_H__

#if (!M_MIL_LITE) // MIL FULL ONLY

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif


/***************************************************************************/
/*                      MilStr CAPI defines                                */
/***************************************************************************/

/***************************************************************************/
/* Index values                                                            */
/***************************************************************************/
#define M_FONT_MASK                             0x00800000L
#define M_STRING_MASK                           0x00400000L
#define M_REAL_INDEX_MASK                       0x000000FFL

#define M_DEFAULT                               0x10000000L // Also defined in mil.h
#define M_CONTEXT                               0x08000000L
#define M_ALL                                   0x40000000L // Also defined in Milmod.h
#define M_GENERAL                               0x20000000L // Also defined in Milmod.h

#define M_FONT_INDEX(i)                 (M_FONT_MASK | (i))
#define M_STRING_INDEX(i)             (M_STRING_MASK | (i))


/***************************************************************************/
/* MstrAlloc()                                                             */
/***************************************************************************/

/* Object types */
#define M_FEATURE_BASED                         0x00002000L

/***************************************************************************/
/* MstrControl() and MstrInquire()                                         */
/***************************************************************************/

/* ControlType and InquireType for context */
#define M_STRING_ADD                                   700L
#define M_STRING_DELETE                                701L
#define M_FONT_ADD                                     702L
#define M_FONT_DELETE                                  703L

#define M_STRING_NUMBER                                 74L // Also defined in milocr.h
#define M_SPEED					                           8L // Also defined in milpat.h
#define M_MINIMUM_CONTRAST                             600L
#define M_ENCODING                              0x00008005L // Also defined in milcode.h
#define M_TIMEOUT                                     2077L // already define in mil.h
#define M_STOP_READ                                    610L 
#define M_STRING_SEPARATOR                             910L 
#define M_SPACE_CHARACTER                              911L

#define M_NUMBER_OF_STRING_MODELS                      900L
#define M_NUMBER_OF_FONTS                              901L
#define M_OWNER_SYSTEM                                1001L // Also defined in mil.h
#define M_PREPROCESSED                                  14L // Also defined in MilPat.h
#define M_MODIFICATION_COUNT                          5010L // Also defined in mil.h
#define M_OBJECT_TYPE                           0x80000000L // Already define in Mil.h
#define M_CONTEXT_TYPE                        M_OBJECT_TYPE  
#define M_SCORE_TYPE                                    37L // Already defined in milpat.h

#define M_DRAW_LAST_SIZE_X                             905L
#define M_DRAW_LAST_SIZE_Y                             906L
//Operation for context
#define M_DRAW_CHAR                                    907L

/*String Type pass to MstrControll() with M_STRING_ADD*/
#define M_USER_DEFINED                                  21L // Also defined in Mil.h
#define M_UNIVERSAL                              0x00001000

/* ControlType and InquireType for string model */
#define M_STRING_NUMBER                                 74L // Also defined in milocr.h
#define M_STRING_SIZE_MIN                              500L
#define M_STRING_SIZE_MAX                               62L // Also defined in milocr.h
#define M_STRING_ANGLE                                  87L // Also defined in milocr.h
#define M_STRING_ANGLE_DELTA_NEG                       106L // Also defined in milocr.h
#define M_STRING_ANGLE_DELTA_POS                       107L // Also defined in milocr.h
#define M_STRING_SCALE                                 502L
#define M_STRING_SCALE_MIN_FACTOR                      503L
#define M_STRING_SCALE_MAX_FACTOR                      504L
#define M_STRING_ASPECT_RATIO                          505L
#define M_STRING_ASPECT_RATIO_MIN_FACTOR               506L
#define M_STRING_ASPECT_RATIO_MAX_FACTOR               507L
#define M_FOREGROUND_VALUE                               4L // Also defined in milocr.h
#define M_CHAR_ACCEPTANCE                               25L // Also defined in milocr.h
#define M_CHAR_SIMILARITY_ACCEPTANCE                    26L
#define M_CHAR_HOMOGENEITY_ACCEPTANCE                   27L
#define M_STRING_ACCEPTANCE                             24L // Also defined in milocr.h
#define M_CHAR_SCALE_MIN_FACTOR                        509L
#define M_CHAR_SCALE_MAX_FACTOR                        510L
#define M_CHAR_ASPECT_RATIO_MIN_FACTOR                 511L
#define M_CHAR_ASPECT_RATIO_MAX_FACTOR                 512L
#define M_CHAR_MAX_BASELINE_DEVIATION                  513L
#define M_SPACE_MAX_CONSECUTIVE                        514L
#define M_STRING_TYPE                                  515L
#define M_NUMBER_OF_CONSTRAINTS                        516L

#define M_CONSTRAINT_POSITION                          520L
#define M_CONSTRAINT_TYPE                       0x00008000L // Also defined in milocr.h
#define M_CONSTRAINT_FONT                              524L
#define M_CONSTRAINT_ALLOC_SIZE                        521L
#define M_CONSTRAINT                            0x04000000L // Also defined in milocr.h
#define M_DEFAULT_CONSTRAINT_ALLOC_SIZE                522L
#define M_DEFAULT_CONSTRAINT_FONT                      525L
#define M_DEFAULT_CONSTRAINT_TYPE                      523L
#define M_DEFAULT_CONSTRAINT                    0x08000000L

/* Mask for constraint index */
#define M_CONSTRAINT_INDEX_MASK                 0x000000FFL



/* ControlType and InquireType for font */
#define M_FONT_TYPE                                     31L // Also defined in milocr.h
#define M_SPACE_WIDTH                                  800L
#define M_SPACE_SIZE                          M_SPACE_WIDTH
#define M_DRAW_BOX_MARGIN_X                            801L
#define M_DRAW_BOX_MARGIN_Y                            802L
#define M_CHAR_M_WIDTH                                 803L  
#define M_CHAR_M_HEIGHT                                804L

#define M_NUMBER_OF_CHARS                              814L
#define M_CHAR_VALUE                                   815L
#define M_CHAR_TYPE                                    816L
#define M_CHAR_BASELINE                                  4L
#define M_CHAR_SIZE_X                                   37L // Also defined in milocr.h
#define M_CHAR_SIZE_Y                                   38L // Also defined in milocr.h
#define M_CHAR_THICKNESS                                39L // Also defined in milocr.h

#define M_DEFINITION_OFFSET_X                          817L
#define M_DEFINITION_OFFSET_Y                          818L

/* Inquire only for font */
#define M_CHAR_BASELINE_VALUE                          5L

/* InquireType modifiers */
#define M_DEFAULT                               0x10000000L // Also defined in mil.h
#define M_SUPPORTED                             0x20000000L // Also defined in milmod.h
#define M_TYPE_CHAR                             0x00010000L // Also defined in mil.h
#define M_TYPE_SHORT                            0x00020000L // Also defined in mil.h
#define M_TYPE_LONG                             0x00040000L // Also defined in mil.h
#define M_TYPE_FLOAT                            0x00080000L // Also defined in mil.h
#define M_TYPE_DOUBLE                           0x00100000L // Also defined in mil.h
#define M_TYPE_MIL_ID                           0x00400000L // Also defined in mil.h
#define M_TYPE_STRING                           0x00800000L // Also defined in mil.h


/* ControlValue  */
#define M_DEFAULT                               0x10000000L // Also defined in mil.h
#define M_ORIGINAL                              0x00000199L // Already define in MilPat.h
#define M_ALL                                   0x40000000L // Also defined in Milmod.h
#define M_DISABLE                                    -9999L // Also defined in mil.h

#define M_USER_DEFINED                                  21L // Also defined in Mil.h

#define M_FOREGROUND_WHITE                            0x80L // Also defined in MilBlob.h 
#define M_FOREGROUND_BLACK                           0x100L // Also defined in MilBlob.h 
#define M_FOREGROUND_ANY        M_FOREGROUND_WHITE+M_FOREGROUND_BLACK // Also defined in milmod.h


#define M_LOW                                            1L // Also defined in Milocr.h
#define M_MEDIUM                                         2L // Also defined in Milocr.h
#define M_HIGH                                           3L // Also defined in Milocr.h

#define M_ASCII                                          1L
#define M_UNICODE                                        2L

#define M_NONE                                  0x08000000L
#define M_AUTO_COMPUTE                          0x08000001L

#define M_MIN_CHAR_WIDTH                        0x01000000L
#define M_MEAN_CHAR_WIDTH                       0x02000000L
#define M_MAX_CHAR_WIDTH                        0x03000000L
#define M_QUARTER_MAX_CHAR_WIDTH                0x04000000L
#define M_INFINITE                              0xFFFFFFFFL // Also defined in Mil.h

// Score Type...
#define M_GREYSCALE                                 0x0200L // Already defined in mil.h
#define M_GRAYSCALE                             M_GREYSCALE // Already defined in mil.h
#define M_FEATURE_BASED                         0x00002000L


/***************************************************************************/
/* MstrGetResult()                                                         */
/***************************************************************************/

#define M_STRING_NUMBER                                 74L // Also defined in milocr.h
#define M_CHAR_NUMBER                                   32L // Also defined in milocr.h
#define M_TIMEOUT_END                                   10L // Also defined in milmod.h
#define M_TEXT_SIZE                                     17L 
#define M_CONTEXT_ID                            0x00001010L // Also defined in milmod.h
#define M_TEXT                                          16L // Also defined in milocr.h

#define M_STRING_LENGTH                         0x00000028L // Also defined in milocr.h (must be bitwise - 2 bit)
#define M_STRING_SIZE                       M_STRING_LENGTH // Also defined in milocr.h
#define M_STRING_ALLOC_SIZE_BYTE                       591L // Also defined in milocr.h
#define M_STRING                                         3L // Also defined in milocr.h
#define M_STRING_MODEL_INDEX                           585L
#define M_STRING_ANGLE                                  87L // Also defined in milocr.h
#define M_STRING_CHAR_ANGLE                            794L  
#define M_STRING_SCALE                                 502L
#define M_STRING_ASPECT_RATIO                          505L
#define M_STRING_POSITION_X                            586L
#define M_STRING_POSITION_Y                            587L
#define M_STRING_SCORE                          0x00001400L // Also defined in milocr.h
#define M_STRING_CHAR_SCORE_MIN                        588L
#define M_STRING_CHAR_SCORE_MAX                        589L
#define M_STRING_FOREGROUND_VALUE                      508L
#define M_FORMATTED_STRING_SIZE                        792L
#define M_FORMATTED_STRING                             793L

#define M_CHAR_VALUE                                   815L
#define M_CHAR_FONT                                    785L
#define M_CHAR_INDEX                                   790L
#define M_CHAR_SCORE                                     5L // Also defined in milocr.h
#define M_CHAR_HOMOGENEITY_SCORE                       788L
#define M_CHAR_SIMILARITY_SCORE                        789L
#define M_CHAR_SCALE                                   786L
#define M_CHAR_ASPECT_RATIO                            787L
#define M_CHAR_POSITION_X                                6L // Also defined in milocr.h
#define M_CHAR_POSITION_Y                                7L // Also defined in milocr.h
#define M_CHAR_CONSECUTIVE_SPACE                       791L

#define M_A_FORWARD                             0x00004001L // Also defined in milmod.h
#define M_B_FORWARD                             0x00004002L // Also defined in milmod.h
#define M_C_FORWARD                             0x00004003L // Also defined in milmod.h
#define M_D_FORWARD                             0x00004004L // Also defined in milmod.h
#define M_E_FORWARD                             0x00004005L // Also defined in milmod.h
#define M_F_FORWARD                             0x00004006L // Also defined in milmod.h
#define M_A_REVERSE                             0x00004011L // Also defined in milmod.h
#define M_B_REVERSE                             0x00004012L // Also defined in milmod.h
#define M_C_REVERSE                             0x00004013L // Also defined in milmod.h
#define M_D_REVERSE                             0x00004014L // Also defined in milmod.h
#define M_E_REVERSE                             0x00004015L // Also defined in milmod.h
#define M_F_REVERSE                             0x00004016L // Also defined in milmod.h


/* ResultType modifiers */
#define M_DEFAULT                               0x10000000L // Also defined in mil.h
#define M_SUPPORTED                             0x20000000L // Also defined in milmod.h
#define M_TYPE_CHAR                             0x00010000L // Also defined in mil.h
#define M_TYPE_SHORT                            0x00020000L // Also defined in mil.h
#define M_TYPE_LONG                             0x00040000L // Also defined in mil.h
#define M_TYPE_FLOAT                            0x00080000L // Also defined in mil.h
#define M_TYPE_DOUBLE                           0x00100000L // Also defined in mil.h
#define M_TYPE_MIL_ID                           0x00400000L // Also defined in mil.h
#define M_TYPE_STRING                           0x00800000L // Also defined in mil.h



/***************************************************************************/
/* MstrDraw()                                                              */
/***************************************************************************/

/* Operation */
#define M_DRAW_STRING                           0x00000001L
#define M_DRAW_STRING_CONTOUR                   0x00000002L
#define M_DRAW_STRING_BOX                       0x00000004L
#define M_DRAW_STRING_CHAR_BOX                  0x00000008L
#define M_DRAW_STRING_CHAR_POSITION             0x00000010L



/***************************************************************************/
/* MstrEditFont()                                                          */
/***************************************************************************/

/* Operation */
#define M_CHAR_ADD                                       1L
#define M_CHAR_DELETE                                    2L
#define M_CHAR_NORMALIZE                                 3L
#define M_CHAR_BASELINE                                  4L
#define M_CHAR_SORT                                      5L
#define M_CHAR_TYPE                                    816L

/* Operation mode */
#define M_DEFAULT                               0x10000000L // Also defined in mil.h
#define M_USER_DEFINED                                  21L // Also defined in Mil.h
#define M_SYSTEM_FONT                                   12L
#define M_SEMI_M13_88                                  0x2L // Also defined in milocr.h
#define M_SEMI_M12_92                                  0x1L // Also defined in milocr.h
#define M_OCR_A                                         10L
#define M_OCR_B                                         11L
#define M_ASCENDING                                      1L
#define M_DESCENDING                                     2L

/* Operation mode modifiers */
#define M_SINGLE                                0x01000000L // Also defined in Mil.h

#define M_FOREGROUND_WHITE                            0x80L // Also defined in MilBlob.h 
#define M_FOREGROUND_BLACK                           0x100L // Also defined in MilBlob.h 


/* Operation parameters 1 */
#define M_NONE                                  0x08000000L
#define M_REGULAR                               0x00020000L // Also defined in Mil.h
#define M_PUNCTUATION                                    1L



/***************************************************************************/
/* MstrSetConstraint()                                                     */
/***************************************************************************/

/* Constraint Type */
#define M_DEFAULT                               0x10000000L // Also defined in mil.h
#define M_ANY                                   0x11000000L // Also defined in Mil.h
#define M_LETTER                                0x00020000L // Also defined in milocr.h
#define M_DIGIT                                 0x00040000L // Also defined in milocr.h
#define M_UPPERCASE                             0x00010000L // Also defined in milocr.h
#define M_LOWERCASE                             0x00008000L // Also defined in milocr.h
#define M_FONT_MASK                             0x00800000L
#define M_REAL_INDEX_MASK                       0x000000FFL



/***************************************************************************/
/*                 MilStr CAPI function prototypes                         */
/***************************************************************************/

#ifndef __midl // MIDL compiler used by ActiveMIL

MFTYPE32 MIL_ID MFTYPE MstrAlloc         (MIL_ID   SystemId, 
                                          long     ContextType, 
                                          long     ControlFlag,
                                          MIL_ID*  ObjectIdPtr);

MFTYPE32 MIL_ID MFTYPE MstrAllocResult   (MIL_ID   SystemId,
                                          long     ControlFlag, 
                                          MIL_ID*  ObjectIdPtr);

MFTYPE32 void MFTYPE MstrFree            (MIL_ID ObjectId);

MFTYPE32 void MFTYPE MstrControl         (MIL_ID   ContextOrResultId,
                                          long     Index,
                                          long     ControlType,
                                          double   ControlValue);

MFTYPE32 long MFTYPE MstrInquire         (MIL_ID   ContextOrResultId,
                                          long     Index,
                                          long     InquireType,
                                          void*    UserVarPtr);

MFTYPE32 void MFTYPE MstrEditFont        (MIL_ID   ContextId,
                                          long     FontIndex,
                                          long     Operation,
                                          long     OperationMode,
                                          long     Param1,
                                          void*    Param2,
                                          void*    Param3);

MFTYPE32 void MFTYPE MstrSetConstraint   (MIL_ID   ContextId,
                                          long     StringIndex,
                                          long     CharPos,
                                          long     ConstraintType,
                                          void*    CharList);

MFTYPE32 void MFTYPE MstrPreprocess      (MIL_ID   ContextId,
                                          long     ControlFlag);

MFTYPE32 void MFTYPE MstrRead            (MIL_ID   ContextId,
                                          MIL_ID   TargetImageId,
                                          MIL_ID   ResultId);

MFTYPE32 void MFTYPE MstrGetResult       (MIL_ID   ResultId,
                                          long     Index,
                                          long     ResultType,
                                          void*    ResultArrayPtr);

MFTYPE32 void MFTYPE MstrDraw            (MIL_ID   GraphContId,
                                          MIL_ID   ContextOrResultId,
                                          MIL_ID   DestImageId,
                                          long     Operation,
                                          long     Index,
                                          void*    CharList,
                                          long     ControlFlag);

MFTYPE32 void MFTYPE MstrSave            (MIL_TEXT_PTR  FileName,
                                          MIL_ID        ContextId,
                                          long          ControlFlag);

MFTYPE32 MIL_ID MFTYPE MstrRestore       (MIL_TEXT_PTR  FileName,
                                          MIL_ID        SystemId,
                                          long          ControlFlag,
                                          MIL_ID*       ContextIdPtr);

MFTYPE32 void MFTYPE MstrStream          (MIL_TEXT_PTR  MemPtrOrFileName,
                                          MIL_ID        SystemId,
                                          long          Operation,
                                          long          StreamType,
                                          double        Version,
                                          long          ControlFlag,
                                          MIL_ID*       ObjectIdPtr,
                                          long         *SizeByteVarPtr);

#endif /* #ifdef __midl */

/* C++ directive if needed */
#ifdef __cplusplus
}
#endif

#endif // !M_MIL_LITE

#endif /* __MILSTR_H__ */
