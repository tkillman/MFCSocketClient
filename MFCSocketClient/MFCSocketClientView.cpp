
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
END_MESSAGE_MAP()

// CMFCSocketClientView 생성/소멸

CMFCSocketClientView::CMFCSocketClientView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFCSocketClientView::~CMFCSocketClientView()
{
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
	if (!m_socket.Create()) 
	{
		TRACE(_T("소켓 생성 실패 : %d\n"), m_socket.GetLastError());
		return;
	}
	m_socket.StartConnect(_T("127.0.0.1"), 9000);
}

// CMFCSocketClientView 그리기

void CMFCSocketClientView::OnDraw(CDC* /*pDC*/)
{
	CMFCSocketClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
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
