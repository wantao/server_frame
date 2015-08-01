#ifndef __SHARE_PTR_LIST_H__
#define __SHARE_PTR_LIST_H__

#include "sh_objpool.h"

namespace sh
{

/**
* @brief
*存放指针的链表，不提供线程安全。如果用户需要应用于多线程，
*则需要用户提供线程安全措施
*链表的节点由线程池提供。
*如果不是高频插入删除的指针链表，则使用stl里的list就可以啦，否则
*使用内存池的指针链表会带来高效率
*/
class ptr_list
{
public:
    typedef struct node
    {
        void_ptr data;
        struct node *prior, *next;
    }dlink;

    ptr_list(void)
    {
        m_node_pool.init(1000, false);
        m_phead = m_node_pool.malloc();
        _reset();
    }

    explicit ptr_list(uint32 init_node_number)
    {
        m_node_pool.init(init_node_number, false);
        m_phead = m_node_pool.malloc();
        _reset();
    }

    ptr_list(const ptr_list& right)
    {
        m_node_pool.init(right.size()+1, false);
        m_phead = m_node_pool.malloc();
        _reset();
        dlink *pcur = right.m_phead->next;
        while (pcur != right.m_phead)
        {
            this->push_back(pcur->data);
            pcur = pcur->next;
        }
    }

    void operator = (const ptr_list& right)
    {
        if (this == &right)
            return;

        clear();
        dlink *pcur = right.m_phead->next;
        while (pcur != right.m_phead)
        {
            this->push_back(pcur->data);
            pcur = pcur->next;
        }
    }

    dlink* find(void_ptr p)
    {
        return _find(p);
    }

    void erase(dlink* dp)
    {
        return _erase(dp);
    }

    void push_back(void_ptr p)
    {
        dlink *pnew = m_node_pool.malloc();
        pnew->data = p;
        _push_back(pnew);
    }

    void remove(void_ptr p)
    {
        dlink *pcur = _find(p);
        if (NULL == pcur) return;
        _erase(pcur);
    }

    uint32  size() const
    {
        return m_size;
    }

    bool empty() const
    {
        return m_size==0 ? true : false;
    }

    void_ptr front() const
    {
        if (0 == m_size) return NULL;
        return m_phead->next->data;
    }

    void_ptr pop_front()
    {
        if (0 == m_size) return NULL;
        void_ptr result = m_phead->next->data;
        _erase(m_phead->next);
        return result;
    }

    void_ptr tail() const
    {
        if (0 == m_size) return NULL;
        return m_phead->prior->data;
    }

    void_ptr pop_back()
    {
        if (0 == m_size) return NULL;
        void_ptr result = m_phead->prior->data;
        _erase(m_phead->prior);
        return result;
    }

    // 使当前的节点指针=链表头指针
    void reset_current()
    {
        m_pcurrent = m_phead;
    }

    // 返回当前节点的值
    void_ptr current()
    {
        return m_pcurrent->data;
    }

    dlink* current_link()
    {
        return m_pcurrent;
    }

    // 如果还有下一个节点，则返回true，并且把当前的
    // 的节点置为下一个节点。如果已经是最后一个节点了，则返回false.
    bool next()
    {
        if (m_phead == m_pcurrent->next) return false;
        m_pcurrent = m_pcurrent->next;
        return true;
    }

    void clear()
    {
        dlink *pcur = m_phead->next;
        while (pcur != m_phead)
        {
            dlink *pdelete = pcur;
            pcur = pcur->next;
            m_node_pool.free(pdelete);
        }
        _reset();
    }

    void display()
    {
        /*
        dlink *pcur = m_phead->next;
        while (pcur != m_phead)
        {
            printf("address:0X%p, val=%d\n",(uint64)(pcur->data), *((uint32*)(pcur->data)));
            pcur = pcur->next;
        }
        */
    }

private:
    void _reset()
    {
        m_size = 0;
        m_phead->prior = m_phead->next = m_phead;
        m_pcurrent = m_phead;
    }

    dlink* _find(void_ptr p)
    {
        dlink *pcur = m_phead->next;
        while (pcur != m_phead)
        {
            if (pcur->data == p)
            {
                return pcur;
            }
            pcur = pcur->next;
        }
        return NULL;
    }

    void _erase(dlink* dp)
    {
        dlink *pfront = dp->prior;
        dlink *ptail = dp->next;
        pfront->next = ptail;
        ptail->prior = pfront;
        m_node_pool.free(dp);

        --m_size;
    }

