################ PROJECT NAME #####################
cmake_minimum_required (VERSION 2.8)

# ���ϵͳ���ж��������, ���ʹ��Ĭ�ϱ�����, �����ע���������к�,��cmake
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

# �������ִ���ļ�
SET( EXECUTABLE_OUTPUT_PATH ../../../bin )
# ������ɿ�
#SET( LIBRARY_OUTPUT_PATH ../../../lib )

# ���ù�������
set (PRONAME "libevent-gs-server-test")
PROJECT (PRONAME)

# �����Ƿ�debug
set( CMAKE_BUILD_TYPE Debug)
set( CMAKE_DEBUG_POSTFIX d)	
MESSAGE(STATUS "DEBUG ON")

# ���ñ���ѡ��
add_definitions( -std=c++11  -O0 -Wall -g -ggdb -rdynamic )



################ INCLUDE #####################
include_directories("/usr/include/mysql")
include_directories("/usr/local/include")
include_directories("../../../include")
include_directories("../../../src")
include_directories("../../../proto")
include_directories("../../../tests/gs_server")



################ SOURCE_FILES #####################
SET (HEADER_FILES
../../../include/MarkupSTL.h
../../../include/sh_filelog.h 
../../../include/sh_type.h 
../../../include/sh_include.h 
../../../include/sh_listener.h 
../../../include/sh_net_util.h 
../../../include/sh_reactor_core.h 
../../../include/sh_server_application.h 
../../../include/sh_singleton.h 
../../../include/sh_socket.h 
../../../include/sh_timer.h 
../../../include/sh_libevent_include.h
../../../include/sh_lock.h
../../../include/sh_ptrlist.h
../../../include/sh_objpool.h
../../../include/sh_systemtime.h
../../../include/sh_dbsql.h
../../../include/sh_mysql.h
../../../tests/gs_server/gs_application.h 
../../../tests/gs_server/gs_client.h 
../../../tests/gs_server/gs_client_actions_mananger.h 
../../../tests/gs_server/gs_client_listener.h 
../../../tests/gs_server/gs_proc_db_event.h 
../../../proto/options.pb.h
../../../proto/ProtocType.pb.h
../../../proto/gameserver_gate_client.pb.h
)

SET (SOURCE_FILES  
../../../src/MarkupSTL.cpp 
../../../src/sh_filelog.cpp
../../../src/sh_net_util.cpp
../../../src/sh_reactor_core.cpp
../../../src/sh_server_application.cpp
../../../src/sh_socket.cpp
../../../src/sh_timer.cpp
../../../tests/gs_server/gs_application.cpp
../../../tests/gs_server/gs_client.cpp
../../../tests/gs_server/gs_client_actions_mananger.cpp
../../../tests/gs_server/gs_client_listener.cpp
../../../tests/gs_server/gs_proc_db_event.cpp 
../../../proto/options.pb.cc
../../../proto/ProtocType.pb.cc
../../../proto/gameserver_gate_client.pb.cc
../../../src/sh_common.cpp
../../../src/sh_dbsql.cpp
../../../src/sh_mysql.cpp
../../../src/sh_systemtime.cpp
../../../tests/gs_server/gs_server_main.cpp
)


###############LINK#########################
# link_directories ("../lib/linux")
# �������ɾ�̬��
#ADD_LIBRARY(${PRONAME} STATIC  ${HEADER_FILES}  ${SOURCE_FILES}  )
LINK_DIRECTORIES(/usr/local/lib)
LINK_DIRECTORIES(/usr/lib64/mysql/)
# �������ɶ�̬��
#ADD_LIBRARY(${PRONAME} SHARED  ${HEADER_FILES}  ${SOURCE_FILES}  )

#��������Ŀ¼
ADD_EXECUTABLE(${PRONAME}  ${HEADER_FILES}  ${SOURCE_FILES}  )
TARGET_LINK_LIBRARIES(${PRONAME} event protobuf protobuf-lite mysqlclient)

# ���ø������ӿ�
#TARGET_LINK_LIBRARIES( ${PRONAME} pthread )


