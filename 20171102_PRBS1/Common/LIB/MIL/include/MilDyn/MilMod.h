/***************************************************************************/
/*

    Filename:  MilMod.h
    Owner   :  Matrox Imaging dept.
    Revision:  9.02.0580
    Content :  This file contains the defines and the prototypes for the
               MIL geometric pattern matching module. (Mmod...).

    Copyright © Matrox Electronic Systems Ltd., 1992-2010.
    All Rights Reserved

*/
/***************************************************************************/

#ifndef __MILMOD_H__
#define __MILMOD_H__

#define BW_COMPATIBILITY 0x092

#if (!M_MIL_LITE) // MIL FULL ONLY

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif


/***************************************************************************/
/*                      MilMod CAPI defines                                */
/***************************************************************************/

/***************************************************************************/
/* MmodAlloc()                                                             */
/***************************************************************************/

/* Object types */
#define M_RESULT                0x00001000L

#define M_GEOMETRIC             0x00002000L
#define M_GEOMETRIC_CONTROLLED  0x00008000L


/***************************************************************************/
/* MmodGetResult()                                                         */
/***************************************************************************/
/* For ResultIndex */
#define M_ALL              0x40000000L  // already defined in Mil.h

/* For Result Type */
#define M_NUMBER                 1009L  // already defined in Mil.h
#define M_GENERAL          0x20000000L  // also defined in milreg.h
#define M_CONTEXT_ID       0x00001010L
#define M_TIMEOUT_END              10L
#define M_SCORE            0x00001400L  // already defined in MilPat.h
#define M_TARGET_COVERAGE  0x00001500L
#define M_MODEL_COVERAGE   0x00001600L
#define M_FIT_ERROR        0x00001700L
#define M_SCORE_TARGET     0x00001800L
#define M_POSITION_X       0x00003400L  // already defined in MilPat.h
#define M_POSITION_Y       0x00004400L  // already defined in MilPat.h
#define M_ANGLE            0x00000800L  // already defined in Mil.h
#define M_SCALE            0x00008010L  // already defined in Milpat.h
#define M_A_FORWARD        0x00004001L
#define M_B_FORWARD        0x00004002L
#define M_C_FORWARD        0x00004003L
#define M_D_FORWARD        0x00004004L
#define M_E_FORWARD        0x00004005L
#define M_F_FORWARD        0x00004006L
#define M_A_REVERSE        0x00004011L
#define M_B_REVERSE        0x00004012L
#define M_C_REVERSE        0x00004013L
#define M_D_REVERSE        0x00004014L
#define M_E_REVERSE        0x00004015L
#define M_F_REVERSE        0x00004016L
#define M_RADIUS          M_MAJOR_AXIS 
#define M_MAJOR_AXIS              215L 
#define M_MINOR_AXIS       0x0000000AL 
#define M_ARC_PORTION             225L
#define M_ELASTICITY              109L
#define M_TARGET_EDGE_SIZE        222L
#define M_TARGET_EDGES_X          223L
#define M_TARGET_EDGES_Y          224L

#define M_MODEL_ASSOCIATED_CALIBRATION 768L

#ifndef M_MIL_75_COMPATIBLE
#define M_AVAILABLE        0x40000000L  // already defined in MilEdge.h
#else
#define M_AVAILABLE        0x08000000L  // already defined in MilEdge.h
#endif //M_MIL_75_COMPATIBLE

/***************************************************************************/
/* MmodDefine()                                                            */
/***************************************************************************/

#define M_DELETE                    3L   // 3   Already defined in Milblob.h

#define M_IMAGE            0x00000004L   // 4   Already defined in  Mil.h       
#define M_CIRCLE           0x00000008L   // 8   Already defined in  MilBga.h        
#define M_ELLIPSE          0x00000010L   // 16
#define M_SQUARE           0x00000020L   // 32
#define M_RECTANGLE        0x00000040L   // 64
#define M_LINE             0x00000080L   // 128
#define M_RING             0x00000100L   // 256
#define M_DXF_FILE         0x00000200L   // 512
#define M_CROSS            0x00002000L   // 8192 Already defined in  MilBga.h
#define M_DIAMOND          0x00008000L   // 32768
#define M_TRIANGLE         0x00010000L   // 65536
#define M_MERGE_MODEL      0x00020000L   // 131072
#define M_AUTO_DEFINE      0x00040000L   // 262144


/***************************************************************************/
/* MmodDefineFromFile()                                                    */
/***************************************************************************/
#ifndef M_MIL_75_COMPATIBLE
#define M_MEMORY           (M_APP_INQUIRE_SYS_START+120L) /*5120*/  // Already defined in Mil.h
#else
#define M_MEMORY                             5L  // Already defined in Mil.h
#endif //M_MIL_75_COMPATIBLE
#define M_INTERACTIVE           M_NULL   // Already defined in mil.h, milcal.h, milcode.h, miledge.h, milmeas.h, milocr.h, milpat.h
#define M_DXF_FILE         0x00000200L   // 512


