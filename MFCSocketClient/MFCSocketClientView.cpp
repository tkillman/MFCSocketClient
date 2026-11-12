
// MFCSocketClientView.cpp: CMFCSocketClientView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFCSocketClient.h"
#endif

#include "MFCSocketClientDoc.h"
#include "MFCSocketClientView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCSocketClientView

IMPLEMENT_DYNCREATE(CMFCSocketClientView, CView)

BEGIN_MESSAGE_MAP(CMFCSocketClientView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCSocketClientView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CMFCSocketClientView 생성/소멸

CMFCSocketClientView::CMFCSocketClientView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFCSocketClientView::~CMFCSocketClientView()
{
	KillTimer(RECONNECT_TIMER_ID);
	m_socket.Close();
}

BOOL CMFCSocketClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

void CMFCSocketClientView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_socket.SetOutputView(this);

	if (!m_socket.Create()) 
	{
		TRACE(_T("소켓 생성 실패 : %d\n"), m_socket.GetLastError());
		return;
	}
	m_socket.StartConnect(m_host, m_port);
}

void CMFCSocketClientView::RequestReconnect()
{
	if (m_reconnectScheduled)
	{
		return;
	}

	UINT delayMs = 1000;
	for (UINT i = 0; i < m_retryCount && delayMs < 30000; ++i)
	{
		delayMs *= 2;
	}
	if (delayMs > 30000)
	{
		delayMs = 30000;
	}

	++m_retryCount;
	m_reconnectScheduled = true;
	SetTimer(RECONNECT_TIMER_ID, delayMs, nullptr);

	CString message;
	message.Format(_T("%u초 후 재연결을 시도합니다."), delayMs / 1000);
	AppendSocketLog(message);
}

void CMFCSocketClientView::OnSocketConnected()
{
	m_retryCount = 0;
}

void CMFCSocketClientView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == RECONNECT_TIMER_ID)
	{
		KillTimer(RECONNECT_TIMER_ID);
		m_reconnectScheduled = false;

		m_socket.Close();
		if (!m_socket.Create())
		{
			CString message;
			message.Format(_T("재연결용 소켓 생성 실패: %d"), m_socket.GetLastError());
			AppendSocketLog(message);
			RequestReconnect();
			return;
		}

		AppendSocketLog(_T("서버 재연결을 시도합니다."));
		m_socket.StartConnect(m_host, m_port);
		return;
	}

	CView::OnTimer(nIDEvent);
}

// CMFCSocketClientView 그리기

void CMFCSocketClientView::OnDraw(CDC* pDC)
{
	int y = 20;

	for (const CString& message : m_socketLogs)
	{
		pDC->TextOut(20, y, message);
		y += 24;
	}
}


// CMFCSocketClientView 인쇄


void CMFCSocketClientView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCSocketClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFCSocketClientView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFCSocketClientView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMFCSocketClientView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCSocketClientView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CMFCSocketClientView::AppendSocketLog(const CString& message)
{
	m_socketLogs.push_back(message);

	if (m_socketLogs.size() > 20)
	{
		m_socketLogs.erase(m_socketLogs.begin());
	}

	Invalidate(FALSE);
}

// CMFCSocketClientView 진단

#ifdef _DEBUG
void CMFCSocketClientView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCSocketClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCSocketClientDoc* CMFCSocketClientView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCSocketClientDoc)));
	return (CMFCSocketClientDoc*)m_pDocument;
}
#endif //_DEBUG




// CMFCSocketClientView 메시지 처리기
