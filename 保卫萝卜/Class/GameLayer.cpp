#include "GameLayer.h"
#include "DataMgr.h"
#include "LevelLayer.h"
#include "LoadingLayer.h"
#include "GameLogo.h"

CGameLayer *CGameLayer::m_pInstance = nullptr;
CGameLayer::CGameLayer()
{
	m_bIsSpeeed = false;
	m_bIsPause = false;
	m_bIsOpenMenu = false;
	stopCreateMonster = false;
	m_bIsCloseTouch = false;
	m_bIsClearAllObstacle = false;
	m_nTotalWave = 0;
	m_nCurWave = 0;
	m_nCarrotHp = 10;
	m_nTimeCount = 0;
	m_nAnimateTimeCount = 0;
	m_bIsPass = false;
}

CGameLayer::~CGameLayer()
{
}

Scene *CGameLayer::createScene()
{
	Scene *pScene = Scene::create();
	CGameLayer *pLayer = CGameLayer::getInstance();
	pScene->addChild(pLayer);
	return pScene;
}
//初始化
bool CGameLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	
	//关掉外部背景音乐
	SimpleAudioEngine::getInstance()->stopBackgroundMusic("Music/Main/BGMusic.ogg");

	//播放该主题背景音乐
	SimpleAudioEngine::getInstance()->playBackgroundMusic(CDataMgr::getInstance()->getMapDtMgr()->getThemeMusic().c_str(),true);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	SpriteFrameCache *pCache = SpriteFrameCache::getInstance();
	string CurTiledMap = CDataMgr::getInstance()->getLevelDtMgr()->getCurTiledMap();
	//加载地图
	m_pTiledMap = TMXTiledMap::create(CurTiledMap);
	this->addChild(m_pTiledMap);

	//获取路径层
	m_pPathLayer = m_pTiledMap->getLayer("Path");
	
	//获取路径中的点
	TMXObjectGroup *m_pPointGroup = m_pTiledMap->getObjectGroup("PathObject");
	ValueVector vecPoint = m_pPointGroup->getObjects();
	for (int i=0;i<vecPoint.size();++i)
	{
		ValueMap vMap = vecPoint.at(i).asValueMap();
		float fX = vMap["x"].asFloat();
		float fY = vMap["y"].asFloat();
		Vec2 tiled = this->getTiledByPos(Vec2(fX, fY));
		Vec2 CenterPos = this->getCenterPosByTiled(tiled);
		m_VecPathPoint.push_back(CenterPos);
	}

	//获取障碍物
	TMXObjectGroup *m_pPointGroup1 = m_pTiledMap->getObjectGroup("Obstacle");
	ValueVector vecPoint1 = m_pPointGroup1->getObjects();
	for (int i = 0; i < vecPoint1.size(); ++i)
	{
		ValueMap vMap = vecPoint1.at(i).asValueMap();
		float fX = vMap["x"].asFloat();
		float fY = vMap["y"].asFloat();
		m_VecObstacle.push_back(Vec2(fX,fY));
	}

	//顶部信息栏
	Sprite *pInfoBg = Sprite::createWithSpriteFrameName("MenuBG.png");
	pInfoBg->setAnchorPoint(Vec2(0.5,1));
	pInfoBg->setScaleX(visibleSize.width / pInfoBg->getContentSize().width);
	pInfoBg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height));
	this->addChild(pInfoBg);

	//金币
	GoldLabel= Label::createWithTTF("", "fonts/Marker Felt.ttf", 28);
	GoldLabel->setPosition(Vec2(visibleSize.width / 8, visibleSize.height-35));
	m_nGold = CDataMgr::getInstance()->getLevelDtMgr()->getCurLevelInitGold();
	GoldLabel->setString(StringUtils::format("%d",m_nGold));
	this->addChild(GoldLabel);


	//显示第几波怪物
	pWaveBg = Sprite::createWithSpriteFrameName("MenuCenter_01_CN.png");
	pWaveBg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - pWaveBg->getContentSize().height / 2));
	this->addChild(pWaveBg);

	//暂停背景
	pPauseBg= Sprite::createWithSpriteFrameName("MenuCenter_02_CN.png");
	pPauseBg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - pWaveBg->getContentSize().height / 2));
	pPauseBg->setVisible(false);
	this->addChild(pPauseBg);

	//总波数
	m_nTotalWave = CDataMgr::getInstance()->getLevelDtMgr()->getCurLevelWave();
	m_pTotalWave = Label::createWithTTF("", "fonts/Marker Felt.ttf", 28);
	m_pTotalWave->setString(StringUtils::format("%d", m_nTotalWave));
	m_pTotalWave->setPosition(Vec2(pWaveBg->getContentSize().width/2+10,pWaveBg->getContentSize().height/2+7));
	pWaveBg->addChild(m_pTotalWave);
	m_pTotalWave->setVisible(true);
	
	//当前波数
	m_pCurWave = Label::createWithTTF("", "fonts/Marker Felt.ttf", 28);
	m_pCurWave->setString(StringUtils::format("0     %d", 1));
	m_pCurWave->setTextColor(Color4B(255, 255, 80, 255));
	m_pCurWave->setPosition(Vec2(pWaveBg->getContentSize().width / 2-82, pWaveBg->getContentSize().height / 2 + 7));
	pWaveBg->addChild(m_pCurWave,100);

	//快进重置回常速
	Director::getInstance()->getScheduler()->setTimeScale(1);

	//加速开关
	m_pSpeed = MenuItemImage::create("","",CC_CALLBACK_1(CGameLayer::btnCallBack,this));
	m_pSpeed->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("speed11.png"));
	m_pSpeed->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("speed12.png"));
	m_pSpeed->setPosition(Vec2(visibleSize.width / 4*2.8, visibleSize.height-m_pSpeed->getContentSize().height/2));

	m_pPause = MenuItemImage::create("", "", CC_CALLBACK_1(CGameLayer::btnCallBack, this));
	m_pPause->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("pause01.png"));
	m_pPause->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("pause02.png"));
	m_pPause->setPosition(Vec2(visibleSize.width / 4 * 3.2, visibleSize.height - m_pSpeed->getContentSize().height / 2));

	m_pMenu = MenuItemImage::create("", "", CC_CALLBACK_1(CGameLayer::btnCallBack, this));
	m_pMenu->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("menu01.png"));
	m_pMenu->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("menu02.png"));
	m_pMenu->setPosition(Vec2(visibleSize.width / 4 * 3.6, visibleSize.height - m_pSpeed->getContentSize().height / 2));

	Menu *pImageMenu = Menu::create(m_pSpeed,m_pPause,m_pMenu,nullptr);
	pImageMenu->setPosition(Vec2::ZERO);
	this->addChild(pImageMenu);

	//菜单层
	m_pMenuLayer = LayerColor::create(Color4B(50, 50, 50,100),visibleSize.width,visibleSize.height);
	m_pMenuLayer->setCascadeOpacityEnabled(false);
	Sprite *pMenu = Sprite::createWithSpriteFrameName("menu_bg.png");
	pMenu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	m_pMenuLayer->addChild(pMenu);
	m_pMenuLayer->setVisible(false);
	this->addChild(m_pMenuLayer,101);

	//游戏结束
	m_pGameOverLayer = LayerColor::create(Color4B(50, 50, 50, 100), visibleSize.width, visibleSize.height);
	m_pGameOverLayer->setCascadeOpacityEnabled(false);
	Sprite *pMenu1 = Sprite::createWithSpriteFrameName("lose_bg.png");
	pMenu1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	Sprite *pMenu1CH = Sprite::createWithSpriteFrameName("lose_bg_CN.png");
	pMenu1CH->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	m_pGameOverLayer->addChild(pMenu1);
	m_pGameOverLayer->addChild(pMenu1CH);
	m_pGameOverLayer->setVisible(false);

	//总共波数
	m_pTotal = Label::createWithTTF("15", "fonts/Marker Felt.ttf", 30);
	m_pTotal->setPosition(pMenu1->getContentSize().width / 1.6, pMenu1->getContentSize().height / 2 + 25);
	pMenu1->addChild(m_pTotal);

	//击退了几波
	pJituiWave = Label::createWithTTF(" 0     1", "fonts/Marker Felt.ttf", 28);
	pJituiWave->setTextColor(Color4B(255, 255, 80, 255));
	pJituiWave->setPosition(pMenu1->getContentSize().width/2,pMenu1->getContentSize().height/2+25);
	pMenu1->addChild(pJituiWave);
	//当前关卡
	pCurLevel = Label::createWithTTF("0 1", "fonts/Marker Felt.ttf", 20);
	pCurLevel->setTextColor(Color4B(255, 255, 80, 255));
	pCurLevel->setPosition(pMenu1->getContentSize().width /2.4, pMenu1->getContentSize().height / 2-30);
	pMenu1->addChild(pCurLevel);

	this->addChild(m_pGameOverLayer, 102);


	//通过当前关卡
	m_pPassLayer = LayerColor::create(Color4B(50, 50, 50, 100), visibleSize.width, visibleSize.height);
	m_pPassLayer->setCascadeOpacityEnabled(false);
	Sprite *pMenu2 = Sprite::createWithSpriteFrameName("win_bg.png");
	pMenu2->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	Sprite *pMenu2CH = Sprite::createWithSpriteFrameName("win_bg_CN.png");
	pMenu2CH->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	m_pPassLayer->addChild(pMenu2);
	m_pPassLayer->addChild(pMenu2CH);

	//总共波数
	m_pTotal = Label::createWithTTF("15", "fonts/Marker Felt.ttf", 30);
	m_pTotal->setPosition(pMenu2->getContentSize().width / 1.6, pMenu2->getContentSize().height / 2 + 25);
	pMenu2->addChild(m_pTotal);
	//击退了几波
	pJituiWave = Label::createWithTTF(" 0     1", "fonts/Marker Felt.ttf", 28);
	pJituiWave->setTextColor(Color4B(255, 255, 80, 255));

	pJituiWave->setPosition(pMenu2->getContentSize().width / 2, pMenu2->getContentSize().height / 2 + 25);
	pMenu2->addChild(pJituiWave);
	//当前关卡
	pCurLevel = Label::createWithTTF("0 1", "fonts/Marker Felt.ttf", 20);
	pCurLevel->setPosition(pMenu2->getContentSize().width / 2.4, pMenu2->getContentSize().height / 2 - 30);
	pCurLevel->setTextColor(Color4B(255, 255, 80, 255));
	pMenu2->addChild(pCurLevel);

	m_pPassLayer->setVisible(false);
	this->addChild(m_pPassLayer, 103);


	//游戏暂停菜单
	m_pContinue = MenuItemImage::create("","",CC_CALLBACK_1(CGameLayer::btnCallBack,this));
	m_pContinue->setNormalSpriteFrame(pCache->getSpriteFrameByName("menu_resume_normal_CN.png"));
	m_pContinue->setSelectedSpriteFrame(pCache->getSpriteFrameByName("menu_resume_pressed_CN.png"));
	m_pContinue->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2+97));

	m_pRestart = MenuItemImage::create("", "", CC_CALLBACK_1(CGameLayer::btnCallBack, this));
	m_pRestart->setNormalSpriteFrame(pCache->getSpriteFrameByName("menu_restart_normal_CN.png"));
	m_pRestart->setSelectedSpriteFrame(pCache->getSpriteFrameByName("menu_restart_pressed_CN.png"));
	m_pRestart->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2+2));

	m_pSelect = MenuItemImage::create("", "", CC_CALLBACK_1(CGameLayer::btnCallBack, this));
	m_pSelect->setNormalSpriteFrame(pCache->getSpriteFrameByName("menu_quit_normal_CN.png"));
	m_pSelect->setSelectedSpriteFrame(pCache->getSpriteFrameByName("menu_quit_pressed_CN.png"));
	m_pSelect->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2-92));

	Menu *pGameMenu = Menu::create(m_pContinue,m_pSelect,m_pRestart, nullptr);
	pGameMenu->setPosition(Vec2::ZERO);
	m_pMenuLayer->addChild(pGameMenu);

	//游戏结束菜单
	m_pOverSelect = MenuItemImage::create("", "", CC_CALLBACK_1(CGameLayer::btnCallBack, this));
	m_pOverSelect->setNormalSpriteFrame(pCache->getSpriteFrameByName("menu_quit_normal_CN.png"));
	m_pOverSelect->setSelectedSpriteFrame(pCache->getSpriteFrameByName("menu_quit_pressed_CN.png"));
	m_pOverSelect->setScale(0.7);
	m_pOverSelect->setPosition(Vec2(visibleSize.width/2+90,visibleSize.height/2-100));

	m_pOverRestart = MenuItemImage::create("", "", CC_CALLBACK_1(CGameLayer::btnCallBack, this));
	m_pOverRestart->setNormalSpriteFrame(pCache->getSpriteFrameByName("menu_restart_normal_CN.png"));
	m_pOverRestart->setSelectedSpriteFrame(pCache->getSpriteFrameByName("menu_restart_pressed_CN.png"));
	m_pOverRestart->setScale(0.7);
	m_pOverRestart->setPosition(Vec2(visibleSize.width / 2 - 90, visibleSize.height / 2 - 100));

	Menu *pGameOverMenu = Menu::create(m_pOverSelect, m_pOverRestart, nullptr);
	pGameOverMenu->setPosition(Vec2::ZERO);
	m_pGameOverLayer->addChild(pGameOverMenu);

	//游戏过关菜单
	m_pPassRestart = MenuItemImage::create("", "", CC_CALLBACK_1(CGameLayer::btnCallBack, this));
	m_pPassRestart->setNormalSpriteFrame(pCache->getSpriteFrameByName("menu_restart_normal_CN.png"));
	m_pPassRestart->setSelectedSpriteFrame(pCache->getSpriteFrameByName("menu_restart_pressed_CN.png"));
	m_pPassRestart->setScale(0.7);
	m_pPassRestart->setPosition(Vec2(visibleSize.width / 2 - 90, visibleSize.height / 2 - 100));

	m_pPassContinue = MenuItemImage::create("", "", CC_CALLBACK_1(CGameLayer::btnCallBack, this));
	m_pPassContinue->setNormalSpriteFrame(pCache->getSpriteFrameByName("menu_resume_normal_CN.png"));
	m_pPassContinue->setSelectedSpriteFrame(pCache->getSpriteFrameByName("menu_resume_pressed_CN.png"));
	m_pPassContinue->setScale(0.7);
	m_pPassContinue->setPosition(Vec2(visibleSize.width / 2 + 90, visibleSize.height / 2 - 100));

	Menu *pGamePassMenu = Menu::create(m_pPassRestart, m_pPassContinue, nullptr);
	pGamePassMenu->setPosition(Vec2::ZERO);
	m_pPassLayer->addChild(pGamePassMenu);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/Items/Items01-hd.plist");
	//萝卜
	m_pCarrot = Sprite::createWithSpriteFrameName("hlb21.png");
	m_pCarrot->setAnchorPoint(Vec2(0.5, 0));
	m_pCarrot->setPosition(m_VecPathPoint.back().x+17, m_VecPathPoint.back().y-15);
	this->addChild(m_pCarrot,1);

	//萝卜血量
	m_pCarrotHpSprite = Sprite::createWithSpriteFrameName("BossHP10.png");
	m_pCarrotHpSprite->setPosition(m_pCarrot->getContentSize().width-15,m_pCarrot->getContentSize().height/2-15);
	m_pCarrot->addChild(m_pCarrotHpSprite);

	//开始位置牌
	m_pStartPoint = Sprite::createWithSpriteFrameName("StartPoint.png");
	m_pStartPoint->setAnchorPoint(Vec2(0.5,0));
	m_pStartPoint->setPosition(m_VecPathPoint.front());
	this->addChild(m_pStartPoint,1);

	//怪物管理者
	m_pMonsterMgr = CMonterMgr::create();
	this->addChild(m_pMonsterMgr,1);

	//障碍物管理者
	m_pObstacleMgr = CObstacleMgr::createWithData(m_VecObstacle);
	this->addChild(m_pObstacleMgr);

	//子弹管理者
	m_pBulletMgr = CBulletMgr::create();
	this->addChild(m_pBulletMgr);

	//武器管理者
	m_pWeaponMgr = CWeaponMgr::create();
	this->addChild(m_pWeaponMgr);

	//卡片管理者
	m_pCarMgr = CCardMgr::create();
	m_pCarMgr->setVisible(false);
	m_pCarMgr->setGlobalZOrder(200);
	this->addChild(m_pCarMgr,1);
	
	//动画管理者
	m_pAnimateMgr = CAnimateMgr::create();
	this->addChild(m_pAnimateMgr,1);

	//添加鼠标事件监听
	this->addTouch();

	//获得空地
	this->getBlock();

	//开启计时器
	this->scheduleUpdate();

	return true;
}
//添加点击
void CGameLayer::addTouch()
{
	EventListenerTouchOneByOne *pListener = EventListenerTouchOneByOne::create();
	pListener->onTouchBegan=[=](Touch *pTouch,Event *pEvent)
	{
		//只有菜单未打开时点击事件才有效
		if (m_bIsOpenMenu||m_bIsCloseTouch)
		{
			return false;
		}
		return true;
	};
	pListener->onTouchEnded = [=](Touch *pTouch, Event *pEvent)
	{
		Vec2 clickPos = pTouch->getLocation();
		Vec2 tiledPos = this->getTiledByPos(clickPos);
		Vec2 centerPos = this->getCenterPosByTiled(tiledPos);
		
		/*Sprite *pSprite = m_pPathLayer->getTileAt(tiledPos);
		if (pSprite!=nullptr)
		{
			CCLOG("!Tower");
		}
		else
		{
			CCLOG("Tower");
		}*/
		int nGid = m_pPathLayer->getTileGIDAt(tiledPos);
		if (nGid == 0&& tiledPos.y!=0)
		{
			CCLOG("Tower");
		}
		else
		{
			CCLOG("!Tower");
		}
		m_pCarMgr->clickEvent(clickPos, nGid);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener,this);
}
//按钮回调
void CGameLayer::btnCallBack(Ref * pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("Music/Items/MenuSelect.ogg",false);
	//加速按钮
	if (pSender==m_pSpeed)
	{
		m_bIsSpeeed = !m_bIsSpeeed;
		if (m_bIsSpeeed)
		{
			m_pSpeed->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("speed21.png"));
			Director::getInstance()->getScheduler()->setTimeScale(2);
		}
		else
		{
			m_pSpeed->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("speed11.png"));
			Director::getInstance()->getScheduler()->setTimeScale(1);
		}
	}
	//处理暂停按钮
	else if (pSender==m_pPause)
	{
		m_bIsPause = !m_bIsPause;
		if (m_bIsPause)
		{
			pWaveBg->setVisible(false);
			pPauseBg->setVisible(true);
			m_pPause->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("pause12.png"));
			Director::getInstance()->pause();
		}
		else
		{
			pWaveBg->setVisible(true);
			pPauseBg->setVisible(false);
			m_pPause->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("pause01.png"));
			Director::getInstance()->resume();
		}
	}

	//是否显示菜单按钮
	else if (pSender==m_pMenu)
	{
		m_bIsOpenMenu = !m_bIsOpenMenu;
		if (m_bIsOpenMenu)
		{
			pWaveBg->setVisible(false);
			pPauseBg->setVisible(true);
			m_pMenuLayer->setVisible(true);
			Director::getInstance()->pause();
		}
		else
		{
			pWaveBg->setVisible(true);
			pPauseBg->setVisible(false);
			m_pMenuLayer->setVisible(false);
			Director::getInstance()->resume();
		}
	}

	else if (pSender==m_pContinue)
	{
		m_bIsOpenMenu = false;
		m_pMenuLayer->setVisible(false);
		Director::getInstance()->resume();
	}
	else if (pSender== m_pRestart)
	{
		Director::getInstance()->resume();
		this->getInstance()->destoryInstance();
		TransitionFade *pfade = TransitionFade::create(1.2, CGameLayer::createScene());
		Director::getInstance()->replaceScene(pfade);
	}
	else if (pSender== m_pSelect)
	{
		Director::getInstance()->resume();
		CDataMgr::getInstance()->levelDtUpdate();
		TransitionFade *pfade = TransitionFade::create(1.5, CLevelLayer::createScene());
		Director::getInstance()->replaceScene(pfade);
	}
	

	//结束菜单按钮
	else if (pSender==m_pOverRestart)
	{
		TransitionFade *pfade = TransitionFade::create(0.1, CLoadingLayer::createScene());
		Director::getInstance()->replaceScene(pfade);

	}

	else if (pSender == m_pOverSelect)
	{
		TransitionFade *pfade=TransitionFade::create(1.5, CLevelLayer::createScene());
		Director::getInstance()->replaceScene(pfade);
	}

	else if (pSender==m_pPassRestart)
	{
		TransitionFade *pfade = TransitionFade::create(0.1, CLoadingLayer::createScene());
		Director::getInstance()->replaceScene(pfade);
	}
	else if (pSender==m_pPassContinue)
	{
		int num = CDataMgr::getInstance()->getLevelDtMgr()->getCurLevel() + 1;
		CDataMgr::getInstance()->getLevelDtMgr()->setCurLevel(CDataMgr::getInstance()->getLevelDtMgr()->getCurLevel() + 1);
		CGameLayer::getInstance()->destoryInstance();
		TransitionFade *pfade = TransitionFade::create(2, CGameLayer::createScene());
		Director::getInstance()->replaceScene(pfade);
	}
}
//将坐标转化成格子地图坐标
Vec2 CGameLayer::getTiledByPos(const Vec2 & pos)
{
	Size TiledSize = m_pPathLayer->getMapTileSize();
	//获得地图的大小（格子数）
	Size TiledMapSize = m_pTiledMap->getMapSize();

	int TiledX = pos.x / TiledSize.width;
	int nY = pos.y / TiledSize.height;
	int TiledY = TiledMapSize.height - 1 - nY;

	return Vec2(TiledX,TiledY);
}
//得到当前格子地图格子的中心点
cocos2d::Vec2 CGameLayer::getCenterPosByTiled(const Vec2& tiled)
{
	Size MapSize = m_pTiledMap->getMapSize();
	Size TiledSize = m_pPathLayer->getMapTileSize();
	float fX = (tiled.x)*TiledSize.width + TiledSize.width / 2;
	float fY = (MapSize.height-tiled.y - 1)*TiledSize.height + TiledSize.height / 2;
	return Vec2(fX,fY);
}
//得到路径点
vector<cocos2d::Vec2> CGameLayer::getPathPoint()
{
	return m_VecPathPoint;
}
//得到障碍物的点
std::vector<cocos2d::Vec2> CGameLayer::getObstaclePoint()
{
	return m_VecObstacle;
}
//减钱接口
void CGameLayer::setBuyGold(int nPrice)
{
	m_nGold -= nPrice;
	updateGoldSting();
}
//加钱接口
void CGameLayer::setAddGold(int nPrice)
{
	m_nGold += nPrice;
	updateGoldSting();
	if (!m_pCarMgr->getCanCreate())
	{
		m_pCarMgr->cardUpdate();
	}
	//金币发生改变时判断武器是否可以升级
	judgeCanUpdate();
}
//更新卡片信息
void CGameLayer::judgeCanUpdate()
{
	//判断武器是否可以升级
	Vector<Node*> vNode = m_pWeaponMgr->getChildren();
	for (Node * pNode : vNode)
	{
		CWeapon*pWeapon = static_cast<CWeapon*>(pNode);
		if (pWeapon!=nullptr&&m_nGold > pWeapon->getUpgradePrice())
		{

			pWeapon->getChildByName("tray")->getChildByName("biaoji")->setVisible(true);
			pWeapon->getChildByName("tray")->getChildByName("biaoji")->stopAllActions();
			pWeapon->getChildByName("tray")->getChildByName("biaoji")->runAction(RepeatForever::create(m_pAnimateMgr->getAnimateByID(E_ANIMATE_WUQIBIAOJI)->clone()));
		}
		else
		{
			pWeapon->getChildByName("tray")->getChildByName("biaoji")->stopAllActions();
			pWeapon->getChildByName("tray")->getChildByName("biaoji")->setVisible(false);
		}
	}
	//判断是否可以建塔
	vector<CCard*>vecCard = m_pCarMgr->getvecCard();
	for (int i = 0; i < vecCard.size(); ++i)
	{
		sWeaponDt *pDetail = CDataMgr::getInstance()->getWeaponDtMgr()
			->getWeaponDetailDtByID(vecCard[i]->getnType());
		//判断当前金币是否大于塔的建造价格
		if (CGameLayer::getInstance()->getGold() < pDetail->vecCreatePrice[0])
		{
			vecCard[i]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(pDetail->vecShopImg[0]));
		}
		else
		{
			vecCard[i]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(pDetail->vecShopImg[1]));
		}
	}
}
//设置当前波数标签
void CGameLayer::setCurWaveString()
{
	int curWave = m_pMonsterMgr->getCurWave();
	if (curWave<10)
	{
		m_pCurWave->setString(StringUtils::format(" 0    %d", curWave));
	}
	else 
	{
		m_pCurWave->setString(StringUtils::format(" 1    %d", curWave-10));
	}
	//m_pCurWave->setCascadeColorEnabled(false);
	m_pCurWave->setTextColor(Color4B(255,255,0,255));
	
}

