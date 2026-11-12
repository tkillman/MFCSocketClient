
// MFCSocketClientView.h: CMFCSocketClientView 클래스의 인터페이스
//

#pragma once
#include "ClientSocket.h"
#include <vector>

class CMFCSocketClientDoc;

class CMFCSocketClientView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMFCSocketClientView() noexcept;
	DECLARE_DYNCREATE(CMFCSocketClientView)

// 특성입니다.
public:
	CMFCSocketClientDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void AppendSocketLog(const CString& message);
	void RequestReconnect();
	void OnSocketConnected();

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnInitialUpdate();

	ClientSocket m_socket;
	std::vector<CString> m_socketLogs;
	CString m_host = _T("127.0.0.1");
	UINT m_port = 9000;
	UINT m_retryCount = 0;
	bool m_reconnectScheduled = false;

	static constexpr UINT_PTR RECONNECT_TIMER_ID = 1;

// 구현입니다.
public:
	virtual ~CMFCSocketClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MFCSocketClientView.cpp의 디버그 버전
inline CMFCSocketClientDoc* CMFCSocketClientView::GetDocument() const
   { return reinterpret_cast<CMFCSocketClientDoc*>(m_pDocument); }
#endif