/***************************************************************************/
/* MmodControl() / MmodInquire()                                           */
/***************************************************************************/
  
// For Index 
#define M_DEFAULT                      0x10000000L  // already defined in Mil.h                     
#define M_CONTEXT                      0x08000000L
#define M_ALL                          0x40000000L  // already defined in Mil.h

// Modifier 
#define M_SUPPORTED                    0x20000000L

// Context controls/inquires 
#define M_IMAGE_ID                     0x04000000L
#define M_EDGE_RESULT_ID               (M_IMAGE_ID|0x1L)
#define M_NUMBER                       1009L  // already defined in Mil.h
#define M_REFERENCE_X                  100L  // also defined in milreg.h
#define M_REFERENCE_Y                  101L  // also defined in milreg.h
#define M_REFERENCE_ANGLE              110L
#define M_SEARCH_OFFSET_X              102L
#define M_SEARCH_OFFSET_Y              103L
#define M_SEARCH_SIZE_X                104L
#define M_SEARCH_SIZE_Y                105L
#define M_ACCURACY                     106L
#define M_SPEED					           8L  // Already define in milpat.h
#define M_SMOOTHNESS                   108L   // TBM
#define M_KERNEL_WIDTH                 109L
#define M_FILTER_MODE                  122L  // allready defined in miledge.h
#define M_DETAIL_LEVEL                 111L
#define M_SEARCH_ANGLE_RANGE           113L  // also defined in milreg.h
#define M_SEARCH_SCALE_RANGE           114L  // also defined in milreg.h
#define M_SEARCH_POSITION_RANGE        119L  // also defined in milreg.h
#define M_SEARCH_ASPECT_RATIO_RANGE    117L
#define M_ASPECT_RATIO_MODE            118L
#define M_TIMEOUT                     2077L    // already define in mil.h
#define M_STOP_FIND                    115L
#define M_LINE_EXTRACTOR               116L
#define M_SHARED_EDGES                 220L
#define M_TARGET_CACHING                39L    // Already define in milpat.h
#define M_SAVE_TARGET_EDGES            121L    // Already define in milpat.h
#define M_OVERSCAN                      53L    // Already define in mil.h
#define M_OVERSCAN_REPLACE_VALUE        54L    // Already define in mil.h
#define M_CONTEXT_CONVERT              535L    // Already define in milocr.h
#define M_SQR_AVERAGE_DEV_TOLERANCE    123L
#define M_LINE_SEGMENT_ANGLE_TOLERANCE 135L
#define M_SCALED_EDGES                 124L

// Context controls/inquires for M_GEOMETRIC_CONTROLLED
#define M_FIRST_LEVEL                   31L //Already defined in milpat.h
#define M_LAST_LEVEL                    32L //Already defined in milpat.h
#define M_AUTO                         444L //Already defined in milocr.h

// Context controls/inquires for circle finder
#define M_ARC_PORTION_MIN              107L
#define M_ELASTICITY                   109L
#define M_MODEL_BASED                  112L


// Model controls/inquires
#define M_ACCEPTANCE                         200L
#define M_ACCEPTANCE_TARGET                  201L
#define M_CERTAINTY                          202L           
#define M_CERTAINTY_TARGET                   203L
#define M_SCORE_UNCERTAINTY_FACTOR           204L
#define M_SCORE_TARGET_UNCERTAINTY_FACTOR    205L
#define M_POLARITY                           0x00004000L  // already defined in MilMeas.h 
#define M_POLARITY_CHECK                     M_POLARITY  
#define M_MIN_SEPARATION_X                   M_MIN_SPACING_X  // already defined in MilPat.h 
#define M_MIN_SEPARATION_Y                   M_MIN_SPACING_Y  // already defined in MilPat.h 
#define M_SCALE                              0x00008010L      // already defined in MilPat.h 
#define M_SCALE_MIN_FACTOR                   210L   // also defined in milreg.h
#define M_SCALE_MAX_FACTOR                   211L   // also defined in milreg.h
#define M_ASPECT_RATIO                       6001L // (Already defined in milcal.h) 
#define M_ASPECT_RATIO_MIN_FACTOR            230L 
#define M_ASPECT_RATIO_MAX_FACTOR            231L 
#define M_EMPTY                              236L
#define M_AUTO_DEFINE_MAX_DISPLACEMENT_X     237L
#define M_AUTO_DEFINE_MAX_DISPLACEMENT_Y     238L

#define M_PRESEARCH_METHOD                   400L
#define M_PRESEARCH_LOCAL                    401L
#define M_PRESEARCH_GLOBAL                   402L
#define M_PRESEARCH_GLOBAL_FACTOR            403L