void CGameLayer::startCreateMonster()
{
	m_pMonsterMgr->setbBegin();
}

//显示胜利菜单
void CGameLayer::showWinMenu()
{
	m_pPassLayer->setVisible(true);
	m_bIsPass = true;
	setMenuString();
	m_bIsCloseTouch = true;

	//写入数据
	int nTheme = CDataMgr::getInstance()->getMapDtMgr()->getCurTheme();
	int nLevel = CDataMgr::getInstance()->getLevelDtMgr()->getCurLevel();
	vector<int> vHonor = { };
	for (int i=0;i<2;i++)
	{
		vHonor.push_back(0);
	}

	if (m_bIsClearAllObstacle)
	{
		vHonor[0] = 1;
	}
	if (m_nCarrotHp==10)
	{
		vHonor[1] = 3;
	}
	else if (m_nCarrotHp>6)
	{
		vHonor[1] = 2;
	}
	else
	{
		vHonor[1] = 1;
	}

	CDataMgr::getInstance()->getLevelDtMgr()->UpdateFile(nTheme, nLevel, vHonor);
	
}
//得到当前金币
int CGameLayer::getGold()
{
	return m_nGold;
}

//设置萝卜血量
void CGameLayer::setCarrotHp()
{
	//萝卜被吃音效
	SimpleAudioEngine::getInstance()->playEffect("Music/Items/Crash.ogg");
	SimpleAudioEngine::getInstance()->playEffect("carrot2.ogg");
	//萝卜被吃动画
	m_pAnimateMgr->playAnimate(Vec2(m_pCarrot->getPositionX(), m_pCarrot->getPositionY()+ m_pCarrot->getContentSize().height / 2), E_ANIMATE_EATCARROT);
	
	if (m_nCarrotHp >= 1)
	{
		m_nCarrotHp--;
	}
	char szName[32] = {};
	if (m_nCarrotHp>=1)
	{
		sprintf(szName, "BossHP%.2d.png", m_nCarrotHp);
		m_pCarrotHpSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName));
	}
	//血量为0时还是显示1
	else
	{
		sprintf(szName, "BossHP01.png");
		m_pCarrotHpSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName));
	}
	char szHpPng[32] = {};

	//更新萝卜图片
	if (m_nCarrotHp == 0)
	{
		sprintf(szHpPng, "hlb1.png");
	}
	else if (m_nCarrotHp<4&&m_nCarrotHp>0)
	{
		sprintf(szHpPng, "hlb%d.png",m_nCarrotHp);
	}
	else if (m_nCarrotHp==5)
	{
		sprintf(szHpPng, "hlb4.png");
	}
	else if (m_nCarrotHp>5&&m_nCarrotHp<=7)
	{
		sprintf(szHpPng, "hlb6.png");
	}
	else
	{
		sprintf(szHpPng, "hlb%d.png",m_nCarrotHp);
	}
	m_pCarrot->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szHpPng));
	//血量为0时，播放失败音乐，并且显示失败界面
	if (m_nCarrotHp==0)
	{
		SimpleAudioEngine::getInstance()->playEffect("Music/Items/Lose.ogg");
		m_pGameOverLayer->setVisible(true);
		m_bIsCloseTouch = true;
		stopCreateMonster = true;
		setMenuString();
		m_pMonsterMgr->stopAllActions();
	}
	if (m_nCarrotHp<=3)
	{
		//萝卜图片改变，位置微调
		m_pCarrot->setPosition(m_VecPathPoint.back().x, m_VecPathPoint.back().y - 15);
	}
}

