#ifndef __SHARE_DEFINE_H__
#define __SHARE_DEFINE_H__


//////////////////////////////////////////////////////////////////////////
/// max, min
/// 取消 max，min的宏定义，使用标准库提供的max，min函数
//////////////////////////////////////////////////////////////////////////
#undef max
#undef min

//#ifndef max
//#define max(a,b) (a) > (b) ? (a) : (b)
//#endif
//#ifndef min
//#define min(a,b) (a) < (b) ? (a) : (b)
//#endif


//////////////////////////////////////////////////////////////////////////
//// defined NULL
//////////////////////////////////////////////////////////////////////////
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif


//////////////////////////////////////////////////////////////////////////
//// API 函数调用方式
//////////////////////////////////////////////////////////////////////////
#if (defined(WIN32) || defined(WIN64))
#define SHARED_API DWORD WINAPI
#else
#define SHARED_API void* 
#endif


//////////////////////////////////////////////////////////////////////////
//// FORMAT INT64 
//////////////////////////////////////////////////////////////////////////
#if !(defined(WIN32) || defined(WIN64))
#define FORMAT_I64  "%lld"
#define FORMAT_U64  "%llu"
#define FORMAT_X64  "%llx"
#else
#define FORMAT_I64  "%I64d"
#define FORMAT_U64  "%I64u"
#define FORMAT_X64  "%I64x"
#endif



//////////////////////////////////////////////////////////////////////////
/// string 
//////////////////////////////////////////////////////////////////////////
#if (defined(WIN32) || defined(WIN64))
#pragma   warning(disable:   4996)
#define snprintf _snprintf
#define swprintf _snwprintf
#else
#define snprintf snprintf
#define swprintf swprintf
#endif


//////////////////////////////////////////////////////////////////////////
/// other 
//////////////////////////////////////////////////////////////////////////
#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#if (defined(WIN32) || defined(WIN64))
//////////////////////////////////////////////////////////////////////////
//// disable warning
//////////////////////////////////////////////////////////////////////////
//#pragma warning(disable:4996) // suppress VS 2005 deprecated function warnings
//#pragma warning(disable:4786) // for string
#else
#define SOCKET int
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR            (-1)
#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
#endif //#if (defined(WIN32) || defined(WIN64))



#endif //__SHARE_DEFINE_H__

