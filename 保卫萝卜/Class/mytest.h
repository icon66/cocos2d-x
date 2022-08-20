#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;

USING_NS_CC;

class CTest:public Layer
{
public:
	CTest();
	~CTest();
	static Scene *createScene();
	virtual bool init();
	CREATE_FUNC(CTest);
	bool onTouchBegan(Touch *pTouch, Event  *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
private:
	Vec2 touchBeginPos;
	float maxDistance;
	ListView *pListView;

};

