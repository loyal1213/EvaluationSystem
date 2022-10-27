// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "EvaluationSystem.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);
	if (!m_wndStatusBar.Create(this)){
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	//在状态栏显示系统时间
	// 设置要显示的内容 
	CString strTitlePane1;
	CString strTitlePane2 = CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S"));
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1,TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, FALSE), strTitlePane2);


	//设置定时器，每秒执行一次
	this->SetTimer(1,1000,NULL);

	// 启用 Visual Studio 2005 样式停靠窗口行为
	/*CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);*/

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style &=~WS_MAXIMIZEBOX;//禁用最大化按钮
	cs.style &=~WS_SIZEBOX;
	cs.style &=~FWS_ADDTOTITLE;

	cs.lpszName = _T("三维讲评系统");
	cs.cx = 970;
	cs.cy = 650;

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG



void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 设置状态栏显示时间  
	CMFCRibbonBaseElement* pElement;
	CString strTime = CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S"));
	pElement = m_wndStatusBar.FindElement(ID_STATUSBAR_PANE2);
	pElement->SetText(strTime);
	// 先别加
	pElement->Redraw();

	CFrameWndEx::OnTimer(nIDEvent);
}
