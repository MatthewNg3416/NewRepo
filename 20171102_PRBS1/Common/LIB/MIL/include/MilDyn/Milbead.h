/***************************************************************************/
/*

   Filename:  MilBead.h
   Owner   :  Matrox Imaging dept.
   Revision:  9.02.0580
   Content :  This file contains the defines and the prototypes for the
   MIL bead verifier. (Mbead...)

   Copyright © Matrox Electronic Systems Ltd., 1992-2010.
   All Rights Reserved

*/
/***************************************************************************/
#ifndef __MIL_BEAD_H__
#define __MIL_BEAD_H__

#define BW_COMPATIBILITY 0x092

#if (!M_MIL_LITE) // MIL FULL ONLY

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif

	/***************************************************************************/
	/*                        MBead CAPI defines                               */
	/***************************************************************************/
#define M_CONTEXT                                  0x08000000L // Already defined in milmod.h, milreg.h, milstr.h, milocr.h, milcol.h, ...
#define M_GENERAL                                  0x20000000L // Already defined in milmod.h, milreg.h, milstr.h, milocr.h, milcol.h, ...
#define M_ALL                                      0x40000000L // Already defined in milmod.h, milmeas.h, milocr.h,  milpat.h,  mil.h, miledge.h, milcol.h, ...

	// Control Flags for Context
#define M_TRAINING_BOX_WIDTH                       17L         // Width of the search boxes in world units.
#define M_BOX_WIDTH_MARGIN                         2L          // Additional width margin of the search boxes in %.
#define M_TRAINING_BOX_HEIGHT                      3L          // Height of the search boxes in world units.
#define M_TRAINED_BOX_SPACING                      25L         // Readable: inquire the real spacing used for verify operation.
#define M_TRAINING_BOX_SPACING                     40L         // Space between search boxes in world units.
#define M_TRAINING_BOX_INPUT_UNITS                 78L         //
#define M_TEMPLATE_INPUT_UNITS                     79L         //
#define M_SMOOTHNESS                               108L        // Sets the degree of smoothness (strength of denoising), Already defined in milmod.h, milmeas.h, miledge.h, milmetrol.h, mil.h, ...
#define M_TRAINED_WIDTH_NOMINAL                    92L         //
#define M_THRESHOLD_MODE                           33L         // Sets the threshold mode setting, Already defined in milcode.h, miledge.h, milmetrol.h, milstr.h, ...
#define M_THRESHOLD_VALUE                          953L        // Sets the threshold value, Already defined in milcode.h, milstr.h, ...
#define M_FOREGROUND_VALUE                         4L          // Set the color of the bead, Already defined in milblob.h, milcal.h, milcode.h, miledge.h, milmod.h, milocr.h, milstr.h, ...
#define M_WIDTH_NOMINAL                            5L          // Set the nominal width of the bead, Already defined in milmeas.h, milblob,h, ...
#define M_WIDTH_DELTA_NEG                          6L          // Set the delta neg tolerance of the bead's width
#define M_WIDTH_DELTA_POS                          7L          // Set the delta pos tolerance of the bead's width
#define M_OFFSET_MAX                               8L          // Set the max offset of the bead position relative to the Template, in world units.
#define M_GAP_MAX_LENGTH                           9L          // Set the max valid gap length in world units
#define M_GAP_COVERAGE                             216L        // Retrieve the % of bead found for a Template to be valid
#define M_ACCEPTANCE                               200L        // Set the min % of valid beads for a Template to be valid, Already defined in milmod.h, milpat.h, milreg.h, milcol.h
#define M_WIDTH_NOMINAL_MODE                       12L         // Set the nominal width mode of the bead
#define M_TRAINING_IMAGE_ID                        101L        // Set the training image id
#define M_ASSOCIATED_CALIBRATION                   125L        // Set the associated calibration to the training image. // Already defined in milmod.h, milmet.h,...
#define M_CLOSEST_POINT_MAX_DISTANCE               176L        // Set the max distance value for template closest operations
#define M_GLOBAL_FRAME_OFFSET_X                    1000L
#define M_GLOBAL_FRAME_OFFSET_Y                    1001L
#define M_GLOBAL_FRAME_ROTATION                    1002L
#define M_TRAINING_GLOBAL_FRAME_OFFSET_X           1003L
#define M_TRAINING_GLOBAL_FRAME_OFFSET_Y           1004L
#define M_TRAINING_GLOBAL_FRAME_ROTATION           1005L
#define M_TRAINING_IS_CALIBRATED                   1006L
#define M_POSITION_MODE                            1462L
#define M_TEMPLATE_INDEX_FROM_LABEL                M_INDEX_VALUE
#define M_DRAW_INDEX_OFFSET                        1014L       // Already defined in milmetrol.h
#define M_RESTORED_CALIBRATION                     1491L       // Already in milmod.h
#define M_TRAINED_GAP_MAX_LENGTH                   203L
#define M_TRAINED_WIDTH_DELTA_POS                  204L
#define M_TRAINED_WIDTH_DELTA_NEG                  205L
#define M_TRAINED_OFFSET_MAX                       206L


	// Control Flags for Context or Result
