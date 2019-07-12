/************************************************************************/
/* lock module                                                          */
/* autor: neco                                                          */
/* date: 2016.02.27                                                     */
/************************************************************************/

#pragma once

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
#ifdef _MSC_VER

#include <windows.h>

    class LSLIB_API CMutexLock
    {
    public:
        CMutexLock();
        virtual ~CMutexLock();

        virtual void Lock();
        virtual void Unlock();

    private:
        void *m_mutex;
    };

#else

#include <pthread.h>

    class LSLIB_API CMutexLock
    {
    public:
        CMutexLock();
        virtual ~CMutexLock();

        virtual void Lock();
        virtual void Unlock();

    private:
        pthread_mutex_t m_mutex;
    };
#endif

//////////////////////////////////////////////////////////////////////////
    class LSLIB_API CAutoLock
    {
    public:
        CAutoLock(CMutexLock &mtx);
        ~CAutoLock();

    private:
        CMutexLock &m_lock;
    };
}
