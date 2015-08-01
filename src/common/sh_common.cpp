#include "sh_common.h"
#include "sh_systemtime.h"

namespace sh
{

#if (defined(WIN32) || defined(WIN64))
    static bool __CheckFileFilter(const char* pszFileName, const size_t nFileNameLen, const char* pszFilter)
    {
        int idx = int (nFileNameLen - 1);
        while (idx > 0){
            if (*(pszFileName+idx) == '.')
                break;
            --idx;
        }
        if (idx > 0 && idx < int(nFileNameLen - 1)){
            if (strcmp(pszFileName+idx, pszFilter) == 0)
                return true;
        }
        return false;
    }
#else
#endif

size_t enum_files(std::vector<std::string>&files, const char* pszDir, const char* pszFilter, bool bIncludeChildDir)
{
#if (defined(WIN32) || defined(WIN64))
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    char parten[MAX_PATH]={0};
    safe_sprintf(parten, "%s/*.*", pszDir);
    hFind = FindFirstFile(parten, &findFileData);
    if (INVALID_HANDLE_VALUE == hFind)
    {
        return 0;
    }
    else
    {
        do
        {
            if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0)
                continue;
            if (findFileData.dwFileAttributes == _A_SUBDIR)
            {
                if (bIncludeChildDir) {
                    char nextdir[MAX_PATH] = {0};
                    safe_sprintf(nextdir, "%s/%s", pszDir, findFileData.cFileName);
                    enum_files(files, nextdir, pszFilter, bIncludeChildDir);
                }
            }
            else if (strcmp(".*", pszFilter) == 0)
            {
                files.push_back(findFileData.cFileName);
            }
            else
            {
                if (__CheckFileFilter(findFileData.cFileName, strlen(findFileData.cFileName), pszFilter))
                {
                    files.push_back(findFileData.cFileName);
                }
            }
        }while (FindNextFile(hFind, &findFileData));
    }
#else
#endif
    return files.size();
}

uint32 a2u(const std::string& s)
{
    return a2u(s.c_str());
}

uint32 a2u(const char* pStr)
{
    assert(pStr);
#if (defined(WIN32) || defined(WIN64))
    if (NULL == pStr) return 0;
    return strtoul(pStr, NULL, 10);
#else
    if (NULL == pStr) return 0;
    uint32 result = 0;
    const char * pos = pStr;

    if ('+' == *pos)
    {
        pos++;
    }

    while (*pos != 0)
    {
        if (isdigit((unsigned char)*pos))
        {
            uint32 d = *pos - '0';
            result = result * 10 + d;
        }
        else
        {
            break;
        }
        pos ++;
    }
    return result;
#endif
}

uint64 a2u64(const std::string& s)
{
    return a2u64(s.c_str());
}

uint64 a2u64(const char* pStr)
{
    assert(pStr);
#if (defined(WIN32) || defined(WIN64))
    if (NULL == pStr) return 0;
    return _strtoui64(pStr, NULL, 10);
#else
    if (NULL == pStr) return 0;
    uint64 result = 0;
    const char * pos = pStr;
    
    if ('+' == *pos)
    {
        pos++;
    }

    while (*pos != 0 )
    {
        if (isdigit((unsigned char)*pos))
        {
            uint64 d = *pos - '0';
            result = result * 10 + d;
        }
        else
        {
            break;
        }
        pos ++;
    }
    return result;
#endif
}

std::string s2hex(const unsigned char *pBuf, uint32 bufLen, const char *pSplitter, int32 lineLen)
{
    assert(pBuf && pSplitter);
    static const unsigned char szHex[] = "0123456789abcdef";

    char* p = (char*)pBuf;
    std::string strResult;
    for (unsigned int i = 0; i < bufLen; ++i)
    {
        strResult += szHex[(p[i] >> 4) & 0xF];
        strResult += szHex[p[i] & 0xF];
        if (pSplitter != NULL && *pSplitter && i < bufLen - 1)
        {
            strResult.append(pSplitter);
        }
        if (lineLen > 0  && (i + 1) % lineLen == 0)
        {
            strResult += "\n";
        }
    }
    return strResult;
}

std::string i2hex(int32 num)
{
    char szBuf[16];
    snprintf(szBuf, sizeof(szBuf)-1, "0x%x",num);
    szBuf[sizeof(szBuf)-1] = '\0';
    return szBuf;
}

