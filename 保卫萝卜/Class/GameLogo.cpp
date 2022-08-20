#include "GameLogo.h"
#include "GameMenu.h"

CGameLogo::CGameLogo()
{
	 m_nCurCount=0;
	 m_nTotalCount=0;
	 m_nIndex=0;
}

CGameLogo::~CGameLogo()
{
}

Scene * CGameLogo::createScene()
{
	Scene *pScene = Scene::create();
	Layer *pLayer = CGameLogo::create();
	pScene->addChild(pLayer);
	return pScene;
}

bool CGameLogo::init()
{
	if (!Layer::init())
	{
		return false;
	}
	vecPng = CDataMgr::getInstance()->getResDt()->vecPng;
	vecPlist=CDataMgr::getInstance()->getResDt()->vecPlist;
	m_nTotalCount = vecPng.size() + vecPlist.size();
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//����ͼ
	Sprite *pLogo = Sprite::create("Logo.png");
	pLogo->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	pLogo->setScaleX(visibleSize.width / pLogo->getContentSize().width);
	pLogo->setScaleY(visibleSize.height / pLogo->getContentSize().height);
	this->addChild(pLogo);
	
	//������
	Sprite *pLoadbarRec = Sprite::create("ui/ui_ProgressBar_01.png");
	pLoadbarRec->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 5));
	pLoadbarRec->setScaleX(2);
	this->addChild(pLoadbarRec);
	pLoadBar = LoadingBar::create("ui/ui_ProgressBar_02.png");
	pLoadBar->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 5));
	pLoadBar->setScaleX(2);
	pLoadBar->setDirection(LoadingBar::Direction::LEFT);
	this->addChild(pLoadBar);

	//��ǩ
	loadNum = LabelTTF::create(" ", " Arial", 18);
	loadNum->setPosition(Vec2(visibleSize.width / 7 * 5.5, visibleSize.height / 5));
	this->addChild(loadNum);
	this->scheduleUpdate();
	return true;
}


void CGameLogo::changeScene(float delta)
{
	Director::getInstance()->replaceScene(CGameMenu::createScene());
}

void CGameLogo::update(float fDelta)
{
	
	if (m_nCurCount < vecPng.size())
	{
		TextureCache *pCache = Director::getInstance()->getTextureCache();
		pCache->addImage(vecPng[m_nIndex]);
		m_nIndex++;
		m_nCurCount++;
		if (m_nIndex == vecPng.size())
		{
			m_nIndex = 0;
		}
	}
	else
	{
		if (m_nCurCount<m_nTotalCount)
		{
			SpriteFrameCache *pCache = SpriteFrameCache::getInstance();
			pCache->addSpriteFramesWithFile(vecPlist[m_nIndex]);
			m_nIndex++;
			m_nCurCount++;
		}
	}

	if (m_nCurCount == m_nTotalCount)
	{
		Director::getInstance()->replaceScene(CGameMenu::createScene());
	}
	
	float FP = (float)m_nCurCount / m_nTotalCount*100;

	pLoadBar->setPercent(FP);

	//��ǩ
	loadNum->setString(StringUtils::format("%.2f%%", FP));
}

