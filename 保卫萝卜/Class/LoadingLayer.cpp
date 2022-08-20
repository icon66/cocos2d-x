#include "LoadingLayer.h"
#include "GameMenu.h"
#include "GameLayer.h"

CLoadingLayer::CLoadingLayer()
{
}

CLoadingLayer::~CLoadingLayer()
{
}

Scene * CLoadingLayer::createScene()
{
	Scene *pScene = Scene::create();
	Layer *pLayer = CLoadingLayer::create();
	pScene->addChild(pLayer);
	return pScene;
}

bool CLoadingLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Sprite *pBg = Sprite::create("Themes/scene/loading-hd.png");
	pBg->setScaleX(visibleSize.width / pBg->getContentSize().width);
	pBg->setScaleY(visibleSize.height / pBg->getContentSize().height);
	pBg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(pBg);
	this->schedule(schedule_selector(CLoadingLayer::changeScene),1.0);
	return true;
}

void CLoadingLayer::changeScene(float fDelta)
{
	CGameLayer::getInstance()->destoryInstance();
	TransitionFadeTR *fade = TransitionFadeTR::create(0.8,CGameLayer::createScene());
	Director::getInstance()->replaceScene(fade);
}