std::string time2str(time_t t)
{
    char szData[50];
    struct tm ptm;
    localtime_safe(&ptm, &t);
    sprintf(szData, "%d-%02d-%02d %02d:%02d:%02d", ptm.tm_year+1900, ptm.tm_mon+1, ptm.tm_mday,
        ptm.tm_hour, ptm.tm_min, ptm.tm_sec);
    return szData;
}

std::string time2datestr(time_t t)
{
    char szData[50];
    struct tm ptm;
    localtime_safe(&ptm, &t);
    sprintf(szData, "%d-%02d-%02d", ptm.tm_year+1900, ptm.tm_mon+1, ptm.tm_mday);
    return szData;
}

time_t str2time(const std::string& strDT)
{
    ::tm t={0};
    sscanf(strDT.c_str(), "%d-%02d-%02d %02d:%02d:%02d", &t.tm_year, &t.tm_mon, &t.tm_mday,
        &t.tm_hour, &t.tm_min, &t.tm_sec);
    t.tm_year -= 1900;
    t.tm_mon -= 1;
    return mktime(&t);
}

size_t split(std::vector<std::string>& v, const std::string& s)
{
#define IS_SPACE(val)	(val == ' ' || val == '\n' || val == '\t' || val == '\r' || val == '\0')
    std::string::value_type val;
    std::string::const_iterator i, last, end = s.end();
    for (last=i=s.begin(); i != end; ++i)
    {
        val = *i;
        if (IS_SPACE(val))
        {
            val = *last;
            if (!IS_SPACE(val))
            {
                v.push_back(std::string(last, i));
                last = i + 1;
            }
        }
        if (last == end)
            break;
        val = *last;
        if (IS_SPACE(val) && last<i)
            last = i;
    }

    if (last != i && last != end)
        v.push_back(std::string(last, i));

    return v.size();
#undef IS_SPACE
}

size_t split(std::vector<std::string>& v, const std::string& s, std::string::value_type separator)
{
    std::string::const_iterator i, last, end = s.end();
    for (last=i=s.begin(); i != end; ++i)
    {
        if (*i == separator)
        {
            v.push_back(std::string(last, i));
            last = i + 1;
        }
    }

    if (last != i)
        v.push_back(std::string(last, i));

    return v.size();
}

size_t split(std::vector<std::string>& v, const std::string& s, std::string separator)
{
    std::string::const_iterator i, last, end = s.end();
    for (last=i=s.begin(); i != end; ++i)
    {
        size_t len = std::min<size_t>(separator.length(), end-i);
        if (std::string(i, i + len) == separator)
        {
            if (last > i) continue;
            v.push_back(std::string(last, i));
            last = i + separator.length();
        }
    }

    if (last != i)
        v.push_back(std::string(last, i));

    return v.size();
}

std::string &lower(std::string& s)
{
    std::string::iterator i, end = s.end();
    for (i = s.begin(); i != end; ++i)
        if (*i >= 'A' && *i <= 'Z')
            *i = (*i) + ('a' - 'A');
    return s;
}

std::string &upper(std::string& s)
{
    std::string::iterator i, end = s.end();
    for (i = s.begin(); i != end; ++i)
        if (*i >= 'a' && *i <= 'z')
            *i = (*i) - ('a' - 'A');
    return s;
}

char* trim(char* src)
{
    char* start=NULL, *end=NULL;
    if(src==NULL) return NULL;
    while( *src == ' ' || *src == '\t' ) ++src;
    for(start=src;(*src)!='\0';++src)
    {
        if ( !(*src == ' ' || *src == '\t' || *src == '\n' || *src == '\r') )
            end=src;
    }
    if(end)  *(end+1) = '\0';
    return start;
}

void copytrim(std::string& des, const std::string& src)
{
    if (src.empty()) return ;
    std::string::const_iterator start=src.begin(), end=src.end();
    while (start!=end && (*start == ' ' || *start == '\t')) ++start;
    if (start == end) 
    {
        des = std::string(start, end);
        return;
    }
    --end;
    while (start!=end && (*end == ' ' || *end == '\t')) --end;
    des = std::string(start, end+1);
}

