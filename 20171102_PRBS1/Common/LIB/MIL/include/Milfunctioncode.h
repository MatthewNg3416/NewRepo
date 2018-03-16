//////////////////////////////////////////////////////////////////////////////
//
// Filename          : MilFunctionCode.h
// Content           : Functions opcodes :
// Revision          : 9.02.0580
//                     Values returned by MappGetHookInfo (M_CURRENT_FCT)
//
//
// Copyright � Matrox Electronic Systems Ltd., 1992-2010.
// All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __FUNCTIONCODES_H__
#define __FUNCTIONCODES_H__

#define M_NO_FUNCTION                       0x00000000

#define M_SYS_ALLOC                         0x00000001
#define M_SYS_FREE                          0x00000002
#define M_SYS_INQUIRE                       0x00000003
#define M_SYS_CONTROL                       0x00000004
#define M_SYS_CONFIG_ACCESS                 0x00000005
#define M_SYS_ALLOC_REMOTE                  0x00000006
#define M_SYS_HOOK_FUNCTION                 0x00000007
#define M_SYS_GET_HOOK_INFO                 0x00000008

#define M_BUF_ALLOC_2D                      0x00000201
#define M_BUF_CHILD_2D                      0x00000202
#define M_BUF_FREE                          0x00000203
#define M_BUF_GET_2D                        0x00000204
#define M_BUF_PUT_2D                        0x00000205
#define M_BUF_COPY                          0x00000206
#define M_BUF_COPY_COLOR                    0x00000207
#define M_BUF_GET_COLOR_2D                  0x00000208
#define M_BUF_PUT_COLOR_2D                  0x00000209
#define M_BUF_INQUIRE                       0x0000020A
#define M_BUF_CHILD_COLOR_2D                0x0000020B
#define M_BUF_CLEAR                         0x0000020C
#define M_BUF_COPY_MASK                     0x0000020D
#define M_BUF_COPY_COND                     0x0000020E
#define M_BUF_CONTROL_NEIGHBORHOOD          0x0000020F
#define M_BUF_CONTROL                       0x00000210
#define M_BUF_GET_LINE                      0x00000211
#define M_BUF_PUT_LINE                      0x00000212
#define M_BUF_CREATE_COLOR                  0x00000213
#define M_BUF_COMPRESSION                   0x00000214
#define M_BUF_COPY_COLOR_2D                 0x00000215
#define M_BUF_SHIFT                         0x00000216
#define M_BUF_BINARIZE                      0x00000217
#define M_BUF_CLIP                          0x00000218
#define M_BUF_LUT_MAP                       0x00000219
#define M_BUF_RESIZE                        0x0000021A
#define M_BUF_CONVERT                       0x0000021B
#define M_BUF_HOOK_FUNCTION                 0x0000021C
#define M_BUF_ALLOC_1D                      0x0000021D
#define M_BUF_ALLOC_COLOR                   0x0000021E
#define M_BUF_BAYER                         0x0000021F
#define M_BUF_CHILD_1D                      0x00000220
#define M_BUF_CHILD_COLOR                   0x00000221
#define M_BUF_COPY_CLIP                     0x00000222
#define M_BUF_CREATE_2D                     0x00000223
#define M_BUF_DISK_INQUIRE                  0x00000224
#define M_BUF_EXPORT                        0x00000225
#define M_BUF_EXPORT_SEQUENCE               0x00000226
#define M_BUF_GET                           0x00000227
#define M_BUF_GET_1D                        0x00000228
#define M_BUF_GET_COLOR                     0x00000229
#define M_BUF_IMPORT                        0x0000022A
#define M_BUF_IMPORT_SEQUENCE               0x0000022B
#define M_BUF_LOAD                          0x0000022C
#define M_BUF_CONTROL_REGION                0x0000022D
#define M_BUF_PUT                           0x0000022E
#define M_BUF_PUT_1D                        0x0000022F
#define M_BUF_PUT_COLOR                     0x00000230
#define M_BUF_RESTORE                       0x00000231
#define M_BUF_SAVE                          0x00000232
#define M_BUF_TRANSFER                      0x00000233
#define M_BUF_GET_HOOK_INFO                 0x00000234
#define M_BUF_GET_ARC                       0x00000235
#define M_BUF_FIND_EXTREME                  0x00000236
#define M_BUF_CHILD_MOVE                    0x00000237
#define M_BUF_SET_REGION                    0x00000238

#define M_DISP_ALLOC                        0x00000401
#define M_DISP_FREE                         0x00000402
#define M_DISP_ZOOM                         0x00000403
#define M_DISP_PAN                          0x00000404
#define M_DISP_SELECT                       0x00000405
#define M_DISP_LUT                          0x00000406
#define M_DISP_INQUIRE                      0x00000407
#define M_DISP_CONTROL                      0x0000040A
#define M_DISP_HOOK_FUNCTION                0x0000040B
#define M_DISP_SELECT_WINDOW                0x0000040C
#define M_DISP_UPDATE                       0x0000040E
#define M_DISP_GET_HOOK_INFO                0x0000040F