#define M_CORNER_RADIUS                      239L
#define M_ANGLE                              0x00000800L  // already defined in Mil.h
#define M_ANGLE_DELTA_NEG                    0x00000200L  // Already define in MilPat.h 
#define M_ANGLE_DELTA_POS                    0x00000400L  // Already define in MilPat.h 
#define M_INDEX                              217L
#define M_FIT_ERROR_WEIGHTING_FACTOR         221L
#define M_USER_LABEL                         228L   // also defined in milreg.h
#define M_INDEX_FROM_LABEL                   229L   // also defined in milreg.h
#define M_NUMBER_OF_CHAINED_PIXELS           56L    // Already define in MilBlob.h
#define M_NUMBER_OF_CHAINED_EDGELS           M_NUMBER_OF_CHAINED_PIXELS
#define M_CHAIN_X                            65L    // Already define in MilBlob.h
#define M_CHAIN_Y                            144L   // Already define in MilBlob.h
#define M_CHAIN_ANGLE                        258L   // Already define in MilEdge.h
#define M_CHAIN_INDEX                        67L    // Already define in MilBlob.h

#define M_MIN_SEPARATION_SCALE               68L
#define M_MIN_SEPARATION_ANGLE               69L
#define M_ASSOCIATED_CALIBRATION             125L   // Already define in MilCal.h
#define M_USER_ASSOCIATED_CALIBRATION        126L
#define M_RESTORED_CALIBRATION               1491L
#define M_CALIBRATED                         0x0000300L

#define M_POSITION_DELTA_NEG_X               127L  // also defined in milreg.h
#define M_POSITION_DELTA_POS_X               128L  // also defined in milreg.h
#define M_POSITION_DELTA_NEG_Y               129L  // also defined in milreg.h
#define M_POSITION_DELTA_POS_Y               130L  // also defined in milreg.h
#define M_EDGE_POSITION_UNCERTAINTY          132L

#define M_WAIT_CONFIDENCE_FACTOR             131L

#define M_LINE_SEGMENT_LENGTH_UNCERTAINTY    133L
#define M_LINE_SEGMENT_LENGTH_TOLERANCE      134L

#define M_CANDIDATE_SCORE_CERTAINTY          136L

#define M_REFERENCE_PIXEL_X                  137L
#define M_REFERENCE_PIXEL_Y                  138L
#define M_REFERENCE_PIXEL_ANGLE              139L

#define M_REFERENCE_WORLD_X                  140L
#define M_REFERENCE_WORLD_Y                  141L
#define M_REFERENCE_WORLD_ANGLE              142L

#define M_REFERENCE_FIXTURE_X                143L
#define M_REFERENCE_FIXTURE_Y                145L
#define M_REFERENCE_FIXTURE_ANGLE            146L

//Controlls/inquires for Synthetic models

#define M_BOX_MARGIN_LEFT                    308L
#define M_BOX_MARGIN_RIGHT                   309L
#define M_BOX_MARGIN_TOP                     310L
#define M_BOX_MARGIN_BOTTOM                  311L
#define M_PIXEL_SCALE                        316L
#define M_CAD_Y_AXIS                         318L

// Controls/inquires for results
#define M_RESULT_OUTPUT_UNITS          1300L // Also in milim.h


// CAD Y Axis control values
#define M_FLIP                        0x00010000L   // Already defined in Mil.h
#define M_NO_FLIP                     0x04000000L   // Already defined in Mil.h


// Types of polarity check
#define M_SAME                               -1L   // Already defined in MilMeas.h
#define M_REVERSE                             4L   // Already defined in Mil.h
#define M_SAME_OR_REVERSE                     5L
#define M_ANY                        0x11000000L   // Already defined in MilMeas.h
#define M_MIX                              M_ANY

// To remove user label
#define M_NO_LABEL                   0x20000000L   // Also defined in milreg.h

// Control values
#define M_VERY_LOW                             0L  // Already define in milpat.h
#define M_LOW                                  1L  // Already define in milpat.h
#define M_MEDIUM                               2L  // Already define in milpat.h
#define M_HIGH                                 3L  // Already define in milpat.h
#define M_VERY_HIGH                            4L  // Already define in milpat.h

#define M_GRADIENT_TYPE                        1L
#define M_BLOB_TYPE                            2L

#define M_KERNEL_DEPTH                       368L  // Already defined in miledge.h
#define M_SIZE                             0xC00L  // Already define  in medge.h
#define M_RECURSIVE                    0x00040000  // Already defined in mildev.h
#ifdef M_MIL_75_COMPATIBLE
#define M_KERNEL                      0x00000200L  // Already defined in milmod.h
#else
#define M_KERNEL                      0x00400000L  // Already defined in mil.h
#endif //M_MIL_75_COMPATIBLE

// Parameters for output units
#define M_PIXEL                              0x1000L
#define M_WORLD                              0x2000L
#define M_ACCORDING_TO_CALIBRATION             1301L
#define M_ACCORDING_TO_RESULT                  1302L


