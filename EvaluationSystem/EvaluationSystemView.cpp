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

// EvaluationSystemView.cpp : CEvaluationSystemView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "EvaluationSystem.h"
#endif

#include "EvaluationSystemDoc.h"
#include "EvaluationSystemView.h"

#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEvaluationSystemView

IMPLEMENT_DYNCREATE(CEvaluationSystemView, CView)

BEGIN_MESSAGE_MAP(CEvaluationSystemView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEvaluationSystemView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_BTN_FLYTO, &CEvaluationSystemView::OnFlyTo)
	ON_UPDATE_COMMAND_UI(ID_CHECK_TRACK, &CEvaluationSystemView::OnUpdateCheckTrack)
	ON_COMMAND(ID_CHECK_TRACK, &CEvaluationSystemView::OnCheckTrack)
	ON_COMMAND(ID_EDIT_LONGITUDE, &CEvaluationSystemView::OnEditLongitude)
	ON_COMMAND(ID_EDIT_LATITUDE, &CEvaluationSystemView::OnEditLatitude)
	ON_COMMAND(ID_EDIT_ALTITUDE, &CEvaluationSystemView::OnEditAltitude)
	ON_COMMAND(ID_EDIT_HEADING, &CEvaluationSystemView::OnEditHeading)
	ON_UPDATE_COMMAND_UI(ID_EDIT_HEADING, &CEvaluationSystemView::OnUpdateEditHeading)
	ON_COMMAND(IDC_EDIT_PITCH, &CEvaluationSystemView::OnEditPitch)
	ON_UPDATE_COMMAND_UI(IDC_EDIT_PITCH, &CEvaluationSystemView::OnUpdateEditPitch)
	ON_COMMAND(ID_EDIT_RANGE, &CEvaluationSystemView::OnEditRange)
	ON_UPDATE_COMMAND_UI(ID_EDIT_RANGE, &CEvaluationSystemView::OnUpdateEditRange)
	ON_COMMAND(ID_BTN_TAKEOFF, &CEvaluationSystemView::OnBtnTakeoff)
	ON_COMMAND(ID_BTN_SITUATION, &CEvaluationSystemView::OnBtnSituation)
END_MESSAGE_MAP()

// CEvaluationSystemView 构造/析构

CEvaluationSystemView::CEvaluationSystemView():
	mOSG(0L),
	is_track_(false)
{
	// TODO: 在此处添加构造代码

}

CEvaluationSystemView::~CEvaluationSystemView()
{
}

BOOL CEvaluationSystemView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CEvaluationSystemView 绘制

void CEvaluationSystemView::OnDraw(CDC* /*pDC*/)
{
	CEvaluationSystemDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CEvaluationSystemView 打印


void CEvaluationSystemView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CEvaluationSystemView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CEvaluationSystemView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CEvaluationSystemView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CEvaluationSystemView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CEvaluationSystemView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	// theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE); // MFC单文档项目右键菜单
#endif
}


// CEvaluationSystemView 诊断

#ifdef _DEBUG
void CEvaluationSystemView::AssertValid() const
{
	CView::AssertValid();
}

void CEvaluationSystemView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEvaluationSystemDoc* CEvaluationSystemView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEvaluationSystemDoc)));
	return (CEvaluationSystemDoc*)m_pDocument;
}
#endif //_DEBUG


// CEvaluationSystemView 消息处理程序


int CEvaluationSystemView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	mOSG = new COSG(m_hWnd);

	return 0;
}


void CEvaluationSystemView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// Pass Key Presses into OSG
	//mOSG->getViewer()->getEventQueue()->keyPress(nChar);

	// Close Window on Escape Key
	if(nChar == VK_ESCAPE)
	{
		GetParent()->SendMessage(WM_CLOSE);
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CEvaluationSystemView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (mOSG != 0)
		delete mOSG;
	//等待线程完成了再继续做其他事情
	WaitForSingleObject(mThreadHandle,1000);

}


BOOL CEvaluationSystemView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (0 == mOSG){
		return CView::OnEraseBkgnd(pDC);
	}else{
		return FALSE;
	}

	return CView::OnEraseBkgnd(pDC);
}


void CEvaluationSystemView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	// Get Filename from DocumentOpen Dialog
	// CString csFileName = "C:/Program Files (x86)/OsgEarth/osgearth_src/tests/online.earth"; //gdal_interp.earth";
	CString csFileName = "../data/earth_files/mapbox_example.earth";
	//CString csFileName = GetDocument()->GetFileName();
	
	// Init the osg class
	mOSG->InitOSG(csFileName.GetString());

	// Start the thread to do OSG Rendering
	//mThreadHandle = (HANDLE)_beginthread(&cOSG::Render, 0, mOSG); 
	mThreadHandle = new CRenderingThread(mOSG);
	mThreadHandle->start();
}


