/***************************************************************************/
/*

    Filename:  MILMEAS.H
    Owner   :  Matrox Imaging dept.
    Revision:  9.02.0580
    Content :  This file contains the defines for the MIL measurement
               module. (Mmeas...).

    Copyright © Matrox Electronic Systems Ltd., 1992-2010.
    All Rights Reserved

*/
/***************************************************************************/

#ifndef __MILMEAS_H__
#define __MILMEAS_H__

#define BW_COMPATIBILITY 0x092

#if (!M_MIL_LITE) // MIL FULL ONLY

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif

////////////////////////////////////////////////////////////////////////////
// WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
//
// If we get any warning with any of these defines, then there are
// important changes to do with the Save and Restore functions.
//
// When it happens, you will have to declare the current define as "old"
// and redeclare the define with the new value. Then you will have to
// modify both the Save and the Restore functions so that they use the good
// define depending on the version.
// This is important because it will affect both backward and forward
// compatibility with other versions of MIL.
//
// WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
////////////////////////////////////////////////////////////////////////////

/**************************************************************************/
/* CAPI defines                                                           */
/**************************************************************************/

/**************************************************************************/
/* MmeasAllocMarker                                                       */
/**************************************************************************/

#define M_POINT                           1L
#define M_EDGE                            2L
#define M_STRIPE                          3L
#define M_CIRCLE                 0x00000008L   // 8   Already defined in milmod.h
#define M_EDGE_LIST                   M_EDGE
#define M_STRIPE_LIST               M_STRIPE


/**************************************************************************/
/* MmeasAllocResult                                                       */
/**************************************************************************/
#define M_CALCULATE                       1L   // Already defined in mil.h


/**************************************************************************/
/* Bitwise values that the followings cannot take                         */
/**************************************************************************/

//#define M_MULTI_MARKER_MASK               0x000003FFL not used anymore, could be confusing
#define M_EDGE_FIRST                      0x20000000L//0x00100000L 
#define M_EDGE_SECOND                     0x40000000L//0x00200000L  
#define M_WEIGHT_FACTOR                   0x01000000L
#define M_MIN                             0x02000000L
#define M_MAX                             0x04000000L
#define M_MEAN                            0x08000000L
#define M_STANDARD_DEVIATION              0x10000000L 
#define M_MEAS_FUTURE_USE_1               0x20000000L 
#define M_MEAS_FUTURE_USE_2               0x40000000L 


/**************************************************************************/
/* MmeasInquire(), MmeasSetMarker(), MmeasGetResult(), MmeasFindMarker(), */
/* MmeasCalculate() parameters :                                          */
/**************************************************************************/

#define M_POSITION_VARIATION              0x00008000L
#define M_ANGLE                           0x00000800L
#define M_WIDTH                           0x00010000L
#define M_WIDTH_VARIATION                 0x00020000L
#define M_POLARITY                        0x00004000L
#define M_CONTRAST                        0x00001000L
#define M_LENGTH                          0x00002000L
#define M_LINE_EQUATION                   0x00800000L
#define M_LINE_EQUATION_SLOPE             0x00801000L
#define M_LINE_EQUATION_INTERCEPT         0x00802000L
#define M_LINE_A                          0x00003000L // also defined in milmetrol.h
#define M_LINE_B                          0x00003001L // also defined in milmetrol.h
#define M_LINE_C                          0x00003002L // also defined in milmetrol.h
#define M_EDGE_INSIDE                     0x00000080L
#define M_POSITION                        0x00000402L
#define M_POSITION_IN_MARKER                    1377L
#define M_INCLUSION_POINT                       1581L
#define M_SPACING                         0x00000200L
#define M_NUMBER                                1009L
#define M_NUMBER_MIN                            1010L
#define M_FIT_ERROR                       0x00001700L  // already defined in MilMod.h
#define M_FIT_ERROR_MAX                         1378L
#define M_SEARCH_REGION_INPUT_UNITS             1392L  // 0x00000570L 
#define M_BOX_INPUT_UNITS M_SEARCH_REGION_INPUT_UNITS  // Deprecated but released in PP2 EA2, here for PP2 EA2 backward compatibility
#define M_MARKER_REFERENCE_INPUT_UNITS          1393L  // 0x00000571L 
#define M_POINT_INPUT_UNITS                     1394L  // 0x00000572L
#define M_INCLUSION_POINT_INPUT_UNITS           1459L
#define M_MAX_ASSOCIATION_DISTANCE_INPUT_UNITS  1584L
#define M_SPACING_VARIATION               0x00000100L      
#define M_SCORE                           0x00001400L
#define M_ORIENTATION                     0x00002400L
#define M_POSITION_X                      0x00003400L  // deprecated in MIL 9.0 PP2
#define M_POSITION_Y                      0x00004400L  // deprecated in MIL 9.0 PP2
#define M_CONTRAST_VARIATION              0x00005400L
#define M_EDGE_STRENGTH                   0x00006400L
#define M_EDGE_STRENGTH_VARIATION         0x00007400L
#define M_EDGE_INSIDE_VARIATION           0x00008400L
#define M_BOX_ORIGIN                      0x00009400L
#define M_BOX_SIZE                        0x0000A400L
#define M_BOX_CENTER                      0x0000B400L    // applies to edges and stripes
#define M_RING_CENTER                     M_BOX_CENTER   // applies to circles
#define M_BOX_FIRST_CENTER                0x0000C400L       // to be deprecated
#define M_BOX_SECOND_CENTER               0x0000D400L       // to be deprecated
#define M_BOX_ANGLE_MODE                  0x0000E400L
#define M_BOX_ANGLE                       0x0000F400L
#define M_MULTIPLE_POINT_ANGLE            M_BOX_ANGLE // New flag name since MIL 9.0 PP2 for point markers, need to be at the same value as M_BOX_ANGLE 
#define M_BOX_ANGLE_DELTA_NEG             0x00010400L
#define M_BOX_ANGLE_DELTA_POS             0x00011400L
#define M_BOX_ANGLE_TOLERANCE             0x00012400L
#define M_BOX_ANGLE_ACCURACY              0x00013400L
#define M_SEARCH_REGION_ANGLE_INTERPOLATION_MODE   0x00014400L                      // New name in PP2
#define M_BOX_ANGLE_INTERPOLATION_MODE    M_SEARCH_REGION_ANGLE_INTERPOLATION_MODE  // Deprecated in PP2
#define M_BOX_INTERPOLATION_MODE          M_SEARCH_REGION_ANGLE_INTERPOLATION_MODE  // Deprecated in PP2
#define M_EDGE_THRESHOLD                  0x00015400L
#define M_MARKER_REFERENCE                0x00016400L
#define M_BOX_ANGLE_SIZE                  0x00017400L
#define M_MARKER_TYPE                     0x00018400L
#define M_CONTROL_FLAG                    0x00019400L
#define M_POSITION_MIN                    0x0001A400L
#define M_POSITION_MAX                    0x0001B400L
#define M_BOX_EDGE_VALUES                 0x0001C400L
#define M_ANY_ANGLE                       0x0001D400L
#define M_FOUND_BOX_ANGLE                 M_ANY_ANGLE
#define M_VALID_FLAG                      0x0001E400L
#define M_BOX_CORNER_TOP_LEFT             0x0001F400L
#define M_BOX_CORNER_TOP_RIGHT            0x00020400L
#define M_BOX_CORNER_BOTTOM_LEFT          0x00021400L
#define M_BOX_CORNER_BOTTOM_RIGHT         0x00022400L
#define M_BOX_EDGE_VALUES_NUMBER          0x00023400L
#define M_INCLUSION_POINT_INSIDE_STRIPE   0x00024400L
#define M_POSITION_INSIDE_STRIPE          M_INCLUSION_POINT_INSIDE_STRIPE  // Deprecated in PP2
#define M_BOX_ANGLE_REFERENCE             0x00025400L
#define M_TOTAL_SCORE                     0x00029400L
#define M_BOX_RADIUS                      0x0002A400L
#define M_BOX_START_ANGLE                 0x0002B400L
#define M_BOX_END_ANGLE                   0x0002C400L
#define M_ALL                             0x40000000L // Already defined in Mil.h
#define M_ALL_OLD                                  0L

