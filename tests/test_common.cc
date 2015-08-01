#include "sh_filelog.h"
#include "sh_common.h"
#include "sh_ptrlist.h"
#include "sh_systemtime.h"
//#include "sh_iconv.h"


struct CSimpleObject
{
public:
    CSimpleObject():m_data(0){printf("CSimpleOjbect Construct\r\n");}
    CSimpleObject(sh::int32 data):m_data(data){printf("CSimpleOjbect Construct\r\n");}
    CSimpleObject(sh::int32 data, std::string str):m_data(data){printf("CSimpleOjbect Construct\r\n");}
    ~CSimpleObject(){printf("CSimpleOjbect ~Construct\r\n");}
    void Print() {printf("CSimpleObject Print********** %d\r\n", m_data);}
private:
    sh::int32 m_data;
};

void showhex (const char * what, const char * a, size_t len)
{
    printf ("%s: ", what);
    for (size_t i = 0; i < len; i++) {
        printf ("%02X", (unsigned char) a[i]);
        if (i < len - 1)
            printf (" ");
    }
    printf ("\n");
}

int main()
{

    //////////////////////////////////////////////////////////////////////////

    //CPool ptr_list 
    //////////////////////////////////////////////////////////////////////////

    sh::CPool<CSimpleObject> pool;
    sh::ptr_list ptr_sip_list;
    size_t pool_size = 5;
    pool.init((sh::int32)pool_size, false);
    for (size_t i=0; i<pool_size; ++i){
        CSimpleObject* pSimObj = pool.malloc(i, "test");
        if (NULL == pSimObj) break;
        ptr_sip_list.push_back(pSimObj);
    }

    for (ptr_sip_list.reset_current(); ptr_sip_list.next(); ) {
        CSimpleObject *pSimObj = (CSimpleObject *)ptr_sip_list.current();
        if (NULL != pSimObj){
            pSimObj->Print();
            pool.free(pSimObj);
        }
    }
    ptr_sip_list.clear();

    printf("\r\nCPoll test-----------------------------------------------------\r\n");

    //////////////////////////////////////////////////////////////////////////

    //CDateTime
    //////////////////////////////////////////////////////////////////////////

    time_t t_now = time(NULL);
    const std::string str_now = sh::time2str(t_now);
    printf("current_time %s \r\n", str_now.c_str());

    sh::CDateTime cur_time(t_now);
    printf("class cdatetime: %s \r\n", cur_time.to_string().c_str());
    printf("class cdatetime: %d-%d-%d %d:%d:%d \r\n", cur_time.get_year(), cur_time.get_month(), cur_time.get_day(),
        cur_time.get_hour(), cur_time.get_minute(), cur_time.get_second());

    sh::CDateTime new_time(cur_time);
    new_time.inc_year(2);
    new_time.inc_month(2);
    new_time.inc_day(2);
    new_time.inc_hour(2);
    new_time.inc_minute(2);
    new_time.inc_second(2);
    printf("class cdatetime: after inc 2 %s \r\n", cur_time.to_string().c_str());

    new_time.dec_year(3);
    new_time.dec_month(3);
    new_time.dec_day(3);
    new_time.dec_hour(3);
    new_time.dec_minute(3);
    new_time.dec_second(3);
    printf("class cdatetime: after dec 1 %s \r\n", cur_time.to_string().c_str());

    new_time.set_date(1999, 9, 9);
    printf("class cdatetime: after set_date %s \r\n", new_time.to_string().c_str());
    new_time.set_time(8, 8, 20);
    printf("class cdatetime: after set_time %s \r\n", new_time.to_string().c_str());
    new_time.set_date_time(2022, 2, 3, 11, 23, 12);
    printf("class cdatetime: after set_date_time %s \r\n", new_time.to_string().c_str());

    sh::int64 diff = new_time.diff_second(cur_time);
    printf("new_time.diff_second(cur_time) "FORMAT_I64" \r\n", diff);
    diff = new_time.diff_minute(cur_time);
    printf("new_time.diff_minute(cur_time) "FORMAT_I64" \r\n", diff);
    diff = new_time.diff_hour(cur_time);
    printf("new_time.diff_hour(cur_time) "FORMAT_I64" \r\n", diff);
    diff = new_time.diff_day(cur_time);
    printf("new_time.diff_day(cur_time) "FORMAT_I64" \r\n", diff);
    diff = new_time.diff_week(cur_time);
    printf("new_time.diff_week(cur_time) "FORMAT_I64" \r\n", diff);
    diff = new_time.diff_month(cur_time);
    printf("new_time.diff_month(cur_time) "FORMAT_I64" \r\n", diff);

    printf("get_time_value:  %lu \r\n", cur_time.get_time_value());

    printf("get_system_time:  "FORMAT_I64" \r\n", sh::get_system_time());
    printf("get_tick_count:  %lu \r\n", sh::get_tick_count());
    printf("time_micro_sec:  "FORMAT_I64" \r\n", sh::time_micro_sec());
    printf("time_milli_sec:  "FORMAT_I64" \r\n", sh::time_milli_sec());
    char sz_print[256];
    sh::safe_sprintf(sz_print, "time_secs: %lld \r\n", sh::time_secs());
    printf(sz_print);


    printf("\r\nCDatetime test-----------------------------------------------------\r\n");

    //////////////////////////////////////////////////////////////////////////

    //common
    //////////////////////////////////////////////////////////////////////////

    printf("-----------------------------------------------------\r\n");

    std::string str = sh::i2a(-99999);
    printf("i2a test: %s \r\n", str.c_str());
    str = sh::u2a(99999);
    printf("u2a test: %s \r\n", str.c_str());
    str = sh::i2a64(-99999999999999);
    printf("i2a64 test: %s \r\n", str.c_str());
    str = sh::u2a64(99999999999999);
    printf("u2a64 test: %s \r\n", str.c_str());
    sh::int32 i = sh::a2i("-9999999");
    printf("a2i test: %d \r\n", i);
    long l= sh::a2l("99999");
    printf("a2l test: %ld \r\n", l);
    sh::int64 ll = sh::a2i64("-99999999999999");
    printf("a2i64 test: %lld \r\n", ll);
    sh::uint32 u= sh::a2u("99999");
    printf("a2u test: %u \r\n", u);
    sh::uint64 ul= sh::a2u64("99999999999999");
    printf("a2u64 test: %llu \r\n", ul);

    std::string str_i2hex = sh::i2hex(32);
    printf("i2hex:%s\r\n", str_i2hex.c_str());
    char szc[256];
    sh::safe_sprintf(szc,"sdf323fsdfsd");
    sh::uint32 szc_len = strlen(szc);
    std::string s2_hex = sh::s2hex((const unsigned char*)szc, szc_len);
    printf("i2hex:%s\r\n", s2_hex.c_str());

    std::string str_split("as\treee 09oipo\nfsdafasdf\rlkjoij\0rerer3");
    std::vector<std::string> v;
    sh::split(v, str_split);
    for (std::vector<std::string>::const_iterator itr=v.begin(); itr!=v.end(); ++itr){
        const std::string& str = *itr;
        printf("str split %s\r\n", str.c_str());
    }
    str_split = "2:3:1,4:4:6,23:90:9,";
    sh::split(v, str_split, ',');
    for (std::vector<std::string>::const_iterator itr=v.begin(); itr!=v.end(); ++itr){
        const std::string& str = *itr;
        printf("str split %s\r\n", str.c_str());
    }
    str_split = "2::1,4::6,23::9,";
    sh::split(v, str_split, "::");
    for (std::vector<std::string>::const_iterator itr=v.begin(); itr!=v.end(); ++itr){
        const std::string& str = *itr;
        printf("str split %s\r\n", str.c_str());
    }

    std::string str_er = "DKLFJasOIEFJLKD";
    const std::string& str_low = sh::lower(str_er);
    printf("str lower %s\r\n", str_low.c_str());
    str_er = "DKefJasjlijokj";
    std::string str_uper = sh::upper(str_er);
    printf("str uper %s\r\n", str_uper.c_str());
    char str_err[256];
    sh::safe_sprintf(str_err,"\rDKL\rFJa\nsOIE FJL\tKD\r");
    std::string str_trim = sh::trim(str_err);
    printf("str trim %s\r\n", str_trim.c_str());
    std::string str_trimm = "\rDKL\rFJa\nsOIE FJL\tKD\r";
    std::string str_copytrim;
    sh::copytrim(str_copytrim, str_trimm);
    printf("str trim %s\r\n", str_trimm.c_str());

    std::string str_cmp1 = "safDFDFlk";
    std::string str_cmp2 = "safDFdflk";
    if (0 == sh::stricmp(str_cmp1.c_str(), str_cmp2.c_str())){
        printf("str_cmp1==str_cmp2\r\n");
    }else{
        printf("str_cmp1!=str_cmp2\r\n");
    }

    std::string str_time = sh::time2str(t_now);
    printf("time2datestr now:%s\r\n", str_time.c_str());
    time_t t = sh::str2time(str_time);
    if (t == t_now){
        printf("t == t_now\r\n");
    }

    char sz[256];
    sh::get_current_directory(sz, 256);
    std::string cur_dir(sz, strlen(sz));
    printf("get_current_directory: %s\r\n", cur_dir.c_str());

    //cur_dir="/mnt/windowsDir/xxx/sh/bin";
    if (sh::is_directory_exist(cur_dir.c_str())){
        printf("%s is exist \r\n", cur_dir.c_str());
    }else{
        printf("%s is not exist \r\n", cur_dir.c_str());
    }
    printf("get_current_exe_directory: %s\r\n", sh::get_current_exe_directory());

    printf("get_execute_file: %s\r\n", sh::get_execute_file().c_str());
    printf("get_current_exe_full_path: %s\r\n", sh::get_current_exe_full_path().c_str());
    std::string filename = sh::get_filename_by_filepath(sh::get_current_exe_full_path().c_str());
    printf("get_filename_by_filepath: %s\r\n", filename.c_str());

    sh::uint32 cur_value = 0;
    cur_value = sh::get_crc32_code(cur_value,(unsigned char*)szc, szc_len);
    printf("cur_value: %u\r\n", cur_value);

    std::vector<sh::int32> v_n;
    const size_t rand_len = 500;
    for (size_t i=0; i<rand_len; ++i){
        sh::int32 j = sh::rand_int(1, rand_len);
        v_n.push_back(j);
    }
    std::sort(v_n.begin(), v_n.end());
    for (size_t i=0; i<v_n.size(); ++i){
        printf("%d,", v_n[i]);
    }
    printf("\r\n");

    std::string strID = "1-3-34";
    sh::uint32 n_id = sh::str_id_to_uint32(strID, sh::G_STRID_FORMAT_BBW);
    printf("n_id =%d\r\n", n_id);
    std::string str_id_to_uint = sh::uint32_to_str_id(n_id, sh::G_STRID_FORMAT_BBW);
    printf("str_id_to_uint =%s\r\n", str_id_to_uint.c_str());
    strID = "1-3-34-6456";
    sh::uint64 n_id_64 = sh::str_id_to_uint64(strID, sh::G_STRID_FORMAT_BBWD);
    printf("n_id ="FORMAT_U64"\r\n", n_id_64);
    std::string str_id_to_uint_64 = sh::uint64_to_str_id(n_id_64, sh::G_STRID_FORMAT_BBWD);
    printf("str_id_to_uint_64 =%s\r\n", str_id_to_uint_64.c_str());

    //std::string str_ans("45fsd中文");
    //showhex("str_ans", str_ans.c_str(), str_ans.size());
    //std::string str_utf8000 = sh::utf8_to_ansi(str_ans.c_str());
    //showhex("str_utf8000", str_utf8000.c_str(), str_utf8000.size());
    //std::string str_utf8_ = sh::ansi_to_utf8(str_utf8000.c_str());
    //showhex("str_utf8_", str_utf8_.c_str(), str_utf8_.size());

    //std::wstring wstrsss(L"asyyysdf76576我爱zhong国! ％＃＠＃yydf7657你说什么6");
    //showhex("wstrsss", (const char*)wstrsss.c_str(), wcslen(wstrsss.c_str())*sizeof(wchar_t));
    //std::string str_utf8 = sh::unicode_to_utf8(wstrsss.c_str());
    //showhex("str_utf8", str_utf8.c_str(), str_utf8.size());
    //printf("ansi_to_utf8 %s\r\n", str_utf8.c_str());
    //std::wstring wstr_ = sh::utf8_to_unicode(str_utf8.c_str());
    //showhex("wstr_", (const char*)wstr_.c_str(),  wcslen(wstr_.c_str())*sizeof(wchar_t));

    //std::string wstrsss_("asy568{}67ydf65!@##hg"); 
    //showhex("str_ansi", (const char*)wstrsss_.c_str(), wstrsss_.size());
    //std::wstring uwstr_ = sh::ansi_to_unicode(wstrsss_.c_str());
    //showhex("uwstr_", (const char*)uwstr_.c_str(), wcslen(uwstr_.c_str())*sizeof(wchar_t));
    //std::string str_ansi = sh::unicode_to_ansi(uwstr_.c_str());
    //showhex("str_ansi", (const char*)str_ansi.c_str(),  str_ansi.size());

    printf("\r\ncommon test-----------------------------------------------------\r\n");

    //////////////////////////////////////////////////////////////////////////


    return 0;
}



