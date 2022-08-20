#pragma once
#include "cocos2d.h"

USING_NS_CC;

class CLoadingLayer:public Layer
{
public:
	CLoadingLayer();
	~CLoadingLayer();
	static Scene *createScene();
	virtual bool init();
	void changeScene(float fDelta);
	CREATE_FUNC(CLoadingLayer);
private:

};

