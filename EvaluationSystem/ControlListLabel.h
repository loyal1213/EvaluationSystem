#pragma once


// ControlListLabel ��ͼ

class ControlListLabel : public CListView
{
	DECLARE_DYNCREATE(ControlListLabel)

protected:
	ControlListLabel();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~ControlListLabel();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