/***************************************************************************/
/* MmodInquire()                                                           */
/***************************************************************************/
#define M_VALID                        0x00000001L  // Already define in Mil.h
#define M_CONTEXT_TYPE                 M_OBJECT_TYPE
#define M_MODEL_TYPE                   M_OBJECT_TYPE
#define M_PREPROCESSED                 14L          // Already define in MilPat.h
#define M_KERNEL_SIZE                  301L
#define M_NUMBER_MODELS                302L
#define M_MODIFICATION_COUNT           5010L        // Already define in Mil.h
#define M_OWNER_SYSTEM                 1101L        // already define in mil.h
#define M_SIZE_BYTE                    5061L        // already defined in Mil.h
#define M_ALLOC_OFFSET_X               15L          // Already define in MilPat.h
#define M_ALLOC_OFFSET_Y               16L          // Already define in MilPat.h
#define M_ALLOC_SIZE_X                 2L           // Already define in MilPat.h
#define M_ALLOC_SIZE_Y                 3L           // Already define in MilPat.h
#define M_ORIGINAL_X                   6L           // Already define in MilPat.h
#define M_ORIGINAL_Y                   7L           // Already define in MilPat.h
#define M_NUMBER_OF_WARNING            305L
#define M_WARNINGS                     306L
#if (M_MIL_USE_UNICODE && !M_MIL_UNICODE_API && !M_COMPILING_MILDLL)
#define M_STRING_OF_WARNING            (307L|M_CLIENT_ASCII_MODE)
#else
#define M_STRING_OF_WARNING            307L
#endif
#define M_ALLOC_SIZE_BAND              321L
#define M_ALLOC_TYPE                   322L         // Already defined in MilPat.h
#define M_ALLOC_SIGN                   323L
#define M_ALLOC_SIZE_BIT                26L         // Already defined in MilPat.h

// Model inquires for Misc Synthetic Model 
#define M_AXIS_RATIO_MIN                     212L
#define M_AXIS_RATIO_MAX                     213L
#define M_MAJOR_AXIS_MIN                     214L
#define M_MAJOR_AXIS                         215L         // Alredy defided for MmodGetResult
#define M_MAJOR_AXIS_MAX                     216L
#define M_RADIUS                             M_MAJOR_AXIS
#define M_RADIUS_MIN                         M_MAJOR_AXIS_MIN
#define M_RADIUS_MAX                         M_MAJOR_AXIS_MAX
#define M_FOREGROUND_VALUE                   4L       // Already defined in MilBlob.h         
#define M_FOREGROUND_WHITE                   0x80L    // Already define in MilBlob.h 
#define M_FOREGROUND_BLACK                   0x100L   // Already define in MilBlob.h 
#define M_FOREGROUND_ANY                     M_FOREGROUND_WHITE+M_FOREGROUND_BLACK

#define M_LENGTH                             0x00002000L // already defined in Mil.h
#define M_WIDTH                              0x00010000L // Already defined in Milmeas.h
#define M_HEIGHT                             317L
#define M_THICKNESS                          10L  // already defined in Mil.h
#define M_HORIZONTAL_THICKNESS               233L
#define M_VERTICAL_THICKNESS                 232L
#define M_INNER_RADIUS                       234L
#define M_OUTER_RADIUS                       235L
                                             
#define M_BOX_OFFSET_X                       312L
#define M_BOX_OFFSET_Y                       313L
#define M_BOX_SIZE_X                         314L
#define M_BOX_SIZE_Y                         315L

#define M_MOD_DEFINE_COMPATIBLE              330L


/***************************************************************************/
/* MmodDraw()                                                              */
/***************************************************************************/

#define M_DRAW_IMAGE                    0x00000002L   // Already define in MilPat.h
#define M_DRAW_EDGES                    0x00000004L
#define M_DRAW_DONT_CARES               0x00000008L   // Already define in MilPat.h
#define M_DRAW_FLAT_REGIONS             0x00000010L
#define M_DRAW_BOX                      0x00000020L   // Already define in MilPat.h
#define M_DRAW_POSITION                 0x00000040L   // Already define in MilPat.h
#define M_DRAW_GRADIENT                 0x00000080L 
#define M_DRAW_WEIGHT_REGIONS           0x00000100L
#define M_DRAW_AXIS                     0x00000200L   // Already define in MilBlob.h

#define M_ORIGINAL                      0x00000199L   // Already define in MilPat.h
#ifndef M_MIL_75_COMPATIBLE
#define M_MODEL                         0x00010000L   //65536
#else
#define M_MODEL                         0X10000L      //65536
#endif //M_MIL_75_COMPATIBLE
#define M_TARGET                        0x00080000L   // Also defined in milreg.h

#define M_DRAW_RELATIVE_ORIGIN_X               319L   // Already define in MilEdge.h
#define M_DRAW_RELATIVE_ORIGIN_Y               320L   // Already define in MilEdge.h
#define M_DRAW_SCALE_X                        3203L   // Already define in MilEdge.h
#define M_DRAW_SCALE_Y                        3204L   // Already define in MilEdge.h

/***************************************************************************/
/* MmodMask()                                                              */
/***************************************************************************/

#define M_DONT_CARES            M_DONT_CARE // Already defined in mil.h
#define M_FLAT_REGIONS                 501L
#define M_WEIGHT_REGIONS               502L