//更新过关和失败菜单的标签
void CGameLayer::setMenuString()
{
	//如果是过关状态，要多减1
	int nCha = 0;;
	if (m_bIsPass)
	{
		nCha = 1;
	}
	if (m_pMonsterMgr->getCurWave() < 10)
	{
		pJituiWave->setString(StringUtils::format(" 0     %d", m_pMonsterMgr->getCurWave() - 1-nCha));
	}
	else
	{
		pJituiWave->setString(StringUtils::format(" 1     %d", m_pMonsterMgr->getCurWave() - 10-nCha));
	}
	m_pTotal->setString(StringUtils::format("%d", CDataMgr::getInstance()->getLevelDtMgr()->getCurLevelWave()));
	pCurLevel->setString(StringUtils::format("  %.2d", CDataMgr::getInstance()->getLevelDtMgr()->getCurLevel()));
	pJituiWave->setTextColor(Color4B(255, 255, 80, 255));
	pCurLevel->setTextColor(Color4B(255, 255, 80, 255));

}
//播放倒计时音乐（CallFun）
void CGameLayer::playMusic()
{
	SimpleAudioEngine::getInstance()->playEffect("Music/Items/CountDown.ogg");
}
//销毁当前场景单例
void CGameLayer::destoryInstance()
{
	m_pInstance = nullptr;
}
//播放开场动画
void CGameLayer::playStartAnimate()
{
	Size visibleSize = Director::getInstance()->getWinSize();
	//开场动画以及音乐
	pStartSprite = Sprite::createWithSpriteFrameName("countdown_11.png");
	pStartSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(pStartSprite,1);

	Sprite *pSpriteRotate = Sprite::createWithSpriteFrameName("countdown_12.png");
	pSpriteRotate->setPosition(Vec2(pStartSprite->getContentSize().width / 2, pStartSprite->getContentSize().height / 2));
	pSpriteRotate->runAction(RepeatForever::create(RotateBy::create(1, -360)));
	pStartSprite->addChild(pSpriteRotate,1);

	m_pAnimateMgr->playAnimate(Vec2(visibleSize.width / 2, visibleSize.height / 2), E_ANIMATE_GAMESTART);
	Sequence *pSeq = Sequence::createWithTwoActions(CallFunc::create(CC_CALLBACK_0(CGameLayer::playMusic, this)), DelayTime::create(1.0));
	Sequence *pSeq1 = Sequence::create(Repeat::create(pSeq, 3), CallFunc::create(CC_CALLBACK_0(CGameLayer::startdisappear, this)), nullptr);
	this->runAction(pSeq1);

	Sequence *pSeq2 = Sequence::create(FadeIn::create(0.3), DelayTime::create(0.1), FadeOut::create(0.6),nullptr);
	for (int i=0;i<m_VecpBlock.size();++i)
	{
		Sprite *pSprite = Sprite::createWithSpriteFrameName("select_00.png");
		pSprite->setCascadeColorEnabled(false);
		pSprite->setColor(Color3B(40,40,40));
		pSprite->setPosition(m_VecpBlock[i]);
		Sequence *pSeq3 = Sequence::create(Repeat::create(pSeq2->clone(), 3), RemoveSelf::create(), nullptr);
		pSprite->runAction(pSeq3);
		this->addChild(pSprite,0);
	}

	//开始提示
	for (int i = 0; i < 4; ++i)
	{
		Sprite *m_pStartTiShi = Sprite::createWithSpriteFrameName("arrow.png");
		if (m_VecPathPoint[0].x==m_VecPathPoint[1].x)
		{
			if (m_VecPathPoint[0].y>m_VecPathPoint[1].x)
			{
				m_pStartTiShi->setRotation(90);
				m_pStartTiShi->setPosition(m_VecPathPoint.front().x, m_VecPathPoint.front().y - 35 * i);
			}
			else
			{
				m_pStartTiShi->setRotation(-90);
				m_pStartTiShi->setPosition(m_VecPathPoint.front().x, m_VecPathPoint.front().y + 35 * i);
			}
		}
		else if (m_VecPathPoint[0].y == m_VecPathPoint[1].y)
		{
			if (m_VecPathPoint[0].x > m_VecPathPoint[1].x)
			{
				m_pStartTiShi->setRotation(180);
				m_pStartTiShi->setPosition(m_VecPathPoint.front().x - 35 * i, m_VecPathPoint.front().y);
			}
			else
			{
				m_pStartTiShi->setPosition(m_VecPathPoint.front().x + 35 * i, m_VecPathPoint.front().y);
			}
		}
		Sequence *pSeq = Sequence::create(FadeIn::create(0.21*i), FadeOut::create(1 - 0.21*i), nullptr);
		Sequence *pRepeat = Sequence::create(Repeat::create(pSeq, 6), CallFunc::create(CC_CALLBACK_0(CGameLayer::startCreateMonster,this)),RemoveSelf::create(),nullptr);
		m_pStartTiShi->runAction(pRepeat);
		this->addChild(m_pStartTiShi, 1);
	}
}
//关闭点击事件
void CGameLayer::setCloseTouch(bool isClose)
{
	m_bIsCloseTouch = isClose;
}