    void _push_back(dlink *pnew)
    {
        dlink *ptail = m_phead->prior;

        pnew->next = ptail->next;
        pnew->next->prior = pnew;
        ptail->next = pnew;
        pnew->prior = ptail;

        ++m_size;
    }

private:
    CPool<dlink>    m_node_pool;
    dlink*          m_phead;
    dlink*          m_pcurrent;
    volatile uint32 m_size;
};

class obj_ptr;

class share_obj
{
    friend class obj_ptr;
public:
    share_obj() : m_reference_use_count(0) {}
    virtual ~share_obj() {}

protected:
    virtual std::mutex* get_reference_use_count_mutex()=0;
    virtual void release_obj()=0;
private:
    long  m_reference_use_count;
};

#ifdef USE_SHARE_SMART_PTR
/*
* 用于用户自定义对象的引用计数指针。
* 引用计数的使用由用户提供（可能是多线程，也可能是单线程）
* 对象析构的办法，也是由用户提供
* 也是解决不了环引用。
*/
class obj_ptr
{
public:
    obj_ptr(): m_p(NULL)
    {
    }

    explicit obj_ptr( share_obj * p ): m_p( p )
    {
        std::lock_guard<std::mutex> lock(*m_p->get_reference_use_count_mutex());
        m_p->m_reference_use_count = 1;
    }

    obj_ptr(const obj_ptr& r)
    {
        m_p = r.m_p;
        std::lock_guard<std::mutex> lock(*m_p->get_reference_use_count_mutex());
        m_p->m_reference_use_count = r.m_p->m_reference_use_count;
        m_p->m_reference_use_count++;
    }

    ~obj_ptr()
    {
        long use_count = 0;
        {
            std::lock_guard<std::mutex> lock(*m_p->get_reference_use_count_mutex());
            m_p->m_reference_use_count --;
            use_count = m_p->m_reference_use_count;
        }

        if (0 == use_count) //引用计数为 0
        {
            m_p->release_obj();
            m_p = NULL;
        }
    }

    obj_ptr& operator=(const obj_ptr& r)
    {
        if (&r == this)
        {
            return *this;
        }
        else
        {
            long use_count = 0;
            {
                std::lock_guard<std::mutex> lock(*m_p->get_reference_use_count_mutex());
                m_p->m_reference_use_count --;
                use_count = m_p->m_reference_use_count;
            }

            if (0 == use_count) //引用计数为 0
            {
                m_p->release_obj();
                m_p = NULL;
            }

            m_p = r.m_p;
            m_p->m_reference_use_count = r.m_p->m_reference_use_count;
            m_p->m_reference_use_count ++; // 引用计数加1
            return *this;
        }
    }

    share_obj& operator*()
    {
        return *m_p;
    }

    share_obj* operator->()
    {
        return m_p;
    }

private:
    share_obj*          m_p;
};

/// 单线程智能指针，注意：这个智能指针不能处理循环引用
/// 这个可以用来管理比较简单引用指针，防止忘记释放的地方。
template<class T> class single_ptr
{
public:
    single_ptr(): m_p(NULL)
    {
        m_use_count = g_single_ptr_reference_pool.malloc();
        *m_use_count = 1;
    }

    explicit single_ptr( T * p ): m_p( p )
    {
        m_use_count = g_single_ptr_reference_pool.malloc();
        *m_use_count = 1;
    }

    single_ptr(const single_ptr& r)
    {
        m_p = r.m_p;
        m_use_count = r.m_use_count;
        (*m_use_count) ++; // 引用计数加1
    }

    ~single_ptr()
    {
        (*m_use_count) --;
        if (0 == *m_use_count) //引用计数为 0
        {
            delete m_p;
            m_p = NULL;
            g_single_ptr_reference_pool.free(m_use_count);
            m_use_count = NULL;
        }
    }

    single_ptr& operator=(const single_ptr& r)
    {
        if (&r == this)
        {
            return *this;
        }
        else
        {
            (*m_use_count) --;
            if (0 == *m_use_count) //引用计数为 0
            {
                delete m_p;
                m_p = NULL;
                g_single_ptr_reference_pool.free(m_use_count);
                m_use_count = NULL;
            }
            m_p = r.m_p;
            m_use_count = r.m_use_count;
            (*m_use_count) ++; // 引用计数加1
            return *this;
        }
    }

    T& operator*()
    {
        return *m_p;
    }

    T* operator->()
    {
        return m_p;
    }

private:
    T*          m_p;
    long*       m_use_count;

    static  CPool<long>   g_single_ptr_reference_pool;
};

template<class T>
CPool<long> single_ptr<T>::g_single_ptr_reference_pool(10000, false);

#endif

}

#endif //__SHARE_PTR_LIST_H__

