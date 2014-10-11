

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Tue Sep 30 02:50:36 2014
 */
/* Compiler settings for fsNVRPlugin.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0595 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_fsNVRPluginLib,0x7268A885,0xF493,0x46F2,0x8D,0xED,0xF0,0x61,0x62,0x5C,0x78,0xBA);


MIDL_DEFINE_GUID(IID, DIID__DfsNVRPlugin,0x83DDB94A,0x2188,0x4895,0xBC,0x62,0xAF,0xFE,0xE1,0xA4,0xA5,0x9E);


MIDL_DEFINE_GUID(IID, DIID__DfsNVRPluginEvents,0xA89C02DB,0x6452,0x43F4,0x94,0x65,0xEA,0xBC,0x6B,0x7C,0x8B,0xD8);


MIDL_DEFINE_GUID(CLSID, CLSID_fsNVRPlugin,0x35EA3D63,0x87A2,0x4F31,0xA5,0x96,0xA1,0xDF,0x79,0x33,0xDF,0xA5);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