/***************************************************************************/
/* MmodPreprocess()                                                        */
/***************************************************************************/
#define M_RESET                          9L
#define M_COMPLETE              0x00000000L // Already defined in Mil.h
#define M_PARTIAL               0x00000001L // Already defined in Mil.h



/***************************************************************************/
/* MmodSave()/MmodRestore()/MmodStream()                                   */
/***************************************************************************/
#ifndef M_MIL_75_COMPATIBLE
#define M_MEMORY                         (M_APP_INQUIRE_SYS_START+120L) /*5120*/  // Already defined in Mil.h
#else
#define M_MEMORY                             5L  // Already defined in Mil.h
#endif //M_MIL_75_COMPATIBLE
#define M_INTERACTIVE                    M_NULL // Already defined in    mil.h, milcal.h, milcode.h, milmeas.h, milocr.h, milpat.h, milmod.h

#define M_SAVE                             0x100L

#ifdef M_MIL_75_COMPATIBLE
#define M_RESTORE                              0L
#else
#define M_RESTORE                           0x10L
#endif // M_MIL_75_COMPATIBLE

#define M_LOAD                                 1L
#define M_INQUIRE_SIZE_BYTE                    2L

#define M_WITH_CALIBRATION            0x00020000L // Already in mil.h


/***************************************************************************/
/*                 WARNINGS definitions and string table                   */
/***************************************************************************/
#define M_MAX_WARNING_STRING_LENGTH       64L
#define M_LAST_WARNING_VALUE              13L

/* Warning definitions */
#define M_POOR_GEOMETRIC_MODEL             1L
#define M_HIGH_GEOMETRIC_COMPLEXITY        2L
#define M_UNSTABLE_ANGLE                   3L
#define M_UNSTABLE_SCALE                   4L
#define M_UNSTABLE_POSITION                5L
#define M_BETTER_OUT_OF_ANGLE_RANGE        6L
#define M_BETTER_OUT_OF_SCALE_RANGE        7L
#define M_BETTER_OUT_OF_SEARCH_REGION      8L
#define M_POOR_GEOMETRIC_TARGET            9L
#define M_SEARCH_OUTSIDE_TARGET           11L
#define M_NO_INITIAL_CALIBRATION          12L
#define M_USER_FIRST_LEVEL_CANNOT_BE_USED 13L


/***************************************************************************/
/*                 MilMod CAPI function prototypes                         */
/***************************************************************************/

#ifndef __midl // MIDL compiler used by ActiveMIL

MFTYPE32 MIL_ID MFTYPE MmodAlloc         (MIL_ID   SystemId, 
                                          MIL_INT     MethodType, 
                                          MIL_INT     ControlFlag,
                                          MIL_ID*  ObjectIdPtr);

MFTYPE32 MIL_ID MFTYPE MmodAllocResult   (MIL_ID   SystemId,
                                          MIL_INT     ControlFlag, 
                                          MIL_ID*  ResultIdPtr);

#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs
MFTYPE32 void MFTYPE MmodDefineInt64     (MIL_ID      ContextId,
                                          MIL_INT     ModelType, 
                                          MIL_INT64   Param1,
                                          double   Param2,
                                          double   Param3,
                                          double   Param4,
                                          double   Param5);

MFTYPE32 void MFTYPE MmodDefineDouble    (MIL_ID   ContextId,
                                          MIL_INT  ModelType, 
                                          double   Param1,
                                          double   Param2,
                                          double   Param3,
                                          double   Param4,
                                          double   Param5);
#else
// Prototypes for 32 bits OSs
#define MmodDefineInt64  MmodDefine
#define MmodDefineDouble MmodDefine
MFTYPE32 void MFTYPE MmodDefine          (MIL_ID   ContextId,
                                          MIL_INT     ModelType, 
                                          double   Param1,
                                          double   Param2,
                                          double   Param3,
                                          double   Param4,
                                          double   Param5);
#endif

#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs
MFTYPE32 void MFTYPE MmodControlInt64    (MIL_ID   ContextId, 
                                          MIL_INT     Index,
                                          MIL_INT     ControlType, 
                                          MIL_INT64   ControlValue);

MFTYPE32 void MFTYPE MmodControlDouble   (MIL_ID   ContextId, 
                                          MIL_INT     Index,
                                          MIL_INT     ControlType, 
                                          double   ControlValue);

#else
// Prototypes for 32 bits OSs
#define MmodControlInt64  MmodControl
#define MmodControlDouble MmodControl
MFTYPE32 void MFTYPE MmodControl         (MIL_ID   ContextId, 
                                          MIL_INT     Index,
                                          MIL_INT     ControlType, 
                                          double   ControlValue);
#endif

MFTYPE32 MIL_INT MFTYPE MmodInquire         (MIL_ID   ContextId, 
                                          MIL_INT     Index,
                                          MIL_INT     InquireType,
                                          void*    UserVarPtr);

