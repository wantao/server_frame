#ifndef __SHARE_TYPE_H__
#define __SHARE_TYPE_H__

/*
编码规范:
命名:
    0. 文件命名: 全小写,单词之间直接连接. 以sh_ 作为前缀
    1. 类名: C开头 单词组合,首字母大写. eg: CFile, CFileLog
       结构体名: S开头 单词组合,首字母大写. eg: SFile, SFileLog
       注意:抽象类, 接口, 以 I 开头, 单词组合,首字母大写. eg: IFile, IFileLog

    2. typedef 命名的类型: 全小写,单词之间直接连接, eg: file, file_log

    3. 变量命名: 局部变量: 全小写,  eg: name, id, i,j
                 类成员变量: m_开头,全小写,单词之间用_连接 eg: m_id, m_name
                 全局变量: g_开头,全小写,单词之间用_连接 eg: g_id, g_name
                 常量, 枚举: 全大写,单词之间用_连接 eg: ONE, TOW,
                 宏: 全大写,单词之间用_连接 eg: ONE, TOW, ONE_DAY

    4. 函数命名: 非私有函数: 全小写, 单词之间用_连接 eg: fun(), push_back()
                 私有函数: _开头,全小写, 单词之间用_连接 eg: _fun(), _push_back()
*/

namespace sh
{

//////////////////////////////////////////////////////////////////////////
//// base type
//////////////////////////////////////////////////////////////////////////
typedef char                int8;
typedef short               int16;
typedef int                 int32;
typedef long long           int64;
typedef unsigned long long  uint64;
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;

#if (defined(WIN32) || defined(WIN64))
typedef unsigned long       DWORD;
#endif

typedef unsigned long       ulong;
typedef unsigned int        uint;

typedef void*               void_ptr;


}

#endif //__SHARE_TYPE_H__