#define M_DIG_ALLOC                         0x00000601
#define M_DIG_FREE                          0x00000602
#define M_DIG_GRAB                          0x00000603
#define M_DIG_CHANNEL                       0x00000604
#define M_DIG_HALT                          0x00000605
#define M_DIG_LUT                           0x00000606
#define M_DIG_INQUIRE                       0x00000607
#define M_DIG_REFERENCE                     0x00000608
#define M_DIG_CONTROL                       0x00000609
#define M_DIG_GRAB_WAIT                     0x0000060A
#define M_DIG_AVERAGE                       0x0000060B
#define M_DIG_HOOK_FUNCTION                 0x0000060C
#define M_DIG_FOCUS                         0x0000060D
#define M_DIG_GRAB_CONTINUOUS               0x0000060E
#define M_DIG_GRAB_ABORT                    0x0000060F
#define M_DIG_PROCESS                       0x00000610
#define M_DIG_GET_HOOK_INFO                 0x00000611
#define M_DIG_CONTROL_FEATURE               0x00000612
#define M_DIG_INQUIRE_FEATURE               0x00000613

#define M_IM_EDGE_DETECT                    0x00000801
#define M_IM_RANK                           0x00000802
#define M_IM_CONVOLVE                       0x00000803
#define M_IM_DILATE                         0x00000804
#define M_IM_ERODE                          0x00000805
#define M_IM_CLOSE                          0x00000806
#define M_IM_OPEN                           0x00000807
#define M_IM_LABEL                          0x00000808
#define M_IM_CONNECT_MAP                    0x00000809
#define M_IM_THIN                           0x0000080A
#define M_IM_THICK                          0x0000080B
#define M_IM_DISTANCE                       0x0000080C
#define M_IM_WATERSHED                      0x0000080D
#define M_IM_ZONE_OF_INFLUENCE              0x0000080E
#define M_IM_MORPHIC                        0x0000080F
#define M_IM_THRESHOLD                      0x00000810
#define M_IM_PROJECT                        0x00000811
#define M_IM_LOCATE_EVENT                   0x00000812
#define M_IM_COUNT_DIFFERENCE               0x00000813
#define M_IM_STAT                           0x00000814
#define M_IM_LOCATE_PEAK_1D                 0x00000815
#define M_IM_TRANSLATE                      0x00000816
#define M_IM_ROTATE                         0x00000817
#define M_IM_WARP                           0x00000818
#define M_IM_POLAR_TRANSFORM                0x00000819
#define M_IM_TRANSFORM                      0x0000081A
#define M_IM_DRAW                           0x0000081B
#define M_IM_RESTORE                        0x0000081C
#define M_IM_SAVE                           0x0000081D
#define M_IM_STREAM                         0x0000081E
#define M_IM_CONVOLVE_3X3                   0x0000081F
#define M_IM_CONVOLVE_MXN                   0x00000820
#define M_IM_SMOOTH                         0x00000821
#define M_IM_SHARPEN                        0x00000822
#define M_IM_SHARPEN_2                      0x00000823
#define M_IM_VERT_EDGE                      0x00000824
#define M_IM_HORIZ_EDGE                     0x00000825
#define M_IM_EDGE_DETECT_2                  0x00000826
#define M_IM_CONVOLVE_5X5                   0x00000827
#define M_IM_CONVOLVE_2X2                   0x00000828
#define M_IM_SHEN                           0x00000829
#define M_IM_DERICHE                        0x0000082A
#define M_IM_DILATE_MXN                     0x0000082B
#define M_IM_ERODE_MXN                      0x0000082C
#define M_IM_THIN_MXN                       0x0000082D
#define M_IM_THICK_MXN                      0x0000082E
#define M_IM_MATCH_MXN                      0x0000082F
#define M_IM_AREA_OPEN                      0x00000830
#define M_IM_AREA_CLOSE                     0x00000831
#define M_IM_FFT                            0x00000832
#define M_IM_DCT                            0x00000833
#define M_IM_DRAW_PEAKS                     0x00000834
#define M_IM_TRANSFORM_POLAR                0x00000835
#define M_IM_STAT_MULTIPLE                  0x00000836
#define M_IM_DRAW_STAT_MULTIPLE_RESULT      0x00000837
#define M_IM_MATCH                          0x00000838
#define M_IM_LEVEL_MXN                      0x00000839