MFTYPE32 void MFTYPE MmodMask            (MIL_ID   ContextId,
                                          MIL_INT     Index,
                                          MIL_ID   MaskBufferId,
                                          MIL_INT     MaskType,
                                          MIL_INT     ControlFlag);

MFTYPE32 void MFTYPE MmodPreprocess      (MIL_ID   ContextId, 
                                          MIL_INT     ControlFlag);

MFTYPE32 void MFTYPE MmodFind            (MIL_ID   ContextId, 
                                          MIL_ID   TargetImageId,
                                          MIL_ID   ResultId);

MFTYPE32 void MFTYPE MmodGetResult       ( MIL_ID   ResultId, 
                                           MIL_INT     ResultIndex,
                                           MIL_INT     ResultType,
                                           void    *ResultArrayPtr );

MFTYPE32 void MFTYPE MmodGetResultOutputUnits(MIL_ID ResultId, 
                                              MIL_INT   ResultIndex,
                                              MIL_INT   ResultType,
                                              MIL_INT   OutputUnits,
                                              void*  ResultArrayPtr);

MFTYPE32 void MFTYPE MmodDraw            (MIL_ID GraphContId,
                                          MIL_ID ContextOrResultId,
                                          MIL_ID DestImageBufId, 
                                          MIL_INT   Operation,
                                          MIL_INT   Index,
                                          MIL_INT   ControlFlag);
#if M_MIL_USE_UNICODE

MFTYPE32 void MFTYPE MmodDefineFromFileW (MIL_ID         ContextId,
                                          MIL_INT        FileType,
                                          MIL_API_CONST_TEXT_PTR   FileName,
                                          MIL_INT        ControlFlag );

MFTYPE32 void MFTYPE MmodSaveW           (MIL_API_CONST_TEXT_PTR   FileName,
                                          MIL_ID         ContextId,
                                          MIL_INT        ControlFlag);

MFTYPE32 MIL_ID  MFTYPE MmodRestoreW     (MIL_API_CONST_TEXT_PTR   FileName,
                                          MIL_ID         SystemId, 
                                          MIL_INT        ControlFlag,
                                          MIL_ID*        ContextIdPtr);

MFTYPE32 void MFTYPE MmodStreamW         (MIL_TEXT_PTR   MemPtrOrFileName,
                                          MIL_ID         SystemId,
                                          MIL_INT        Operation,
                                          MIL_INT        StreamType,
                                          double         Version,
                                          MIL_INT        ControlFlag,
                                          MIL_ID         *ObjectIdPtr,
                                          MIL_INT        *SizeByteVarPtr);

MFTYPE32 void MFTYPE MmodDefineFromFileA (MIL_ID         ContextId,
                                          MIL_INT        FileType,
                                          MIL_API_CONST char*          FileName,
                                          MIL_INT        ControlFlag );

MFTYPE32 void MFTYPE MmodSaveA           (MIL_API_CONST char*          FileName,
                                          MIL_ID         ContextId,
                                          MIL_INT        ControlFlag);

MFTYPE32 MIL_ID  MFTYPE MmodRestoreA     (MIL_API_CONST char*          FileName,
                                          MIL_ID         SystemId, 
                                          MIL_INT        ControlFlag,
                                          MIL_ID*        ContextIdPtr);

MFTYPE32 void MFTYPE MmodStreamA         (char*          MemPtrOrFileName,
                                          MIL_ID         SystemId,
                                          MIL_INT        Operation,
                                          MIL_INT        StreamType,
                                          double         Version,
                                          MIL_INT        ControlFlag,
                                          MIL_ID         *ObjectIdPtr,
                                          MIL_INT        *SizeByteVarPtr);
#if M_MIL_UNICODE_API

#define MmodDefineFromFile MmodDefineFromFileW
#define MmodSave           MmodSaveW
#define MmodRestore        MmodRestoreW
#define MmodStream         MmodStreamW

#else

#define MmodDefineFromFile MmodDefineFromFileA
#define MmodSave           MmodSaveA
#define MmodRestore        MmodRestoreA
#define MmodStream         MmodStreamA

#endif

#else

MFTYPE32 void MFTYPE MmodDefineFromFile  (MIL_ID         ContextId,
                                          MIL_INT        FileType,
                                          MIL_API_CONST_TEXT_PTR   FileName,
                                          MIL_INT        ControlFlag );

MFTYPE32 void MFTYPE MmodSave            (MIL_API_CONST_TEXT_PTR   FileName,
                                          MIL_ID         ContextId,
                                          MIL_INT        ControlFlag);

MFTYPE32 MIL_ID  MFTYPE MmodRestore      (MIL_API_CONST_TEXT_PTR   FileName,
                                          MIL_ID         SystemId, 
                                          MIL_INT        ControlFlag,
                                          MIL_ID*        ContextIdPtr);

MFTYPE32 void MFTYPE MmodStream          (MIL_TEXT_PTR   MemPtrOrFileName,
                                          MIL_ID         SystemId,
                                          MIL_INT        Operation,
                                          MIL_INT        StreamType,
                                          double         Version,
                                          MIL_INT        ControlFlag,
                                          MIL_ID         *ObjectIdPtr,
                                          MIL_INT        *SizeByteVarPtr);