#define M_RING_RADII                      0x0002F400L
#define M_BOX_RING_RADIUS                 M_RING_RADII   // Deprecated but released in PP2 EA2, here for PP2 EA2 backward compatibility

#define M_RESULT_OUTPUT_UNITS                   1300L    // Also in milim.h
#define M_NUMBER_OF_OUTLIERS                    1379L
#define M_SEARCH_REGION_WAS_CLIPPED             1467L                         // New in PP2
#define M_BOX_WAS_CLIPPED                       M_SEARCH_REGION_WAS_CLIPPED   // deprecated in MIL PP2

#define M_MAJOR_AXIS_MIN                         214L       // Already defined in MilMod.h
#define M_MAJOR_AXIS                             215L       // Already defined in MilMod.h 
#define M_MAJOR_AXIS_MAX                         216L       // Already defined in MilMod.h                         
#define M_RADIUS                          M_MAJOR_AXIS      // Already defined in MilMod.h
#define M_RADIUS_MIN                      M_MAJOR_AXIS_MIN  // Already defined in MilMod.h
#define M_RADIUS_MAX                      M_MAJOR_AXIS_MAX  // Already defined in MilMod.h
#define M_RADIUS_VARIATION                       217L   
#define M_SUB_EDGES_NUMBER                       218L
#define M_SUB_REGIONS_NUMBER              M_SUB_EDGES_NUMBER
#define M_SUB_EDGES_NUMBER_MIN                   219L
#define M_SUB_EDGES_POSITION                     220L
#define M_SUB_EDGES_WEIGHT                       222L
#define M_SUB_EDGES_MARKER_INDEX                 223L
#define M_SUB_EDGES_INDEX                        M_SUB_EDGES_MARKER_INDEX // Deprecated name since MIL 9.0 PP2 (needed for all MIL 9 PPs, to be removed in MIL 10)
#define M_SUB_EDGES_OFFSET                       224L
#define M_SUB_EDGES_STRENGTH                     227L
#define M_SUB_EDGES_CONTRAST                     228L
#define M_SUB_EDGES_FIT_ERROR                    229L

#define M_SUB_REGIONS_OFFSET              M_SUB_EDGES_OFFSET
#define M_SUB_EDGES_SIZE                         225L
#define M_SUB_REGIONS_SIZE                M_SUB_EDGES_SIZE
#define M_SUB_REGIONS_CHORD_ANGLE                226L                      // New in PP2
#define M_SUB_BOX_MAXIMUM_ANGLE                  M_SUB_REGIONS_CHORD_ANGLE // Deprecated but released in PP2 EA2, here for PP2 EA2 backward compatibility
#define M_MAX_ASSOCIATION_DISTANCE              1503L

#define M_CONTROL_FLAG                    0x00019400L
#define M_ZERO_OFFSET_X                            1L
#define M_ZERO_OFFSET_Y                            2L
#define M_PIXEL_ASPECT_RATIO                       5L
#define M_PIXEL_ASPECT_RATIO_INPUT                 6L
#define M_PIXEL_ASPECT_RATIO_OUTPUT                7L
#define M_DRAW_RELATIVE_ORIGIN_X                 319L        // Already defined in    milmod.h
#define M_DRAW_RELATIVE_ORIGIN_Y                 320L        // Already defined in    milmod.h
#define M_DRAW_SCALE_X                          3203L       // Already defined in    milmod.h
#define M_DRAW_SCALE_Y                          3204L       // Already defined in    milmod.h

#define M_RESULT_TYPE                              1L

#define M_DISTANCE                        0x00080000L       // Also defined in milim.h
#define M_DISTANCE_X                      0x00081000L
#define M_DISTANCE_Y                      0x00082000L
#define M_ANGLE                           0x00000800L
#define M_LINE_EQUATION                   0x00800000L
#define M_LINE_EQUATION_SLOPE             0x00801000L
#define M_LINE_EQUATION_INTERCEPT         0x00802000L

#define M_LINE_END_POINT_FIRST            0x00807000L
#define M_FIRST_END_POINT_LINE            M_LINE_END_POINT_FIRST  // Deprecated synonym since MIL 9.0 PP2 (needed for all MIL 9 PPs, to be removed in MIL 10)