#define M_DRAW_RELATIVE_ORIGIN_X                   319L        // Set the drawing offset X, Already defined in mil.h, ...
#define M_DRAW_RELATIVE_ORIGIN_Y                   320L        // Set the drawing offset Y, Already defined in mil.h, ...
#define M_DRAW_SCALE_X                             3203L       // Set the drawing scale X, Already defined in milblob.h, miledge.h, milmeas.h, milmetrolh, milmod.h, milreg.h, ...
#define M_DRAW_SCALE_Y                             3204L       // Set the drawing scale Y, Already defined in milblob.h, miledge.h, milmeas.h, milmetrolh, milmod.h, milreg.h, ...
#define M_WORLD                                    0x2000L     // Already defined in milmeas.h,...
#define M_PIXEL                                    0x1000L     // Already defined in milmeas.h,...
#define M_ENHANCED                                 1L

	// Control values 
#define M_USER_DEFINED                             21L         // Already defined in milcal.h, milcode.h, milcolor.h, miledge.h, milmetrol.h, milocr.h, milstr.h, ...
#define M_FOREGROUND_BLACK                         0x100L      // Already defined in    milblob.h, milmod.h,  milcode.h, milocr.h, miledge.h
#define M_FOREGROUND_WHITE                         0x80L       // Already defined in    milblob.h, milmod.h,  milcode.h, milocr.h, miledge.h
#define M_AUTO                                     444L        // Already defined in mil.h, ...
#define M_DISABLE                                 -9999L       // Already defined in milblob.h, milcal.h, milmeas.h, mil.h, ...

	// Strategy Flag
#define M_MEAS_STRATEGY                            1L          // Set the strategy to measurement

	// Inquire Flags for Context or Result
#define M_MODIFICATION_COUNT                       5010L       // To retrieve the modification counter, Already defined in mil.h, ...
#define M_NUMBER_OF_TEMPLATES                      1L          // To retrieve the number of Template in context or result
#define M_LABEL_VALUE                              1L          // Already defined in mil.h, ...
#define M_INDEX_VALUE                              M_INDEX     // Already defined in mil.h, ...
#define M_SIZE                                     0xC00L      // Retrieve the size of a template. Already defined in milmod, miledge,...
#define M_POSITION_X                               0x00003400L // Already defined in miledge.h, milcode.h, ...
#define M_POSITION_Y                               0x00004400L // Already defined in miledge.h, milcode.h, ...

	// Template function flags
#define M_BEAD_STRIPE                              1L          // Type (stripe) of template to add
#define M_BEAD_EDGE                                2L          // Type (edge) of template to add
#define M_ADD                                      0x0000L     // Add a new Template in the context, Already defined in milcode.h, milim.h, milocr.h, ...
#define M_REPLACE                                  0x01000060L // Already defined in mil.h, milocr.h, miledge.h
#define M_INSERT                                   5L          // Insert points to an existing Template
#define M_DELETE                                   3L          // Delete a Template in the context, Already defined in milblob.h, milcode.h, milcolor.h, miledge.h, milmetrol.h, milmod.h, mil.h, ...
#define M_TRANSLATE_POINTS                         8L          // Move all points of a Template (rigid transformation only)
#define M_SCALE_POINTS                             9L          // Scale all points of a Template
#define M_ROTATE_POINTS                            10L         // Rotate all points of a Template

   // GetNeighbors function flags
