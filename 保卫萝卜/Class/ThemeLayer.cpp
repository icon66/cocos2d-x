#include "ThemeLayer.h"
#include "GameMenu.h"
#include "HelpLayer.h"
#include "LevelLayer.h"
#include "DataMgr.h"

CThemeLayer::CThemeLayer()
{
	m_nCurIndex = 1;
}

CThemeLayer::~CThemeLayer()
{
}

Scene * CThemeLayer::createScene()
{
	Scene *pScene = Scene::create();
	Layer *pLayer = CThemeLayer::create();
	pScene->addChild(pLayer);
	return pScene;
}

bool CThemeLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	SpriteFrameCache *pCache = SpriteFrameCache::getInstance();

	//背景图
	Sprite *pBg = Sprite::createWithSpriteFrameName("ss_bg.png");
	pBg->setScaleX(visibleSize.width / pBg->getContentSize().width);
	pBg->setScaleY(visibleSize.height / pBg->getContentSize().height);
	pBg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(pBg);


	//轮播菜单
	m_pPageView = PageView::create();
	m_pPageView->setPropagateTouchEvents(true);
	m_pPageView->setDirection(ScrollView::Direction::HORIZONTAL);
	m_pPageView->setBounceEnabled(true);
	char szName[32] = {};
	char szNameCH[32] = {};
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/scene/themeScene.plist");
	//添加主题页
	for (int i = 1; i <= 5; i++)
	{
		sprintf_s(szName, "theme_pack0%d.png", i);
		Sprite *pTheme = Sprite::createWithSpriteFrameName(szName);
		pTheme->setScaleY(0.95);
		pTheme->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2.1));
		//主题页
		Layout *pLayout = Layout::create();
		pLayout->setSize(Size(visibleSize.width / 3, 520));
		pLayout->addChild(pTheme);
		m_pPageView->addPage(pLayout);

		//主题名称
		sprintf_s(szNameCH, "theme_pack0%d_CN.png", i);
		Sprite *theThemeCH = Sprite::createWithSpriteFrameName(szNameCH);
		theThemeCH->setPosition(Vec2(visibleSize.width / 1.95, visibleSize.height / 2.15));
		pLayout->addChild(theThemeCH, 1);
		
		int nIsLock = CDataMgr::getInstance()->getMapDtMgr()->getCurThemeLockByID(i);
		if (nIsLock == 0||i==5)
		{
			//锁
			Sprite *pLock = Sprite::createWithSpriteFrameName("theme_locked.png");
			pLock->setPosition(Vec2(visibleSize.width / 4 * 3.13, visibleSize.height / 4.15+10));
			pLayout->addChild(pLock);
			vecLockThemeIndex.push_back(i);
		}
		else
		{   //关卡数标签
			int num = CDataMgr::getInstance()->getLevelDtMgr()->getPassLevelCountByTheme(i)-1;
			Sprite *pLevel = Sprite::createWithSpriteFrameName(CDataMgr::getInstance()->getCardDtMgr()->getMarkImgByHasPass(num));
			pLevel->setScaleY(0.95);
			pLevel->setPosition(Vec2(visibleSize.width / 4 * 2.9, visibleSize.height / 8.5+10));
			pLayout->addChild(pLevel);
		}
	}

	m_pPageView->setContentSize(Size(visibleSize.width, 560));
	//插入最前面的一页
	Sprite *newSprite = Sprite::createWithSpriteFrameName("giftthisapp.png");
	newSprite->setScaleY(0.95);
	newSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2.1));
	Sprite *newSpriteCH = Sprite::createWithSpriteFrameName("giftthisapp_CN.png");
	newSpriteCH->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2.1));
	Layout *pInsertPage = Layout::create();
	pInsertPage->addChild(newSprite);
	pInsertPage->addChild(newSpriteCH);
	m_pPageView->addWidgetToPage(pInsertPage, 0, false);
	m_pPageView->setCurPageIndex(m_nCurIndex);
	this->addChild(m_pPageView);

	//添加遮盖层处理点击事件
	Layer *pClickLayer = Layer::create();
	pClickLayer->setAnchorPoint(Vec2(0.5, 0.5));
	pClickLayer->setContentSize(Size(visibleSize.width / 2, visibleSize.height / 2));
	pClickLayer->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	pClickLayer->setZOrder(100);
	this->addChild(pClickLayer);

	EventListenerTouchOneByOne *pListener = EventListenerTouchOneByOne::create();
	pListener->onTouchBegan = CC_CALLBACK_2(CThemeLayer::onTouchBegan, this);
	pListener->onTouchEnded = CC_CALLBACK_2(CThemeLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, pClickLayer);
	
	//选择关卡字样
	Sprite *pOptionCh = Sprite::createWithSpriteFrameName("ss_bg_CN.png");
	pOptionCh->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(pOptionCh);

	//回到主页按钮
	m_pHome = MenuItemImage::create("", "", CC_CALLBACK_1(CThemeLayer::btnCallBack, this));
	m_pHome->setNormalSpriteFrame(pCache->getSpriteFrameByName("theme_home_normal.png"));
	m_pHome->setSelectedSpriteFrame(pCache->getSpriteFrameByName("theme_home_pressed.png"));
	m_pHome->setPosition(Vec2(visibleSize.width / 10 * 0.6, visibleSize.height / 10 * 9.5));
	//帮助按钮
	m_pHelp = MenuItemImage::create("", "", CC_CALLBACK_1(CThemeLayer::btnCallBack, this));
	m_pHelp->setNormalSpriteFrame(pCache->getSpriteFrameByName("ss_help_normal.png"));
	m_pHelp->setSelectedSpriteFrame(pCache->getSpriteFrameByName("ss_help_pressed.png"));
	m_pHelp->setPosition(Vec2(visibleSize.width / 10 * 9.4, visibleSize.height / 10 * 9.5));
	//上一主题
	m_pPreTheme = MenuItemImage::create("", "", CC_CALLBACK_1(CThemeLayer::btnCallBack, this));
	m_pPreTheme->setNormalSpriteFrame(pCache->getSpriteFrameByName("theme_pointleft_normal.png"));
	m_pPreTheme->setSelectedSpriteFrame(pCache->getSpriteFrameByName("theme_pointleft_pressed.png"));
	m_pPreTheme->setPosition(Vec2(visibleSize.width / 10 * 0.6, visibleSize.height / 2));
	//下一主题
	m_pNextTheme = MenuItemImage::create("", "", CC_CALLBACK_1(CThemeLayer::btnCallBack, this));
	m_pNextTheme->setNormalSpriteFrame(pCache->getSpriteFrameByName("theme_pointright_normal.png"));
	m_pNextTheme->setSelectedSpriteFrame(pCache->getSpriteFrameByName("theme_pointright_pressed.png"));
	m_pNextTheme->setPosition(Vec2(visibleSize.width / 10 * 9.4, visibleSize.height / 2));


	//菜单
	Menu *pMenu = Menu::create(m_pHome, m_pHelp, m_pPreTheme, m_pNextTheme, NULL);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu);

	//轮播下面的点
	for (int i = 0; i < 6; i++)
	{
		Sprite *pPoint = Sprite::create("Themes/Scene/dot.png");
		pPoint->setScale(0.5);
		pPoint->setPosition(visibleSize.width / 5 * 2.15 + i * 30, visibleSize.height / 13.8);
		if (i == m_nCurIndex)
		{
			pPoint->setColor(Color3B(255, 255, 255));
		}
		else
		{
			pPoint->setColor(Color3B(110, 110, 110));
		}
		m_vecPoint.pushBack(pPoint);
		this->addChild(pPoint);

	}
	this->scheduleUpdate();
	return true;
}