#define M_IM_ARITH                          0x00000A01
#define M_IM_ARITH_MULTIPLE                 0x00000A02
#define M_IM_SHIFT                          0x00000A03
#define M_IM_BINARIZE                       0x00000A04
#define M_IM_CLIP                           0x00000A05
#define M_IM_CONVERT                        0x00000A06
#define M_IM_FLIP                           0x00000A07
#define M_IM_HISTOGRAM                      0x00000A08
#define M_IM_HISTOGRAM_EQUALIZE             0x00000A09
#define M_IM_LUT_MAP                        0x00000A0A
#define M_IM_RESIZE                         0x00000A0B
#define M_IM_FIND_EXTREME                   0x00000A0C
#define M_IM_DEINTERLACE                    0x00000A0D
#define M_IM_ALLOC                          0x00000A0E
#define M_IM_CONTROL                        0x00000A0F
#define M_IM_ALLOC_RESULT                   0x00000A10
#define M_IM_FREE                           0x00000A11
#define M_IM_INQUIRE                        0x00000A12
#define M_IM_GET_RESULT_1D                  0x00000A13
#define M_IM_GET_RESULT                     0x00000A14
#define M_IM_ADD                            0x00000A15
#define M_IM_ADD_CONST                      0x00000A16
#define M_IM_SUB                            0x00000A17
#define M_IM_SUB_ABS                        0x00000A18
#define M_IM_SUB_CONST                      0x00000A19
#define M_IM_CONST_SUB                      0x00000A1A
#define M_IM_MULT                           0x00000A1B
#define M_IM_MULT_CONST                     0x00000A1C
#define M_IM_DIV                            0x00000A1D
#define M_IM_DIV_FIXED_POINT                0x00000A1E
#define M_IM_DIV_CONST                      0x00000A1F
#define M_IM_CONST_DIV                      0x00000A20
#define M_IM_DIV_CONST_FIXED_POINT          0x00000A21
#define M_IM_CONST_DIV_FIXED_POINT          0x00000A22
#define M_IM_AND                            0x00000A23
#define M_IM_AND_CONST                      0x00000A24
#define M_IM_NAND                           0x00000A25
#define M_IM_NAND_CONST                     0x00000A26
#define M_IM_OR                             0x00000A27
#define M_IM_OR_CONST                       0x00000A28
#define M_IM_XOR                            0x00000A29
#define M_IM_XOR_CONST                      0x00000A2A
#define M_IM_NOR                            0x00000A2B
#define M_IM_NOR_CONST                      0x00000A2C
#define M_IM_XNOR                           0x00000A2D
#define M_IM_XNOR_CONST                     0x00000A2E
#define M_IM_MIN                            0x00000A2F
#define M_IM_MIN_CONST                      0x00000A30
#define M_IM_MAX                            0x00000A31
#define M_IM_MAX_CONST                      0x00000A32
#define M_IM_NOT                            0x00000A33
#define M_IM_NEG                            0x00000A34
#define M_IM_ABS                            0x00000A35
#define M_IM_OFFSET_GAIN                    0x00000A36
#define M_IM_WEIGHTED_AVERAGE               0x00000A37
#define M_IM_MULTIPLY_ACCUMULATE_1          0x00000A38
#define M_IM_MULTIPLY_ACCUMULATE_2          0x00000A39
#define M_IM_SQRT                           0x00000A3A
#define M_IM_SQUARE                         0x00000A3B
#define M_IM_CUBE                           0x00000A3C
#define M_IM_LN                             0x00000A3D
#define M_IM_LOG10                          0x00000A3E
#define M_IM_LOG2                           0x00000A3F
#define M_IM_EXP_CONST                      0x00000A40
#define M_IM_FLAT_FIELD                     0x00000A41
#define M_IM_DEAD_PIXEL_CORRECTION          0x00000A42
#define M_IM_REARRANGE                      0x00000A43
#define M_IM_GET_RESULT_2D                  0x00000A44
#define M_IM_PUT                            0x00000A45
#define M_IM_GET                            0x00000A46

#define M_DRI_GET_PROCESS_STATUS            0x00001801
#define M_DRI_GET_ERROR                     0x00001802
#define M_DRI_GET_INFO                      0x00001803
#define M_DRI_THREAD_DETACH                 0x00001804
#define M_DRI_THREAD_ATTACH                 0x00001805

#define M_GRA_CLEAR                         0x00001A01
#define M_GRA_DOT                           0x00001A02
#define M_GRA_LINE                          0x00001A03
#define M_GRA_RECT                          0x00001A04
#define M_GRA_RECT_FILL                     0x00001A05
#define M_GRA_ARC                           0x00001A06
#define M_GRA_ARC_FILL                      0x00001A07
#define M_GRA_FILL                          0x00001A08
#define M_GRA_TEXT                          0x00001A09
#define M_GRA_BACK_COLOR                    0x00001A0A
#define M_GRA_COLOR                         0x00001A0B
#define M_GRA_DOTS                          0x00001A0C
#define M_GRA_LINES                         0x00001A0D
#define M_GRA_ALLOC                         0x00001A0E
#define M_GRA_FREE                          0x00001A0F
#define M_GRA_CONTROL                       0x00001A10
#define M_GRA_FONT                          0x00001A11
#define M_GRA_FONT_SCALE                    0x00001A12
#define M_GRA_INQUIRE                       0x00001A13
#define M_GRA_ALLOC_LIST                    0x00001A14
#define M_GRA_CONTROL_LIST                  0x00001A15
#define M_GRA_DRAW                          0x00001A16
#define M_GRA_DRAW_ZOOM                     0x00001A17
#define M_GRA_GET_HOOK_INFO                 0x00001A1A
#define M_GRA_HOOK_FUNCTION                 0x00001A1B
#define M_GRA_INQUIRE_LIST                  0x00001A1C
#define M_GRA_RECT_ANGLE                    0x00001A1E
#define M_GRA_ARC_ANGLE                     0x00001A1F
#define M_GRA_SAVE                          0x00001A20
#define M_GRA_RESTORE                       0x00001A21
#define M_GRA_STREAM                        0x00001A22
#define M_GRA_IMAGE                         0x00001A23
#define M_GRA_GET_BOUNDING_BOX              0x00001A24
#define M_GRA_PROCESS                       0x00001A25

