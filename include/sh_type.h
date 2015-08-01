#ifndef __SHARE_TYPE_H__
#define __SHARE_TYPE_H__

/*
����淶:
����:
    0. �ļ�����: ȫСд,����֮��ֱ������. ��sh_ ��Ϊǰ׺
    1. ����: C��ͷ �������,����ĸ��д. eg: CFile, CFileLog
       �ṹ����: S��ͷ �������,����ĸ��д. eg: SFile, SFileLog
       ע��:������, �ӿ�, �� I ��ͷ, �������,����ĸ��д. eg: IFile, IFileLog

    2. typedef ����������: ȫСд,����֮��ֱ������, eg: file, file_log

    3. ��������: �ֲ�����: ȫСд,  eg: name, id, i,j
                 ���Ա����: m_��ͷ,ȫСд,����֮����_���� eg: m_id, m_name
                 ȫ�ֱ���: g_��ͷ,ȫСд,����֮����_���� eg: g_id, g_name
                 ����, ö��: ȫ��д,����֮����_���� eg: ONE, TOW,
                 ��: ȫ��д,����֮����_���� eg: ONE, TOW, ONE_DAY

    4. ��������: ��˽�к���: ȫСд, ����֮����_���� eg: fun(), push_back()
                 ˽�к���: _��ͷ,ȫСд, ����֮����_���� eg: _fun(), _push_back()
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