void CThemeLayer::btnCallBack(Ref * pSender)
{
	//播放按钮音效
	SimpleAudioEngine::getInstance()->playEffect("Music/Main/Select.ogg", false);

	if (m_pHome == pSender)
	{
		Director::getInstance()->replaceScene(CGameMenu::createScene());
	}
	else if (m_pHelp == pSender)
	{
		TransitionFade *pFade = TransitionFade::create(0.5, CHelpLayer::createScene());
		Director::getInstance()->replaceScene(pFade);
	}
	else if (m_pPreTheme == pSender)
	{
		m_nCurIndex = m_pPageView->getCurPageIndex() - 1;
		m_pPageView->scrollToPage(m_nCurIndex);
	}
	else if (m_pNextTheme == pSender)
	{
		m_nCurIndex = m_pPageView->getCurPageIndex() + 1;
		m_pPageView->scrollToPage(m_nCurIndex);
	}
	
}

void CThemeLayer::callBack(Ref * pSender)
{

}

bool CThemeLayer::onTouchBegan(Touch * pTouch, Event * pEvent)
{

	m_StartPos = pTouch->getLocation();
	
	return true;
}

void CThemeLayer::onTouchEnded(Touch * pTouch, Event * pEvent)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	int nDistance = pTouch->getLocation().x - m_StartPos.x;
	if (abs(nDistance)<5)
	{
		Rect layerRec = Rect((visibleSize.width - 500) / 2, (visibleSize.height - 400) / 2,500,400);
		if (layerRec.containsPoint(pTouch->getLocation()))
		{
			//设置当前关卡
			if (m_pPageView->getCurPageIndex()==0||!judgeLock(m_pPageView->getCurPageIndex()))
			{
				return;
			}
			CDataMgr::getInstance()->getMapDtMgr()->setCurTheme(m_pPageView->getCurPageIndex());
			CDataMgr::getInstance()->getLevelDtMgr()->setCurTheme(m_pPageView->getCurPageIndex());
			TransitionFade *pFade = TransitionFade::create(0.5, CLevelLayer::createScene());
			Director::getInstance()->replaceScene(pFade);
		}
		
	}
	else if(abs(nDistance)>30)
	{
		//播放翻页音效
		SimpleAudioEngine::getInstance()->playEffect("Music/Items/MenuSelect.ogg", false);
	}

}

bool CThemeLayer::judgeLock(const int & nIndex)
{
	for (int i=0;i<vecLockThemeIndex.size();++i)
	{
		if (vecLockThemeIndex[i]==nIndex)
		{
			return false;
		}
	}
	return true;
}



void CThemeLayer::update(float fDelta)
{
	if (m_pPageView->getCurPageIndex() == 0)
	{
		m_pPreTheme->setVisible(0);
	}
	else
	{
		m_pPreTheme->setVisible(1);
	}
	if (m_pPageView->getCurPageIndex() == 5)
	{
		m_pNextTheme->setVisible(0);
	}
	else
	{
		m_pNextTheme->setVisible(1);
	}
	for (int i = 0; i < m_vecPoint.size(); i++)
	{
		if (m_pPageView->getCurPageIndex() == i)
		{
			m_vecPoint.at(i)->setColor(Color3B::WHITE);
		}
		else
		{
			m_vecPoint.at(i)->setColor(Color3B(110, 110, 110));
		}
	}

}


