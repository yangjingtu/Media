/***********************************************************************************************************************
Copyright:  
FileName: Frame.h   
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/25/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef COMMON_FRAME_H_
#define COMMON_FRAME_H_
#include "BaseType.h"
#include "CommDef.h"
#ifdef RUN_WIN32
#include "Win32Def.h"
#elif
Error "Please choose platfrom"
#endif

/************************************************************************************************************************/
#define FRAME_TAG    *(U32*)"H_FM"
#define FRAME_END    *(U32*)"E_FM"
#define FRAME_N		 'N'              //all frame type
#define FRAME_I		 'I'
#define FRAME_P      'P'
#define FRAME_A      'A'
#define FRAME_M      'M'

#define ENC_H264	  0x04
#define ENC_MJ		  0x10
#define ENC_JP		  0x20
#define ENC_MP4		  0x30
#define ENC_PCM		  0x40
#define ENC_AAC		  0x41
#define FS_FRAME_TAG_LEN    (sizeof(U32))

/************************************************************************************************************************/
typedef struct VideoHead_
{
    U16  VWidth;
    U16  VHeight;
    U32  VFrameRate;
    U32  VBitRate;
} ATTRIBUTE_PACKED VideoHead_t;

typedef struct AudioHead_t
{
    U32  SampleRate;
    U32  BitsPerSample;
} ATTRIBUTE_PACKED AudioHead_t;

typedef struct Frame_
{
    U32 FrameTag;
	U8  EncodeType;
	U8  FrameType;
	U16 Id;
    U32 FrameNo;
    S32 DataLen;
    S64 Pts;
    S64 Time;
    union
    {
        VideoHead_t Video;
        AudioHead_t Audio;
    };
    U8  Rv[8];
    S8  Data[0];
} ATTRIBUTE_PACKED Frame_t;


#define  IS_FRAMEEND(f)  \
    ( ( *(U32*)( (S8*)f + sizeof(Frame_t) + f->DataLen - FS_FRAME_TAG_LEN) ) ==  FRAME_END ?  1 : 0 )

#define  IS_FRAME(f)  \
    ( ( f->frameTag == FrameTag && ( f->FrameType == FRAME_I || f->FrameType == FRAME_P || f->FrameType == FRAME_M || f->FrameType == FRAME_A ) ) ?  1 : 0 )

#define  IS_VFRAME(f) \
    ( ( f->frameTag == FrameTag && ( f->FrameType == FRAME_I || f->FrameType == FRAME_P) ) ?  1 : 0 )

#define  IS_AFRAME(f) \
    ( ( f->frameTag == FrameTag && f->FrameType == FRAME_A ) ?  1 : 0 )


#define  IS_IFRAME(f) \
    ( ( f->frameTag == FrameTag && f->FrameType == FRAME_I ) ?  1 : 0 )

#define  IS_PFRAME(f) \
    ( ( f->frameTag == FrameTag && f->FrameType == FRAME_P ) ?  1 : 0 )

#define  IS_MFRAME(f) \
    ( ( f->frameTag == FrameTag && f->FrameType == FRAME_M ) ?  1 : 0 )

/***************************************************************/
#endif /* COMMON_FRAME_H_ */
