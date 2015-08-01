#ifndef __SH_SINGLETON_H__
#define __SH_SINGLETON_H__
#include <cstdlib>
template<class T>
class CSingleton
{

    protected:
        CSingleton(){}
        ~CSingleton(){}
    public:
        static T* get_instance()
        {
            if(m_instance == NULL)
            {
                m_instance = new T;
            }
            return m_instance;
        }


    private:
        static T* m_instance;
};

#endif


