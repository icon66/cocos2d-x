#include "HelpLayer.h"
#include "ThemeLayer.h"

CHelpLayer::CHelpLayer()
{
}

CHelpLayer::~CHelpLayer()
{
}

Scene * CHelpLayer::createScene()
{
	Scene *pScene = Scene::create();
	Layer *pLayer = CHelpLayer::create();
	pScene->addChild(pLayer);
	return pScene;
}

bool CHelpLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//ÆÁÄ»
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//¼ÓÔØ¾«ÁéÖ¡
	SpriteFrameCache *pCache = SpriteFrameCache::getInstance();
	pCache->addSpriteFramesWithFile("Themes/scene/help_3-hd.plist");
	pCache->addSpriteFramesWithFile("Themes/scene/help_1-hd.plist");
	//±³¾°Í¼
	Sprite *pBg = Sprite::createWithSpriteFrameName("help_bg.png");
	pBg->setScaleX(visibleSize.width / pBg->getContentSize().width);
	pBg->setScaleY(visibleSize.height / pBg->getContentSize().height);
	pBg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(pBg);
	//HOME¼ü
	m_BtnHome = MenuItemImage::create("", "", CC_CALLBACK_1(CHelpLayer::btnCallBack, this));
	m_BtnHome->setNormalSpriteFrame(pCache->getSpriteFrameByName("help_home_normal.png"));
	m_BtnHome->setSelectedSpriteFrame(pCache->getSpriteFrameByName("help_home_pressed.png"));
	m_BtnHome->setPosition(Vec2(visibleSize.width / 10 * 0.5, visibleSize.height / 10 * 9.2));
	m_BtnHome->setScale(0.95);

	//°ïÖú°´Å¥
	m_BtnHelp = MenuItemImage::create("", "", CC_CALLBACK_1(CHelpLayer::btnCallBack, this));
	m_BtnHelp->setNormalSpriteFrame(pCache->getSpriteFrameByName("tips_selected_CN.png"));
	m_BtnHelp->setSelectedSpriteFrame(pCache->getSpriteFrameByName("tips_selected_CN.png"));
	m_BtnHome->setScale(0.8);
	m_BtnHelp->setPosition(Vec2(visibleSize.width / 10*3.3, visibleSize.height / 10 * 9.2));

	//¹ÖÎï
	m_BtnMonster = MenuItemImage::create("", "", CC_CALLBACK_1(CHelpLayer::btnCallBack, this));
	m_BtnMonster->setNormalSpriteFrame(pCache->getSpriteFrameByName("monster_normal_CN.png"));
	m_BtnMonster->setSelectedSpriteFrame(pCache->getSpriteFrameByName("monster_selected_CN.png"));
	m_BtnHome->setScale(0.8);
	m_BtnMonster->setPosition(Vec2(visibleSize.width / 10 * 5, visibleSize.height / 10 * 9.2));

	//Ëþ
	m_BtnTower = MenuItemImage::create("", "", CC_CALLBACK_1(CHelpLayer::btnCallBack, this));
	m_BtnTower->setNormalSpriteFrame(pCache->getSpriteFrameByName("tower_normal_CN.png"));
	m_BtnTower->setSelectedSpriteFrame(pCache->getSpriteFrameByName("tower_selected_CN.png"));
	m_BtnHome->setScale(0.8);
	m_BtnTower->setPosition(Vec2(visibleSize.width / 10 * 6.7, visibleSize.height / 10 * 9.2));

	//²Ëµ¥
	Menu *pMenu = Menu::create(m_BtnHome, m_BtnHelp, m_BtnMonster, m_BtnTower, NULL);
	pMenu->setAnchorPoint(Vec2::ZERO);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu);

	//ÂÖ²¥

	PageView *pPageView = PageView::create();
	pPageView->setContentSize(Size(visibleSize.width, visibleSize.height/5*4.5));
	char szName[32] = {};
	char szTip[32] = {};
	for (int i = 1; i <= 4; i++)
	{

		Layout *pLayout = Layout::create();
		sprintf_s(szName, "tips_%d.png", i);
		sprintf_s(szTip, "tips_text_%d_CN.png", i);
		pLayout->setContentSize(Size(visibleSize.width, visibleSize.height / 5 * 4.5));
		Sprite *pPic = Sprite::createWithSpriteFrameName(szName);
		pPic->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 5*3));
		Sprite* pTip = Sprite::createWithSpriteFrameName(szTip);
		pTip->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 5 * 1.25));
		pTip->setScale(0.9);
		pLayout->addChild(pPic);
		pLayout->addChild(pTip);
		pPageView->addChild(pLayout);
	}
	this->addChild(pPageView);

	//ÏÔÊ¾µ±Ç°Ò³
	Sprite *pCurIndex = Sprite::createWithSpriteFrameName("bottom.png");
	pCurIndex->setScale(0.8);
	pCurIndex->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 10));
	this->addChild(pCurIndex);

	Sprite *pCurPage = Sprite::createWithSpriteFrameName("bottom_num_HD.png");
	pCurPage->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 8));

	Sprite *pTotalPage = Sprite::createWithSpriteFrameName("bottom_num_HD.png");
	pTotalPage->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 15.5));


	Sprite *pLine = Sprite::createWithSpriteFrameName("bottom_num_HD.png");
	pLine->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 5));

	pCurIndex->addChild(pCurPage);
	pCurIndex->addChild(pTotalPage);
	return true;
}

void CHelpLayer::btnCallBack(Ref *pSender)
{
	if (pSender == m_BtnHome)
	{
		TransitionFade *pFade = TransitionFade::create(1, CThemeLayer::createScene());
		Director::getInstance()->replaceScene(pFade);
	}
}
