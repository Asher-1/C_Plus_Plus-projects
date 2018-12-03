

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Sun Aug 21 19:28:37 2016
 */
/* Compiler settings for STLViewerHandlers.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
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

MIDL_DEFINE_GUID(IID, IID_IPreview,0x7E9EFDA0,0xDB6E,0x4611,0x84,0xF2,0x1F,0xC8,0xE4,0x59,0xC4,0xDB);


MIDL_DEFINE_GUID(IID, IID_IThumbnail,0x7FD06204,0xB970,0x42EB,0xA8,0xC4,0x4C,0x59,0x2B,0x7F,0xDA,0xEA);


MIDL_DEFINE_GUID(IID, IID_ISearch,0x231329B8,0xAA89,0x4AB6,0xA0,0x21,0x73,0xEB,0x61,0xB0,0x6B,0x04);


MIDL_DEFINE_GUID(IID, LIBID_STLViewerHandlersLib,0xC7C98F1E,0x53DF,0x48FC,0x9D,0x8A,0x02,0x6E,0x08,0x66,0x96,0x9E);


MIDL_DEFINE_GUID(CLSID, CLSID_Preview,0x13B23B27,0xD454,0x49B1,0x97,0xB2,0x6E,0x6F,0x9C,0x11,0x8A,0xAD);


MIDL_DEFINE_GUID(CLSID, CLSID_Thumbnail,0x800809D9,0x32ED,0x4992,0x84,0x5D,0xA1,0xC4,0x35,0x38,0x66,0x09);


MIDL_DEFINE_GUID(CLSID, CLSID_Search,0xDAA928BA,0x4465,0x4BEE,0x93,0xE8,0xA1,0x71,0x3B,0x32,0x62,0x73);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