#define M_CLOSEST_TEMPLATE                         11L         // Retrieve closest Templates from a user-defined point
#define M_CLOSEST_POINT                            12L         // Retrieve closest point of a Template from a user-defined point

	// Train operation flag (bitwise)
#define M_POSITION_DISABLE                         1L          // The training is done only to search the nominal width and other values
#define M_SAVE_TRAINING_IMAGE                      2L          // To save the training image into the context
#define M_FREE_TRAINING_IMAGE                      3L          // To release the internal training image if any
#define M_TRAINING_IMAGE_SIZE_X                    15L         // Size of the training image
#define M_TRAINING_IMAGE_SIZE_Y                    16L         // Size of the training image
#define M_TRAINING_IMAGE_TYPE                      17L         // Type of the training image

	// Context status flag
#define M_COMPLETE                                 0x00000000L // Already defined in mil.h, milmod.h
#define M_PARTIAL                                  0x00000001L // Already defined in mil.h, milmod.h
#define M_NOT_TRAINED                              2L       

	// Result type Flag 
#define M_STATUS                                   0x00008002L // Already defined in milcode.h, milcolor.h, miledge.h, ...
#define M_NUMBER                                   1009L       // Already defined in mil.h, ...
#define M_GAP_TOLERANCE                            216L        // Already defined in milmetrol.h, ...
#define M_WIDTH_MIN                                74L         // 
#define M_WIDTH_MAX                                30L         // 
#define M_WIDTH_AVERAGE                            3L          // 
#define M_POSITION_X                               0x00003400L // Already defined in milmod, milpat.h, ...
#define M_POSITION_Y                               0x00004400L // Already defined in milmod, milpat.h, ...
#define M_POSITION_TRAINED_X                       34L
#define M_POSITION_TRAINED_Y                       35L
#define M_OFFSET                                   2L
#define M_ANGLE                                    0x00000800L // Already defined in milmod, milpat.h, ...
#define M_TRAINED_BOX_ANGLE                        33L
#define M_TRAINED_BOX_WIDTH                        48L
#define M_TRAINED_BOX_HEIGHT                       49L
#define M_WIDTH_MIN_INDEX                          10L         // 
#define M_WIDTH_MAX_INDEX                          20L         // 
#define M_STATUS_GAP_TOLERANCE                     22L
#define M_STATUS_GAP_MAX                           23L
#define M_STATUS_SCORE                             24L
#define M_TRAINED_INDEX                            25L
#define M_STATUS_SEARCH                            26L           
#define M_STATUS_OFFSET                            27L
#define M_STATUS_WIDTH_MIN                         28L
#define M_STATUS_WIDTH_MAX                         29L
#define M_WIDTH_VALUE                              31L
#define M_STATUS_FOUND                             32L
#define M_SCORE                                    0x00001400L // Already defined in milmod, milpat.h, ...
#define M_STATUS_FOREGROUND_COLOR_MIN              50L
#define M_STATUS_FOREGROUND_COLOR_MAX              51L
#define M_FOREGROUND_COLOR                         52L
#define M_TRAINING_WIDTH_NOMINAL                   60L
#define M_TRAINING_WIDTH_SCALE_MIN                 61L
#define M_TRAINING_WIDTH_SCALE_MAX                 62L
#define M_FOREGROUND_COLOR_NOMINAL_MODE            80L
#define M_FOREGROUND_COLOR_NOMINAL                 81L
#define M_FOREGROUND_COLOR_DELTA_POS               82L
#define M_FOREGROUND_COLOR_DELTA_NEG               83L