#define M_LINE_END_POINT_SECOND           0x00808000L
#define M_SECOND_END_POINT_LINE           M_LINE_END_POINT_SECOND // Deprecated synonym since MIL 9.0 PP2 (needed for all MIL 9 PPs, to be removed in MIL 10)

#define M_SEARCH_REGION_CLIPPING          0x00809000L                // New name in PP2
#define M_BOX_CLIPPING                    M_SEARCH_REGION_CLIPPING   // Deprecated but released in PP2 EA2, here for PP2 EA2 backward compatibility

/* Score flags */
#define M_MAX_POSSIBLE_VALUE              0x10000001L
#define M_STRENGTH_SCORE                  0x10100000L
#define M_RADIUS_SCORE                    0x10200000L
#define M_CONTRAST_SCORE                  0x10300000L
#define M_POSITION_IN_MARKER_SCORE        0x10600000L
#define M_SPACING_SCORE                   0x10700000L
#define M_WIDTH_SCORE                     0x10800000L
#define M_EDGE_INSIDE_SCORE               0x10900000L

/* Internal */
#define M_REF_POSITION_MARKER_FIRST       0x00803000L
#define M_REF_POSITION_MARKER_SECOND      0x00804000L
#define M_ASPECT_RATIO_MARKER_FIRST       0x00805000L
#define M_ASPECT_RATIO_MARKER_SECOND      0x00806000L
#define M_CIRCLE_ACCURACY                 0x00030400L
#define M_SCORE_BACKWARD_COMPATIBILITY    0x00031400L
#define M_MAX_ITERATIONS                        1504L
#define M_INCLUSION_POINT_X                     1582L
#define M_INCLUSION_POINT_Y                     1583L

/* Internal result */
#define M_SUB_REGIONS_NUMBER_INTERNAL     0x0002D400L


/**************************************************************************/
/* MmeasInquire(), MmeasSetMarker(), MmeasGetResult(), MmeasFindMarker(), */
/* MmeasCalculate() values :                                              */
/**************************************************************************/

#define M_VERTICAL                      1L
#define M_HORIZONTAL                    2L
#define M_ANY                  0x11000000L
#define M_POSITIVE                      2L
#define M_NEGATIVE                     -2L /*Must be the additive inverse of M_POSITIVE*/
#define M_OPPOSITE                      3L
#define M_SAME                         -1L /* For spacing */
#ifndef M_MIL_75_COMPATIBLE
#define M_NORMAL                        8L
#else
#define M_NORMAL                        1L
#endif //M_MIL_75_COMPATIBLE
#define M_CORRECTED                     2L
#define M_DISABLE                   -9999L
#define M_ENABLE                    -9997L
#define M_NEAREST_NEIGHBOR     0x00000040L
#define M_BILINEAR             0x00000008L
#define M_BICUBIC              0x00000010L
#define M_YES                           1L
#define M_NO                            0L
#define M_TRUE                          1L
#define M_FALSE                         0L
#define M_INVALID                      -1
#define M_FILTER_MODE                 122L         // Already defined in milmod.h
#define M_SMOOTHNESS                  108L         // Already defined in milmod.h
#define M_FILTER_SMOOTHNESS           M_SMOOTHNESS // Already defined in miledge.h
#define M_SHEN                        0x802L
#define M_PREWITT                     M_EDGE_DETECT2 // Already defined in mil.h
#define M_EULER                       0x806L

// Parameters for output units
#define M_PIXEL                              0x1000L
#define M_WORLD                              0x2000L
#define M_ACCORDING_TO_CALIBRATION             1301L
#define M_ACCORDING_TO_RESULT                  1302L

/**************************************************************************/
/* MmeasDraw()                                                            */
/**************************************************************************/

#define M_DRAW_SEARCH_REGION_CENTER    0x00000001L       // new name in PP2
#define M_DRAW_BOX_CENTER              0x00000001L       // deprecated for meas in PP2, but used by Mblobdraw (M_DRAW_SEARCH_REGION_CENTER must equal M_DRAW_BOX_CENTER to be 'backward compatible')
#define M_DRAW_LINE                0x00000002L
#define M_DRAW_EDGES               0x00000004L
#define M_DRAW_WIDTH               0x00000008L
#define M_DRAW_POSITION_VARIATION  0x00000010L
#define M_DRAW_SEARCH_REGION       0x00000020L           // New name in PP2
#define M_DRAW_BOX                 0x00000020L           // deprecated for meas in PP2, M_DRAW_BOX also used by Mblobdraw and MpatDraw and Mmoddraw (M_DRAW_SEARCH_REGION must equal M_DRAW_BOX to be 'backward compatible')
#define M_DRAW_POSITION            0x00000040L    // also used by Mblobdraw and MpatDraw and Mmoddraw 
#define M_DRAW_EDGES_PROFILE       0x00000080L
#define M_DRAW_SPACING             0x00000100L
#define M_DRAW_WIDTH_VARIATION     0x00000200L
#define M_DRAW_SUB_POSITIONS       0x00000400L
#define M_DRAW_ARROW               0X00000800L
#define M_DRAW_ALL_SUB_POSITIONS   0x00001000L
#define M_DRAW_IN_BOX              0x00100000L

#define M_RESULT                   0x00001000L    // Also used by MmodDraw 
#define M_MARKER                   0x000001F8L

#define M_ALL                      0x40000000L    // Also used by MmodDraw

/* Internal result */
#define M_DRAW_SUB_REGION_BOX      0x00002000L

/**************************************************************************/
/* MmeasRestoreMarker(), MmeasSaveMarker                                  */
/**************************************************************************/
#define M_INTERACTIVE                      M_NULL // Already defined in mil.h, milcal.h, milcode.h, miledge.h, milocr.h, milpat.h, milmod.h


/**************************************************************************/
/* Utility defines                                                        */
/**************************************************************************/
#define M_INFINITE_SLOPE      MIL_INT32_MAX


// !!! MAP FOR OLD DEFINES                                
#if OldDefinesSupport                                     
#define M_BOX_EDGES_STRENGTH              M_BOX_EDGE_VALUES
#define M_BOX_EDGES_STRENGTH_NUMBER       M_BOX_EDGE_VALUES_NUMBER
#endif
// !!! MAP FOR OLD DEFINES                                

/**************************************************************************/
/* Function prototypes                                                    */
/**************************************************************************/

