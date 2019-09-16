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
    /// ������
    class LSLIB_API CMutexLock
    {
    public:
        CMutexLock();
        virtual ~CMutexLock();

        virtual void Lock();    ///< ����
        virtual void Unlock();  ///< ����

    private:
#ifdef _MSC_VER
        void* m_mutex;
#else
        pthread_mutex_t m_mutex;
#endif
    };

    //////////////////////////////////////////////////////////////////////////
    /// �Զ���������ʱ����������ʱ����
    class LSLIB_API CAutoLock
    {
    public:
        CAutoLock(CMutexLock& mtx);
        ~CAutoLock();

    private:
        CMutexLock& m_lock;
    };
}
