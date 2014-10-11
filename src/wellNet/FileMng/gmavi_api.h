#ifndef __GM_AVI_API_H__
#define __GM_AVI_API_H__

#include "gmtypes.h"

#ifndef TRUE
  #define TRUE (1==1)
#endif
#ifndef FALSE
  #define FALSE (1==0)
#endif

//typedef void* HANDLE;

#define GMAVI_FILEMODE_CREATE       0X1
#define GMAVI_FILEMODE_READ         0X2
#define GMAVI_FILEMODE_WRITE        0X4
#define GMAVI_FILEMODE_FIXED_SIZE   0X8

#define GMAVI_SECTION_AVI_HEADER      0x1
#define GMAVI_SECTION_STREAM_HEADER   0x2
#define GMAVI_SECTION_STREAM_DATA     0x4
#define GMAVI_SECTION_INDEX           0x8


/* Only for GM function: GMAVIConditionIndexSearch() */
#define GMAVI_SEARCH_GivenTime          1
#define GMAVI_SEARCH_StartTag           2
#define GMAVI_SEARCH_GivenIndex			3

#ifndef _WIN32
#define ATTRIBUTE_PACKED __attribute__((packed))
#else
#define ATTRIBUTE_PACKED
#pragma pack(push)
#pragma pack(1)
#endif

//======================================================//
typedef struct tagAviMainHeader {
    unsigned char fcc[4];
    int  cb;
    int  dwMicroSecPerFrame;
    int  dwMaxBytesPerSec;
    int  dwPaddingGranularity;
    int  dwFlags;
    int  dwTotalFrames;
    int  dwInitialFrames;
    int  dwStreams;
    int  dwSuggestedBufferSize;
    int  dwWidth;
    int  dwHeight;
    int  dwReserved[4];
} ATTRIBUTE_PACKED AviMainHeader;

typedef struct tagAviStreamHeader {
     unsigned char fcc[4];
     int  cb;
     unsigned char fccType[4];
     unsigned char fccHandler[4];
     int  dwFlags;
     unsigned short   wPriority;
     unsigned short   wLanguage;
     int  dwInitialFrames;
     int  dwScale;
     int  dwRate;
     int  dwStart;
     int  dwLength;
     int  dwSuggestedBufferSize;
     int  dwQuality;
     int  dwSampleSize;
     struct {
         short int left;
         short int top;
         short int right;
         short int bottom;
     }  rcFrame;
} ATTRIBUTE_PACKED AviStreamHeader;

typedef struct tagBitMapInfoHeader{
  int   biSize; 
  int   biWidth; 
  int   biHeight; 
  short int   biPlanes; 
  short int   biBitCount; 
  int   biCompression; 
  int   biSizeImage; 
  int   biXPelsPerMeter; 
  int   biYPelsPerMeter; 
  int   biClrUsed; 
  int   biClrImportant; 
} ATTRIBUTE_PACKED BitMapInfoHeader;

typedef struct tagRGBQuad {
  unsigned char    rgbBlue; 
  unsigned char    rgbGreen; 
  unsigned char    rgbRed; 
  unsigned char    rgbReserved; 
} RGBQuad ; 

typedef struct tagBitmapInfo { 
  BitMapInfoHeader bmiHeader; 
  //RGBQuad          bmiColors[1]; 
} ATTRIBUTE_PACKED BitmapInfo;

typedef struct tagWaveFormateX{ 
  short int  wFormatTag; 
  short int  nChannels; 
  int nSamplesPerSec; 
  int nAvgBytesPerSec; 
  short int  nBlockAlign; 
  short int  wBitsPerSample; 
  short int  cbSize; 
}  ATTRIBUTE_PACKED WaveFormateX;

typedef struct tagAviIndex {
  int   dwChunkId;
  int   dwFlags;
  int   dwOffset;
  int   dwSize;
} ATTRIBUTE_PACKED AviIndex;

/* for GM only (START) */
typedef struct tagGMTagStreamFormat {
  int       total_channels;
  int       channel;
  GM_TIME   start;
  GM_TIME   end;
  int       fragment_count;
} ATTRIBUTE_PACKED GMTagStreamFormat;
/* for GM only (END) */