void CGameLayer::startend()
{
	pStartSprite->runAction(RemoveSelf::create());
}

void CGameLayer::getBlock()
{
	Vector<Node*> vNode = m_pObstacleMgr->getChildren();
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 1; j < 8; ++j)
		{
			int Gid=m_pPathLayer->getTileGIDAt(Vec2(i, j));
			if (Gid == 0)
			{
				Vec2 vPos = this->getCenterPosByTiled(Vec2(i, j));

				for (int m=0; m<vNode.size();++m)
				{
					CObstacle* pObs= static_cast<CObstacle*>(vNode.at(m));
					if (pObs->getBoundingBox().containsPoint(vPos))
					{
						break;
					}
					else if (m==vNode.size()-1)
					{
						m_VecpBlock.push_back(vPos);
					}
				}	
				
			}
		}
	}
	int num = m_VecpBlock.size();
	CCLOG("blockNum:%d",m_VecpBlock.size());
}

//设置所有障碍物是否被清除
void CGameLayer::setAllObstacleClear(bool isClear)
{
	m_bIsClearAllObstacle = isClear;
}

//开场动画中GO消失(CallFun中用到)
void CGameLayer::startdisappear()
{
	SimpleAudioEngine::getInstance()->playEffect("Music/Items/GO.ogg");
	Sprite *pSprite1 = Sprite::createWithSpriteFrameName("countdown_13.png");
	pSprite1->setPosition(Vec2(pStartSprite->getContentSize().width / 2, pStartSprite->getContentSize().height/2));
	pStartSprite->addChild(pSprite1);
	Sequence *pSeq = Sequence::create(DelayTime::create(0.1),RotateBy::create(0.1,-180),ScaleTo::create(0.1,0.2),
	CallFunc::create(CC_CALLBACK_0(CGameLayer::startend,this)),nullptr);
	pSprite1->runAction(pSeq);
}

