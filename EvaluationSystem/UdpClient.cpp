#include "StdAfx.h"
#include "UdpClient.h"
#include <winsock2.h>
#include "NavigationZoneAndLineStruct.h"
#pragma comment(lib, "ws2_32.lib")

CThread::CThread(IRunnable * run)
	: m_run(NULL),
     m_hThread(NULL)
{
	m_run = run;
}

CThread::~CThread()
{
	::CloseHandle(m_hThread);
	m_hThread = NULL;
}

static DWORD WINAPI ThreadFunc(LPVOID param)
{
	CThread* thread = (CThread*)param;
	if (thread->m_run != NULL)
	{
		thread->m_run->Run();
	}
	return 0;
}


void CThread::Start()
{
	if (m_hThread == NULL)
	{
		DWORD dwThreadID = 0;
		m_hThread = ::CreateThread(NULL, 0, ThreadFunc, this, 0, &dwThreadID);
	}
}

void CThread::WaitFinish()
{
	if (m_hThread != NULL)
		::WaitForSingleObject(m_hThread, INFINITE);
}


CUdpClient::CUdpClient(IUdpRecvCallback* callback)
	:m_thread(nullptr),
	m_running(FALSE),
	m_recvCallback(nullptr)
{
	m_recvCallback = callback;
}


CUdpClient::~CUdpClient()
{
	Stop();
}

BOOL CUdpClient::Start(USHORT localPort)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		TRACE(_T("WSAStartup error\n"));
		return FALSE;
	}
	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
	{
		TRACE(_T("WSAStartup version is not right\n"));
		WSACleanup();
		return FALSE;
	}


	m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_sock == INVALID_SOCKET) {
		TRACE(_T("socket failed with error: %ld\n"), WSAGetLastError());
		WSACleanup();
		return FALSE;
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(localPort);

	if (-1 == bind(m_sock, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)))
	{
		TRACE(_T("bind failed with error: %ld\n"), WSAGetLastError());
		WSACleanup();
		return FALSE;
	}



	int nRecvBuf = 700 * 1024;//ÉèÖÃÎª700K
	if (0 != setsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int)))
	{
		TRACE(_T("setsockopt buf size failed.\n"), WSAGetLastError());
		return FALSE;
	}


	if (m_thread == nullptr)
	{
		m_thread = new CThread(this);
		m_running = TRUE;
		m_thread->Start();
	}

	return TRUE;
}

void CUdpClient::Stop()
{
	closesocket(m_sock);
	m_sock = INVALID_SOCKET;
	m_running = FALSE;
	m_thread->WaitFinish();
	delete m_thread;
	WSACleanup();
}

BOOL CUdpClient::SendTo(const char * buf, USHORT len, const char* remoteIp, USHORT remotePort)
{
	if (INVALID_SOCKET == m_sock)
	{
		TRACE(_T("socket is invalid.\n"));
		return FALSE;
	}

	sockaddr_in recvAddr;
	int iResult;

	recvAddr.sin_family = AF_INET;
	recvAddr.sin_port = htons(remotePort);
	recvAddr.sin_addr.s_addr = inet_addr(remoteIp);

	iResult = sendto(m_sock,(const char *)buf, len, 0, (SOCKADDR *)& recvAddr, sizeof(recvAddr));
	if (iResult == SOCKET_ERROR) {
		TRACE(_T("sendto failed with error: %d\n"), WSAGetLastError());
		return FALSE;
	}
	return TRUE;
}


static char s_recvBuf[1024] = {0};
void CUdpClient::Run()
{
	TRACE(_T("Run\n"));
	while (m_running)
	{
		int bufLen = 1024;
		sockaddr_in senderAddr;
		int senderAddrSize = sizeof(senderAddr);
		int result = recvfrom(m_sock, s_recvBuf, bufLen, 0, (SOCKADDR *)& senderAddr, &senderAddrSize);
		if (result < 0) 
		{
			TRACE(_T("recvfrom failed with error %d\n"), WSAGetLastError());
			continue;
		}
		char *fromip = inet_ntoa(senderAddr.sin_addr);
		if (m_recvCallback != nullptr)
		{
			m_recvCallback->OnRecv(s_recvBuf, result, fromip, senderAddr.sin_port);
		}
	}
}