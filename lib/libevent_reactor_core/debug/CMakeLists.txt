################ PROJECT NAME #####################
cmake_minimum_required (VERSION 2.8)

# ���ϵͳ���ж��������, ���ʹ��Ĭ�ϱ�����, �����ע���������к�,��cmake
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

# �������ִ���ļ�
#SET( EXECUTABLE_OUTPUT_PATH ../../../bin )
# ������ɿ�
SET( LIBRARY_OUTPUT_PATH ../../../lib/libevent_reactor_core/debug )

# ���ù�������
set (PRONAME "sh_libevent_reactor_core")
PROJECT (PRONAME)

# �����Ƿ�debug
set( CMAKE_BUILD_TYPE DEBUG)
set( CMAKE_DEBUG_POSTFIX d)	
MESSAGE(STATUS "DEBUG ON")

# ���ñ���ѡ��
add_definitions( -std=c++11 -Wno-literal-suffix -O0 -Wall -g -ggdb -rdynamic )



################ INCLUDE #####################
include_directories("/usr/include/mysql")
include_directories("/usr/local/include")
include_directories("../../../src/libevent_reactor_core")
include_directories("../../../include")
include_directories("../../../src/mysql")
include_directories("../../../src/net_util")
include_directories("../../../src/common")




################ SOURCE_FILES #####################
SET (HEADER_FILES
../../../include/MarkupSTL.h
../../../include/sh_filelog.h
../../../include/sh_type.h
../../../include/sh_define.h
../../../include/sh_include.h
../../../include/sh_common.h
../../../include/sh_ptrlist.h
../../../include/sh_objpool.h
../../../include/sh_systemtime.h
../../../src/mysql/sh_mysql.h 
../../../include/sh_dbsql.h
../../../include/sh_singleton.h 

../../../src/libevent_reactor_core/sh_reactor_core.h
../../../src/net_util/sh_net_util.h
../../../src/net_util/sh_listener.h
)

SET (SOURCE_FILES  

../../../src/libevent_reactor_core/sh_reactor_core.cpp

)

link_directories (/usr/local/lib)
link_directories ("/usr/lib64/mysql/")#mysql
###############LINK#########################
# �������ɾ�̬��
#ADD_LIBRARY(${PRONAME} STATIC  ${HEADER_FILES}  ${SOURCE_FILES}  )

# �������ɶ�̬��
ADD_LIBRARY(${PRONAME} SHARED  ${HEADER_FILES}  ${SOURCE_FILES}  )

#��������Ŀ¼
#ADD_EXECUTABLE(${PRONAME}  ${HEADER_FILES}  ${SOURCE_FILES}  )

# ���ø������ӿ�
TARGET_LINK_LIBRARIES( ${PRONAME} event pthread sh_commond sh_dbd mysqlclient)