bool is_directory_exist(const char* lpdirectory)
{
    assert(lpdirectory);
#if (defined(WIN32) || defined(WIN64))
    DWORD dwRet = GetFileAttributes(lpdirectory);
    if (!((dwRet != 0xffffffff) && (dwRet & FILE_ATTRIBUTE_DIRECTORY)))
        return false;
    return true;
#else
    if (NULL == opendir(lpdirectory))
        return false;
    return true;
#endif
}

bool get_current_directory(char* lpdir, const int size)
{
    assert(lpdir);
#if (defined(WIN32) || defined(WIN64))
    if (::GetCurrentDirectory(size-1, lpdir) == 0)
        return false;
    return true;
#else
    if (getcwd(lpdir, size-1) == NULL)
        return false;
    return true;
#endif
}

bool create_directory(const char* lpdir)
{
    assert(lpdir);
#if (defined(WIN32) || defined(WIN64))
    if (::CreateDirectory(lpdir, NULL)==0)
        return false;
    return true;
#else
    int ret = mkdir(lpdir, 0777);
    if (-1 == ret)
        return false;
    return true;
#endif
}

const char* get_current_exe_directory()
{
#if (defined(WIN32) || defined(WIN64))
    static char szDirPath[MAX_PATH];
    if (0 == szDirPath[0])
    {
        if (::GetModuleFileName( NULL, szDirPath, sizeof(szDirPath)-1 ) == 0)
            return false;
        std::string s = szDirPath;
        size_t i = s.rfind('\\');
        s = s.substr(0, i);
        snprintf(szDirPath, sizeof(szDirPath)-1, "%s", s.c_str());
    }
    return szDirPath;
#else
    static char szDirPath[256];
    if (getcwd(szDirPath, sizeof(szDirPath)-1) == NULL)
        return "";
    return szDirPath;
#endif
}

std::string get_filename_by_filepath(const char* pFilePath)
{
    assert(pFilePath);
    const long len = (long)strlen(pFilePath);
    long i=len;
    while ( i > 0 )
    {
        -- i;
        if (*(pFilePath+i) == '\\' || *(pFilePath+i) == '/' )
            break;
    }
    if (i <= 0)
    {
        return pFilePath;
    }
    return (pFilePath+i+1);
}

std::string get_execute_file()
{
    char szDirPath[MAX_PATH];
#if (defined(WIN32) || defined(WIN64))
    if (::GetModuleFileName( NULL, szDirPath, sizeof(szDirPath)-1 ) == 0)
        return "";
    return get_filename_by_filepath(szDirPath);
#else
    const int len = readlink ("/proc/self/exe", szDirPath, MAX_PATH);
    if (-1 == len) return "";
    szDirPath[len] = '\0';
    return get_filename_by_filepath(szDirPath);
#endif
}

std::string get_current_exe_full_path()
{
    char szDirPath[MAX_PATH];
#if (defined(WIN32) || defined(WIN64))
    const DWORD len = ::GetModuleFileName( NULL, szDirPath, sizeof(szDirPath)-1 );
    if (len == 0)
        return "";
    szDirPath[len] = '\0';
    return szDirPath;
#else
    const int len = readlink ("/proc/self/exe", szDirPath, MAX_PATH);
    if (-1 == len) return "";
    szDirPath[len] = '\0';
    return szDirPath;
#endif
}

long GetFileLength(const char* pFilePath)
{
    assert(pFilePath);
#if (defined(WIN32) || defined(WIN64))
    int handle;
    handle = open(pFilePath, 0x0100); //open file for read
    long length = filelength(handle); //get length of file
    close(handle);
    return length;
#else
    int handle;
    handle = open(pFilePath, O_RDONLY);
    struct stat s = {0};
    fstat(handle, &s);
    close(handle);
    return s.st_size;
#endif
}

