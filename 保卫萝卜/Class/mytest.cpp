#include "mytest.h"

CTest::CTest()
{
}

CTest::~CTest()
{
}

Scene * CTest::createScene()
{
	Scene *pScene = Scene::create();
	Layer *pLayer = CTest::create();
	pScene->addChild(pLayer);
	return pScene;
}

bool CTest::init()
{
	if (!Layer::init())
	{
		return false;
	}


	Size visibleSize = Director::getInstance()->getVisibleSize();
	SpriteFrameCache *pCache = SpriteFrameCache::getInstance();
	pCache->addSpriteFramesWithFile("Themes/scene/stages_theme1-hd.plist");


	pListView = ListView::create();

	pListView->setScrollBarEnabled(false);
	pListView->setPropagateTouchEvents(false);
	pListView->setSwallowTouches(false);
	pListView->setBounceEnabled(true);

	pListView->setDirection(ScrollView::Direction::HORIZONTAL);

	char szName[32] = {};
	for (int i = 1; i <= 5; i++)
	{
		sprintf_s(szName, "ss_map0%d.png", i);
		Sprite *pTheme = Sprite::createWithSpriteFrameName(szName);
		pTheme->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		Layout *pLayout = Layout::create();
		pLayout->addChild(pTheme);
		pLayout->setTag(i);
		pLayout->setSize(Size(visibleSize.width / 2, visibleSize.height / 4*3.5));
		pListView->addChild(pLayout);
		
	}
	pListView->setSize(Size(visibleSize.width, visibleSize.height / 5 * 4));
	Vector<Node*> VecNode =pListView->getChildren();
	//pListView->addTouchEventListener(CC_CALLBACK_2(CTest::listCellTouchEvent, this));
	//Layout* pL = (Layout*)VecNode.at(0);
	//pL->addTouchEventListener(CC_CALLBACK_2(CTest::listCellTouchEvent, this));
	this->addChild(pListView);

	//添加遮盖层处理点击事件
	Layer *pClickLayer = Layer::create();
	pClickLayer->setAnchorPoint(Vec2(0.5, 0.5));
	pClickLayer->setContentSize(Size(visibleSize.width / 2, visibleSize.height / 2));
	pClickLayer->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	pClickLayer->setZOrder(100);
	this->addChild(pClickLayer);

	EventListenerTouchOneByOne *pListener = EventListenerTouchOneByOne::create();
	pListener->onTouchBegan = CC_CALLBACK_2(CTest::onTouchBegan, this);
	pListener->onTouchEnded = CC_CALLBACK_2(CTest::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, pClickLayer);
	return true;
}

bool CTest::onTouchBegan(Touch * pTouch, Event * unused_event)
{
	CCLOG("start");
	return true;
}

void CTest::onTouchEnded(Touch * touch, Event * unused_event)
{
	CCLOG("ended");
}

