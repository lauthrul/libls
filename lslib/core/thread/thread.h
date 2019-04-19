#pragma once


namespace lslib
{
	//////////////////////////////////////////////////////////////////////////
	typedef bool (*CompareMessageFunc)		(MSG, int, MSG, int);

	//////////////////////////////////////////////////////////////////////////
	enum EThreadState
	{
		TS_NONE,
		TS_WORKING,
		TS_SUSPENDED,
	};

	//////////////////////////////////////////////////////////////////////////
	class LSLIB_API CThread
	{
	public :
		CThread(bool bSuspend);
		virtual ~CThread();

	public:
		virtual LPCTSTR		GetName() = 0;

		bool				Suspend();
		bool				Resume();
		virtual bool		Stop(); // must call in subclass destructor firstly
		unsigned int		GetThreadID() const;
		HANDLE				GetThreadHandle() const;
		EThreadState		GetThreadState() const;
		int					GetPriority() const;
		bool				SetPriority(int nPriority);
		bool				WaitFor(DWORD dwMilliseconds); // return: [true]-OK; [false]-timeout
		bool				PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0, int nLevel = 0);
		LRESULT				SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
		bool				FindMessage(MSG msg, int level, CompareMessageFunc pfunc);

	protected:
		bool				GetMessage(LPMSG lpMsg);

		virtual void		Execute();
		virtual void		OnExecute();
		virtual LRESULT		HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		static unsigned __stdcall _ThreadEntry(void* pParam);

	protected:
		EThreadState		m_eThreadState;
		int					m_nPriority;
		unsigned int		m_uThreadID;
		HANDLE				m_hThreadHandle;
		CMutexLock			m_mutexMsgs;
		map<int, list<MSG>>	m_mapMsgs;		// <level, list<msg>>
	};

} // lslib
