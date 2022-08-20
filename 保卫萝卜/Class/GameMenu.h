#pragma once
#include "cocos2d.h"
USING_NS_CC;

class CGameMenu :public Layer
{
public:
	CGameMenu();
	~CGameMenu();
	static Scene* createScene();
	virtual bool init();
	void changeScene();
	virtual void update(float fdelta);
	void cloudMove(Node *pNode,float fSpeed,float fdelta);
	void btn_CallBack(Ref* pSender);
	void leafRotate(Node *pNode,float angle,float fdelta);
	CREATE_FUNC(CGameMenu);
private:
	Sprite * pCarrot;
	Sprite *m_pCloudSmall;
	Sprite *m_pCloudBig;
	Sprite *m_pleafLeft;
	Sprite *m_pleafRight;
	Sprite *m_pleafMid;
	MenuItemImage *m_pBtnSetting;
	MenuItemImage *m_pBtnHelp;
	MenuItemImage *m_pBtnAdventure;
	MenuItemImage *m_pBtnBoss;
	MenuItemImage *m_pBtnNest;
	int m_nTimeCount;
};

