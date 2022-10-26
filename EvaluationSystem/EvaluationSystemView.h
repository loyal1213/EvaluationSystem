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

// EvaluationSystemView.h : CEvaluationSystemView 类的接口
//

#pragma once

#include "OSG.h"

class CEvaluationSystemView : public CView
{
protected: // 仅从序列化创建
	CEvaluationSystemView();
	DECLARE_DYNCREATE(CEvaluationSystemView)

// 特性
public:
	CEvaluationSystemDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CEvaluationSystemView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	COSG* mOSG;
	//HANDLE mThreadHandle;
	CRenderingThread* mThreadHandle;

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnInitialUpdate();
	afx_msg void OnFlyTo();
	afx_msg void OnUpdateCheckTrack(CCmdUI *pCmdUI);
private:
	bool is_track_;
public:
	afx_msg void OnCheckTrack();
	afx_msg void OnEditLongitude();
	afx_msg void OnEditLatitude();
	afx_msg void OnEditAltitude();
	afx_msg void OnEditHeading();
	afx_msg void OnUpdateEditHeading(CCmdUI *pCmdUI);
	afx_msg void OnEditPitch();
	afx_msg void OnUpdateEditPitch(CCmdUI *pCmdUI);
	afx_msg void OnEditRange();
	afx_msg void OnUpdateEditRange(CCmdUI *pCmdUI);
	// 飞机开始起飞
	int StartTakeOff(void);
	afx_msg void OnBtnTakeoff();
	afx_msg void OnBtnSituation();
};

#ifndef _DEBUG  // EvaluationSystemView.cpp 中的调试版本
inline CEvaluationSystemDoc* CEvaluationSystemView::GetDocument() const
   { return reinterpret_cast<CEvaluationSystemDoc*>(m_pDocument); }
#endif

