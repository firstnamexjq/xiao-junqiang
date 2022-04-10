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
    DWORD dwThreadID;                           //����һ���ص�������ͨ���̴߳���Ȼ��ص�������RUN��������ͨ����־λm_bExit�����غ������
    HANDLE hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, this, 0, &dwThreadID);//����stop����֮�󣬽������;
    ASSERT(hThread);
    m_hThread = hThread;
}
 
 
DWORD CThread::ThreadProc(LPVOID pParam)
{
    CThread* pThis = (CThread*)pParam;//ǿ��ת��
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
        ::WaitForSingleObject(m_hThread, INFINITE);//�ȴ��߳���ȫ�˳�
        ::CloseHandle(m_hThread);
        m_hThread = NULL;
    }
}
 
void CThread::run()//����run�麯�������ж��߳��Ƿ�һֱ����
{
    Sleep(100);//��������ã���Ȼ�ڴ�ᱬ��
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