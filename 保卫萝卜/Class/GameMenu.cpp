#include "GameMenu.h"
#include "ThemeLayer.h"
#include "LoadingLayer.h"
#include "DataMgr.h"

#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

CGameMenu::CGameMenu()
{
	m_nTimeCount = 0;
}

CGameMenu::~CGameMenu()
{
}

Scene * CGameMenu::createScene()
{
	Scene* pScene = Scene::create();
	Layer* pLayer = CGameMenu::create();
	pScene->addChild(pLayer);
	return pScene;
}

bool CGameMenu::init()
{
	if (!Layer::init())
	{
		return false;
	}

	SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/Main/BGMusic.ogg",true);

	CDataMgr::getInstance();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	SpriteFrameCache *pCache = SpriteFrameCache::getInstance();
	
	//背景图
	Sprite *pBg = Sprite::createWithSpriteFrameName("mainbg.png");
	pBg->setScaleX(visibleSize.width / pBg->getContentSize().width);
	pBg->setScaleY(visibleSize.height / pBg->getContentSize().height);
	pBg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(pBg);

	//云
	m_pCloudSmall = Sprite::createWithSpriteFrameName("cloud1.png");
	m_pCloudSmall->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 4 * 3));
	this->addChild(m_pCloudSmall);

	m_pCloudBig = Sprite::createWithSpriteFrameName("cloud2.png");
	m_pCloudBig->setPosition(Vec2(visibleSize.width / 5 * 3, visibleSize.height / 5 * 4.3));
	this->addChild(m_pCloudBig);

	
	ScaleBy *pScaleBy = ScaleBy::create(0.1, 10);
	//萝卜叶
	m_pleafLeft = Sprite::createWithSpriteFrameName("leaf-1.png");
	m_pleafLeft->setPosition(Vec2(visibleSize.width / 2.25, visibleSize.height / 4 * 2.8));
	m_pleafLeft->setScale(0.1);
	m_pleafLeft->runAction(pScaleBy->clone());
	this->addChild(m_pleafLeft);

	m_pleafRight = Sprite::createWithSpriteFrameName("leaf-3.png");
	m_pleafRight->setAnchorPoint(Vec2(0, 0));
	m_pleafRight->setPosition(Vec2(visibleSize.width / 1.95, visibleSize.height / 4 * 2.4));
	m_pleafRight->setScale(0.1);
	m_pleafRight->runAction(pScaleBy->clone());
	this->addChild(m_pleafRight);

	m_pleafMid = Sprite::createWithSpriteFrameName("leaf-2.png");
	m_pleafMid->setAnchorPoint(Vec2(0.5, 0));
	m_pleafMid->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 4 * 2.42));
	m_pleafMid->setScale(0.1);
	m_pleafMid->runAction(pScaleBy->clone());
	this->addChild(m_pleafMid);

	//萝卜
	pCarrot = Sprite::createWithSpriteFrameName("carrot.png");
	pCarrot->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 4*2.2));
	pCarrot->setScale(0.1);
	pCarrot->runAction(pScaleBy->clone());
	this->addChild(pCarrot);



	//保卫萝卜字样
	Sprite *pBgCh = Sprite::createWithSpriteFrameName("mainbg_CN.png");
	pBgCh->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 6*2.9));
	this->addChild(pBgCh);

	//设置按钮
	m_pBtnSetting = MenuItemImage::create("", "", CC_CALLBACK_1(CGameMenu::btn_CallBack, this));
	m_pBtnSetting->setNormalSpriteFrame(pCache->getSpriteFrameByName("btn_setting_normal.png"));
	m_pBtnSetting->setSelectedSpriteFrame(pCache->getSpriteFrameByName("btn_setting_pressed.png"));
	m_pBtnSetting->setPosition(Vec2(visibleSize.width/10*2,visibleSize.height/3*1));

	//帮助按钮
	m_pBtnHelp = MenuItemImage::create("", "", CC_CALLBACK_1(CGameMenu::btn_CallBack, this));
	m_pBtnHelp->setNormalSpriteFrame(pCache->getSpriteFrameByName("btn_help_normal.png"));
	m_pBtnHelp->setSelectedSpriteFrame(pCache->getSpriteFrameByName("btn_help_pressed.png"));
	m_pBtnHelp->setPosition(Vec2(visibleSize.width / 10 * 8, visibleSize.height / 3 * 1));

	//冒险模式按钮
	m_pBtnAdventure = MenuItemImage::create("", "", CC_CALLBACK_1(CGameMenu::btn_CallBack, this));
	m_pBtnAdventure->setNormalSpriteFrame(pCache->getSpriteFrameByName("btn_adventure_normal_CN.png"));
	m_pBtnAdventure->setSelectedSpriteFrame(pCache->getSpriteFrameByName("btn_adventure_pressed_CN.png"));
	m_pBtnAdventure->setPosition(Vec2(visibleSize.width / 10*2, visibleSize.height / 7.5));

	//BOSS模式按钮
	m_pBtnBoss = MenuItemImage::create("", "", CC_CALLBACK_1(CGameMenu::btn_CallBack, this));
	m_pBtnBoss->setNormalSpriteFrame(pCache->getSpriteFrameByName("btn_boss_normal_CN.png"));
	m_pBtnBoss->setSelectedSpriteFrame(pCache->getSpriteFrameByName("btn_boss_pressed_CN.png"));
	m_pBtnBoss->setPosition(Vec2(visibleSize.width /10*5, visibleSize.height / 7.5));

	

	//怪物窝按钮
	m_pBtnNest = MenuItemImage::create("", "", CC_CALLBACK_1(CGameMenu::btn_CallBack, this));
	m_pBtnNest->setNormalSpriteFrame(pCache->getSpriteFrameByName("btn_nest_normal_CN.png"));
	m_pBtnNest->setSelectedSpriteFrame(pCache->getSpriteFrameByName("btn_nest_pressed_CN.png"));
	m_pBtnNest->setPosition(Vec2(visibleSize.width /10*8, visibleSize.height / 7.5));

	//按钮菜单
	Menu *PMenu = Menu::create(m_pBtnAdventure,m_pBtnBoss, m_pBtnNest, m_pBtnHelp, m_pBtnSetting,NULL);
	PMenu->setPosition(Vec2::ZERO);
	this->addChild(PMenu);

	//锁
	Sprite *pLock = Sprite::createWithSpriteFrameName("locked.png");
	pLock->setPosition(Vec2(visibleSize.width / 1.59, visibleSize.height /9.5 ));
	this->addChild(pLock,1);

	//小鸟
	Sprite *pBird = Sprite::createWithSpriteFrameName("bird.png");
	pBird->setPosition(Vec2(visibleSize.width / 4.5, visibleSize.height / 4.1 * 3));
	MoveBy *toUP = MoveBy::create(2.0, Vec2(0, 30));
	MoveBy *toDown = MoveBy::create(2.0, Vec2(0, -30));
	Sequence *sequ = Sequence::create(toUP, toDown, NULL);
	RepeatForever *rep = RepeatForever::create(sequ);
	pBird->runAction(rep);
	this->addChild(pBird);
	this->scheduleUpdate();
	return true;
}

