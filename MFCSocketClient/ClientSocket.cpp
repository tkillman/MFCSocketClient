#include "pch.h"
#include "ClientSocket.h"

void ClientSocket::StartConnect(const CString& host, UINT port)
{
	if (!Connect(host, port)) 
	{ 
		int error = GetLastError();

		// 비동기 연결 요청이 정상적으로 접수된 상태
		if (error != WSAEWOULDBLOCK) 
		{
			// StartConnect를 두 번 요청하면 WSAEWOULDBLOCK 상태가 아니여서 해당 if로 진입
			// WSAEALREADY 10037 이미 비동기 연결 요청이 진행중
			TRACE(_T("Connect 실패 : %d\n"), error);
		}
	}
}

void ClientSocket::OnConnect(int nErrorCode) 
{
	TRACE(_T("OnConnect :%d\n"), nErrorCode);
	
	// WSAECONNREFUSED 서버가 연결을 거부함.
	if (nErrorCode == 0) 
	{
		const char message[] = "hello\n";
		Send(message, sizeof(message) - 1);
	}
}

void ClientSocket::OnReceive(int nErrorCode) 
{
	if (nErrorCode != 0) 
	{
		TRACE(_T("OnReceive 오류 %d\n"), nErrorCode);
		return;
	}

	char buffer[4096];

	while(true)
	{
		int received = Receive(buffer, sizeof(buffer));

		if (received > 0) 
		{
			CStringA text(buffer, received);
			TRACE(_T("수신: %hs\n"), text.GetString());
			continue;
		}

		// WSAECONNRESET 서버가 연결을 강제로 끊었습니다.
		// WSAENOTCONN 연결되지 않은 소켓에 수신을 시도했습니다.
		// WSAESHUTDOWN 소켓 송수신이 종료된 상태입니다.
		// WSAENETDOWN 네트워크 시스템을 사용할 수 없는 상태입니다.
		if (received == SOCKET_ERROR && GetLastError() != WSAEWOULDBLOCK) 
		{
			TRACE(_T("Receive 실패: %d\n"), GetLastError());
		}

		return;
	}
}

void ClientSocket::OnClose(int nErrorCode) 
{
	/*
	*	내가 Close() 호출
		→ 연결 종료 요청

		상대 서버가 연결 종료
		또는 연결이 끊김
		→ OnClose() 호출 
	*/

	TRACE(_T("OnClose: %d\n"), nErrorCode);
	Close();
}