typedef union GmAviStreamFormatTag {
    BitmapInfo    video_format;
    WaveFormateX  audio_format;

/* for GM only (START) */
    GMTagStreamFormat gmtag_format;
/* for GM only (END) */

}ATTRIBUTE_PACKED GmAviStreamFormat;

//======================================================//


typedef struct GmAviChunkSizeTag {
    int     stream_header_size;
    int     stream_data_size;
    int     index_size;
} ATTRIBUTE_PACKED GmAviChunkSize;

typedef struct GmAviChunkOffsetTag {
    int     stream_data_offset;
    int     index_offset;
} ATTRIBUTE_PACKED GmAviChunkOffset;

#ifdef _WIN32
#pragma pack(pop)
#endif

typedef enum GmAviFilePositionTag {
    // general
/*
    GMAVI_SEEK_SET, // start of file
    GMAVI_SEEK_CUR,
    GMAVI_SEEK_END,
    GMAVI_SEEK_STREAM_HEADER_OFFSET,
    GMAVI_SEEK_STREAM_DATA_OFFSET,
    GMAVI_SEEK_INDEX_OFFSET,
*/
    // special for GM
    GMAVI_SEEK_TO_BEGINNING,
    GMAVI_SEEK_TO_END,
    GMAVI_SEEK_GIVEN_INDEX
}GmAviFilePosition;

typedef enum GmAviFileTellTag {
    // general
    GMAVI_TELL_CUR,     //current position
    // special for GM
    GMAVI_TELL_STREAM_HEADER_OFFSET,
    GMAVI_TELL_STREAM_DATA_OFFSET,
    GMAVI_TELL_INDEX_OFFSET
}GmAviFileTell;


#define DLL_API int _declspec(dllexport)

#ifdef __cplusplus
extern "C" {
#endif

HANDLE _declspec(dllexport) GMAVIOpen(char* filename, int mode ,int size);
DLL_API GMAVIClose(HANDLE handle);
DLL_API GMAVISetChunkSize(HANDLE handle, GmAviChunkSize *chunk_size);
DLL_API GMAVISeek(HANDLE handle, int whence, GMSearchIndex *offset);
DLL_API GMAVISeekToFrame(HANDLE handle, int frame, int *pOffset);
DLL_API GMAVITell(HANDLE handle, int tell_what, GMSearchIndex *offset);
DLL_API GMAVIReset(HANDLE handle, int items);
DLL_API GMAVISetAviMainHeader(HANDLE handle, AviMainHeader *avi_main_header);
DLL_API GMAVISetStreamHeader(HANDLE handle, AviStreamHeader *avi_stream_header, GmAviStreamFormat *avi_stream_format, int *out_streamid);
DLL_API GMAVISetStreamDataAndIndex(HANDLE handle, int streamid, unsigned char *data, int length, int indx_flag, unsigned char *extra_data, int extra_length,int totalsize);
DLL_API GMAVIGetAviMainHeader(HANDLE handle, AviMainHeader *avi_main_header);
DLL_API GMAVIGetStreamHeaderNum(HANDLE handle, int *count);
DLL_API GMAVIGetStreamHeader(HANDLE handle, int num, AviStreamHeader *avi_stream_header, GmAviStreamFormat *avi_stream_format, int *out_streamid);
DLL_API GMAVIGetStreamDataAndIndex(HANDLE handle, int *streamid, unsigned char *data, int *length, int *indx_flag, unsigned char *extra_data, int *extra_length, int frame_no,int reverse,int *pos, int* newPos);
DLL_API GMAVIConditionIndexSearch(HANDLE handle, GMSearchIndex *out_value, int mode, int value1, int value2);
DLL_API GMAVIUpdateStreamHeader(HANDLE handle, int num, AviStreamHeader *avi_stream_header, GmAviStreamFormat *avi_stream_format);
DLL_API GMAVIFillAviMainHeaderValues(AviMainHeader *header, int width, int height, int framerate, int bitrate, int framecount);
DLL_API GMAVIFillVideoStreamHeaderValues(AviStreamHeader *header, GmAviStreamFormat *format, int type, int width, int height, int framerate, int bitrate, int framecount);
DLL_API GMAVIFillAudioStreamHeaderValues(AviStreamHeader *header, GmAviStreamFormat *format, int type, int channels, int sample_rate, int bitrate, int framecount);


#ifdef __cplusplus
}
#endif

#endif /* __GM_AVI_API_H__ */