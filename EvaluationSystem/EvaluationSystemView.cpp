// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// EvaluationSystemView.cpp : CEvaluationSystemView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
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

// CEvaluationSystemView ����/����

CEvaluationSystemView::CEvaluationSystemView():
	mOSG(0L),
	is_track_(false)
{
	// TODO: �ڴ˴���ӹ������

}

CEvaluationSystemView::~CEvaluationSystemView()
{
}

BOOL CEvaluationSystemView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CEvaluationSystemView ����

void CEvaluationSystemView::OnDraw(CDC* /*pDC*/)
{
	CEvaluationSystemDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CEvaluationSystemView ��ӡ


void CEvaluationSystemView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CEvaluationSystemView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CEvaluationSystemView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CEvaluationSystemView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CEvaluationSystemView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CEvaluationSystemView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	// theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE); // MFC���ĵ���Ŀ�Ҽ��˵�
#endif
}


// CEvaluationSystemView ���

#ifdef _DEBUG
void CEvaluationSystemView::AssertValid() const
{
	CView::AssertValid();
}

void CEvaluationSystemView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEvaluationSystemDoc* CEvaluationSystemView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEvaluationSystemDoc)));
	return (CEvaluationSystemDoc*)m_pDocument;
}
#endif //_DEBUG


// CEvaluationSystemView ��Ϣ�������


int CEvaluationSystemView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	mOSG = new COSG(m_hWnd);

	return 0;
}


void CEvaluationSystemView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

	// TODO: �ڴ˴������Ϣ����������
	if (mOSG != 0)
		delete mOSG;
	//�ȴ��߳�������ټ�������������
	WaitForSingleObject(mThreadHandle,1000);

}


BOOL CEvaluationSystemView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

	// TODO: �ڴ����ר�ô����/����û���
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
	{  // ��ȡ����
		CMFCRibbonEdit *p_edit = dynamic_cast<CMFCRibbonEdit*>(p_wnd->GetRibbonBar()->FindByID(ID_EDIT_LONGITUDE));
		if (p_edit){
			CString str  = p_edit->GetEditText();
			std::string strTemp(str.GetBuffer());
			longitude = std::atof(strTemp.c_str());
			if ((longitude<-180) || (longitude>180)){
				AfxMessageBox("���ȱ������(-180,180)֮�� !",MB_OK,MB_ICONEXCLAMATION);
				return ;
			}
		}

	}

	{	// ��ȡγ��
		CMFCRibbonEdit *p_edit = dynamic_cast<CMFCRibbonEdit*>(p_wnd->GetRibbonBar()->FindByID(ID_EDIT_LATITUDE));
		if (p_edit){
			CString str  = p_edit->GetEditText();
			std::string strTemp(str.GetBuffer());
			latitude = std::atof(strTemp.c_str());
			if ((latitude<-90) || (latitude>90)){
				AfxMessageBox("γ�ȱ�����ڣ�-90,90��֮�� !",MB_OK,MB_ICONEXCLAMATION);
				return ;
			}
		}
	}

	{	// ��ȡ�߶�
		CMFCRibbonEdit *p_edit = dynamic_cast<CMFCRibbonEdit*>(p_wnd->GetRibbonBar()->FindByID(ID_EDIT_ALTITUDE));
		if (p_edit){
			CString str  = p_edit->GetEditText();
			std::string strTemp(str.GetBuffer());
			altitude = std::atof(strTemp.c_str());
			if ((altitude<0)){
				AfxMessageBox("�߶ȱ���Ϊ��ֵ !",MB_OK,MB_ICONEXCLAMATION);
				return ;
			}
		}
	}

	{	// ��ȡˮƽ����
		CMFCRibbonEdit *p_edit = dynamic_cast<CMFCRibbonEdit*>(p_wnd->GetRibbonBar()->FindByID(ID_EDIT_HEADING));
		if (p_edit){
			CString str  = p_edit->GetEditText();
			std::string strTemp(str.GetBuffer());
			heading = std::atof(strTemp.c_str());
			if ((heading<0 || heading >360)){
				AfxMessageBox("��λ��ȡֵ��Χ[0,360] !",MB_OK,MB_ICONEXCLAMATION);
				return ;
			}
		}
	}

	{	// ��ȡ��ֱ����
		CMFCRibbonEdit *p_edit = dynamic_cast<CMFCRibbonEdit*>(p_wnd->GetRibbonBar()->FindByID(ID_EDIT_PITCH));
		if (p_edit){
			CString str  = p_edit->GetEditText();
			std::string strTemp(str.GetBuffer());
			pitch = std::atof(strTemp.c_str());
			if ((pitch<-90 || pitch > 0)){
				AfxMessageBox("������ȡֵ��Χ[-90,0] !",MB_OK,MB_ICONEXCLAMATION);
				return ;
			}
		}
	}

	{	// ��ȡ����
		CMFCRibbonEdit *p_edit = dynamic_cast<CMFCRibbonEdit*>(p_wnd->GetRibbonBar()->FindByID(ID_EDIT_RANGE));
		if (p_edit){
			CString str  = p_edit->GetEditText();
			std::string strTemp(str.GetBuffer());
			range = std::atof(strTemp.c_str());
			if ((range<0)){
				AfxMessageBox("�������Ϊ��ֵ�����λ�õ�����ľ��룬��λ���� !",MB_OK,MB_ICONEXCLAMATION);
				return ;
			}
		}
	}

	mOSG->FlyTo(longitude,latitude,altitude,heading,heading,pitch,range);
}


void CEvaluationSystemView::OnUpdateCheckTrack(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(is_track_);
}


void CEvaluationSystemView::OnCheckTrack()
{
	// TODO: �ڴ���������������
	is_track_=!is_track_;
	// mOSG->RealTimeSimulation(is_track_);
	
}


void CEvaluationSystemView::OnEditLongitude()
{
	// TODO: �ڴ���������������
}


void CEvaluationSystemView::OnEditLatitude()
{
	// TODO: �ڴ���������������
}


void CEvaluationSystemView::OnEditAltitude()
{
	// TODO: �ڴ���������������
}


void CEvaluationSystemView::OnEditHeading()
{
	// TODO: �ڴ���������������
}


void CEvaluationSystemView::OnUpdateEditHeading(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}


void CEvaluationSystemView::OnEditPitch()
{
	// TODO: �ڴ���������������
}


void CEvaluationSystemView::OnUpdateEditPitch(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}


void CEvaluationSystemView::OnEditRange()
{
	// TODO: �ڴ���������������
}


void CEvaluationSystemView::OnUpdateEditRange(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}


// �ɻ���ʼ���
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
	// TODO: �ڴ���������������
	mOSG->DisplaySituation();
}