static const uint32 crc32_table[256] =
{
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419,
    0x706af48f, 0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4,
    0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07,
    0x90bf1d91, 0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de,
    0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7, 0x136c9856,
    0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
    0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4,
    0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
    0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3,
    0x45df5c75, 0xdcd60dcf, 0xabd13d59, 0x26d930ac, 0x51de003a,
    0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599,
    0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190,
    0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f,
    0x9fbfe4a5, 0xe8b8d433, 0x7807c9a2, 0x0f00f934, 0x9609a88e,
    0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
    0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed,
    0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
    0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3,
    0xfbd44c65, 0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2,
    0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a,
    0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5,
    0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa, 0xbe0b1010,
    0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17,
    0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6,
    0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615,
    0x73dc1683, 0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8,
    0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1, 0xf00f9344,
    0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
    0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a,
    0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
    0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1,
    0xa6bc5767, 0x3fb506dd, 0x48b2364b, 0xd80d2bda, 0xaf0a1b4c,
    0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef,
    0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe,
    0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31,
    0x2cd99e8b, 0x5bdeae1d, 0x9b64c2b0, 0xec63f226, 0x756aa39c,
    0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
    0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b,
    0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
    0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1,
    0x18b74777, 0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
    0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45, 0xa00ae278,
    0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7,
    0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc, 0x40df0b66,
    0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605,
    0xcdd70693, 0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8,
    0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b,
    0x2d02ef8d
};

uint32 get_crc32_code(uint32 crc, unsigned char *buf, size_t len)
{
    unsigned char *end;
    crc = ~crc;
    for (end = buf + len; buf < end; ++buf)
        crc = crc32_table[(crc ^ *buf) & 0xff] ^ (crc >> 8);
    return ~crc;
}

int32 rand_int(int32 nMin, int32 nMax)
{
    int nDelta = nMax - nMin;
    if (nDelta <= 0)
        return nMax;
#if (defined(WIN32) || defined(WIN64))
    return ((::rand() << 15) + ::rand())%(nDelta+1) + nMin;//rand(0,32767)
#else
    return ::rand()%(nDelta+1) + nMin;//rand(0,2147483647)
#endif
}

uint64 str_id_to_uint64(const std::string& strID, const STR_ID_FORMAT_TYPE strFormat)
{
    std::vector<std::string> v;
    sh::split(v, strID, '-');
    uint64 qwID(0);
    uint32 x1(0),x2(0),x3(0),x4(0),x5(0),x6(0);

    if (strFormat == G_STRID_FORMAT_DD && v.size()>1){
        x1 = a2u(v[0]);
        x2 = a2u(v[1]);
        qwID = ((uint64)x1<<32) + x2;
    }else if (strFormat == G_STRID_FORMAT_WWD && v.size()>2){
        x1 = a2u(v[0]);
        x2 = a2u(v[1]);
        x3 = a2u(v[2]);
        qwID = ((uint64)x1<<48) + ((uint64)x2<<32) + x3;
    }else if (strFormat == G_STRID_FORMAT_BBWD && v.size()>3){
        x1 = a2u(v[0]);
        x2 = a2u(v[1]);
        x3 = a2u(v[2]);
        x4 = a2u(v[3]);
        qwID = ((uint64)x1<<56) + ((uint64)x2<<48) + ((uint64)x3<<32) + x4;
    }else if (strFormat == G_STRID_FORMAT_WWWW && v.size()>3){
        x1 = a2u(v[0]);
        x2 = a2u(v[1]);
        x3 = a2u(v[2]);
        x4 = a2u(v[3]);
        qwID = ((uint64)x1<<48) + ((uint64)x2<<32) + ((uint64)x3<<16) + x4;
    }else if (strFormat == G_STRID_FORMAT_BBWWW && v.size()>4){
        x1 = a2u(v[0]);
        x2 = a2u(v[1]);
        x3 = a2u(v[2]);
        x4 = a2u(v[3]);
        x5 = a2u(v[4]);
        qwID = ((uint64)x1<<56) + ((uint64)x2<<48) + ((uint64)x3<<32) + ((uint64)x4<<16) + x5;
    }else if (strFormat == G_STRID_FORMAT_BBBBWW && v.size()>5){
        x1 = a2u(v[0]);
        x2 = a2u(v[1]);
        x3 = a2u(v[2]);
        x4 = a2u(v[3]);
        x5 = a2u(v[4]);
        x6 = a2u(v[5]);
        qwID = ((uint64)x1<<56) + ((uint64)x2<<48) + ((uint64)x3<<40) + ((uint64)x4<<32) + ((uint64)x5<<16) + x6;
    }else{
        assert(false && "wrong format");
        return qwID;
    }
    return qwID;
}