#define M_GEN_LUT_RAMP                      0x00001C01
#define M_GEN_LUT_FUNCTION                  0x00001C02
#define M_GEN_WARP_PARAMETER                0x00001C03

#define M_NAT_ENTER_NATIVE_MODE             0x00001E01
#define M_NAT_LEAVE_NATIVE_MODE             0x00001E02
#define M_NAT_ACCESS_SYSTEM_INFO            0x00001E03

#define M_PAT_ALLOC_MODEL                   0x00002401
#define M_PAT_ALLOC_RESULT                  0x00002402
#define M_PAT_ALLOC_ROTATED_MODEL           0x00002403
#define M_PAT_COPY                          0x00002404
#define M_PAT_FIND_MODEL                    0x00002405
#define M_PAT_FREE                          0x00002406
#define M_PAT_GET_NUMBER                    0x00002407
#define M_PAT_GET_RESULT                    0x00002408
#define M_PAT_INQUIRE                       0x00002409
#define M_PAT_PREPROC_MODEL                 0x0000240A
#define M_PAT_READ                          0x0000240B
#define M_PAT_RESTORE                       0x0000240C
#define M_PAT_SAVE                          0x0000240D
#define M_PAT_SET_ACCEPTANCE                0x0000240E
#define M_PAT_SET_ACCURACY                  0x0000240F
#define M_PAT_SET_CENTER                    0x00002410
#define M_PAT_SET_CERTAINTY                 0x00002411
#define M_PAT_SET_DONT_CARE                 0x00002412
#define M_PAT_SET_NUMBER                    0x00002413
#define M_PAT_SET_POSITION                  0x00002414
#define M_PAT_SET_SEARCH_PARAMETER          0x00002415
#define M_PAT_SET_SPEED                     0x00002416
#define M_PAT_WRITE                         0x00002417

#define M_BLOB_ALLOC_FEATURE_LIST           0x00002601
#define M_BLOB_ALLOC_RESULT                 0x00002602
#define M_BLOB_CALCULATE                    0x00002603
#define M_BLOB_CONTROL                      0x00002604
#define M_BLOB_COPY_RESULT                  0x00002605
#define M_BLOB_COPY_RUNS                    0x00002606
#define M_BLOB_FILL                         0x00002607
#define M_BLOB_FREE                         0x00002608
#define M_BLOB_GET_LABEL                    0x00002609
#define M_BLOB_GET_NUMBER                   0x0000260A
#define M_BLOB_GET_RESULT                   0x0000260B
#define M_BLOB_GET_RESULT_SINGLE            0x0000260C
#define M_BLOB_GET_RUNS                     0x0000260D
#define M_BLOB_INQUIRE                      0x0000260E
#define M_BLOB_LABEL                        0x0000260F
#define M_BLOB_SELECT                       0x00002610
#define M_BLOB_SELECT_FEATURE               0x00002611
#define M_BLOB_SELECT_FERET                 0x00002612
#define M_BLOB_SELECT_MOMENT                0x00002613

#define M_APP_ALLOC                         0x00002801
#define M_APP_CHILD                         0x00002802
#define M_APP_CONTROL                       0x00002803
#define M_APP_CONTROL_THREAD                0x00002804
#define M_APP_FREE                          0x00002805
#define M_APP_GET_ERROR                     0x00002806
#define M_APP_GET_HOOK_INFO                 0x00002807
#define M_APP_HOOK_FUNCTION                 0x00002808
#define M_APP_INQUIRE                       0x00002809
#define M_APP_MODIFY                        0x0000280A
#define M_APP_TIMER                         0x0000280B
#define M_APP_LICENSE_SUPERVISOR            0x0000280D
#define M_APP_LICENSE_MANAGER               0x0000280E
#define M_APP_EXECUTE_USER_FUNCTION         0x0000280F
#define M_APP_INTERNAL_THREAD               0x00002810
#define M_APP_FILE_OPERATION                0x00002811
#define M_APP_INQUIRE_MP                    0x00002812
#define M_APP_CONTROL_MP                    0x00002813

#define M_FUNC_ID_GET_OBJECT_TYPE           0x00002A01
#define M_FUNC_ERROR_REPORT                 0x00002A02
#define M_FUNC_CONTROL                      0x00002A03
#define M_FUNC_INQUIRE                      0x00002A04