//更新金币
void CGameLayer::updateGoldSting()
{
	GoldLabel->setString(StringUtils::format("%d", m_nGold));
}

//播放动画接口（主要是子弹碰撞爆炸动画）
void CGameLayer::playBoom(const Vec2 & vPos, Animate * pAnimate)
{
	Sprite *pSprite = Sprite::create();
	pSprite->setPosition(vPos);
	this->addChild(pSprite,3);
	Sequence *pseq = Sequence::create(pAnimate, RemoveSelf::create(), nullptr);
	pSprite->runAction(pseq->clone());
}

//得到实例
CGameLayer * CGameLayer::getInstance()
{
	if (m_pInstance==nullptr)
	{
		m_pInstance = CGameLayer::create();
	}
	return m_pInstance;
}


//数据更新
void CGameLayer::update(float fDelta)
{
	this->collision();
	m_nTimeCount++;
	m_nAnimateTimeCount++;
	if (m_nAnimateTimeCount==50)
	{
		playStartAnimate();
	}

	if (m_nTimeCount==300&&m_nCarrotHp==10)
	{
		m_pCarrot->runAction(m_pAnimateMgr->getAnimateByID(E_ANIMATE_CARROTSHAKE));	
		m_nTimeCount = 0;
	}
	if (stopCreateMonster)
	{
		m_pMonsterMgr->stopSchedule();
		Vector<Node*> vNode = m_pMonsterMgr->getChildren();
		for (Node *pNode : vNode)
		{
			pNode->stopAllActions();
		}
	}
}

