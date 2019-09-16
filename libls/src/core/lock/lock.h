/************************************************************************/
/* lock module                                                          */
/* autor: neco                                                          */
/* date: 2016.02.27                                                     */
/************************************************************************/

#pragma once
#ifdef _MSC_VER
#include <windows.h>
#else
#include <pthread.h>
#endif

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    /// 互斥锁
    class LSLIB_API CMutexLock
    {
    public:
        CMutexLock();
        virtual ~CMutexLock();

        virtual void Lock();    ///< 加锁
        virtual void Unlock();  ///< 解锁

    private:
#ifdef _MSC_VER
        void* m_mutex;
#else
        pthread_mutex_t m_mutex;
#endif
    };

    //////////////////////////////////////////////////////////////////////////
    /// 自动锁，构造时加锁，析构时解锁
    class LSLIB_API CAutoLock
    {
    public:
        CAutoLock(CMutexLock& mtx);
        ~CAutoLock();

    private:
        CMutexLock& m_lock;
    };
}
