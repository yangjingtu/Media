// fsNVRPluginPropPage.cpp : CfsNVRPluginPropPage 属性页类的实现。

#include "stdafx.h"
#include "fsNVRPlugin.h"
#include "fsNVRPluginPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CfsNVRPluginPropPage, COlePropertyPage)



// 消息映射

BEGIN_MESSAGE_MAP(CfsNVRPluginPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CfsNVRPluginPropPage, "FSNVRPLUGIN.fsNVRPluginPropPage.1",
	0x65757c0, 0x4c46, 0x47d8, 0xa9, 0x40, 0x76, 0x47, 0xb4, 0x23, 0x67, 0x8)



// CfsNVRPluginPropPage::CfsNVRPluginPropPageFactory::UpdateRegistry -
// 添加或移除 CfsNVRPluginPropPage 的系统注册表项

BOOL CfsNVRPluginPropPage::CfsNVRPluginPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_FSNVRPLUGIN_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CfsNVRPluginPropPage::CfsNVRPluginPropPage - 构造函数

CfsNVRPluginPropPage::CfsNVRPluginPropPage() :
	COlePropertyPage(IDD, IDS_FSNVRPLUGIN_PPG_CAPTION)
{
}



// CfsNVRPluginPropPage::DoDataExchange - 在页和属性间移动数据

void CfsNVRPluginPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CfsNVRPluginPropPage 消息处理程序