#ifndef __midl // MIDL compiler used by ActiveMIL

MFTYPE32 MIL_ID MFTYPE MmeasAllocMarker  (MIL_ID SystemId,
                                          MIL_INT MarkerType,
                                          MIL_INT ControlFlag,
                                          MIL_ID *MarkerIdPtr);

MFTYPE32 MIL_ID MFTYPE MmeasAllocResult  (MIL_ID SystemId,
                                          MIL_INT ResultBufferType,
                                          MIL_ID MPTYPE *ResultIdPtr);

MFTYPE32 MIL_ID MFTYPE MmeasAllocContext (MIL_ID SystemId,
                                          MIL_INT ControlFlag,
                                          MIL_ID *ContextId);

MFTYPE32 void   MFTYPE MmeasFree         (MIL_ID MarkerOrResultIdOrContextId);


MFTYPE32 void   MFTYPE MmeasSetMarker    (MIL_ID MarkerId,
                                          MIL_INT Parameter,
                                          double FirstValue,
                                          double SecondValue);

MFTYPE32 void MFTYPE MmeasSetScore       (MIL_ID     MarkerId,
                                          MIL_INT    Characteristic,
                                          MIL_DOUBLE Min,
                                          MIL_DOUBLE Low,
                                          MIL_DOUBLE High,
                                          MIL_DOUBLE Max,                                          
                                          MIL_DOUBLE Weight,
                                          MIL_INT    InputUnits,
                                          MIL_INT    ControlFlag);

MFTYPE32 void MFTYPE MmeasGetScore       (MIL_ID      MarkerId,
                                          MIL_INT     Characteristic,
                                          MIL_INT     Index,
                                          MIL_DOUBLE* Param1,
                                          MIL_DOUBLE* Param2,
                                          MIL_DOUBLE* Param3,
                                          MIL_DOUBLE* Param4,
                                          MIL_DOUBLE* Param5,
                                          MIL_INT*    InputUnits,
                                          MIL_INT     ControlFlag);

MFTYPE32 void MFTYPE MmeasGetScoreOutputUnits(MIL_ID      MarkerId,
                                              MIL_INT     Characteristic,
                                              MIL_INT     Index,
                                              MIL_INT     OutputUnits,
                                              MIL_DOUBLE* Param1,
                                              MIL_DOUBLE* Param2,
                                              MIL_DOUBLE* Param3,
                                              MIL_DOUBLE* Param4,
                                              MIL_DOUBLE* Param5,
                                              MIL_INT*    InputUnits,
                                              MIL_INT     ControlFlag);

MFTYPE32 void   MFTYPE MmeasFindMarker   (MIL_ID ContextId,
                                          MIL_ID ImageId,
                                          MIL_ID MarkerId,
                                          MIL_INT   MeasurementList);

MFTYPE32 void   MFTYPE MmeasCalculate    (MIL_ID ContextId,
                                          MIL_ID Marker1Id,
                                          MIL_ID Marker2Id,
                                          MIL_ID ResultId,
                                          MIL_INT   MeasurementList);

MFTYPE32 void   MFTYPE MmeasGetResult    (MIL_ID   MarkerOrResultId,
                                          MIL_INT  ResultType,
                                          void*    FirstResultPtr,
                                          void*    SecondResultPtr);

MFTYPE32 void   MFTYPE MmeasGetResultOutputUnits(MIL_ID   MarkerOrResultId,
                                                 MIL_INT  ResultType,
                                                 MIL_INT  OutputUnits,
                                                 void*    FirstResultPtr,
                                                 void*    SecondResultPtr);

MFTYPE32 void MFTYPE MmeasGetResultSingle(MIL_ID   MarkerOrResultId,
                                          MIL_INT  ResultType,
                                          void*    FirstResultPtr,
                                          void*    SecondResultPtr,
                                          MIL_INT  Index);

MFTYPE32 void MFTYPE MmeasGetResultSingleOutputUnits(MIL_ID   MarkerOrResultId,
                                                     MIL_INT  ResultType,
                                                     MIL_INT  OutputUnits,
                                                     void*    FirstResultPtr,
                                                     void*    SecondResultPtr,
                                                     MIL_INT  Index);

MFTYPE32 MIL_INT   MFTYPE MmeasInquire   (MIL_ID   MarkerIdOrResultIdOrContextId,
                                          MIL_INT  ParamToInquire,
                                          void*    FirstValuePtr,
                                          void*    SecondValuePtr);

#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs
MFTYPE32 void   MFTYPE MmeasControlInt64 (MIL_ID ContextOrResultId,
                                          MIL_INT   ControlType,
                                          MIL_INT64 Value);

MFTYPE32 void   MFTYPE MmeasControlDouble(MIL_ID ContextOrResultId,
                                          MIL_INT   ControlType,
                                          double Value);
#else
// Prototypes for 32 bits OSs
#define MmeasControlInt64  MmeasControl
#define MmeasControlDouble MmeasControl
MFTYPE32 void   MFTYPE MmeasControl      (MIL_ID ContextOrResultId,
                                          MIL_INT ControlType,
                                          double Value);
#endif

MFTYPE32 void MFTYPE MmeasDraw           (MIL_ID GraphContId,
                                          MIL_ID MarkerOrResultId,
                                          MIL_ID DestImageId,
                                          MIL_INT Operation,
                                          MIL_INT Index,
                                          MIL_INT ControlFlag);

#if M_MIL_USE_UNICODE

MFTYPE32 void   MFTYPE MmeasSaveMarkerW  (MIL_API_CONST_TEXT_PTR FileName,
                                          MIL_ID       MarkerId,
                                          MIL_INT      ControlFlag);

MFTYPE32 MIL_ID MFTYPE MmeasRestoreMarkerW(MIL_API_CONST_TEXT_PTR FileName,
                                           MIL_ID       SystemId,
                                           MIL_INT      ControlFlag,
                                           MIL_ID*      MarkerIdPtr);

MFTYPE32 void MFTYPE MmeasStreamW        (MIL_TEXT_PTR MemPtrOrFileName,
                                          MIL_ID       SystemId,
                                          MIL_INT      Operation,
                                          MIL_INT      StreamType,
                                          double       Version,
                                          MIL_INT      ControlFlag,
                                          MIL_ID*      ObjectIdPtr,
                                          MIL_INT*     SizeByteVarPtr);