void CEvaluationSystemView::OnFlyTo()
{
	double longitude=0.0f, latitude=0.0f, altitude=0.0f, heading=0.0f, pitch=0.0f, range = 0.0f;
	CMainFrame* p_wnd = (CMainFrame*)AfxGetMainWnd();
	{  // 获取经度
		CMFCRibbonEdit *p_edit = dynamic_cast<CMFCRibbonEdit*>(p_wnd->GetRibbonBar()->FindByID(ID_EDIT_LONGITUDE));
		if (p_edit){
			CString str  = p_edit->GetEditText();
			std::string strTemp(str.GetBuffer());
			longitude = std::atof(strTemp.c_str());
			if ((longitude<-180) || (longitude>180)){
				AfxMessageBox("经度必须介于(-180,180)之间 !",MB_OK,MB_ICONEXCLAMATION);
				return ;
			}
		}

	}

	{	// 获取纬度
		CMFCRibbonEdit *p_edit = dynamic_cast<CMFCRibbonEdit*>(p_wnd->GetRibbonBar()->FindByID(ID_EDIT_LATITUDE));
		if (p_edit){
			CString str  = p_edit->GetEditText();
			std::string strTemp(str.GetBuffer());
			latitude = std::atof(strTemp.c_str());
			if ((latitude<-90) || (latitude>90)){
				AfxMessageBox("纬度必须介于（-90,90）之间 !",MB_OK,MB_ICONEXCLAMATION);
				return ;
			}
		}
	}

	{	// 获取高度
		CMFCRibbonEdit *p_edit = dynamic_cast<CMFCRibbonEdit*>(p_wnd->GetRibbonBar()->FindByID(ID_EDIT_ALTITUDE));
		if (p_edit){
			CString str  = p_edit->GetEditText();
			std::string strTemp(str.GetBuffer());
			altitude = std::atof(strTemp.c_str());
			if ((altitude<0)){
				AfxMessageBox("高度必须为正值 !",MB_OK,MB_ICONEXCLAMATION);
				return ;
			}
		}
	}

	{	// 获取水平俯仰
		CMFCRibbonEdit *p_edit = dynamic_cast<CMFCRibbonEdit*>(p_wnd->GetRibbonBar()->FindByID(ID_EDIT_HEADING));
		if (p_edit){
			CString str  = p_edit->GetEditText();
			std::string strTemp(str.GetBuffer());
			heading = std::atof(strTemp.c_str());
			if ((heading<0 || heading >360)){
				AfxMessageBox("方位角取值范围[0,360] !",MB_OK,MB_ICONEXCLAMATION);
				return ;
			}
		}
	}

	{	// 获取垂直俯仰
		CMFCRibbonEdit *p_edit = dynamic_cast<CMFCRibbonEdit*>(p_wnd->GetRibbonBar()->FindByID(ID_EDIT_PITCH));
		if (p_edit){
			CString str  = p_edit->GetEditText();
			std::string strTemp(str.GetBuffer());
			pitch = std::atof(strTemp.c_str());
			if ((pitch<-90 || pitch > 0)){
				AfxMessageBox("俯仰角取值范围[-90,0] !",MB_OK,MB_ICONEXCLAMATION);
				return ;
			}
		}
	}

	{	// 获取焦距
		CMFCRibbonEdit *p_edit = dynamic_cast<CMFCRibbonEdit*>(p_wnd->GetRibbonBar()->FindByID(ID_EDIT_RANGE));
		if (p_edit){
			CString str  = p_edit->GetEditText();
			std::string strTemp(str.GetBuffer());
			range = std::atof(strTemp.c_str());
			if ((range<0)){
				AfxMessageBox("焦距必须为正值，相机位置到焦点的距离，单位是米 !",MB_OK,MB_ICONEXCLAMATION);
				return ;
			}
		}
	}

	mOSG->FlyTo(longitude,latitude,altitude,heading,heading,pitch,range);
}


void CEvaluationSystemView::OnUpdateCheckTrack(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(is_track_);
}


void CEvaluationSystemView::OnCheckTrack()
{
	// TODO: 在此添加命令处理程序代码
	is_track_=!is_track_;
	// mOSG->RealTimeSimulation(is_track_);
	
}


void CEvaluationSystemView::OnEditLongitude()
{
	// TODO: 在此添加命令处理程序代码
}


void CEvaluationSystemView::OnEditLatitude()
{
	// TODO: 在此添加命令处理程序代码
}


void CEvaluationSystemView::OnEditAltitude()
{
	// TODO: 在此添加命令处理程序代码
}


void CEvaluationSystemView::OnEditHeading()
{
	// TODO: 在此添加命令处理程序代码
}


void CEvaluationSystemView::OnUpdateEditHeading(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CEvaluationSystemView::OnEditPitch()
{
	// TODO: 在此添加命令处理程序代码
}


void CEvaluationSystemView::OnUpdateEditPitch(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CEvaluationSystemView::OnEditRange()
{
	// TODO: 在此添加命令处理程序代码
}


void CEvaluationSystemView::OnUpdateEditRange(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


// 飞机开始起飞
int CEvaluationSystemView::StartTakeOff(void)
{
	return 0;
}


void CEvaluationSystemView::OnBtnTakeoff()
{
	mOSG->StartFly();
}


void CEvaluationSystemView::OnBtnSituation()
{
	// TODO: 在此添加命令处理程序代码
	mOSG->DisplaySituation();
}
