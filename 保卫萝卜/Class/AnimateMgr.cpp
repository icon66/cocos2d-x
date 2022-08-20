#include "AnimateMgr.h"
#include "GameLayer.h"

CAnimateMgr::CAnimateMgr()
{
	
}

CAnimateMgr::~CAnimateMgr()
{
}


//遍历容器得到相应动画
Animate * CAnimateMgr::getAnimateByID(const int& nType)
{
	for (int i=0;i<vecAnimate.size();++i)
	{
		if (vecAnimate[i]->getTag()==nType)
		{
			return vecAnimate[i];
		}
	}
	return nullptr;
}

Animate * CAnimateMgr::getBulletMoveAnimateByTypeAndGrade(const int & nType, const int & nGrade)
{
	vector<Animate*> vAnimate;
	vAnimate=mapBulletMoveAnimate[nType];
	for (int i = 0; i < vAnimate.size(); ++i)
	{
		if (vAnimate[nGrade]!=nullptr)
		{
			return vAnimate[nGrade];
		}
	}
	return nullptr;
}



bool CAnimateMgr::init()
{
	if (!Node::init())
	{
		return false;
	}

	char szName[32] = {};
	vector<string> vSpriteFrame;//存放精灵帧图片名

	//怪物产生时动画
	vSpriteFrame.clear();
	for (int i = 1; i <= 2; ++i)
	{
		sprintf_s(szName, "mcm0%d.png", i);
		vSpriteFrame.push_back(szName);
	}
	this->createAnimate(vSpriteFrame, 0.2, E_ANIMATE_CREATEMONSTER);

	//怪物死亡爆炸的云动画
	vSpriteFrame.clear();
	for (int i = 1; i <= 5; ++i)
	{
		sprintf_s(szName, "air0%d.png",i);
		vSpriteFrame.push_back(szName);
	}
	this->createAnimate(vSpriteFrame, 0.1, E_ANIMATE_MONSTERDEATH);

	//升级动画
	vSpriteFrame.clear();
	for (int i = 2; i <= 5; ++i)
	{
		sprintf_s(szName, "air3%d.png", i);
		vSpriteFrame.push_back(szName);
	}
	vSpriteFrame.push_back("air31.png");
	this->createAnimate(vSpriteFrame, 0.07, E_ANIMATE_UPGRADE);

	//障碍物被标记动画
	vSpriteFrame.clear();
	Animation *pAnimationBiaoji = Animation::create();
	for (int i = 2; i <= 3; ++i)
	{
		sprintf_s(szName, "point0%d.png", i);
		vSpriteFrame.push_back(szName);
	}
	vSpriteFrame.push_back("point01.png");
	this->createAnimate(vSpriteFrame, 0.07, E_ANIMATE_BIOAJI);

	//游戏开始动画
	vSpriteFrame.clear();
	for (int i = 1; i <= 3; ++i)
	{
		sprintf_s(szName, "countdown_0%d.png", i);
		vSpriteFrame.push_back(szName);
	}
	this->createAnimate(vSpriteFrame, 1, E_ANIMATE_GAMESTART);

	//萝卜被吃动画
	vSpriteFrame.clear();
	for (int i = 1; i <= 5; ++i)
	{
		sprintf_s(szName, "air1%d.png", i);
		vSpriteFrame.push_back(szName);
	}
	this->createAnimate(vSpriteFrame,0.1,E_ANIMATE_EATCARROT);

	//萝卜颤抖动画
	vSpriteFrame.clear();
	for (int i = 0; i <= 8; ++i)
	{
		sprintf_s(szName, "hlb1%d.png", i);
		vSpriteFrame.push_back(szName);
	}
	vSpriteFrame.push_back("hlb21.png");
	this->createAnimate(vSpriteFrame, 0.05, E_ANIMATE_CARROTSHAKE);

	//武器升级标记
	vSpriteFrame.clear();
	Animation *pAnimationBiaoji1 = Animation::create();
	for (int i = 1; i <= 2; ++i)
	{
		sprintf_s(szName, "showupgrade0%d.png", i);
		vSpriteFrame.push_back(szName);
	}
	vSpriteFrame.push_back("showupgrade01.png");
	this->createAnimate(vSpriteFrame, 0.07, E_ANIMATE_WUQIBIAOJI);

	//屎减速动画
	vSpriteFrame.clear();
	for (int i = 1; i <= 2; ++i)
	{
		sprintf_s(szName, "PShit-1%d.png", i);
		vSpriteFrame.push_back(szName);
	}
	this->createAnimate(vSpriteFrame, 0.1, E_ANIMATE_SHIT);


	return true;
}


//播放动画接口
void CAnimateMgr::playAnimate(const Vec2& vPos, const int& nType)
{
	//在指定位置播放指定的动画
	Sprite *pSprite = Sprite::create();
	pSprite->setPosition(vPos);
	CGameLayer::getInstance()->addChild(pSprite,1);
	Sequence *pSeq = Sequence::create(getAnimateByID(nType),RemoveSelf::create(),nullptr);
	pSprite->runAction(pSeq);
}

//添加动画
void CAnimateMgr::createAnimate(vector<string> vecSpriteFrame, float fDelayPerUnit, const int & nType)
{
	//每个动画创建好设置标签放到动画容器中
	Animation *pAnimation = Animation::create();
	for (int i = 0; i < vecSpriteFrame.size(); ++i)
	{
		SpriteFrame *pSpriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(vecSpriteFrame[i]);
		pAnimation->addSpriteFrame(pSpriteFrame);
	}
	pAnimation->setDelayPerUnit(fDelayPerUnit);
	Animate *m_pAnimate = Animate::create(pAnimation);
	m_pAnimate->retain();
	m_pAnimate->setTag(nType);
	vecAnimate.push_back(m_pAnimate);
	
}
