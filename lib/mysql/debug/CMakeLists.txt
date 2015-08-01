################ PROJECT NAME #####################
cmake_minimum_required (VERSION 2.8)

# ���ϵͳ���ж��������, ���ʹ��Ĭ�ϱ�����, �����ע���������к�,��cmake
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

# �������ִ���ļ�
#SET( EXECUTABLE_OUTPUT_PATH ../../../bin )
# ������ɿ�
SET( LIBRARY_OUTPUT_PATH ../../../lib/mysql/debug )

# ���ù�������
set (PRONAME "sh_db")
PROJECT (PRONAME)

# �����Ƿ�debug
set( CMAKE_BUILD_TYPE Debug)
set( CMAKE_DEBUG_POSTFIX d)	
MESSAGE(STATUS "DEBUG ON")

# ���ñ���ѡ��
add_definitions( -std=c++11 -Wno-literal-suffix -O0 -Wall -g -ggdb -rdynamic )



################ INCLUDE #####################
include_directories("/usr/include/mysql")
include_directories("../../../include")
include_directories("../../../src/mysql")



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
../../../include/sh_dbsql.h
../../../src/mysql/sh_mysql.h
)

SET (SOURCE_FILES  
../../../src/mysql/sh_mysql.cpp
../../../src/mysql/sh_dbsql.cpp
)


###############LINK#########################
link_directories ("../../../lib/common/debug")
link_directories ("/usr/lib64/mysql/")#��Ҫ��mysql�ⰲװ����Ŀ¼��
# �������ɾ�̬��
#ADD_LIBRARY(${PRONAME} STATIC  ${HEADER_FILES}  ${SOURCE_FILES}  )

# �������ɶ�̬��
ADD_LIBRARY(${PRONAME} SHARED  ${HEADER_FILES}  ${SOURCE_FILES}  )

#��������Ŀ¼
#ADD_EXECUTABLE(${PRONAME}  ${HEADER_FILES}  ${SOURCE_FILES}  )

# ���ø������ӿ�
TARGET_LINK_LIBRARIES( ${PRONAME} pthread mysqlclient sh_commond)


