// ControlListLabel.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EvaluationSystem.h"
#include "ControlListLabel.h"


// ControlListLabel

IMPLEMENT_DYNCREATE(ControlListLabel, CListView)

ControlListLabel::ControlListLabel()
{

}

ControlListLabel::~ControlListLabel()
{
}

BEGIN_MESSAGE_MAP(ControlListLabel, CListView)
END_MESSAGE_MAP()


// ControlListLabel ���

#ifdef _DEBUG
void ControlListLabel::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void ControlListLabel::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// ControlListLabel ��Ϣ�������
