#include "LevelLayer.h"
#include "ThemeLayer.h"
#include "LoadingLayer.h"
#include "GameLayer.h"

CLevelLayer::CLevelLayer()
{
}

CLevelLayer::~CLevelLayer()
{
}

Scene * CLevelLayer::createScene()
{
	Scene *pScene = Scene::create();
	Layer *pLayer = CLevelLayer::create();
	pScene->addChild(pLayer);
	return pScene;
}

bool CLevelLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	m_vecLevelPng = CDataMgr::getInstance()->getMapDtMgr()->getCurThemeMap();
	m_vecBg = CDataMgr::getInstance()->getMapDtMgr()->getCurThemeBg();


	SpriteFrameCache *pCache = SpriteFrameCache::getInstance();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	
	Sprite *pBg = Sprite::createWithSpriteFrameName("ss_bg.png");
	pBg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	pBg->setScaleX(visibleSize.width/pBg->getContentSize().width);
	pBg->setScaleY(visibleSize.height / pBg->getContentSize().height);
	this->addChild(pBg);

	Sprite *pBgCloudLeft = Sprite::createWithSpriteFrameName(m_vecBg[0]);
	pBgCloudLeft->setAnchorPoint(Vec2(0, 0));
	pBgCloudLeft->setPosition(Vec2::ZERO);
	pBgCloudLeft->setScaleX(visibleSize.width / pBg->getContentSize().width);
	pBgCloudLeft->setScaleY(visibleSize.height / pBg->getContentSize().height);
	this->addChild(pBgCloudLeft);

	Sprite *pBgCloudRight = Sprite::createWithSpriteFrameName(m_vecBg[1]);
	pBgCloudRight->setAnchorPoint(Vec2(1, 0));
	pBgCloudRight->setPosition(Vec2(visibleSize.width,0));
	pBgCloudRight->setScaleX(visibleSize.width / pBg->getContentSize().width);
	pBgCloudRight->setScaleY(visibleSize.height / pBg->getContentSize().height);
	this->addChild(pBgCloudRight);


	//选择关卡字样
	Sprite *pOptionCh = Sprite::createWithSpriteFrameName("ss_bg_CN.png");
	pOptionCh->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(pOptionCh);

	//回到主页按钮
	m_pBtnBack = MenuItemImage::create("", "", CC_CALLBACK_1(CLevelLayer::btnCallBack, this));
	m_pBtnBack->setNormalSpriteFrame(pCache->getSpriteFrameByName("ss_back_normal.png"));
	m_pBtnBack->setSelectedSpriteFrame(pCache->getSpriteFrameByName("ss_back_pressed.png"));
	m_pBtnBack->setPosition(Vec2(visibleSize.width / 10 * 0.6, visibleSize.height / 10 * 9.5));
	//帮助按钮
	m_pBtnHelp = MenuItemImage::create("", "", CC_CALLBACK_1(CLevelLayer::btnCallBack, this));
	m_pBtnHelp->setNormalSpriteFrame(pCache->getSpriteFrameByName("ss_help_normal.png"));
	m_pBtnHelp->setSelectedSpriteFrame(pCache->getSpriteFrameByName("ss_help_pressed.png"));
	m_pBtnHelp->setPosition(Vec2(visibleSize.width / 10 * 9.4, visibleSize.height / 10 * 9.5));

	//开始游戏按钮
	m_pBtnStart = MenuItemImage::create("", "", CC_CALLBACK_1(CLevelLayer::btnCallBack, this));
	m_pBtnStart->setNormalSpriteFrame(pCache->getSpriteFrameByName("ss_play_normal_CN.png"));
	m_pBtnStart->setSelectedSpriteFrame(pCache->getSpriteFrameByName("ss_play_pressed_CN.png"));
	m_pBtnStart->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 8));
	m_pBtnStart->setScale(0.9);

	Menu *pMenu = Menu::create(m_pBtnBack, m_pBtnHelp,m_pBtnStart, NULL);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu, 1);

	pPageView = PageView::create();
	pPageView->setScrollBarEnabled(false);
	pPageView->setPropagateTouchEvents(true);
	pPageView->setSwallowTouches(false);
	pPageView->setBounceEnabled(true);
	pPageView->setSize(Size(visibleSize.width, visibleSize.height / 5 * 4.5));
	//添加关卡页面
	char szName[32] = {};
	for (int i = 0; i < m_vecLevelPng.size(); ++i)
	{
		Sprite *pTheme = Sprite::createWithSpriteFrameName(m_vecLevelPng[i]);
		pTheme->setScaleY(0.95);
		pTheme->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 1.8));

		int nisLock = CDataMgr::getInstance()->getLevelDtMgr()->getCurLevelLockByID(i);
		if (nisLock==0)
		{
			Sprite *pSprite = Sprite::createWithSpriteFrameName("ss_locked_icon.png");
			pSprite->setPosition(pTheme->getContentSize().width - pSprite->getContentSize().width / 2 - 25, pSprite->getContentSize().height / 2);
			pTheme->addChild(pSprite);
			pTheme->setColor(Color3B(198,198,198));
			vecLockLevelIndex.push_back(i);//记录未解锁的关卡的索引
		}
		else
		{
			//获得的荣誉
			vector<int> vHonor = CDataMgr::getInstance()->getLevelDtMgr()->getCurLevelHonorByID(i);
			if (vHonor[0] == 1)
			{
				Sprite *pHonor1 = Sprite::createWithSpriteFrameName("gainhonor_4.png");
				pHonor1->setPosition(Vec2(pTheme->getContentSize().width - pHonor1->getContentSize().width / 2 * 3 - 25, pHonor1->getContentSize().height / 2 - 7));
				pTheme->addChild(pHonor1);
			}
			if (vHonor[1] == 1)
			{
				Sprite *pHonor2 = Sprite::createWithSpriteFrameName("honor1.png");
				pHonor2->setPosition(Vec2(pTheme->getContentSize().width - pHonor2->getContentSize().width / 2 - 25, pHonor2->getContentSize().height / 2));
				pTheme->addChild(pHonor2);
			}
			else if (vHonor[1] == 2)
			{
				Sprite *pHonor2 = Sprite::createWithSpriteFrameName("honor2.png");
				pHonor2->setPosition(Vec2(pTheme->getContentSize().width - pHonor2->getContentSize().width / 2 - 25, pHonor2->getContentSize().height / 2));
				pTheme->addChild(pHonor2);
			}
			else if (vHonor[1] == 3)
			{
				Sprite *pHonor2 = Sprite::createWithSpriteFrameName("honor3.png");
				pHonor2->setPosition(Vec2(pTheme->getContentSize().width - pHonor2->getContentSize().width / 2 - 25, pHonor2->getContentSize().height / 2));
				pTheme->addChild(pHonor2);
			}
		}
		//主题页
		Layout *pLayout = Layout::create();
		pLayout->setContentSize(Size(visibleSize.width/2 , 520));
		pLayout->addChild(pTheme);
		pPageView->addChild(pLayout);
	}
	this->addChild(pPageView);

	//遮盖层
	Layer *pClickLayer = Layer::create();
	pClickLayer->setContentSize(Size(visibleSize.width, visibleSize.height / 5 * 4));
	pClickLayer->setZOrder(100);
	this->addChild(pClickLayer);


	//添加监听
	EventListenerTouchOneByOne *pListener = EventListenerTouchOneByOne::create();
	pListener->onTouchBegan = CC_CALLBACK_2(CLevelLayer::onTouchBegan, this);
	pListener->onTouchEnded = CC_CALLBACK_2(CLevelLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, pClickLayer);


	//显示波数
	m_pWave = Sprite::createWithSpriteFrameName("ss_waves_15.png");
	m_pWave->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 1.18));
	this->addChild(m_pWave);



	for (int i=0;i<5;++i)
	{
		m_vecWeapon.push_back(Sprite::create());
		m_vecWeapon[i]->setVisible(false);
		this->addChild(m_vecWeapon[i]);
	}
	
	this->scheduleUpdate();
	return true;
}