#define M_OBJ_CONTROL                       0x00002C01
#define M_OBJ_INQUIRE                       0x00002C02
#define M_OBJ_HOOK_FUNCTION                 0x00002C03
#define M_OBJ_ALLOC                         0x00002C04
#define M_OBJ_FREE                          0x00002C05
#define M_OBJ_GET_HOOK_INFO                 0x00002C06

#define M_THR_ALLOC                         0x00002E01
#define M_THR_CONTROL                       0x00002E02
#define M_THR_FREE                          0x00002E03
#define M_THR_INQUIRE                       0x00002E04
#define M_THR_WAIT                          0x00002E05
#define M_THR_WAIT_MULTIPLE                 0x00002E06
#define M_THR_INQUIRE_MP                    0x00002E07
#define M_THR_CONTROL_MP                    0x00002E08

#define M_MISC_OPEN_COMMUNICATION           0x00003001
#define M_MISC_BUF_LINK                     0x00003002
#define M_MISC_MON_GAMMA_LUT                0x00003003
#define M_MISC_INTERNAL_THREAD              0x00003004
#define M_MISC_DISP_WNDPROC                 0x00003005
#define M_MISC_DISP_USERWNDPROC             0x00003006
#define M_MISC_DEVICE_LOSING_THREAD         0x00003007
#define M_MISC_SPECIAL_OS_MESSAGE           0x00003008
#define M_MISC_DATA_TRANSFER                0x00003009
#define M_MISC_DMIL_ADD_HOOK_THREAD         0x0000300A
#define M_MISC_ALLOC_NON_PAGED_MEMORY_CE    0x0000300B
#define M_MISC_MALLOC_CE                    0x0000300C
#define M_MISC_DMIL_CREATE_PROCESSING_THRE  0x0000300D
#define M_MISC_DMIL_CREATE_CLEANUP_THREAD   0x0000300E
#define M_MISC_DMIL_FREE_INTERNAL_ID        0x0000300F
#define M_MISC_DMIL_SLAVE_SHUTDOWN          0x00003010
#define M_MISC_DMIL_CREATE_CONTROL_THREAD   0x00003011
#define M_MISC_DMIL_CREATE_CONNECTION_THRE  0x00003012
#define M_MISC_DMIL_CREATE_DELETION_HOOK_T  0x00003013
#define M_MISC_DMIL_CLEAN_UP_MIL_RESOURCES  0x00003014
#define M_MISC_DMIL_CREATE_INTERNAL_THREAD  0x00003015
#define M_MISC_THR_WAIT_START               0x00003016
#define M_MISC_THR_WAIT_END                 0x00003017
#define M_MISC_THR_WAIT_MULTIPLE_START      0x00003018
#define M_MISC_THR_WAIT_MULTIPLE_END        0x00003019

#define M_FPGA_CONTROL                      0x00003201
#define M_FPGA_INQUIRE                      0x00003202

#define M_VGA_ALLOC_DIB_INFO                0x00006001
#define M_VGA_HOOK_MODIFIED                 0x00006002
#define M_VGA_DISP_SELECT_CLIENT_AREA       0x00006003
#define M_VGA_DISP_CONTROL                  0x00006004
#define M_VGA_DISP_CAPTURE                  0x00006005
#define M_VGA_DISP_INQUIRE                  0x00006006
#define M_VGA_DISP_PROTECT_AREA             0x00006007
#define M_VGA_DISP_SET_TITLE_NAME           0x00006008

#define M_BLOB_ALLOC_FEATURE_LIST_NEW       0x00008001
#define M_BLOB_ALLOC_RESULT_NEW             0x00008002
#define M_BLOB_CALCULATE_NEW                0x00008003
#define M_BLOB_CONTROL_NEW                  0x00008004
#define M_BLOB_FILL_NEW                     0x00008005
#define M_BLOB_FREE_NEW                     0x00008006
#define M_BLOB_GET_LABEL_NEW                0x00008007
#define M_BLOB_GET_NUMBER_NEW               0x00008008
#define M_BLOB_GET_RESULT_NEW               0x00008009
#define M_BLOB_GET_RESULT_SINGLE_NEW        0x0000800A
#define M_BLOB_GET_RUNS_NEW                 0x0000800B
#define M_BLOB_INQUIRE_NEW                  0x0000800C
#define M_BLOB_LABEL_NEW                    0x0000800D
#define M_BLOB_RECONSTRUCT_NEW              0x0000800E
#define M_BLOB_SELECT_NEW                   0x0000800F
#define M_BLOB_SELECT_FEATURE_NEW           0x00008010
#define M_BLOB_SELECT_FERET_NEW             0x00008011
#define M_BLOB_SELECT_MOMENT_NEW            0x00008012
#define M_BLOB_DRAW_NEW                     0x00008013
#define M_BLOB_MERGE_NEW                    0x00008014

