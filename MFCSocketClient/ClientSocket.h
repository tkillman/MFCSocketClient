#pragma once
#include <afxsock.h>

class ClientSocket : public CAsyncSocket
{
public:
	void StartConnect(const CString& host, UINT port);

protected:
	void OnConnect(int nErrorCode) override;
	void OnReceive(int nErrorCode) override;
	void OnClose(int nErrorCode) override;
};