MFTYPE32 void   MFTYPE MmeasSaveMarkerA  (MIL_API_CONST char*        FileName,
                                          MIL_ID       MarkerId,
                                          MIL_INT      ControlFlag);

MFTYPE32 MIL_ID MFTYPE MmeasRestoreMarkerA(MIL_API_CONST char*       FileName,
                                           MIL_ID      SystemId,
                                           MIL_INT     ControlFlag,
                                           MIL_ID*     MarkerIdPtr);

MFTYPE32 void MFTYPE MmeasStreamA        (char*        MemPtrOrFileName,
                                          MIL_ID       SystemId,
                                          MIL_INT      Operation,
                                          MIL_INT      StreamType,
                                          double       Version,
                                          MIL_INT      ControlFlag,
                                          MIL_ID*      ObjectIdPtr,
                                          MIL_INT*     SizeByteVarPtr);

#if M_MIL_UNICODE_API

#define MmeasSaveMarker    MmeasSaveMarkerW
#define MmeasRestoreMarker MmeasRestoreMarkerW
#define MmeasStream        MmeasStreamW

#else

#define MmeasSaveMarker    MmeasSaveMarkerA
#define MmeasRestoreMarker MmeasRestoreMarkerA
#define MmeasStream        MmeasStreamA

#endif

#else

MFTYPE32 void   MFTYPE MmeasSaveMarker   (MIL_API_CONST_TEXT_PTR FileName,
                                          MIL_ID       MarkerId,
                                          MIL_INT      ControlFlag);

MFTYPE32 MIL_ID MFTYPE MmeasRestoreMarker(MIL_API_CONST_TEXT_PTR FileName,
                                          MIL_ID       SystemId,
                                          MIL_INT      ControlFlag,
                                          MIL_ID*      MarkerIdPtr);

MFTYPE32 void MFTYPE MmeasStream         (MIL_TEXT_PTR MemPtrOrFileName,
                                          MIL_ID       SystemId,
                                          MIL_INT      Operation,
                                          MIL_INT      StreamType,
                                          double       Version,
                                          MIL_INT      ControlFlag,
                                          MIL_ID*      ObjectIdPtr,
                                          MIL_INT*     SizeByteVarPtr);
#endif // M_MIL_USE_UNICODE

#endif /* #ifdef __midl */



/* C++ directive if needed */
#ifdef __cplusplus
}
#endif

#if M_MIL_USE_64BIT
#ifdef __cplusplus
//////////////////////////////////////////////////////////////
// MmeasControl function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_MIL_USE_LINUX
inline void MmeasControl (MIL_ID ContextOrResultId,
                          MIL_INT ControlType,
                          int  Value)
   {
   MmeasControlInt64(ContextOrResultId, ControlType, Value);
   };
#endif

inline void MmeasControl (MIL_ID ContextOrResultId,
                          MIL_INT   ControlType,
                          MIL_INT32 Value)
   {
   MmeasControlInt64(ContextOrResultId, ControlType, Value);
   };

inline void MmeasControl(MIL_ID ContextOrResultId,
                         MIL_INT   ControlType,
                         MIL_INT64 Value)
      {
      MmeasControlInt64(ContextOrResultId, ControlType, Value);
      };

inline void MmeasControl (MIL_ID ContextOrResultId,
                          MIL_INT   ControlType,
                          double Value)
         {
         MmeasControlDouble(ContextOrResultId, ControlType, Value);
         };
#else
//////////////////////////////////////////////////////////////
// For C file, call the default function, i.e. Int64 one
//////////////////////////////////////////////////////////////
#define MmeasControl  MmeasControlDouble

#endif // __cplusplus
#endif // M_MIL_USE_64BIT

#if M_MIL_USE_SAFE_TYPE

//////////////////////////////////////////////////////////////
// See milos.h for explanation about these functions.
//////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------
// MmeasGetResult

inline MFTYPE32 void MFTYPE MmeasGetResultUnsafe  (MIL_ID MmeasResultId, MIL_INT ResultType, void          MPTYPE *FirstResultArrayPtr, void          MPTYPE *SecondResultArrayPtr);
inline MFTYPE32 void MFTYPE MmeasGetResultSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_INT32     MPTYPE *FirstResultArrayPtr, int                   SecondResultArrayPtr);
inline MFTYPE32 void MFTYPE MmeasGetResultSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_INT64     MPTYPE *FirstResultArrayPtr, int                   SecondResultArrayPtr);
inline MFTYPE32 void MFTYPE MmeasGetResultSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_DOUBLE    MPTYPE *FirstResultArrayPtr, int                   SecondResultArrayPtr);
inline MFTYPE32 void MFTYPE MmeasGetResultSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, int                   FirstResultArrayPtr, int                   SecondResultArrayPtr);
inline MFTYPE32 void MFTYPE MmeasGetResultSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_INT32     MPTYPE *FirstResultArrayPtr, MIL_INT32     MPTYPE *SecondResultArrayPtr);
inline MFTYPE32 void MFTYPE MmeasGetResultSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_INT64     MPTYPE *FirstResultArrayPtr, MIL_INT64     MPTYPE *SecondResultArrayPtr);
inline MFTYPE32 void MFTYPE MmeasGetResultSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_DOUBLE    MPTYPE *FirstResultArrayPtr, MIL_DOUBLE    MPTYPE *SecondResultArrayPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 void MFTYPE MmeasGetResultSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_UINT32    MPTYPE *FirstResultArrayPtr, int                   SecondResultArrayPtr);
inline MFTYPE32 void MFTYPE MmeasGetResultSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_UINT64    MPTYPE *FirstResultArrayPtr, int                   SecondResultArrayPtr);
inline MFTYPE32 void MFTYPE MmeasGetResultSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_UINT32    MPTYPE *FirstResultArrayPtr, MIL_UINT32    MPTYPE *SecondResultArrayPtr);
inline MFTYPE32 void MFTYPE MmeasGetResultSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_UINT64    MPTYPE *FirstResultArrayPtr, MIL_UINT64    MPTYPE *SecondResultArrayPtr);
#endif

//-------------------------------------------------------------------------------------
// MmeasGetResultSingle