#define M_MEAS_ALLOC_MARKER                 0x00008201
#define M_MEAS_SET_MARKER                   0x00008202
#define M_MEAS_FIND_MARKER                  0x00008203
#define M_MEAS_GET_RESULT                   0x00008204
#define M_MEAS_GET_RESULT_SINGLE            0x00008205
#define M_MEAS_INQUIRE                      0x00008206
#define M_MEAS_SAVE_MARKER                  0x00008207
#define M_MEAS_CALCULATE                    0x00008208
#define M_MEAS_FREE                         0x00008209
#define M_MEAS_ALLOC_RESULT                 0x0000820A
#define M_MEAS_ALLOC_CONTEXT                0x0000820B
#define M_MEAS_CONTROL                      0x0000820C
#define M_MEAS_RESTORE_MARKER               0x0000820D
#define M_MEAS_DRAW                         0x0000820E
#define M_MEAS_STREAM                       0x0000820F
#define M_MEAS_SET_SCORE                    0x00008210
#define M_MEAS_GET_SCORE                    0x00008211

#define M_CAL_ALLOC                         0x00008401
#define M_CAL_GRID                          0x00008402
#define M_CAL_TRANSFORM_IMAGE               0x00008403
#define M_CAL_CONTROL                       0x00008404
#define M_CAL_FREE                          0x00008405
#define M_CAL_INQUIRE                       0x00008406
#define M_CAL_LIST                          0x00008407
#define M_CAL_SAVE                          0x00008408
#define M_CAL_RESTORE                       0x00008409
#define M_CAL_RELATIVE_ORIGIN               0x0000840A
#define M_CAL_TRANSFORM_COORDINATE          0x0000840B
#define M_CAL_TRANSFORM_COORDINATE_LIST     0x0000840C
#define M_CAL_TRANSFORM_RESULT              0x0000840D
#define M_CAL_TRANSFORM_RESULT_IN_REGION    0x0000840E
#define M_CAL_CHILD_NEW                     0x0000840F
#define M_CAL_STREAM                        0x00008410
#define M_CAL_ASSOCIATE                     0x00008411
#define M_CAL_GET_COORDINATE_SYSTEM         0x00008412
#define M_CAL_SET_COORDINATE_SYSTEM         0x00008413
#define M_CAL_DRAW                          0x00008414
#define M_CAL_TRANSFORM_COORDINATE_3D_LIST  0x00008415
#define M_CAL_INQUIRE_SINGLE                0x00008416
#define M_CAL_FIXTURE                       0x00008417
#define M_CAL_TRANSFORM_RESULT_AT_POSITION  0x00008418
#define M_CAL_UNIFORM                       0x00008419

#define M_CODE_ALLOC                        0x00008601
#define M_CODE_CONTROL                      0x00008602
#define M_CODE_FREE                         0x00008603
#define M_CODE_GET_RESULT                   0x00008604
#define M_CODE_INQUIRE                      0x00008605
#define M_CODE_READ                         0x00008606
#define M_CODE_WRITE                        0x00008607
#define M_CODE_SAVE                         0x00008608
#define M_CODE_RESTORE                      0x00008609
#define M_CODE_VERIFY                       0x0000860A
#define M_CODE_STREAM                       0x0000860B
#define M_CODE_DRAW                         0x0000860C
#define M_CODE_ALLOC_RESULT                 0x0000860D
#define M_CODE_MODEL_FUNC                   0x0000860E
#define M_CODE_GET_RESULT_SINGLE            0x0000860F

#define M_OCR_ALLOC_FONT                    0x00008801
#define M_OCR_ALLOC_RESULT                  0x00008802
#define M_OCR_CALIBRATE_FONT                0x00008803
#define M_OCR_CONTROL                       0x00008804
#define M_OCR_COPY_FONT                     0x00008805
#define M_OCR_FREE                          0x00008806
#define M_OCR_GET_RESULT                    0x00008807
#define M_OCR_HOOK_FUNCTION                 0x00008808
#define M_OCR_IMPORT_FONT                   0x00008809
#define M_OCR_INQUIRE                       0x0000880A
#define M_OCR_MODIFY_FONT                   0x0000880B
#define M_OCR_READ_STRING                   0x0000880C
#define M_OCR_RESTORE_FONT                  0x0000880D
#define M_OCR_SAVE_FONT                     0x0000880E
#define M_OCR_SET_CONSTRAINT                0x0000880F
#define M_OCR_VALIDATE_STRING               0x00008810
#define M_OCR_VERIFY_STRING                 0x00008811
#define M_OCR_AUTO_ALLOC_FONT               0x00008812
#define M_OCR_PREPROCESS                    0x00008813
#define M_OCR_LINE_CONTROL                  0x00008814
#define M_OCR_LINE_SET_CONSTRAINT           0x00008815
#define M_OCR_STREAM                        0x00008816
#define M_OCR_DRAW                          0x00008817

