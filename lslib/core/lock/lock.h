#pragma once

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
#ifdef _WIN32

	class LSLIB_API CMutexLock
	{ 
	public: 
		CMutexLock();
		~CMutexLock();

		virtual void Lock();
		virtual void Unlock();

	private: 
		void* m_mutex;
	};

	//
	class LSLIB_API  CAutoLock
	{ 
	public: 
		CAutoLock(CMutexLock& mtx);
		~CAutoLock();

	private: 
		CMutexLock& m_lock;
	}; 

	//////////////////////////////////////////////////////////////////////////
#else

#include "pthread/pthread.h"

	class LSLIB_API CMutexLock
	{ 
	public:
		CMutexLock();
		~CMutexLock();

		virtual void Lock();
		virtual void Unlock();

	private: 
		pthread_mutex_t m_mutex;
	};

#endif
}
