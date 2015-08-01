#ifndef __SHARE_OBJ_POOL_H__
#define __SHARE_OBJ_POOL_H__

#include <mutex>

namespace sh
{

#ifndef max
#define max(a,b) (a) > (b) ? (a) : (b)
#endif
#ifndef min
#define min(a,b) (a) < (b) ? (a) : (b)
#endif

//////////////////////////////////////////////////////////////////////////
//// memory
//////////////////////////////////////////////////////////////////////////
enum MEMORY_POOL_ERROR_NO
{
    POOL_NO_ERROR = 0,
    POOL_NOT_ENOUGH_MEMORY = 1,
    POOL_FREE_NULL_POINTER = 2,
    POOL_FREE_ERROR_POINTER = 3,
};

/**
* @brief
* 静态内存池
*/
template <typename T>
class CPool
{
public:
    typedef struct _POOL_NODE
    {
        char*	buf_start;
        char*	buf_end;
    }POOL_NODE, *PPOOL_NODE;

    typedef std::list<char*> LIST;

    typedef struct _MEM_NODE
    {
        struct _MEM_NODE* next;
    }MEM_NODE, *PMEM_NODE;

    CPool(void) : m_elem_count(0), m_next_count(100),m_free_node(NULL),
        m_last_error_no(POOL_NO_ERROR),m_pmutex(NULL),m_init(false)
    {
    }

    CPool(int elem_next_count, bool mutex) : m_elem_count(0), m_next_count(elem_next_count),
        m_free_node(NULL),m_last_error_no(POOL_NO_ERROR),m_pmutex(NULL),m_init(false)
    {
        _init();
        if (mutex) set_mutex();
        m_init = true;
    }

    bool init(int elem_next_count, bool mutex)
    {
        bool bret=false;
        if (m_init)
        {
            return true;
        }
        m_next_count = elem_next_count;
        bret = _init();
        if (mutex)
        {
            bret = set_mutex();
        }
        if (bret)
        {
            m_init = true;
        }
        return bret;
    }

    ~CPool()
    {
        if (NULL != m_pmutex)
        {
            std::lock_guard<std::mutex> lock(*m_pmutex);
            LIST::iterator it, end = m_list.end();
            for (it=m_list.begin(); it!=end; ++it)
            {
                PPOOL_NODE p = PPOOL_NODE(*it);
                ::free(p->buf_start);
                ::free(p);
            }
            m_list.clear();
        }
        else
        {
            LIST::iterator it, end = m_list.end();
            for (it=m_list.begin(); it!=end; ++it)
            {
                PPOOL_NODE p = PPOOL_NODE(*it);
                ::free(p->buf_start);
                ::free(p);
            }
            m_list.clear();
        }
        delete m_pmutex;
        m_pmutex = NULL;
    }

    bool set_mutex()
    {
        if (NULL == m_pmutex)
        {
            m_pmutex = new(std::nothrow)std::mutex();
        }
        if (NULL == m_pmutex)
        {
            m_last_error_no = POOL_NOT_ENOUGH_MEMORY;
            return false;
        }
        return true;
    }

    T* malloc()
    {
        T* p = _mutex_malloc();
        if (NULL == p)return p;
        new(p)T; //调用构造函数 
        return p;
    }

    template<typename ... Params>
    T* malloc(Params... args)
    {
        T* p = _mutex_malloc();
        if (NULL == p)return p;
        new(p)T(args...);
        return p;
    }

    void free(T* pt)
    {
        if (NULL == pt)
        {
            m_last_error_no = POOL_FREE_NULL_POINTER;
            return;
        }
        pt->~T();

        if (NULL != m_pmutex)
        {
            std::lock_guard<std::mutex> lock(*m_pmutex);
            _free(pt);
        }
        else
        {
            _free(pt);
        }
    }

    bool is_in_this_pool(char* pt)
    {
        bool bret = false;
        if (NULL != m_pmutex)
        {
            std::lock_guard<std::mutex> lock(*m_pmutex);
            bret = _is_in_this_pool(pt);
        }
        else
        {
            bret = _is_in_this_pool(pt);
        }
        return bret;
    }

    int get_memory_size()const 
    {
        volatile uint32 elem_count = m_elem_count;
        return elem_count*(max(sizeof(T), sizeof(T*)));
    }

    MEMORY_POOL_ERROR_NO get_last_error_no() 
    {
        return m_last_error_no;
    }

    void reset_last_error_no() 
    {
        m_last_error_no = POOL_NO_ERROR;
    }

private:
    T* _mutex_malloc()
    {
        if (NULL != m_pmutex)
        {
            std::lock_guard<std::mutex> lock(*m_pmutex);
            return _malloc();
        }
        else
        {
            return _malloc();
        }
    }

    bool _is_in_this_pool(char* pt)
    {
        LIST::const_iterator it, end = m_list.end();
        for (it=m_list.begin(); it!=end; ++it)
        {
            const PPOOL_NODE p = PPOOL_NODE(*it);
            if (pt >= p->buf_start && pt < p->buf_end)
                return true;
        }
        return false;
    }

    bool _init()
    {
        const uint32 size_T = max(sizeof(T), sizeof(T*));
        const uint32 size = size_T * m_next_count;
        m_free_node = (PMEM_NODE)::malloc(size);
        if (NULL == m_free_node)
        {
            m_last_error_no = POOL_NOT_ENOUGH_MEMORY;
            return false;
        }
        PPOOL_NODE pPoolNode = (PPOOL_NODE)::malloc(sizeof(POOL_NODE));
        if (NULL == pPoolNode)
        {
            if (m_free_node)
            {
                ::free(m_free_node);
                m_free_node = NULL;
            }
            m_last_error_no = POOL_NOT_ENOUGH_MEMORY;
            return false;
        }
        pPoolNode->buf_start = (char*)m_free_node;
        pPoolNode->buf_end = pPoolNode->buf_start + size;
        m_list.push_back((char*)pPoolNode);

        m_free_node->next = NULL;
        PMEM_NODE p = m_free_node;

        for (uint32 i=0; i<m_next_count-1; ++i)
        {
            p->next = PMEM_NODE((char*)p+size_T);
            p = p->next;
            p->next = NULL;
        }
        m_elem_count += m_next_count;
        return true;
    }

    T* _malloc()
    {
        if (NULL == m_free_node)
            _init();
        if (NULL == m_free_node) return NULL;
        T* p = (T*)m_free_node;
        m_free_node = m_free_node->next;
        return p;
    }

    void _free(T* pt)
    {
        PMEM_NODE p = PMEM_NODE(pt);
        p->next = m_free_node;
        m_free_node = p;
    }

private:
    uint32			m_elem_count;
    uint32			m_next_count;
    LIST		    m_list;
    PMEM_NODE	    m_free_node;
    MEMORY_POOL_ERROR_NO	    m_last_error_no;
    std::mutex*     m_pmutex;
    bool            m_init;
};

#undef max
#undef min
}

#endif //__SHARE_OBJ_POOL_H__

