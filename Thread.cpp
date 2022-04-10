#include "stdafx.h"
#include "Thread.h"


CThread::CThread(void)
{m_bExit = false;
    m_dwParam = 0;
    m_hThread = NULL;
}


CThread::~CThread(void)
{if (!m_bExit)
    {
        stop();
    }
}
void CThread::start()
{
    DWORD dwThreadID;                           //这是一个回调函数，通过线程创建然后回调，运行RUN函数，在通过标志位m_bExit和重载函数最后
    HANDLE hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, this, 0, &dwThreadID);//运行stop函数之后，结束句柄;
    ASSERT(hThread);
    m_hThread = hThread;
}
 
 
DWORD CThread::ThreadProc(LPVOID pParam)
{
    CThread* pThis = (CThread*)pParam;//强制转换
    ASSERT(pThis);
 
    while (!pThis->m_bExit)
    {
        pThis->run();
    }
    return TRUE;
}
 
void CThread::stop()
{
    if (m_hThread)
    {
        m_bExit = true;
        ::WaitForSingleObject(m_hThread, INFINITE);//等待线程完全退出
        ::CloseHandle(m_hThread);
        m_hThread = NULL;
    }
}
 
void CThread::run()//重载run虚函数可以判断线程是否一直运行
{
    Sleep(100);//必须得设置，不然内存会爆满
}
void CThread::SetThreadData(DWORD dwParam)
{
    if (m_dwParam != dwParam)
    {
        m_dwParam = dwParam;
    }
}
 
 
DWORD CThread::GetThreadData()
{
    return m_dwParam;
}