// General values
#define M_BEGIN                                    -1L
#define M_END                                      -2L
#define M_INFINITE                                 -1L // Also defined in Mil.h


// Result values
#define M_PASS                                     0x0002L     // Already defined in milim.h, ...
#define M_FAIL                                     4L          // Already defined in milmetrol.h, ...
#define M_FAIL_WIDTH_MIN                           7L          // 
#define M_FAIL_WIDTH_MAX                           8L          // 
#define M_FAIL_GAP_MAX                             9L          // 
#define M_FAIL_OFFSET                              10L         //
#define M_FAIL_NOT_FOUND                           11L         //
#define M_FAIL_FOREGROUND_COLOR_MIN                12L         //
#define M_FAIL_FOREGROUND_COLOR_MAX                13L         //

// Draw operation flags
#define M_DRAW_POSITION                            0x00000040L // Already defined in milmod.h, milpat.h, ...
#define M_DRAW_POSITION_POLYLINE                   2L          //
#define M_DRAW_INDEX                               0x00000100L // Already defines in miledge.h
#define M_DRAW_SEARCH_BOX                          6L          //    
#define M_DRAW_WIDTH                               0x00000008L // Already 
#define M_DRAW_BEAD                                4L          //
#define M_DRAW_BEAD_WIDTH_MIN                      5L          //
#define M_DRAW_BEAD_WIDTH_MAX                      6L          //
#define M_DRAW_LABEL                               0x00000001L // Already defines in miledge.h, milmetrol.h, ...
#define M_DRAW_TRAINING_IMAGE                      15L
#define M_DRAW_SEARCH_BOX_FILL                     16L
#define M_DRAW_BEAD_FILL                           17L
#define M_DRAW_GAP                                 18L
#define M_DRAW_GAP_FILL                            19L
#define M_DRAW_POSITION_INDEX                      20L
#define M_DRAW_GLOBAL_FRAME                        21L
#define M_DRAW_CURRENT_GLOBAL_FRAME                22L

// Draw option flags
#define M_USER                                     0x00000001L // Already defines in mil.h,...
#define M_TRAINED                                  12L
#define M_TRAINED_PASS                             11L
#define M_TRAINED_FAIL                             9L
#define M_ALL                                      0x40000000L // Already defines in milmetrol.h,...
#define M_PASS                                     0x0002L     // Already defines in milmetrol.h,...
#define M_FAIL                                     4L          // Already defines in milmetrol.h,...
#define M_FAIL_WIDTH_MAX                           8L 
#define M_FAIL_WIDTH_MIN                           7L 
#define M_FAIL_OFFSET                              10L 

/***************************************************************************/
/* MbeadSave()/MbeadRestore()/MbeadStream()                                   */
/***************************************************************************/
#define M_MEMORY           (M_APP_INQUIRE_SYS_START+120L) /*5120*/  // Already defined in Mil.h
#define M_INTERACTIVE                    M_NULL // Already defined in    mil.h, milcal.h, milcode.h, milmeas.h, milocr.h, milpat.h, milmod.h

#define M_SAVE                             0x100L

#define M_RESTORE                           0x10L

#define M_LOAD                                 1L
#define M_INQUIRE_SIZE_BYTE                    2L

#define M_WITH_CALIBRATION            0x00020000L // Already in mil.h



/***************************************************************************/
/* Utilities                                                               */
/***************************************************************************/

#define M_TEMPLATE_LABEL_TAG                 0x01000000L
#define M_TEMPLATE_INDEX_TAG                 0x02000000L

#define M_TEMPLATE_LABEL(i)                  (M_TEMPLATE_LABEL_TAG   | (i))
#define M_TEMPLATE_INDEX(i)                  (M_TEMPLATE_INDEX_TAG   | (i))

	//*****************************************************************************************
	// CAPI function prototypes

#if M_MIL_USE_64BIT
	// Prototypes for 64 bits OS
	MFTYPE32 void MFTYPE MbeadControlInt64(MIL_ID    ContextOrResultId,
		                                    MIL_INT   Index,
		                                    MIL_INT   ControlType,
		                                    MIL_INT64 ControlValue);

	MFTYPE32 void MFTYPE MbeadControlDouble(MIL_ID   ContextOrResultId,
                                           MIL_INT  Index,
                                           MIL_INT  ControlType,
                                           double   ControlValue);
