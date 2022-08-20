#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
USING_NS_CC;

class CHelpLayer:public Layer
{
public:
	CHelpLayer();
	~CHelpLayer();
	static Scene* createScene();
	virtual bool init();
	void btnCallBack(Ref *pSender);
	CREATE_FUNC(CHelpLayer);
private:
	MenuItemImage * m_BtnHome;
	MenuItemImage * m_BtnHelp;
	MenuItemImage * m_BtnMonster;
	MenuItemImage * m_BtnTower;
};