//按钮回调
void CLevelLayer::btnCallBack(Ref * pSender)
{
	if (m_pBtnBack == pSender)
	{
		TransitionFade *fade = TransitionFade::create(1, CThemeLayer::createScene());
		Director::getInstance()->replaceScene(fade);
	}
	else if (m_pBtnStart==pSender)
	{
		if (!judgeLock(pPageView->getCurPageIndex()))
		{
			return;
		}
		Director::getInstance()->replaceScene(CLoadingLayer::createScene());
	}
}


bool CLevelLayer::onTouchBegan(Touch * pTouch, Event *pEvent)
{
	m_StartPos = pTouch->getLocation();
	int nIndex = pPageView->getCurPageIndex();
	CCLOG("curIndex:%d", nIndex);
	return true;
}


void CLevelLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{


	Size visibleSize = Director::getInstance()->getVisibleSize();
	int nDistance = pTouch->getLocation().x - m_StartPos.x;
	if (abs(nDistance) < 5)
	{
		Rect layerRec = Rect((visibleSize.width - 500) / 2, (visibleSize.height - 400) / 2, 500, 400);
		if (layerRec.containsPoint(pTouch->getLocation()))
		{
			
			CCLOG("click");
			//如果判断关卡是否被锁定，锁定则不进入下移场景
			if (!judgeLock(pPageView->getCurPageIndex()))
			{
				return;
			}
			CGameLayer::getInstance()->destoryInstance();
			Director::getInstance()->replaceScene(CLoadingLayer::createScene());
		}
	}
	else if (abs(nDistance) > 30)
	{
		//播放翻页音效
		SimpleAudioEngine::getInstance()->playEffect("Music/Items/MenuSelect.ogg", false);
	}

}

