#pragma once

class IRunnable
{
public:
	virtual void Run() = 0;
};

class CThread
{
public:
	CThread(IRunnable * run);
	~CThread();

public:
	void Start();
	void WaitFinish();

public:
	IRunnable* m_run;
	HANDLE m_hThread;
};


class IUdpRecvCallback
{
public:
	virtual void OnRecv(const char* buf, USHORT len, const char* fromIp, USHORT fromPort) = 0;
};

class CUdpClient : public IRunnable
{
public:
	CUdpClient(IUdpRecvCallback* callback);
	~CUdpClient();

public:
	BOOL Start(USHORT localPort);
	void Stop();
	BOOL SendTo(const char* buf, USHORT len, const char* remoteIp, USHORT remotePort);

protected:
	virtual void Run() override;

private:
	SOCKET m_sock;
	CThread* m_thread;
	BOOL m_running;
	IUdpRecvCallback* m_recvCallback;
};