#endif // M_MIL_USE_UNICODE


MFTYPE32 void MFTYPE MmodFree            (MIL_ID ObjectId);

#endif /* #ifdef __midl */

/* C++ directive if needed */
#ifdef __cplusplus
}
#endif

#if M_MIL_USE_64BIT
#ifdef __cplusplus
//////////////////////////////////////////////////////////////
// MmodControl function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_MIL_USE_LINUX
inline void MmodControl(MIL_ID   ContextId, 
                        MIL_INT     Index,
                        MIL_INT     ControlType, 
                        int   ControlValue)
   {
   MmodControlInt64(ContextId, Index, ControlType, ControlValue);
   };
#endif
inline void MmodControl(MIL_ID   ContextId, 
                        MIL_INT     Index,
                        MIL_INT     ControlType, 
                        MIL_INT32   ControlValue)
   {
   MmodControlInt64(ContextId, Index, ControlType, ControlValue);
   }
inline void MmodControl(MIL_ID   ContextId, 
                        MIL_INT     Index,
                        MIL_INT     ControlType, 
                        MIL_INT64   ControlValue)
   {
   MmodControlInt64(ContextId, Index, ControlType, ControlValue);
   }
inline void MmodControl(MIL_ID   ContextId, 
                        MIL_INT     Index,
                        MIL_INT     ControlType, 
                        double   ControlValue)
   {
   MmodControlDouble(ContextId, Index, ControlType, ControlValue);
   }
//////////////////////////////////////////////////////////////
// MmodDefine function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_MIL_USE_LINUX
inline void MmodDefine(MIL_ID   ContextId,
                       MIL_INT     ModelType, 
                       int   Param1,
                       double   Param2,
                       double   Param3,
                       double   Param4,
                       double   Param5)
   {
   MmodDefineInt64(ContextId, ModelType, Param1, Param2, Param3, Param4, Param5);
   }
#endif
inline void MmodDefine(MIL_ID   ContextId,
                       MIL_INT     ModelType, 
                       MIL_INT32   Param1,
                       double   Param2,
                       double   Param3,
                       double   Param4,
                       double   Param5)
   {
   MmodDefineInt64(ContextId, ModelType, Param1, Param2, Param3, Param4, Param5);
   }
inline void MmodDefine(MIL_ID   ContextId,
                       MIL_INT     ModelType, 
                       MIL_INT64   Param1,
                       double   Param2,
                       double   Param3,
                       double   Param4,
                       double   Param5)
   {
   MmodDefineInt64(ContextId, ModelType, Param1, Param2, Param3, Param4, Param5);
   }
inline void MmodDefine(MIL_ID   ContextId,
                       MIL_INT     ModelType, 
                       double   Param1,
                       double   Param2,
                       double   Param3,
                       double   Param4,
                       double   Param5)
   {
   MmodDefineDouble(ContextId, ModelType, Param1, Param2, Param3, Param4, Param5);
   }
#else
//////////////////////////////////////////////////////////////
// For C file, call the default function, i.e. Double one
//////////////////////////////////////////////////////////////
#define MmodControl  MmodControlDouble
#define MmodDefine   MmodDefineDouble

#endif // __cplusplus
#endif // M_MIL_USE_64BIT

#if M_MIL_USE_SAFE_TYPE

//////////////////////////////////////////////////////////////
// See milos.h for explanation about these functions.
//////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------
// MmodGetResult

inline MFTYPE32 void MFTYPE MmodGetResultUnsafe  (MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, void          MPTYPE *ResultArrayPtr);
inline MFTYPE32 void MFTYPE MmodGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, int                   ResultArrayPtr);
inline MFTYPE32 void MFTYPE MmodGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_INT32     MPTYPE *ResultArrayPtr);
inline MFTYPE32 void MFTYPE MmodGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_INT64     MPTYPE *ResultArrayPtr);
inline MFTYPE32 void MFTYPE MmodGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_DOUBLE    MPTYPE *ResultArrayPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 void MFTYPE MmodGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_UINT32    MPTYPE *ResultArrayPtr);
inline MFTYPE32 void MFTYPE MmodGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_UINT64    MPTYPE *ResultArrayPtr);
#endif

// ----------------------------------------------------------
// MmodInquire

inline MFTYPE32 MIL_INT MFTYPE MmodInquireUnsafe  (MIL_ID ContextId, MIL_INT Index, MIL_INT InquireType, void          MPTYPE *ResultArrayPtr);
inline MFTYPE32 MIL_INT MFTYPE MmodInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT InquireType, int                   ResultArrayPtr);
inline MFTYPE32 MIL_INT MFTYPE MmodInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT InquireType, MIL_INT32     MPTYPE *ResultArrayPtr);
inline MFTYPE32 MIL_INT MFTYPE MmodInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT InquireType, MIL_INT64     MPTYPE *ResultArrayPtr);
inline MFTYPE32 MIL_INT MFTYPE MmodInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT InquireType, MIL_DOUBLE    MPTYPE *ResultArrayPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 MIL_INT MFTYPE MmodInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT InquireType, MIL_UINT32    MPTYPE *ResultArrayPtr);
inline MFTYPE32 MIL_INT MFTYPE MmodInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT InquireType, MIL_UINT64    MPTYPE *ResultArrayPtr);
#endif