#else

	// Prototypes for 32 bits OS
#define MbeadControlInt64  MbeadControl
#define MbeadControlDouble MbeadControl

	MFTYPE32 void MFTYPE MbeadControl(MIL_ID	ContextOrResultId,
		                               MIL_INT	LabelOrIndex,
		                               MIL_INT	ControlType,
		                               double  ControlValue);
#endif

	MFTYPE32 MIL_ID   MFTYPE  MbeadAlloc(MIL_ID     SystemId, 
		                                  MIL_INT	   Method, 
		                                  MIL_INT	   ControlFlag, 
		                                  MIL_ID		*ContextIdPtr);

	MFTYPE32 MIL_ID   MFTYPE  MbeadAllocResult(MIL_ID		SystemId, 
		                                        MIL_INT	   ControlFlag, 
		                                        MIL_ID		*ResultIdPtr);

	MFTYPE32 void     MFTYPE  MbeadFree(MIL_ID   ContextOrResultId);


	MFTYPE32 MIL_INT  MFTYPE  MbeadInquire(MIL_ID	ContextId, 
		                                    MIL_INT	LabelOrIndex, 
		                                    MIL_INT	InquireType, 
		                                    void		*UserVarPtr);

	MFTYPE32 void     MFTYPE  MbeadTemplate(MIL_ID	   ContextId,
		                                     MIL_INT	   Operation,
                                           MIL_INT	   BeadType,
		                                     MIL_INT	   LabelOrIndex, 
		                                     MIL_INT	   SizeOfArray, 
		                                     MIL_DOUBLE *FirstArrayPtr, 
		                                     MIL_DOUBLE *SecondArrayPtr, 
		                                     MIL_INT    *ThirdArrayPtr,
                                           MIL_INT    ControlFlag);

	MFTYPE32 void     MFTYPE  MbeadTrain(MIL_ID		ContextId, 
                                        MIL_ID     SrcImageId,
		                                  MIL_INT	   ControlFlag);

	MFTYPE32 void     MFTYPE  MbeadVerify(MIL_ID		ContextId, 
		                                   MIL_ID		SrcImageId, 
		                                   MIL_ID		ResultId, 
		                                   MIL_INT	ControlFlag);

	MFTYPE32 void     MFTYPE  MbeadGetResult(MIL_ID		ResultId, 
		                                      MIL_INT	LabelOrIndex, 
		                                      MIL_INT	ResultIndex, 
		                                      MIL_INT	ResultType, 
		                                      void		*UserVarPtr);

   MFTYPE32 void     MFTYPE   MbeadGetNeighbors(MIL_ID     ContextId,                                   
                                                MIL_INT    LabelOrIndex,
                                                MIL_DOUBLE PositionX,
                                                MIL_DOUBLE PositionY,
                                                MIL_INT*   TemplateLabelPtr,
                                                MIL_INT*   PointIndexPtr,
                                                MIL_INT    ControlFlag);

	MFTYPE32 void     MFTYPE  MbeadDraw(MIL_ID		GraphContId,
		                                 MIL_ID		ContextIdOrResultId, 
		                                 MIL_ID		DestImageId, 
		                                 MIL_INT	   DrawOperation, 
                                       MIL_INT	   DrawOption, 
		                                 MIL_INT	   LabelOrIndex, 
		                                 MIL_INT	   ResultIndex, 
		                                 MIL_INT	   ControlFlag);

