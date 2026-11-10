#pragma once
#include <afxsock.h>

class CMFCSocketClientView;

class ClientSocket : public CAsyncSocket
{
public:
	void StartConnect(const CString& host, UINT port);
	void SetOutputView(CMFCSocketClientView* pView);

protected:
	void OnConnect(int nErrorCode) override;
	void OnReceive(int nErrorCode) override;
	void OnClose(int nErrorCode) override;

private:
	void Report(const CString& message);

	CMFCSocketClientView* m_pView = nullptr;
};

