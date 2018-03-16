//MfuncBuf DTK functions

/***************************************************************************/
/*

    Filename:  MILFUNC.H
    Owner   :  Matrox Imaging dept.
    Content :  This file contains the prototypes for the Matrox
               Imaging Library (MIL) MfuncBuf DTK functions.

    COPYRIGHT (c) 2005  Matrox Electronic Systems Ltd.
    All Rights Reserved

*/
/***************************************************************************/

#ifndef __MFUNCBUF_H
#define __MFUNCBUF_H

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif

MFTYPE32 void MFTYPE MfuncBufInCache(void* BufferInfoPtr);
MFTYPE32 void MFTYPE MfuncBufModified(void* BufferInfoPtr);

#ifndef __MILBUFFERINFO_H__
#if !M_MIL_USE_CPP_FUNC_BUF
typedef struct{long Dummy;}  MilBufferInfo;
#define MIL_BUFFER_INFO MilBufferInfo
MFTYPE32 MIL_DATA_PTR  MFTYPE MfuncBufHostAdress(MilBufferInfo* BufInfoPtr);
MFTYPE32 MIL_DATA_PTR  MFTYPE MfuncBufBandHostAdress(MilBufferInfo* BufInfoPtr, long Band);
MFTYPE32 MIL_DATA_PTR  MFTYPE MfuncBufBandPhysAdress(MilBufferInfo* BufInfoPtr, long Band);
MFTYPE32 MIL_DATA_PTR  MFTYPE MfuncBufPhysicalAdress(MilBufferInfo* BufInfoPtr);
MFTYPE32 long MFTYPE MfuncBufSizeX(MilBufferInfo* BufInfoPtr);
MFTYPE32 long MFTYPE MfuncBufSizeY(MilBufferInfo* BufInfoPtr);
MFTYPE32 long MFTYPE MfuncBufSizeBand(MilBufferInfo* BufInfoPtr);
MFTYPE32 long MFTYPE MfuncBufSizeBit(MilBufferInfo* BufInfoPtr);
MFTYPE32 long MFTYPE MfuncBufType(MilBufferInfo* BufInfoPtr);
MFTYPE32 long MFTYPE MfuncBufPitch(MilBufferInfo* BufInfoPtr);
MFTYPE32 long MFTYPE MfuncBufPitchByte(MilBufferInfo* BufInfoPtr);
MFTYPE32 BUFATTRTYPE MFTYPE MfuncBufAttribute(MilBufferInfo* BufInfoPtr);
MFTYPE32 BUFATTRTYPE MFTYPE MfuncBufFormat(MilBufferInfo* BufInfoPtr);
MFTYPE32 long MFTYPE MfuncBufOffsetX(MilBufferInfo* BufInfoPtr);
MFTYPE32 long MFTYPE MfuncBufOffsetY(MilBufferInfo* BufInfoPtr);
MFTYPE32 long MFTYPE MfuncBufOffsetBand(MilBufferInfo* BufInfoPtr);
MFTYPE32 long MFTYPE MfuncBufAncestorOffsetX(MilBufferInfo* BufInfoPtr);
MFTYPE32 long MFTYPE MfuncBufAncestorOffsetY(MilBufferInfo* BufInfoPtr);
MFTYPE32 long MFTYPE MfuncBufAncestorOffsetBand(MilBufferInfo* BufInfoPtr);
MFTYPE32 long MFTYPE MfuncBufAncestorOffsetBit(MilBufferInfo* BufInfoPtr);
MFTYPE32 long MFTYPE MfuncBufNativeId(MilBufferInfo* BufInfoPtr);
MFTYPE32 MIL_ID MFTYPE MfuncBufId(MilBufferInfo* BufInfoPtr);
MFTYPE32 MIL_ID MFTYPE MfuncBufParentId(MilBufferInfo* BufInfoPtr);
MFTYPE32 MIL_ID MFTYPE MfuncBufAncestorId(MilBufferInfo* BufInfoPtr);
MFTYPE32 MIL_ID MFTYPE MfuncBufOwnerSystemId(MilBufferInfo* BufInfoPtr);
MFTYPE32 long MFTYPE MfuncBufOwnerSystemType(MilBufferInfo* BufInfoPtr);
#endif 
#endif 


#ifdef __cplusplus
}
#endif

#endif 