std::string uint64_to_str_id(uint64 qwID, const STR_ID_FORMAT_TYPE strFormat)
{
    std::string strResult;
    strResult.reserve(100);
    char szBuf[100];
    uint32 x1(0),x2(0),x3(0),x4(0),x5(0),x6(0);

    if (strFormat == G_STRID_FORMAT_DD){
        x1 = (uint32)(qwID >> 32);
        x2 = (uint32)(qwID & 0xffffffffULL);
        u2a(szBuf, x1);
        strResult = szBuf;
        strResult += "-";
        u2a(szBuf, x2);
        strResult += szBuf;
    }else if (strFormat == G_STRID_FORMAT_WWD){
        x1 = (uint32)(qwID>>48);
        x2 = (uint32)((qwID>>32)&0xffffULL);
        x3 = (uint32)(qwID & 0xffffffffULL);
        u2a(szBuf, x1);
        strResult = szBuf;
        strResult += "-";
        u2a(szBuf,x2);
        strResult += szBuf;
        strResult += "-";
        u2a(szBuf,x3);
        strResult += szBuf;
    }else if (strFormat == G_STRID_FORMAT_BBWD){
        x1 = (uint32)(qwID>>56);
        x2 = (uint32)((qwID>>48)&0xffULL);
        x3 = (uint32)((qwID>>32)&0xffffULL);
        x4 = (uint32)(qwID&0xffffffffULL);
        u2a(szBuf, x1);
        strResult = szBuf;
        strResult += "-";
        u2a(szBuf, x2);
        strResult += szBuf;
        strResult += "-";
        u2a(szBuf, x3);
        strResult += szBuf;
        strResult += "-";
        u2a(szBuf, x4);
        strResult += szBuf;
    }else if (strFormat == G_STRID_FORMAT_WWWW){
        x1 = (uint32)(qwID>>48);
        x2 = (uint32)((qwID>>32)&0xffffULL);
        x3 = (uint32)((qwID>>16)&0xffffULL);
        x4 = (uint32)(qwID&0xffffULL);
        u2a(szBuf, x1);
        strResult = szBuf;
        strResult += "-";
        u2a(szBuf, x2);
        strResult += szBuf;
        strResult += "-";
        u2a(szBuf, x3);
        strResult += szBuf;
        strResult += "-";
        u2a(szBuf, x4);
        strResult += szBuf;
    }else if (strFormat == G_STRID_FORMAT_BBWWW){
        x1 = (uint32)(qwID>>56);
        x2 = (uint32)((qwID>>48)&0xffULL);
        x3 = (uint32)((qwID>>32)&0xffffULL);
        x4 = (uint32)((qwID>>16)&0xffffULL);
        x5 = (uint32)(qwID&0xffffULL);
        u2a(szBuf, x1);
        strResult = szBuf;
        strResult += "-";
        u2a(szBuf, x2);
        strResult += szBuf;
        strResult += "-";
        u2a(szBuf, x3);
        strResult += szBuf;
        strResult += "-";
        u2a(szBuf, x4);
        strResult += szBuf;
        strResult += "-";
        u2a(szBuf, x5);
        strResult += szBuf;
    }else if (strFormat == G_STRID_FORMAT_BBBBWW){
        x1 = (uint32)(qwID>>56);
        x2 = (uint32)((qwID>>48)&0xffULL);
        x3 = (uint32)((qwID>>40)&0xffULL);
        x4 = (uint32)((qwID>>32)&0xffULL);
        x5 = (uint32)((qwID>>16)&0xffffULL);
        x6 = (uint32)(qwID&0xffffULL);
        u2a(szBuf, x1);
        strResult = szBuf;
        strResult += "-";
        u2a(szBuf, x2);
        strResult += szBuf;
        strResult += "-";
        u2a(szBuf, x3);
        strResult += szBuf;
        strResult += "-";
        u2a(szBuf, x4);
        strResult += szBuf;
        strResult += "-";
        u2a(szBuf, x5);
        strResult += szBuf;
        strResult += "-";
        u2a(szBuf, x6);
        strResult += szBuf;
    }else{
        assert(false && "wrong format");
        return strResult;
    }
    return strResult;
}