// ----------------------------------------------------------
// MmodGetResult

inline MFTYPE32 void MFTYPE MmodGetResultSafeType (MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, int ResultPtr)
   {
   if (ResultPtr)
      SafeTypeError(MT("MmodGetResult"));

   MmodGetResult(ResultId, Index, ResultType, NULL);
   }

inline void MmodGetResultSafeTypeExecute (MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, void       MPTYPE *ResultPtr, MIL_INT GivenType)
   {
   MIL_INT RequiredType = (ResultType & (M_TYPE_MIL_ID | M_TYPE_MIL_INT32 | M_TYPE_MIL_INT64 | M_TYPE_DOUBLE | M_TYPE_MIL_ID));
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MT("MmodGetResult"));

   MmodGetResult(ResultId, Index, ResultType, ResultPtr);
   }

inline MFTYPE32 void   MFTYPE MmodGetResultUnsafe  (MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, void       *ResultPtr) {MmodGetResult(ResultId, Index, ResultType, ResultPtr);}
inline MFTYPE32 void   MFTYPE MmodGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_INT32  *ResultPtr) {MmodGetResultSafeTypeExecute(ResultId, Index, ResultType, ResultPtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 void   MFTYPE MmodGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_INT64  *ResultPtr) {MmodGetResultSafeTypeExecute(ResultId, Index, ResultType, ResultPtr, M_TYPE_MIL_INT64);}
inline MFTYPE32 void   MFTYPE MmodGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_DOUBLE *ResultPtr) {MmodGetResultSafeTypeExecute(ResultId, Index, ResultType, ResultPtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 void   MFTYPE MmodGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_UINT32 *ResultPtr) {MmodGetResultSafeTypeExecute(ResultId, Index, ResultType, ResultPtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 void   MFTYPE MmodGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT ResultType, MIL_UINT64 *ResultPtr) {MmodGetResultSafeTypeExecute(ResultId, Index, ResultType, ResultPtr, M_TYPE_MIL_INT64);}
#endif

// ----------------------------------------------------------
// MmodInquire

inline MFTYPE32 MIL_INT MFTYPE MmodInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT InquireType, int UserVarPtr)
   {
   if (UserVarPtr)
      SafeTypeError(MT("MmodInquire"));

   return MmodInquire(ContextId, Index, InquireType, NULL);
   }

inline MFTYPE32 MIL_INT MFTYPE MmodInquireSafeTypeExecute (MIL_ID ContextId, MIL_INT Index, MIL_INT InquireType, void *UserVarPtr, MIL_INT GivenType)
   {
   MIL_INT RequiredType = (InquireType & (M_TYPE_MIL_INT32 | M_TYPE_MIL_INT64 | M_TYPE_DOUBLE | M_TYPE_MIL_ID ));
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MT("MmodInquire"));

   return MmodInquire(ContextId, Index, InquireType, UserVarPtr);
   }

inline MFTYPE32 MIL_INT MFTYPE MmodInquireUnsafe  (MIL_ID ContextId, MIL_INT Index, MIL_INT InquireType, void       *UserVarPtr) {return MmodInquire               (ContextId, Index, InquireType, UserVarPtr                  );}
inline MFTYPE32 MIL_INT MFTYPE MmodInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT InquireType, MIL_INT32  *UserVarPtr) {return MmodInquireSafeTypeExecute(ContextId, Index, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 MIL_INT MFTYPE MmodInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT InquireType, MIL_INT64  *UserVarPtr) {return MmodInquireSafeTypeExecute(ContextId, Index, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
inline MFTYPE32 MIL_INT MFTYPE MmodInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT InquireType, MIL_DOUBLE *UserVarPtr) {return MmodInquireSafeTypeExecute(ContextId, Index, InquireType, UserVarPtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 MIL_INT MFTYPE MmodInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT InquireType, MIL_UINT32 *UserVarPtr) {return MmodInquireSafeTypeExecute(ContextId, Index, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 MIL_INT MFTYPE MmodInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT InquireType, MIL_UINT64 *UserVarPtr) {return MmodInquireSafeTypeExecute(ContextId, Index, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
#endif

#define MmodGetResult        MmodGetResultSafeType
#define MmodInquire          MmodInquireSafeType

#else // #if M_MIL_USE_SAFE_TYPE

#define MmodGetResultUnsafe        MmodGetResult
#define MmodInquireUnsafe          MmodInquire

#endif // #if M_MIL_USE_SAFE_TYPE

#endif // !M_MIL_LITE

#endif /* __MILMOD_H__ */