#define M_PAT_ALLOC_AUTO_MODEL_NEW          0x00008C01
#define M_PAT_ALLOC_MODEL_NEW               0x00008C02
#define M_PAT_ALLOC_RESULT_NEW              0x00008C03
#define M_PAT_ALLOC_ROTATED_MODEL_NEW       0x00008C04
#define M_PAT_COPY_NEW                      0x00008C05
#define M_PAT_DRAW_NEW                      0x00008C06
#define M_PAT_FIND_MODEL_NEW                0x00008C07
#define M_PAT_FIND_MULTIPLE_MODEL_NEW       0x00008C08
#define M_PAT_FIND_ORIENTATION_NEW          0x00008C09
#define M_PAT_FREE_NEW                      0x00008C0A
#define M_PAT_GET_NUMBER_NEW                0x00008C0B
#define M_PAT_GET_RESULT_NEW                0x00008C0C
#define M_PAT_INQUIRE_NEW                   0x00008C0D
#define M_PAT_PREPROC_MODEL_NEW             0x00008C0E
#define M_PAT_RESTORE_NEW                   0x00008C11
#define M_PAT_SAVE_NEW                      0x00008C12
#define M_PAT_SET_ACCEPTANCE_NEW            0x00008C13
#define M_PAT_SET_ACCURACY_NEW              0x00008C14
#define M_PAT_SET_ANGLE_NEW                 0x00008C15
#define M_PAT_SET_CENTER_NEW                0x00008C16
#define M_PAT_SET_CERTAINTY_NEW             0x00008C17
#define M_PAT_SET_DONT_CARE_NEW             0x00008C18
#define M_PAT_SET_NUMBER_NEW                0x00008C19
#define M_PAT_SET_POSITION_NEW              0x00008C1A
#define M_PAT_SET_SEARCH_PARAMETER_NEW      0x00008C1B
#define M_PAT_SET_SPEED_NEW                 0x00008C1C
#define M_PAT_STREAM_NEW                    0x00008C22

#define M_MOD_ALLOC                         0x00008E01
#define M_MOD_ALLOC_RESULT                  0x00008E02
#define M_MOD_DEFINE                        0x00008E03
#define M_MOD_CONTROL                       0x00008E04
#define M_MOD_PREPROCESS                    0x00008E05
#define M_MOD_DRAW                          0x00008E06
#define M_MOD_FIND                          0x00008E07
#define M_MOD_GET_RESULT                    0x00008E08
#define M_MOD_FREE                          0x00008E09
#define M_MOD_INQUIRE                       0x00008E0A
#define M_MOD_SAVE                          0x00008E0B
#define M_MOD_RESTORE                       0x00008E0C
#define M_MOD_MASK                          0x00008E0D
#define M_MOD_DEFINE_FROM_FILE              0x00008E0E
#define M_MOD_STREAM                        0x00008E0F

#define M_BGA_ALLOC                         0x00009001
#define M_BGA_CALCULATE                     0x00009002
#define M_BGA_CONTROL                       0x00009003
#define M_BGA_DRAW                          0x00009004
#define M_BGA_FREE                          0x00009005
#define M_BGA_GET_RESULT                    0x00009006
#define M_BGA_DEFINE                        0x00009007
#define M_BGA_INQUIRE                       0x00009008
#define M_BGA_DEFINE_BY_LIST                0x00009009
#define M_BGA_PREPROCESS                    0x0000900A
#define M_BGA_RESTORE                       0x0000900B
#define M_BGA_SAVE                          0x0000900C
#define M_BGA_SET_PARAMETER                 0x0000900D
#define M_BGA_SET_POSITION                  0x0000900E

#define M_EDGE_ALLOC                        0x00009401
#define M_EDGE_ALLOC_RESULT                 0x00009402
#define M_EDGE_INQUIRE                      0x00009403
#define M_EDGE_CONTROL                      0x00009404
#define M_EDGE_CALCULATE                    0x00009405
#define M_EDGE_DRAW                         0x00009406
#define M_EDGE_GET_RESULT                   0x00009407
#define M_EDGE_FREE                         0x00009408
#define M_EDGE_SAVE                         0x00009409
#define M_EDGE_RESTORE                      0x0000940A
#define M_EDGE_MASK                         0x0000940B
#define M_EDGE_SELECT                       0x0000940C
#define M_EDGE_GET_NEIGHBORS                0x0000940D
#define M_EDGE_PUT                          0x0000940E
#define M_EDGE_STREAM                       0x0000940F

#define M_STR_ALLOC                         0x00009601
#define M_STR_ALLOC_RESULT                  0x00009602
#define M_STR_FREE                          0x00009603
#define M_STR_CONTROL                       0x00009604
#define M_STR_INQUIRE                       0x00009605
#define M_STR_EDIT_FONT                     0x00009606
#define M_STR_SET_CONSTRAINT                0x00009607
#define M_STR_PREPROCESS                    0x00009608
#define M_STR_READ                          0x00009609
#define M_STR_GET_RESULT                    0x0000960A
#define M_STR_DRAW                          0x0000960B
#define M_STR_SAVE                          0x0000960C
#define M_STR_RESTORE                       0x0000960D
#define M_STR_STREAM                        0x0000960E
#define M_STR_EXPERT                        0x0000960F
#define M_STR_TRAIN                         0x00009610
#define M_STR_HOOK_TRAIN_FUNC               0x00009611
#define M_STR_GENERATE                      0x00009612

