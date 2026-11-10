#include "pch.h"
#include "ClientSocket.h"
#include "MFCSocketClientView.h"

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
			CString message;
			message.Format(_T("Connect 실패 : %d\n"), error);
			Report(message);
			TRACE(message);
		}
	}
}

void ClientSocket::OnConnect(int nErrorCode) 
{
	CString message;
	message.Format(_T("OnConnect 결과: %d"), nErrorCode);
	Report(message);
	
	// WSAECONNREFUSED 서버가 연결을 거부함.
	if (nErrorCode == 0) 
	{
		Report(_T("서버 연결 성공"));

		//const char message[] = "hello\n";
		const char* jwt =
			"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJodHRwczovL2F1dGguZXhhbXBsZS5jb20iLCJhdWQiOiJzb2NrZXQtc2VydmljZSIsInN1YiI6InRlc3QtY2xpZW50LTAwMSIsImNjY29kZSI6IlNFT1VMMDEiLCJzaG9wX2NkIjoiU0hPUDEwMCIsInJvbGVzIjoiT1JERVJfUkVBRCxPUkRFUl9XUklURSIsImlhdCI6MTc5NDMyNjY5MywiZXhwIjoxNzk0MzI2NjkzfQ.K9caqIj3dWCqI7sF4sMFTv97dQzMamdJiOGO270MIEE";
		
		CStringA authMessage("AUTH|");
		authMessage += jwt;
		authMessage += "\n";

		Send(authMessage.GetString(), authMessage.GetLength());
	}
}

void ClientSocket::OnReceive(int nErrorCode) 
{
	if (nErrorCode != 0) 
	{
		CString message;
		message.Format(_T("OnReceive 오류 %d\n"), nErrorCode);
		TRACE(message);
		Report(message);
		return;
	}

	char buffer[4096];

	while(true)
	{
		int received = Receive(buffer, sizeof(buffer));

		if (received > 0) 
		{
			int charCount = MultiByteToWideChar(
				CP_UTF8,
				0,
				buffer,
				received,
				nullptr,
				0
			);

			if (charCount == 0)
			{
				Report(_T("UTF-8 문자열 변환 실패"));
				return;
			}

			CString receivedText;

			LPWSTR output = receivedText.GetBuffer(charCount);

			MultiByteToWideChar(
				CP_UTF8,
				0,
				buffer,
				received,
				output,
				charCount
			);

			receivedText.ReleaseBuffer(charCount);

			CString message = _T("수신: ");
			message += receivedText;

			Report(message);

			continue;
		}

		// WSAECONNRESET 서버가 연결을 강제로 끊었습니다.
		// WSAENOTCONN 연결되지 않은 소켓에 수신을 시도했습니다.
		// WSAESHUTDOWN 소켓 송수신이 종료된 상태입니다.
		// WSAENETDOWN 네트워크 시스템을 사용할 수 없는 상태입니다.
		if (received == SOCKET_ERROR && GetLastError() != WSAEWOULDBLOCK) 
		{
			CString message;
			message.Format(_T("Receive 실패: %d\n"), GetLastError());
			TRACE(message);
			Report(message);
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

void ClientSocket::SetOutputView(CMFCSocketClientView* pView)
{
	m_pView = pView;
}

void ClientSocket::Report(const CString& message)
{
	if (m_pView != nullptr)
	{
		m_pView->AppendSocketLog(message);
	}
}