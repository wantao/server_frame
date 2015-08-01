#ifndef __SH_SOCKET_H__
#define __SH_SOCKET_H__

#include "sh_include.h"
#include "sh_libevent_include.h"
#include "sh_reactor_core.h"
#include "sh_type.h"

#define MAX_NET_PACKET_SIZE 65536

//读事件回调函数
void common_read_cb(struct bufferevent* ev,void *user_data);
//写事件回调函数
void common_write_cb(struct bufferevent* ev,void *user_data);
//异常回调函数
void common_event_cb(struct bufferevent* ev,short int,void *user_data);




//消息包结构 消息size+消息数据
struct SPacketInfo
{
    SPacketInfo():size(0),data(NULL){}
    sh::int32 size;//消息数据的size
    char* data;//消息数据
};

/*
    供上层继承使用的socket(包括被动连接和主动连接)类
*/
class CSocketClient
{
    public:
        //给连接到来的对象的构造器
        CSocketClient(bufferevent* bev,evutil_socket_t socket,sockaddr_in* addr);
      
        //作为连接器时的构造器
        CSocketClient();

        virtual ~CSocketClient();

        //发送消息
        bool send_msg(const char* data,const sh::int32 size);
        //获取待处理的消息包(消息包格式为:4byte(消息体长度)+(消息体))
        sh::int32 check_packet_info(SPacketInfo* packet,evbuffer* read_buffer);
        //初始化读/写/异常处理回调
        void init_cb();
        //读事件处理
        sh::int32 on_read(bufferevent* bev);
        //写事件处理
        sh::int32 on_write(bufferevent* bev);
        //连接断开处理
        void on_disconnection(bufferevent* bev);

        //作为连接器时使用的相关函数///begin
        //连接服务端
        sh::int32 connect_to(const char* host_name,sh::uint32 port);
        //重连
        void re_connect();
        //作为连接器时使用的相关函数///end

    public:
        //以下由继承者实现
        //错误异常处理(如收到数据发生异常（数据包太大，或包格式不对（不是消息体长度（4byte）+消息体数据））时，需销毁该socket)
        virtual void on_error(bufferevent* bev)=0;
        //连接处理
        virtual void on_connection(bufferevent* bev) = 0;
        //处理消息
        /*
            data:消息包
            data_size:4byte(消息体长度)+(消息体))
        */
        virtual sh::int32 process_msg(const char* data, const sh::int32 data_size)=0;

    private:
        //重置一些标志
        void _reset_some_flags();

    private:
        bufferevent* m_bev;//socket 的eventbuffer
        evutil_socket_t         m_socket;
        char*        m_data_buffer;//交给上层处理的数据缓冲区
        sh::uint32   m_read_count;//第几次读数据（数据量大时，需多次读才能把包读全）
        sh::int32   m_msg_body_len;//消息体的长度
        sh::int32   m_has_read_len;//已读到数据的总长度
    private:
        //作为连接器时使用的相关变量///begin
        std::string  m_ip;//服务端ip
        sh::int32          m_port;//服务端端口
        //作为连接器时使用的相关变量///end
};

#endif