uint32 str_id_to_uint32(const std::string& strID, const STR_ID_FORMAT_TYPE strFormat)
{
    std::vector<std::string> v;
    sh::split(v, strID, '-');
    uint32 dwID(0);
    uint16 x1(0),x2(0),x3(0),x4(0);

    if (strFormat == G_STRID_FORMAT_WW && v.size()>1){
        x1 = static_cast<uint16>(a2u(v[0]));
        x2 = static_cast<uint16>(a2u(v[1]));
        dwID = ((uint32)x1<<16) + x2;
    }else if (strFormat == G_STRID_FORMAT_BBW && v.size()>2){
        x1 = static_cast<uint16>(a2u(v[0]));
        x2 = static_cast<uint16>(a2u(v[1]));
        x3 = static_cast<uint16>(a2u(v[2]));
        dwID = ((uint32)x1<<24) + ((uint32)x2<<16) + x3;
    }else if (strFormat == G_STRID_FORMAT_BBBB && v.size()>3){
        x1 = static_cast<uint16>(a2u(v[0]));
        x2 = static_cast<uint16>(a2u(v[1]));
        x3 = static_cast<uint16>(a2u(v[2]));
        x4 = static_cast<uint16>(a2u(v[3]));
        dwID = ((uint32)x1<<24) + ((uint32)x2<<16) + ((uint32)x3<<8) + x4;
    }else{
        assert(false && "wrong format");
        return dwID;
    }
    return dwID;
}

std::string uint32_to_str_id(uint32 dwID, const STR_ID_FORMAT_TYPE strFormat)
{
    std::string strResult;
    strResult.reserve(100);
    char szBuf[100];
    uint16 x1(0),x2(0),x3(0),x4(0);

    if (strFormat == G_STRID_FORMAT_WW){
        x1 = (uint16)(dwID >> 16);
        x2 = (uint16)(dwID & 0xffffUL);
        u2a(szBuf, x1);
        strResult = szBuf;
        strResult += "-";
        u2a(szBuf, x2);
        strResult += szBuf;
    }else if (strFormat == G_STRID_FORMAT_BBW){
        x1 = (uint16)(dwID>>24);
        x2 = (uint16)((dwID>>16)&0xffUL);
        x3 = (uint16)(dwID & 0xffffUL);
        u2a(szBuf, x1);
        strResult = szBuf;
        strResult += "-";
        u2a(szBuf,x2);
        strResult += szBuf;
        strResult += "-";
        u2a(szBuf,x3);
        strResult += szBuf;
    }else if (strFormat == G_STRID_FORMAT_BBBB){
        x1 = (uint16)(dwID>>24);
        x2 = (uint16)((dwID>>16)&0xffUL);
        x3 = (uint16)((dwID>>8)&0xffUL);
        x4 = (uint16)(dwID&0xffUL);
        u2a(szBuf, x1);
        strResult = szBuf;
        strResult += "-";
        u2a(szBuf, x2);
        strResult += szBuf;
        strResult += "-";
        u2a(szBuf, x3);
        strResult += szBuf;
        strResult += "-";
        u2a(szBuf, x4);
        strResult += szBuf;
    }else{
        assert(false && "wrong format");
        return strResult;
    }
    return strResult;
}

CCheckRunTime::CCheckRunTime(const char* pszModuleName, ulong dwLimitTick): m_pszModuleName(pszModuleName),m_dwLimitTick(dwLimitTick)
{
    assert(pszModuleName);
    m_dwBeginTick=get_tick_count();
}

CCheckRunTime::~CCheckRunTime()
{
    const ulong dw = get_tick_count();
    if (dw - m_dwBeginTick >= m_dwLimitTick){
        //USER_LOG_INFO(m_pszModuleName<<" run "<<dw - m_dwBeginTick<<" microsecond");
    }
}

struct ::tm * localtime_safe(struct tm *result, const time_t *timep)
{
#if (defined(WIN32) || defined(WIN64))
    localtime_s(result ,timep);
#else
    localtime_r(timep, result);
#endif
    return result;
}

std::string i2a(int32 nNum)
{
    char buf[128];
    buf[128-1]='\0', snprintf(buf, 128-1, "%d", nNum);
    return buf;
}

std::string u2a(uint dwNum)
{
    char buf[128];
    buf[128-1]='\0', snprintf(buf, 128-1, "%u", dwNum);
    return buf;
}

std::string i2a64(int64 qNum)
{
    char buf[128];
    buf[128-1]='\0', snprintf(buf, 128-1, FORMAT_I64, qNum);
    return buf;
}

std::string u2a64(uint64 dqNum)
{
    char buf[128];
    buf[128-1]='\0', snprintf(buf, 128-1, FORMAT_U64, dqNum);
    return buf;
}

}
