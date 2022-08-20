#pragma once
#include "cocos2d.h"
#include "DataMgr.h"
#include "ui/CocosGUI.h"


using namespace cocos2d::ui;
USING_NS_CC;

class CGameLogo:public Layer
{
public:
	CGameLogo();
	~CGameLogo();
	static Scene* createScene();
	virtual bool init();
	//void changeLoadBar(float delta);
	void changeScene(float delta);
	virtual void update(float fDelta);
	CREATE_FUNC(CGameLogo);

private:
	LoadingBar * pLoadBar;
	LabelTTF *loadNum;
	Label *label;
	vector<string> vecPng;
	vector<string> vecPlist;
	int m_nCurCount;
	int m_nTotalCount;
	int m_nIndex;
};


