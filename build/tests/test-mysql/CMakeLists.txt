################ PROJECT NAME #####################
cmake_minimum_required (VERSION 2.8)

# 如果系统上有多个编译器, 如果使用默认编译器, 则可以注释以下两行后,再cmake
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

# 如果生成执行文件
SET( EXECUTABLE_OUTPUT_PATH ../../../bin )
# 如果生成库
#SET( LIBRARY_OUTPUT_PATH ../../../lib )

# 设置工程名称
set (PRONAME "mysql-test")
PROJECT (PRONAME)

# 设置是否debug
set( CMAKE_BUILD_TYPE Debug)
set( CMAKE_DEBUG_POSTFIX d)	
MESSAGE(STATUS "DEBUG ON")

# 设置编译选项
add_definitions( -std=c++11 -Wno-literal-suffix -O0 -Wall -g -ggdb -rdynamic )



################ INCLUDE #####################
include_directories("/usr/include/mysql")
#include_directories("../../../src/mysql/mysql")
include_directories("/usr/local/include")
include_directories("../../../include")



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
)

SET (SOURCE_FILES  
../../../tests/test_mysql.cc
)


###############LINK#########################
link_directories ("/usr/local/lib/")#需要把libevent库安装到此目录下
link_directories ("/usr/lib64/mysql/")#需要把mysql库安装到此目录下

# 设置生成静态库
#ADD_LIBRARY(${PRONAME} STATIC  ${HEADER_FILES}  ${SOURCE_FILES}  )

# 设置生成动态库
#ADD_LIBRARY(${PRONAME} SHARED  ${HEADER_FILES}  ${SOURCE_FILES}  )

#设置生成目录
ADD_EXECUTABLE(${PRONAME}  ${HEADER_FILES}  ${SOURCE_FILES}  )

# 设置附加链接库
TARGET_LINK_LIBRARIES( ${PRONAME} pthread event mysqlclient sh_commond sh_dbd)