//减速Buff消失(CallFun)
void CGameLayer::BufferTimeOver(CMonster* pMos)
{
	pMos->addSpeed(30);
	pMos->initWithData(pMos->getType());
	pMos->removeChildByName("jiansu");
	pMos->setHasReduceSpeed(false);
}

//碰撞检测
void CGameLayer::collision()
{
	//拿到所有的子弹
	Vector<Node*> vBullet = m_pBulletMgr->getChildren();

	//拿到所有的怪物
	Vector<Node*> vMonster = m_pMonsterMgr->getChildren();

	//得到所有的障碍物
	Vector<Node*> vObstacle = m_pObstacleMgr->getChildren();
	for (Node* pBullet : vBullet)
	{
		CBullet *pBul = static_cast<CBullet*>(pBullet);
		for (Node* pMonster : vMonster)
		{
			CMonster *pMos = static_cast<CMonster*>(pMonster);

			//根据包围盒判断碰撞的子弹
			if (pBul->getType()==E_TOWER_TTHUNDER)
			{
				int num = CGameLayer::getInstance()->getCardMgr()->getCurObstacle()->getReward();
				
					if (pMos->getBoundingBox().intersectsRect(pBul->getBoundingBox()))
					{
						if (pBul->addMonsterTag(pMos->getTag()))
						{
							pMos->setHp(pBul->getAck());
							playBoom(pMos->getPosition(), pBul->getAnimate());
						}
					}
				
			}
			//根据锚点判断碰撞的子弹
			else
			{
				if (pMos->getBoundingBox().containsPoint(pBul->getPosition()))
				{
					if (pBul->addMonsterTag(pMos->getTag()))
					{
						//群攻子弹(比如树叶) 不许要移除
						if (pBul->getType() == E_TOWER_TLEAF)
						{
							
						}
						//减速子弹
						else if (pBul->getType() == E_TOWER_TSHIT)
						{
							if (!pMos->getReduceSpeed())
							{
								pBul->removeFromParent();
								pMos->setSpeed(pBul->getSpeedBuff());
								pMos->initWithData(pMos->getType());
								pMos->setHasReduceSpeed(true);
								//减速特效
								Sprite *pSprite = Sprite::createWithSpriteFrameName("PShit-11.png");
								pSprite->setAnchorPoint(Vec2(0.5, 0));
								pSprite->setPosition(Vec2(pMos->getContentSize().width/2,0));
								pSprite->runAction(RepeatForever::create(m_pAnimateMgr->getAnimateByID(E_ANIMATE_SHIT))->clone());
								pMos->addChild(pSprite,1,"jiansu");
								//3秒后消除
								Sequence *pSeq = Sequence::createWithTwoActions(DelayTime::create(3.0),
								CallFunc::create(CC_CALLBACK_0(CGameLayer::BufferTimeOver, this, pMos)));
								pMos->runAction(pSeq->clone());
							}
							
						}
						else  //其他子弹
						{
							pBul->removeFromParent();
						}
						pMos->setHp(pBul->getAck());
						//播放子弹爆炸动画
						playBoom(pMos->getPosition(), pBul->getAnimate());	
					}
				}
			}	
		}
		//遍历障碍物
		for (Node* pObstacle:vObstacle)
		{
			CObstacle *pObs= static_cast<CObstacle*>(pObstacle);
			//判断当前是否有选到障碍物
			//if (CGameLayer::getInstance()->getCardMgr()->getCurObstacle()->getReward() != 0)
			//{
				//适合通过包围盒判断碰撞的子弹
				if (pBul->getType() == E_TOWER_TTHUNDER)
				{
					if (pObs->getBoundingBox().intersectsRect(pBul->getBoundingBox()))
					{
						if (pBul->addObstacleTag(pObs->getTag()))
						{
							pObs->setHp(pBul->getAck());
							playBoom(pObs->getPosition(), pBul->getAnimate());
							pObs->stopAction();
							pObs->showThenDisappear();//不被攻击的时候血条隐藏
						}
					}
				}
				//适合通过点判断碰撞的子弹
				else
				{
					if (pObs->getBoundingBox().containsPoint(pBul->getPosition()))
					{
						if (pBul->addMonsterTag(pObs->getTag()))
						{
							if (pBul->getType() == E_TOWER_TLEAF)
							{

							}
							else
							{
								pBul->removeFromParent();
							}
							pObs->setHp(pBul->getAck());
							playBoom(pObs->getPosition(), pBul->getAnimate());
							pObs->stopAction();
							pObs->showThenDisappear();//不被攻击的时候血条隐藏
						}
					}
				}
			//}
		}	
	}
}