bool CLevelLayer::judgeLock(const int& nLevel)
{
	for (int i=0;i<vecLockLevelIndex.size();++i)
	{
		if (vecLockLevelIndex[i]==nLevel)
		{
			return false;
		}
	}
	return true;
}

void CLevelLayer::update(float fDelta)
{
	m_vecLevelWeapon = CDataMgr::getInstance()->getLevelDtMgr()->getCurLevelWeapon();
	m_nCurLevelWave = CDataMgr::getInstance()->getLevelDtMgr()->getCurLevelWave();
	SpriteFrameCache *pCache = SpriteFrameCache::getInstance();
	int num = m_vecLevelWeapon.size();

	switch (m_nCurLevelWave)
	{
	case 1:
		m_pWave->setSpriteFrame(pCache->getSpriteFrameByName("ss_waves_1.png"));
		break;
	case 10:
		m_pWave->setSpriteFrame(pCache->getSpriteFrameByName("ss_waves_10.png"));
		break;
	case 15:
		m_pWave->setSpriteFrame(pCache->getSpriteFrameByName("ss_waves_15.png"));
		break;
	case 20:
		m_pWave->setSpriteFrame(pCache->getSpriteFrameByName("ss_waves_20.png"));
		break;
	case 25:
		m_pWave->setSpriteFrame(pCache->getSpriteFrameByName("ss_waves_25.png"));
		break;
	case 30:
		m_pWave->setSpriteFrame(pCache->getSpriteFrameByName("ss_waves_30.png"));
		break;
	default:
		break;
	}
	//控制距离
	switch (num)
	{
	case 1:
		startPos = 2;
		break;
	case 2:
		startPos = 2.13;
		break;
	case 3:
		startPos = 2.25;
		break;
	case 4:
		startPos = 2.38;
		break;
	case 5:
		startPos = 2.51;
		break;
	default:
		break;
	}

	CDataMgr::getInstance()->getLevelDtMgr()->setCurLevel(pPageView->getCurPageIndex()+1);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//当前关卡道具
	for (int i = 0; i < m_vecWeapon.size(); ++i)
	{
		if (i<m_vecLevelWeapon.size())
		{
			sWeaponDt *m_pCurLevelWeaponDetail = CDataMgr::getInstance()->getWeaponDtMgr()->getWeaponDetailDtByID(m_vecLevelWeapon[i]);
			m_vecWeapon[i]->setSpriteFrame(m_pCurLevelWeaponDetail->sIcon);
			m_vecWeapon[i]->setPosition(Vec2(visibleSize.width / startPos + 55 * i, visibleSize.height / 4));
		}
		if (i < num)
		{
			m_vecWeapon[i]->setVisible(true);
		}
		else
		{
			m_vecWeapon[i]->setVisible(false);
		}
	}
}