#if M_MIL_USE_UNICODE

   MFTYPE32 MIL_ID MFTYPE MbeadRestoreW(MIL_CONST_TEXT_PTR FileName, 
                                        MIL_ID     SystemId, 
                                        MIL_INT    ControlFlag, 
                                        MIL_ID     *ContextIdPtr);

   MFTYPE32 void MFTYPE MbeadSaveW(MIL_CONST_TEXT_PTR FileName, 
                                   MIL_ID    ContextId, 
                                   MIL_INT   ControlFlag);

   MFTYPE32 void MFTYPE MbeadStreamW(MIL_TEXT_PTR MemPtrOrFileName, 
                                     MIL_ID     SystemId, 
                                     MIL_INT    Operation, 
                                     MIL_INT    StreamType, 
                                     double     Version, 
                                     MIL_INT    ControlFlag, 
                                     MIL_ID     *ContextIdPtr, 
                                     MIL_INT    *SizeByteVarPtr);

   MFTYPE32 MIL_ID MFTYPE MbeadRestoreA(const char* FileName, 
                                        MIL_ID     SystemId, 
                                        MIL_INT    ControlFlag, 
                                        MIL_ID     *ContextIdPtr);

   MFTYPE32 void MFTYPE MbeadSaveA(const char* FileName, 
                                   MIL_ID    ContextId, 
                                   MIL_INT   ControlFlag);

   MFTYPE32 void MFTYPE MbeadStreamA(char*   MemPtrOrFileName, 
                                     MIL_ID  SystemId, 
                                     MIL_INT Operation, 
                                     MIL_INT StreamType, 
                                     double  Version, 
                                     MIL_INT ControlFlag, 
                                     MIL_ID  *ContextIdPtr, 
                                     MIL_INT *SizeByteVarPtr);

#if M_MIL_UNICODE_API
#define MbeadSave           MbeadSaveW
#define MbeadRestore        MbeadRestoreW
#define MbeadStream         MbeadStreamW
#else
#define MbeadSave           MbeadSaveA
#define MbeadRestore        MbeadRestoreA
#define MbeadStream         MbeadStreamA
#endif

#else

   MFTYPE32 MIL_ID MFTYPE MbeadRestore(MIL_CONST_TEXT_PTR FileName, 
                                       MIL_ID   SystemId, 
                                       MIL_INT  ControlFlag, 
                                       MIL_ID   *ContextIdPtr);

   MFTYPE32 void MFTYPE MbeadSave(MIL_CONST_TEXT_PTR FileName, 
                                  MIL_ID     ContextId, 
                                  MIL_INT    ControlFlag);

   MFTYPE32 void MFTYPE MbeadStream(MIL_TEXT_PTR MemPtrOrFileName, 
                                    MIL_ID   SystemId, 
                                    MIL_INT  Operation, 
                                    MIL_INT  StreamType, 
                                    double   Version, 
                                    MIL_INT  ControlFlag, 
                                    MIL_ID   *ContextIdPtr, 
                                    MIL_INT  *SizeByteVarPtr);
#endif

	//*****************************************************************************************
#ifdef __cplusplus
}
#endif

#if M_MIL_USE_64BIT
#ifdef __cplusplus
//////////////////////////////////////////////////////////////
// M3dmapControl function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_MIL_USE_LINUX
inline void MbeadControl(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT ControlType, int ControlValue)
   {
   MbeadControlInt64(ContextOrResultId, Index, ControlType, ControlValue);
   };
#endif
inline void MbeadControl(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT ControlType, MIL_INT32 ControlValue)
   {
   MbeadControlInt64(ContextOrResultId, Index, ControlType, ControlValue);
   };

inline void MbeadControl(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT ControlType, MIL_INT64 ControlValue)
   {
   MbeadControlInt64(ContextOrResultId, Index, ControlType, ControlValue);
   };

inline void MbeadControl(MIL_ID ContextOrResultId, MIL_INT Index, MIL_INT ControlType, MIL_DOUBLE ControlValue)
   {
   MbeadControlDouble(ContextOrResultId, Index, ControlType, ControlValue);
   };
#else
//////////////////////////////////////////////////////////////
// For C file, call the default function, i.e. Double one
//////////////////////////////////////////////////////////////
#define MbeadControl  MbeadControlDouble

#endif // __cplusplus
#endif // M_MIL_USE_64BIT


#endif // !M_MIL_LITE

#endif // __MIL_BEAD_H__
