/************************************************************************/
/* lock module                                                          */
/* autor: neco                                                          */
/* date: 2016.02.27                                                     */
/************************************************************************/

#include "stdafx.h"
#include "lock.h"

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
#ifdef _MSC_VER

    CMutexLock::CMutexLock()
    {
        m_mutex = ::CreateMutex(NULL, FALSE, NULL);
    }

    CMutexLock::~CMutexLock()
    {
        if (m_mutex) ::CloseHandle(m_mutex);
    }

    void CMutexLock::Lock()
    {
        if (m_mutex) WaitForSingleObject(m_mutex, INFINITE);
    }

    void CMutexLock::Unlock()
    {
        if (m_mutex) ::ReleaseMutex(m_mutex);
    }

//////////////////////////////////////////////////////////////////////////
#else

    CMutexLock::CMutexLock()
    {
        pthread_mutex_init(&m_mutex, NULL);
    }

    CMutexLock::~CMutexLock()
    {
        pthread_mutex_destroy(&m_mutex);
    }

    void CMutexLock::Lock()
    {
        pthread_mutex_lock(&m_mutex);
    }

    void CMutexLock::Unlock()
    {
        pthread_mutex_unlock(&m_mutex);
    }

#endif // end of #ifdef

//////////////////////////////////////////////////////////////////////////
    CAutoLock::CAutoLock(CMutexLock& m) : m_lock(m)
    {
        m_lock.Lock();
    }

    CAutoLock::~CAutoLock()
    {
        m_lock.Unlock();
    }
}
