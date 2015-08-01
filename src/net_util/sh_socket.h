#ifndef __SH_SOCKET_H__
#define __SH_SOCKET_H__

#include "sh_include.h"
#include "sh_libevent_include.h"
#include "sh_reactor_core.h"
#include "sh_type.h"

#define MAX_NET_PACKET_SIZE 65536

//���¼��ص�����
void common_read_cb(struct bufferevent* ev,void *user_data);
//д�¼��ص�����
void common_write_cb(struct bufferevent* ev,void *user_data);
//�쳣�ص�����
void common_event_cb(struct bufferevent* ev,short int,void *user_data);




//��Ϣ���ṹ ��Ϣsize+��Ϣ����
struct SPacketInfo
{
    SPacketInfo():size(0),data(NULL){}
    sh::int32 size;//��Ϣ���ݵ�size
    char* data;//��Ϣ����
};

/*
    ���ϲ�̳�ʹ�õ�socket(�����������Ӻ���������)��
*/
class CSocketClient
{
    public:
        //�����ӵ����Ķ���Ĺ�����
        CSocketClient(bufferevent* bev,evutil_socket_t socket,sockaddr_in* addr);
      
        //��Ϊ������ʱ�Ĺ�����
        CSocketClient();

        virtual ~CSocketClient();

        //������Ϣ
        bool send_msg(const char* data,const sh::int32 size);
        //��ȡ���������Ϣ��(��Ϣ����ʽΪ:4byte(��Ϣ�峤��)+(��Ϣ��))
        sh::int32 check_packet_info(SPacketInfo* packet,evbuffer* read_buffer);
        //��ʼ����/д/�쳣����ص�
        void init_cb();
        //���¼�����
        sh::int32 on_read(bufferevent* bev);
        //д�¼�����
        sh::int32 on_write(bufferevent* bev);
        //���ӶϿ�����
        void on_disconnection(bufferevent* bev);

        //��Ϊ������ʱʹ�õ���غ���///begin
        //���ӷ����
        sh::int32 connect_to(const char* host_name,sh::uint32 port);
        //����
        void re_connect();
        //��Ϊ������ʱʹ�õ���غ���///end

    public:
        //�����ɼ̳���ʵ��
        //�����쳣����(���յ����ݷ����쳣�����ݰ�̫�󣬻����ʽ���ԣ�������Ϣ�峤�ȣ�4byte��+��Ϣ�����ݣ���ʱ�������ٸ�socket)
        virtual void on_error(bufferevent* bev)=0;
        //���Ӵ���
        virtual void on_connection(bufferevent* bev) = 0;
        //������Ϣ
        /*
            data:��Ϣ��
            data_size:4byte(��Ϣ�峤��)+(��Ϣ��))
        */
        virtual sh::int32 process_msg(const char* data, const sh::int32 data_size)=0;

    private:
        //����һЩ��־
        void _reset_some_flags();

    private:
        bufferevent* m_bev;//socket ��eventbuffer
        evutil_socket_t         m_socket;
        char*        m_data_buffer;//�����ϲ㴦������ݻ�����
        sh::uint32   m_read_count;//�ڼ��ζ����ݣ���������ʱ�����ζ����ܰѰ���ȫ��
        sh::int32   m_msg_body_len;//��Ϣ��ĳ���
        sh::int32   m_has_read_len;//�Ѷ������ݵ��ܳ���
    private:
        //��Ϊ������ʱʹ�õ���ر���///begin
        std::string  m_ip;//�����ip
        sh::int32          m_port;//����˶˿�
        //��Ϊ������ʱʹ�õ���ر���///end
};

#endif