inline MFTYPE32 void MFTYPE MmeasGetResultSingleUnsafe  (MIL_ID MmeasResultId, MIL_INT ResultType, void          MPTYPE *FirstResultArrayPtr, void          MPTYPE *SecondResultArrayPtr, MIL_INT ResultIndex);
inline MFTYPE32 void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_INT32     MPTYPE *FirstResultArrayPtr, int                   SecondResultArrayPtr, MIL_INT ResultIndex);
inline MFTYPE32 void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_INT64     MPTYPE *FirstResultArrayPtr, int                   SecondResultArrayPtr, MIL_INT ResultIndex);
inline MFTYPE32 void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_DOUBLE    MPTYPE *FirstResultArrayPtr, int                   SecondResultArrayPtr, MIL_INT ResultIndex);
inline MFTYPE32 void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, int                   FirstResultArrayPtr, int                   SecondResultArrayPtr, MIL_INT ResultIndex);
inline MFTYPE32 void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_INT32     MPTYPE *FirstResultArrayPtr, MIL_INT32     MPTYPE *SecondResultArrayPtr, MIL_INT ResultIndex);
inline MFTYPE32 void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_INT64     MPTYPE *FirstResultArrayPtr, MIL_INT64     MPTYPE *SecondResultArrayPtr, MIL_INT ResultIndex);
inline MFTYPE32 void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_DOUBLE    MPTYPE *FirstResultArrayPtr, MIL_DOUBLE    MPTYPE *SecondResultArrayPtr, MIL_INT ResultIndex);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_UINT32    MPTYPE *FirstResultArrayPtr, int                   SecondResultArrayPtr, MIL_INT ResultIndex);
inline MFTYPE32 void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_UINT64    MPTYPE *FirstResultArrayPtr, int                   SecondResultArrayPtr, MIL_INT ResultIndex);
inline MFTYPE32 void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_UINT32    MPTYPE *FirstResultArrayPtr, MIL_UINT32    MPTYPE *SecondResultArrayPtr, MIL_INT ResultIndex);
inline MFTYPE32 void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_UINT64    MPTYPE *FirstResultArrayPtr, MIL_UINT64    MPTYPE *SecondResultArrayPtr, MIL_INT ResultIndex);
#endif

//-------------------------------------------------------------------------------------
// MmeasInquire

inline MFTYPE32 MIL_INT MFTYPE MmeasInquireUnsafe  (MIL_ID MmeasId, MIL_INT InquireType, void          MPTYPE *FirstUserArrayPtr, void          MPTYPE *SecondUserArrayPtr);
inline MFTYPE32 MIL_INT MFTYPE MmeasInquireUnsafe  (MIL_ID MmeasId, MIL_INT InquireType, int                   FirstUserArrayPtr, int                   SecondUserArrayPtr);
inline MFTYPE32 MIL_INT MFTYPE MmeasInquireSafeType(MIL_ID MmeasId, MIL_INT InquireType, MIL_INT32     MPTYPE *FirstUserArrayPtr, MIL_INT32     MPTYPE *SecondUserArrayPtr);
inline MFTYPE32 MIL_INT MFTYPE MmeasInquireSafeType(MIL_ID MmeasId, MIL_INT InquireType, MIL_INT64     MPTYPE *FirstUserArrayPtr, MIL_INT64     MPTYPE *SecondUserArrayPtr);
inline MFTYPE32 MIL_INT MFTYPE MmeasInquireSafeType(MIL_ID MmeasId, MIL_INT InquireType, MIL_DOUBLE    MPTYPE *FirstUserArrayPtr, MIL_DOUBLE    MPTYPE *SecondUserArrayPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 MIL_INT MFTYPE MmeasInquireSafeType(MIL_ID MmeasId, MIL_INT InquireType, MIL_UINT32    MPTYPE *FirstUserArrayPtr, MIL_UINT32    MPTYPE  *SecondUserArrayPtr);
inline MFTYPE32 MIL_INT MFTYPE MmeasInquireSafeType(MIL_ID MmeasId, MIL_INT InquireType, MIL_UINT64    MPTYPE *FirstUserArrayPtr, MIL_UINT64    MPTYPE  *SecondUserArrayPtr);
#endif

// -------------------------------------------------------------------------
// MmeasGetResult

inline MFTYPE32 void MFTYPE MmeasGetResultSafeType  (MIL_ID MmeasResultId, MIL_INT ResultType, int FirstResultArrayPtr, int SecondResultArrayPtr)
   {
   if(FirstResultArrayPtr || SecondResultArrayPtr)
      SafeTypeError(MT("MmeasGetResult"));

   MmeasGetResult(MmeasResultId, ResultType, NULL, NULL);
   }

inline void MmeasGetResultSafeTypeExecute (MIL_ID MmeasResultId, MIL_INT ResultType, void          MPTYPE *FirstResultArrayPtr, int SecondResultArrayPtr, MIL_INT FirstGivenType)
   {
   MIL_INT RequiredType = (ResultType & (M_TYPE_DOUBLE | M_TYPE_MIL_INT32 | M_TYPE_MIL_INT64));
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;

   if(RequiredType != FirstGivenType || SecondResultArrayPtr)
      SafeTypeError(MT("MmeasGetResult"));

   MmeasGetResult(MmeasResultId, ResultType, FirstResultArrayPtr, NULL);
   }

inline void MmeasGetResultSafeTypeExecute (MIL_ID MmeasResultId, MIL_INT ResultType, void          MPTYPE *FirstResultArrayPtr, void          MPTYPE *SecondResultArrayPtr, MIL_INT FirstGivenType, MIL_INT SecondGivenType)
   {
   MIL_INT RequiredType = (ResultType & (M_TYPE_DOUBLE | M_TYPE_MIL_INT32 | M_TYPE_MIL_INT64));
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;

   if (RequiredType != FirstGivenType || RequiredType != SecondGivenType )
      SafeTypeError(MT("MmeasGetResult"));

   MmeasGetResult(MmeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr);
   }

inline MFTYPE32 void MFTYPE MmeasGetResultUnsafe  (MIL_ID MmeasResultId, MIL_INT ResultType, void       MPTYPE *FirstResultArrayPtr, void          MPTYPE *SecondResultArrayPtr) {MmeasGetResult           (MmeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr);}

inline MFTYPE32 void MFTYPE MmeasGetResultSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_INT32  MPTYPE *FirstResultArrayPtr, int SecondResultArrayPtr               ){MmeasGetResultSafeTypeExecute(MmeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_MIL_INT32 );}
inline MFTYPE32 void MFTYPE MmeasGetResultSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_INT64  MPTYPE *FirstResultArrayPtr, int SecondResultArrayPtr               ){MmeasGetResultSafeTypeExecute(MmeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_MIL_INT64 );}
inline MFTYPE32 void MFTYPE MmeasGetResultSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_DOUBLE MPTYPE *FirstResultArrayPtr, int SecondResultArrayPtr               ){MmeasGetResultSafeTypeExecute(MmeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_DOUBLE    );}