void CGameMenu::update(float fdelta)
{
	m_nTimeCount++;
	cloudMove(m_pCloudSmall, 60, fdelta);
	cloudMove(m_pCloudBig, 30, fdelta);

	if (m_nTimeCount == 100)
	{
		leafRotate(m_pleafMid, 300, fdelta);

	}
	else if (m_nTimeCount == 150)
	{
		leafRotate(m_pleafRight, 350, fdelta);
	}
	else if (m_nTimeCount == 300)
	{
		m_nTimeCount = 0;
	}

	
}

//云的移动
void CGameMenu::cloudMove(Node *pNode, float fSpeed, float fdelta)
{
	pNode->setPositionX(pNode->getPositionX() +fdelta*fSpeed);
	if (pNode->getPositionX() > 1200)
	{
		pNode->setPositionX(-300);
	}
}

//叶子摇动
void CGameMenu::leafRotate(Node *pNode, float angle, float fdelta)
{
	RotateBy *pRotateRight = RotateBy::create(0.1, angle*fdelta);
	RotateBy *pRotateLeft = RotateBy::create(0.1, -angle*fdelta);
	Sequence *sequ = Sequence::create(pRotateRight, pRotateLeft, NULL);
	Repeat *rep = Repeat::create(sequ, 2);
	pNode->runAction(rep);
}

//按钮回调函数
void CGameMenu::btn_CallBack(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("Music/Main/Select.ogg", false);
	if (m_pBtnSetting==pSender)
	{
	}
	else if (m_pBtnHelp == pSender)
	{

	}
	else if (m_pBtnAdventure == pSender)
	{
		TransitionFade *pFade = TransitionFade::create(1.5, CThemeLayer::createScene());
		Director::getInstance()->replaceScene(pFade);
		
	}
	else if (m_pBtnBoss == pSender)
	{

	}
	else if (m_pBtnNest == pSender)
	{
		TransitionFade *pFade = TransitionFade::create(1.5, CLoadingLayer::createScene());
		Director::getInstance()->replaceScene(pFade);
	}
}