#define M_MET_ALLOC                         0x00009A01
#define M_MET_ALLOC_RESULT                  0x00009A02
#define M_MET_CONTROL                       0x00009A03
#define M_MET_INQUIRE                       0x00009A04
#define M_MET_ADD_FEATURE                   0x00009A05
#define M_MET_ADD_TOLERANCE                 0x00009A06
#define M_MET_SET_POSITION                  0x00009A07
#define M_MET_SET_REGION                    0x00009A08
#define M_MET_CALCULATE                     0x00009A09
#define M_MET_GET_RESULT                    0x00009A0A
#define M_MET_DRAW                          0x00009A0B
#define M_MET_SAVE                          0x00009A0C
#define M_MET_RESTORE                       0x00009A0D
#define M_MET_STREAM                        0x00009A0E
#define M_MET_FREE                          0x00009A0F
#define M_MET_NAME                          0x00009A10

#define M_COL_ALLOC                         0x00009C01
#define M_COL_ALLOC_RESULT                  0x00009C02
#define M_COL_DEFINE                        0x00009C03
#define M_COL_MATCH                         0x00009C04
#define M_COL_FREE                          0x00009C05
#define M_COL_CONTROL                       0x00009C06
#define M_COL_INQUIRE                       0x00009C07
#define M_COL_PREPROCESS                    0x00009C08
#define M_COL_GET_RESULT                    0x00009C09
#define M_COL_DISTANCE                      0x00009C0A
#define M_COL_DRAW                          0x00009C0B
#define M_COL_STREAM                        0x00009C0C
#define M_COL_SAVE                          0x00009C0D
#define M_COL_RESTORE                       0x00009C0E
#define M_COL_MASK                          0x00009C0F
#define M_COL_SET_METHOD                    0x00009C10
#define M_COL_PROJECT                       0x00009C11

#define M_3DMAP_ALLOC                       0x00009E01
#define M_3DMAP_ALLOC_RESULT                0x00009E02
#define M_3DMAP_INQUIRE                     0x00009E03
#define M_3DMAP_CONTROL                     0x00009E04
#define M_3DMAP_GET_RESULT                  0x00009E05
#define M_3DMAP_FREE                        0x00009E06
#define M_3DMAP_SAVE                        0x00009E07
#define M_3DMAP_RESTORE                     0x00009E08
#define M_3DMAP_STREAM                      0x00009E09
#define M_3DMAP_ADD_SCAN                    0x00009E0A
#define M_3DMAP_CALIBRATE                   0x00009E0B
#define M_3DMAP_EXTRACT                     0x00009E0C
#define M_3DMAP_TRIANGULATE                 0x00009E0D
#define M_3DMAP_SET_GEOMETRY                0x00009E0E
#define M_3DMAP_DRAW                        0x00009E0F
#define M_3DMAP_STAT                        0x00009E10
#define M_3DMAP_ARITH                       0x00009E11

#define M_REG_ALLOC                         0x0000A001
#define M_REG_ALLOC_RESULT                  0x0000A002
#define M_REG_INQUIRE                       0x0000A003
#define M_REG_CONTROL                       0x0000A004
#define M_REG_CALCULATE                     0x0000A005
#define M_REG_DRAW                          0x0000A006
#define M_REG_GET_RESULT                    0x0000A008
#define M_REG_FREE                          0x0000A009
#define M_REG_SAVE                          0x0000A00B
#define M_REG_RESTORE                       0x0000A00C
#define M_REG_STREAM                        0x0000A00F
#define M_REG_SET_LOCATION                  0x0000A010
#define M_REG_TRANSFORM_IMAGE               0x0000A011
#define M_REG_TRANSFORM_COORDINATE          0x0000A012
#define M_REG_TRANSFORM_COORDINATE_LIST     0x0000A013

#define M_BEAD_ALLOC                        0x0000A801
#define M_BEAD_ALLOC_RESULT                 0x0000A802
#define M_BEAD_FREE                         0x0000A803
#define M_BEAD_CONTROL                      0x0000A804
#define M_BEAD_INQUIRE                      0x0000A805
#define M_BEAD_TRAIN                        0x0000A806
#define M_BEAD_TEMPLATE                     0x0000A807
#define M_BEAD_VERIFY                       0x0000A808
#define M_BEAD_GET_RESULT                   0x0000A809
#define M_BEAD_DRAW                         0x0000A80A
#define M_BEAD_SAVE                         0x0000A80B
#define M_BEAD_RESTORE                      0x0000A80C
#define M_BEAD_STREAM                       0x0000A80D
#define M_BEAD_GETNEIGHBORS                 0x0000A80E

#define M_PPCBENCH_MAIN                     0x00009201

#endif //__FUNCTIONCODES_H__