inline MFTYPE32 void MFTYPE MmeasGetResultSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_INT32  MPTYPE *FirstResultArrayPtr, MIL_INT32  MPTYPE *SecondResultArrayPtr){MmeasGetResultSafeTypeExecute(MmeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_MIL_INT32, M_TYPE_MIL_INT32 );}
inline MFTYPE32 void MFTYPE MmeasGetResultSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_INT64  MPTYPE *FirstResultArrayPtr, MIL_INT64  MPTYPE *SecondResultArrayPtr){MmeasGetResultSafeTypeExecute(MmeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_MIL_INT64, M_TYPE_MIL_INT64 );}
inline MFTYPE32 void MFTYPE MmeasGetResultSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_DOUBLE MPTYPE *FirstResultArrayPtr, MIL_DOUBLE MPTYPE *SecondResultArrayPtr){MmeasGetResultSafeTypeExecute(MmeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_DOUBLE   , M_TYPE_DOUBLE    );}

#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 void MFTYPE MmeasGetResultSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_UINT32 MPTYPE *FirstResultArrayPtr, int SecondResultArrayPtr               ){MmeasGetResultSafeTypeExecute(MmeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_MIL_INT32 );}
inline MFTYPE32 void MFTYPE MmeasGetResultSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_UINT64 MPTYPE *FirstResultArrayPtr, int SecondResultArrayPtr               ){MmeasGetResultSafeTypeExecute(MmeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_MIL_INT64 );}

inline MFTYPE32 void MFTYPE MmeasGetResultSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_UINT32 MPTYPE *FirstResultArrayPtr, MIL_UINT32 MPTYPE *SecondResultArrayPtr){MmeasGetResultSafeTypeExecute(MmeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_MIL_INT32, M_TYPE_MIL_INT32 );}
inline MFTYPE32 void MFTYPE MmeasGetResultSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_UINT64 MPTYPE *FirstResultArrayPtr, MIL_UINT64 MPTYPE *SecondResultArrayPtr){MmeasGetResultSafeTypeExecute(MmeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_MIL_INT64, M_TYPE_MIL_INT64 );}
#endif

// -------------------------------------------------------------------------
// MmeasGetResultSingle

inline MFTYPE32 void MFTYPE MmeasGetResultSingleSafeType  (MIL_ID MmeasResultId, MIL_INT ResultType, int FirstResultArrayPtr, int SecondResultArrayPtr, MIL_INT Index)
   {
   if(FirstResultArrayPtr || SecondResultArrayPtr)
      SafeTypeError(MT("MmeasGetResultSingle"));

   MmeasGetResultSingle(MmeasResultId, ResultType, NULL, NULL, Index);
   }

inline void MmeasGetResultSingleSafeTypeExecute (MIL_ID MmeasResultId, MIL_INT ResultType, void          MPTYPE *FirstResultArrayPtr, int SecondResultArrayPtr, MIL_INT Index, MIL_INT FirstGivenType)
   {
   MIL_INT RequiredType = (ResultType & (M_TYPE_DOUBLE | M_TYPE_MIL_INT32 | M_TYPE_MIL_INT64));
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;

   if(RequiredType != FirstGivenType || SecondResultArrayPtr)
      SafeTypeError(MT("MmeasGetResult"));  

   MmeasGetResultSingle(MmeasResultId, ResultType, FirstResultArrayPtr, NULL, Index);
   }

inline void MmeasGetResultSingleSafeTypeExecute (MIL_ID MmeasResultId, MIL_INT ResultType, void          MPTYPE *FirstResultArrayPtr, void          MPTYPE *SecondResultArrayPtr, MIL_INT Index, MIL_INT FirstGivenType, MIL_INT SecondGivenType)
   {
   MIL_INT RequiredType = (ResultType & (M_TYPE_DOUBLE | M_TYPE_MIL_INT32 | M_TYPE_MIL_INT64));
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;

   if (RequiredType != FirstGivenType || RequiredType != SecondGivenType )
      SafeTypeError(MT("MmeasGetResult"));

   MmeasGetResultSingle(MmeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, Index);
   }


inline MFTYPE32 void MFTYPE MmeasGetResultSingleUnsafe  (MIL_ID MmeasResultId, MIL_INT ResultType, void       MPTYPE *FirstResultArrayPtr, void          MPTYPE *SecondResultArrayPtr, MIL_INT Index) {MmeasGetResultSingle               (MmeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, Index);}

inline MFTYPE32 void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_INT32  MPTYPE *FirstResultArrayPtr, int                SecondResultArrayPtr, MIL_INT Index){MmeasGetResultSingleSafeTypeExecute(MmeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, Index, M_TYPE_MIL_INT32);}
inline MFTYPE32 void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_INT64  MPTYPE *FirstResultArrayPtr, int                SecondResultArrayPtr, MIL_INT Index){MmeasGetResultSingleSafeTypeExecute(MmeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, Index, M_TYPE_MIL_INT64);}
inline MFTYPE32 void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_DOUBLE MPTYPE *FirstResultArrayPtr, int                SecondResultArrayPtr, MIL_INT Index){MmeasGetResultSingleSafeTypeExecute(MmeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, Index, M_TYPE_DOUBLE   );}

