#include "stdafx.h"
#include "ThreadComm.h"
#include "CWinDemoDlg.h"
#include "SerialPort.h"
#include "resource.h"
#include "CWinDemo.h"
 


CThreadComm::CThreadComm(void)
{
}


CThreadComm::~CThreadComm(void)
{
}
void CThreadComm::run()
{
    Sleep(100);
    CWnd* pWinApp = (CWnd*)AfxGetApp()->m_pMainWnd;
    if (NULL == pWinApp)
    {
        return;
    }
 
    CSerialPort* pSerialPort = (CSerialPort*)GetThreadData();
    if (NULL == pSerialPort) return;
 
    DWORD dwError = 0;
    COMSTAT comStat;
    BOOL bRet = TRUE;
    TCHAR recvTemp[512];//临时缓冲区
    ZeroMemory(recvTemp, sizeof(recvTemp));
 
    TCHAR recvBuf[4096];//真正的缓冲区
    ZeroMemory(recvBuf, sizeof(recvBuf));
 
    DWORD dwRead = 0;
    int nLength = 0;
 
    pSerialPort->ClearCommError(&dwError, &comStat);
    if (comStat.cbInQue > 0)//判断输入缓存区是否有数据
    {
        OVERLAPPED overlappedRead;
        ZeroMemory(&overlappedRead,sizeof(OVERLAPPED));
        overlappedRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
 
        if (comStat.cbInQue < 512)
        {
            bRet = pSerialPort->ReadFile(recvTemp, comStat.cbInQue, &dwRead, &overlappedRead);
        }
        else
        {
            bRet = pSerialPort->ReadFile(recvTemp, 500, &dwRead, &overlappedRead);
        }
 
        if (comStat.cbInQue >= dwRead)
        {
            nLength = 0;
            memcpy(recvBuf + nLength, recvTemp, dwRead);
            nLength += dwRead;
        }
 
        CloseHandle(overlappedRead.hEvent);
 
 
        if (comStat.cbInQue == dwRead)
        {
            nLength = 0;
            CEdit* pEditRecv = (CEdit*)pWinApp->GetDlgItem(IDC_EDIT_RECE);
            if (pEditRecv)
            {
                CString strRecv;
                pEditRecv->GetWindowText(strRecv);
                strRecv += recvBuf;
                pEditRecv->SetWindowText(strRecv);
            }
        }
 
        if (!bRet)//判断是否重叠
        {
            if (ERROR_IO_PENDING == GetLastError())
            {
                while (!bRet)
                {
                    bRet = pSerialPort->GetOverlappedResult(NULL, &dwRead, TRUE);
                    if (GetLastError() != ERROR_IO_INCOMPLETE)
                    {
                        pSerialPort->ClearCommError(&dwError, &comStat);
                        break;
                    }
                }
            }
        }
    }
}