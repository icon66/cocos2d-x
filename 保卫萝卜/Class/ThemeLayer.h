#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include<vector>

using namespace std;
using namespace cocos2d::ui;
USING_NS_CC;

class CThemeLayer:public Layer
{
public:
	CThemeLayer();
	~CThemeLayer();
	static Scene *createScene();
	virtual bool init();
	void btnCallBack(Ref * pSender);
	void callBack(Ref * pSender);
	bool onTouchBegan (Touch *pTouch,Event *pEvent);
	void onTouchEnded (Touch *pTouch, Event*pEvent);
	bool judgeLock(const int& nIndex);
	virtual void update(float fDelta);
	CREATE_FUNC(CThemeLayer);
	
private:
	MenuItemImage *m_pHome;
	MenuItemImage *m_pHelp;
	MenuItemImage *m_pPreTheme;
	MenuItemImage *m_pNextTheme;
	PageView *m_pPageView;
	Vec2 m_StartPos;
	Sprite *m_arrSprites[3];
	Vector<Sprite*> m_vecPoint;
	vector<int> vecLockThemeIndex;
	int m_nCurIndex;
};