inline MFTYPE32 void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_INT32  MPTYPE *FirstResultArrayPtr, MIL_INT32  MPTYPE *SecondResultArrayPtr, MIL_INT Index){MmeasGetResultSingleSafeTypeExecute(MmeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, Index, M_TYPE_MIL_INT32, M_TYPE_MIL_INT32);}
inline MFTYPE32 void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_INT64  MPTYPE *FirstResultArrayPtr, MIL_INT64  MPTYPE *SecondResultArrayPtr, MIL_INT Index){MmeasGetResultSingleSafeTypeExecute(MmeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, Index, M_TYPE_MIL_INT64, M_TYPE_MIL_INT64);}
inline MFTYPE32 void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_DOUBLE MPTYPE *FirstResultArrayPtr, MIL_DOUBLE MPTYPE *SecondResultArrayPtr, MIL_INT Index){MmeasGetResultSingleSafeTypeExecute(MmeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, Index, M_TYPE_DOUBLE   , M_TYPE_DOUBLE   );}

#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_UINT32 MPTYPE *FirstResultArrayPtr, int                SecondResultArrayPtr, MIL_INT Index){MmeasGetResultSingleSafeTypeExecute(MmeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, Index, M_TYPE_MIL_INT32);}
inline MFTYPE32 void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_UINT64 MPTYPE *FirstResultArrayPtr, int                SecondResultArrayPtr, MIL_INT Index){MmeasGetResultSingleSafeTypeExecute(MmeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, Index, M_TYPE_MIL_INT64);}

inline MFTYPE32 void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_UINT32 MPTYPE *FirstResultArrayPtr, MIL_UINT32 MPTYPE *SecondResultArrayPtr, MIL_INT Index){MmeasGetResultSingleSafeTypeExecute(MmeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, Index, M_TYPE_MIL_INT32, M_TYPE_MIL_INT32);}
inline MFTYPE32 void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MmeasResultId, MIL_INT ResultType, MIL_UINT64 MPTYPE *FirstResultArrayPtr, MIL_UINT64 MPTYPE *SecondResultArrayPtr, MIL_INT Index){MmeasGetResultSingleSafeTypeExecute(MmeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, Index, M_TYPE_MIL_INT64, M_TYPE_MIL_INT64);}
#endif

// -------------------------------------------------------------------------
// MmeasInquire

inline MFTYPE32 MIL_INT MFTYPE MmeasInquireSafeType  (MIL_ID MmeasId, MIL_INT InquireType, int FirstUserArrayPtr, int SecondUserArrayPtr)
   {
   if(FirstUserArrayPtr || SecondUserArrayPtr)
      SafeTypeError(MT("MmeasInquire"));

   return MmeasInquire(MmeasId,InquireType, NULL, NULL );
   }

inline MFTYPE32 MIL_INT MFTYPE MmeasInquireSafeTypeExecute (MIL_ID MmeasId, MIL_INT InquireType, void          MPTYPE *FirstUserArrayPtr, void          MPTYPE *SecondUserArrayPtr, MIL_INT GivenType)
   {
   MIL_INT RequiredType = (InquireType & (M_TYPE_DOUBLE | M_TYPE_MIL_INT32 | M_TYPE_MIL_INT64));
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;

   if (RequiredType != GivenType)
      SafeTypeError(MT("MmeasInquire"));

   return MmeasInquire(MmeasId, InquireType, FirstUserArrayPtr, SecondUserArrayPtr);
   }

inline MFTYPE32 MIL_INT MFTYPE MmeasInquireUnsafe  (MIL_ID MmeasId, MIL_INT InquireType, void       MPTYPE *FirstUserArrayPtr, void          MPTYPE *SecondUserArrayPtr) {return MmeasInquire               (MmeasId, InquireType, FirstUserArrayPtr, SecondUserArrayPtr);}
inline MFTYPE32 MIL_INT MFTYPE MmeasInquireSafeType(MIL_ID MmeasId, MIL_INT InquireType, MIL_INT32  MPTYPE *FirstUserArrayPtr, MIL_INT32 MPTYPE  *SecondUserArrayPtr){return MmeasInquireSafeTypeExecute(MmeasId, InquireType, FirstUserArrayPtr, SecondUserArrayPtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 MIL_INT MFTYPE MmeasInquireSafeType(MIL_ID MmeasId, MIL_INT InquireType, MIL_INT64  MPTYPE *FirstUserArrayPtr, MIL_INT64 MPTYPE  *SecondUserArrayPtr){return MmeasInquireSafeTypeExecute(MmeasId, InquireType, FirstUserArrayPtr, SecondUserArrayPtr, M_TYPE_MIL_INT64);}
inline MFTYPE32 MIL_INT MFTYPE MmeasInquireSafeType(MIL_ID MmeasId, MIL_INT InquireType, MIL_DOUBLE MPTYPE *FirstUserArrayPtr, MIL_DOUBLE MPTYPE *SecondUserArrayPtr){return MmeasInquireSafeTypeExecute(MmeasId, InquireType, FirstUserArrayPtr, SecondUserArrayPtr, M_TYPE_DOUBLE);}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MFTYPE32 MIL_INT MFTYPE MmeasInquireSafeType(MIL_ID MmeasId, MIL_INT InquireType, MIL_UINT32 MPTYPE *FirstUserArrayPtr, MIL_UINT32 MPTYPE  *SecondUserArrayPtr){return MmeasInquireSafeTypeExecute(MmeasId, InquireType, FirstUserArrayPtr, SecondUserArrayPtr, M_TYPE_MIL_INT32);}
inline MFTYPE32 MIL_INT MFTYPE MmeasInquireSafeType(MIL_ID MmeasId, MIL_INT InquireType, MIL_UINT64 MPTYPE *FirstUserArrayPtr, MIL_UINT64 MPTYPE  *SecondUserArrayPtr){return MmeasInquireSafeTypeExecute(MmeasId, InquireType, FirstUserArrayPtr, SecondUserArrayPtr, M_TYPE_MIL_INT64);}
#endif

#define MmeasGetResult       MmeasGetResultSafeType
#define MmeasGetResultSingle MmeasGetResultSingleSafeType
#define MmeasInquire         MmeasInquireSafeType

#else // #if M_MIL_USE_SAFE_TYPE

#define MmeasGetResultUnsafe       MmeasGetResult
#define MmeasGetResultSingleUnsafe MmeasGetResultSingle
#define MmeasInquireUnsafe         MmeasInquire

#endif // #if M_MIL_USE_SAFE_TYPE

#endif // !M_MIL_LITE

#endif /* __MILMEAS_H__ */
