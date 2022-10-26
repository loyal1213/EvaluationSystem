#pragma once


// ControlListLabel 视图

class ControlListLabel : public CListView
{
	DECLARE_DYNCREATE(ControlListLabel)

protected:
	ControlListLabel();           // 动态创建所使用的受保护的构造函数
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


