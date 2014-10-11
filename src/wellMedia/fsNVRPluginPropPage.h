#pragma once

// fsNVRPluginPropPage.h : CfsNVRPluginPropPage 属性页类的声明。


// CfsNVRPluginPropPage : 有关实现的信息，请参阅 fsNVRPluginPropPage.cpp。

class CfsNVRPluginPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CfsNVRPluginPropPage)
	DECLARE_OLECREATE_EX(CfsNVRPluginPropPage)

// 构造函数
public:
	CfsNVRPluginPropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_FSNVRPLUGIN };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
public:
};

