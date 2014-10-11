

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __wellMediaidl_h__
#define __wellMediaidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DfsNVRPlugin_FWD_DEFINED__
#define ___DfsNVRPlugin_FWD_DEFINED__
typedef interface _DfsNVRPlugin _DfsNVRPlugin;

#endif 	/* ___DfsNVRPlugin_FWD_DEFINED__ */


#ifndef ___DfsNVRPluginEvents_FWD_DEFINED__
#define ___DfsNVRPluginEvents_FWD_DEFINED__
typedef interface _DfsNVRPluginEvents _DfsNVRPluginEvents;

#endif 	/* ___DfsNVRPluginEvents_FWD_DEFINED__ */


#ifndef __fsNVRPlugin_FWD_DEFINED__
#define __fsNVRPlugin_FWD_DEFINED__

#ifdef __cplusplus
typedef class fsNVRPlugin fsNVRPlugin;
#else
typedef struct fsNVRPlugin fsNVRPlugin;
#endif /* __cplusplus */

#endif 	/* __fsNVRPlugin_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_fsNVRPlugin_0000_0000 */
/* [local] */ 

#pragma once
#pragma region Desktop Family
#pragma endregion


extern RPC_IF_HANDLE __MIDL_itf_fsNVRPlugin_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_fsNVRPlugin_0000_0000_v0_0_s_ifspec;


#ifndef __fsNVRPluginLib_LIBRARY_DEFINED__
#define __fsNVRPluginLib_LIBRARY_DEFINED__

/* library fsNVRPluginLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_fsNVRPluginLib;

#ifndef ___DfsNVRPlugin_DISPINTERFACE_DEFINED__
#define ___DfsNVRPlugin_DISPINTERFACE_DEFINED__

/* dispinterface _DfsNVRPlugin */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DfsNVRPlugin;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("83DDB94A-2188-4895-BC62-AFFEE1A4A59E")
    _DfsNVRPlugin : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DfsNVRPluginVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DfsNVRPlugin * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DfsNVRPlugin * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DfsNVRPlugin * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DfsNVRPlugin * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DfsNVRPlugin * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DfsNVRPlugin * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DfsNVRPlugin * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } _DfsNVRPluginVtbl;

    interface _DfsNVRPlugin
    {
        CONST_VTBL struct _DfsNVRPluginVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DfsNVRPlugin_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DfsNVRPlugin_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DfsNVRPlugin_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DfsNVRPlugin_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DfsNVRPlugin_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DfsNVRPlugin_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DfsNVRPlugin_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DfsNVRPlugin_DISPINTERFACE_DEFINED__ */


#ifndef ___DfsNVRPluginEvents_DISPINTERFACE_DEFINED__
#define ___DfsNVRPluginEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DfsNVRPluginEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DfsNVRPluginEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("A89C02DB-6452-43F4-9465-EABC6B7C8BD8")
    _DfsNVRPluginEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DfsNVRPluginEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DfsNVRPluginEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DfsNVRPluginEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DfsNVRPluginEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DfsNVRPluginEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DfsNVRPluginEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DfsNVRPluginEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DfsNVRPluginEvents * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } _DfsNVRPluginEventsVtbl;

    interface _DfsNVRPluginEvents
    {
        CONST_VTBL struct _DfsNVRPluginEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DfsNVRPluginEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DfsNVRPluginEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DfsNVRPluginEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DfsNVRPluginEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DfsNVRPluginEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DfsNVRPluginEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DfsNVRPluginEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DfsNVRPluginEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_fsNVRPlugin;

#ifdef __cplusplus

class DECLSPEC_UUID("35EA3D63-87A2-4F31-A596-A1DF7933DFA5")
fsNVRPlugin;
#endif
#endif /* __fsNVRPluginLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


