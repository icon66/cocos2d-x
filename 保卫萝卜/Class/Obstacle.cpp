#include "Obstacle.h"
#include "GameLayer.h"

CObstacle::CObstacle()
{
	m_nCurHp = 0;
	m_nTotalHp = 0;
}

CObstacle::~CObstacle()
{
}

bool CObstacle::initWithData(Vec2& vPos,string ntype)
{
	if (!Sprite::initWithSpriteFrameName(ntype))
	{
		return false;
	}
	int num = this->getContentSize().height;
	int nObstacleType = 0;
	int size = this->getContentSize().width;
	int size1 = this->getContentSize().height;
	if (this->getContentSize().width<=85)
	{
		nObstacleType = 3001;
	}
	else if (this->getContentSize().width<=165&&this->getContentSize().height<=85)
	{
		nObstacleType = 3002;
	}
	else
	{
		nObstacleType = 3003;
	}
	
	pData = CDataMgr::getInstance()->getCObstacleDtMgr()->getObstacleDtByType(nObstacleType);
	m_nCurHp = pData->hp;
	m_nReward = pData->nReward;
	m_nTotalHp = m_nCurHp;
	//血条
	Sprite *pHpSprite = Sprite::createWithSpriteFrameName("MonsterHP01.png");

	pHpBg = Sprite::createWithSpriteFrameName("MonsterHP02.png");
	pHpBg->setPosition(this->getContentSize().width / 2, this->getContentSize().height + 5);
	m_pHpBar = ProgressTimer::create(pHpSprite);
	this->addChild(pHpBg);
	m_pHpBar->setType(ProgressTimer::Type::BAR);
	m_pHpBar->setMidpoint(Vec2(0, 0));
	m_pHpBar->setBarChangeRate(Vec2(1, 0));
	m_pHpBar->setAnchorPoint(Vec2(0.5, 0.5));
	m_pHpBar->setPosition(this->getContentSize().width / 2, this->getContentSize().height + 5);

	this->disappear();
	this->addChild(m_pHpBar, 2);
	this->setPosition(vPos);

	//标记
	m_pBiaoji = Sprite::createWithSpriteFrameName("point01.png");
	m_pBiaoji->setPosition(this->getContentSize().width / 2, this->getContentSize().height+30);
	m_pBiaoji->setVisible(false);
	this->addChild(m_pBiaoji);

	this->scheduleUpdate();
	return true;
}


CObstacle*  CObstacle::createWithData(Vec2& vPos,string nType)
{
	CObstacle *pObstacle = new CObstacle;
	if (pObstacle&&pObstacle->initWithData(vPos,nType))
	{
		pObstacle->autorelease();
		return pObstacle;
	}
	else
	{
		CC_SAFE_DELETE(pObstacle);
		return nullptr;
	}
}

void CObstacle::update(float fDelta)
{
	float fPercent = (float)m_nCurHp / m_nTotalHp*100;
	m_pHpBar->setPercentage(fPercent);
}

Sprite * CObstacle::getBiaoji()
{
	return m_pBiaoji;
}

void CObstacle::setHp(int nAck)
{
	m_nCurHp -= nAck;
	if (m_nCurHp<=0)
	{
		////播放获得奖励动画
		Sprite *pSprite = Sprite::create();
		pSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(CDataMgr::getInstance()
			->getCardDtMgr()->getRewardImgByMoney(this->m_nReward)));
		pSprite->setPosition(this->getPosition());
		Sequence *pSequ = Sequence::create(MoveBy::create(0.5, Vec2(0, 60)), RemoveSelf::create(), nullptr);
		pSprite->runAction(pSequ->clone());
		CGameLayer::getInstance()->addChild(pSprite, 1);

		//播放障碍物破坏音效和动画
		CGameLayer::getInstance()->getAnimateMgr()->playAnimate(this->getPosition(), E_ANIMATE_EATCARROT);
		SimpleAudioEngine::getInstance()->playEffect("Music/Items/Object.ogg");
		Sequence *pSeq = Sequence::create(RemoveSelf::create(),CallFunc::create(CC_CALLBACK_0(CObstacle::checkAllClear,this)),nullptr);
		this->runAction(pSeq);

		//武器置为未选中状态
		Vector<Node*> vWeapon = CGameLayer::getInstance()->getWeaponMgr()->getChildren();
		for (Node *pWeapon : vWeapon)
		{
			static_cast<CWeapon*>(pWeapon)->resetState();
		}

	}
}


void CObstacle::showThenDisappear()
{
	Sequence *pSeq = Sequence::create(CallFunc::create(CC_CALLBACK_0(CObstacle::show, this)), DelayTime::create(2.0)
		, CallFunc::create(CC_CALLBACK_0(CObstacle::disappear, this)), nullptr);
	m_pHpBar->runAction(pSeq);
	pHpBg->runAction(pSeq);
}

void CObstacle::checkAllClear()
{
	//判断障碍物是否全部清除
	Vector<Node*>vNode = CGameLayer::getInstance()->getObstacleMgr()->getChildren();
	if (vNode.size() == 0)
	{
		CGameLayer::getInstance()->setAllObstacleClear(true);//是否清除所有障碍物置为true

		Size visibleSize = Director::getInstance()->getWinSize();
		SimpleAudioEngine::getInstance()->playEffect("Music/Items/Ach.ogg");
		Sprite *pFinal = Sprite::createWithSpriteFrameName("targetscleard_CN.png");
		pFinal->setScale(0.1);
		pFinal->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 4));
		Sequence *pSeq = Sequence::create(ScaleBy::create(1, 10), DelayTime::create(1.0), RemoveSelf::create(), nullptr);
		pFinal->runAction(pSeq);
		CGameLayer::getInstance()->addChild(pFinal);
	}
}

void CObstacle::show()
{
	m_pHpBar->setVisible(true);
	pHpBg->setVisible(true);
}

bool CObstacle::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	this->m_nReward = 0;
	return true;
}

void CObstacle::disappear()
{
	m_pHpBar->setVisible(false);
	pHpBg->setVisible(false);
}

void CObstacle::stopAction()
{
	m_pHpBar->stopAllActions();
	pHpBg->stopAllActions();
}
