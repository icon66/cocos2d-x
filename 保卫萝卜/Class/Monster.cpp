#include "Monster.h"
#include "GameLayer.h"

CMonster::CMonster()
{
	nIndex = 0;
	fSpeed = 0;
	m_nCurHp = 50;
	m_nTotalHp = 50;
	bIsNewMonster = true;
	bIsReduceSpeed = false;
}

CMonster::~CMonster()
{
}


CMonster * CMonster::createWithData(const int &nType)
{
	CMonster *pMonster = new CMonster;
	if (pMonster&&pMonster->initWithData(nType))
	{
		pMonster->autorelease();
		return pMonster;
	}

	CC_SAFE_DELETE(pMonster);
	return nullptr;
}

bool CMonster::initWithData(const int &nType)
{
	this->stopAllActions();
	if (bIsNewMonster)
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		m_vecPathPoint = CGameLayer::getInstance()->getPathPoint();

		m_nType = nType;
		//根据Id拿到怪物的数据
		m_pMonsterDetailDt = CDataMgr::getInstance()->getCMonsterDtMgr()->getMonsterDtByID(nType);
		vecMonsterMoveImg = m_pMonsterDetailDt->vecMoveImg;
		m_nReward = m_pMonsterDetailDt->nReward;
		m_sdeathMuic = m_pMonsterDetailDt->deathMusic;
		int nWave = CGameLayer::getInstance()->getMonsterMgr()->getCurWave();
		m_nCurHp = m_pMonsterDetailDt->nHp + (nWave - 1) * 40;
		m_nTotalHp = m_nCurHp;
		fSpeed = m_pMonsterDetailDt->nSpeed;
		this->setPosition(Vec2(m_vecPathPoint[0]));
		bIsNewMonster = false;


		if (!Sprite::initWithSpriteFrameName(vecMonsterMoveImg[0]))
		{
			return false;
		}

		//血条
		Sprite *pHpSprite = Sprite::createWithSpriteFrameName("MonsterHP01.png");
		m_pHpBg = Sprite::createWithSpriteFrameName("MonsterHP02.png");
		m_pHpBg->setPosition(this->getContentSize().width / 2, this->getContentSize().height);
		m_pHpBar = ProgressTimer::create(pHpSprite);
		this->addChild(m_pHpBg);
		m_pHpBar->setType(ProgressTimer::Type::BAR);
		m_pHpBar->setMidpoint(Vec2(0, 0));
		m_pHpBar->setBarChangeRate(Vec2(1, 0));
		m_pHpBar->setAnchorPoint(Vec2(0.5, 0.5));
		m_pHpBar->setPosition(this->getContentSize().width / 2, this->getContentSize().height);
		this->addChild(m_pHpBar, 2);
	}
	else
	{
		nIndex--;
	}
	
	SpriteFrameCache *pCache = SpriteFrameCache::getInstance();
	
	Animation *pAnimation = Animation::create();
	for (int i=0;i<vecMonsterMoveImg.size();++i)
	{
		pAnimation->addSpriteFrame(pCache->getSpriteFrameByName(vecMonsterMoveImg[i]));
	}
	pAnimation->setDelayPerUnit(0.2);
	m_pAnimateMove = Animate::create(pAnimation);
	m_pAnimateMove->retain();
	

	//标记
	Sprite *pBiaoji = Sprite::createWithSpriteFrameName("point02.png");
	pBiaoji->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height+25));
	pBiaoji->setVisible(false);
	this->addChild(pBiaoji,1,"Biaoji");

	//播放移动动画
	this->runAction(RepeatForever::create(m_pAnimateMove));
	this->move();
	this->scheduleUpdate();
	return true;
}

void CMonster::move()
{
	if (nIndex < m_vecPathPoint.size() - 1)
	{
		nIndex++;
		float fTime = this->getPosition().distance(m_vecPathPoint[nIndex]) / fSpeed;
		MoveTo *pMove = MoveTo::create(fTime, m_vecPathPoint[nIndex]);
		CallFunc *callFun = CallFunc::create(CC_CALLBACK_0(CMonster::move, this));
		Sequence *pSequ = Sequence::create(pMove, callFun, nullptr);
		this->runAction(pSequ);
		m_pNextPoint = m_vecPathPoint[nIndex];
	}
	else if (nIndex == m_vecPathPoint.size() - 1)
	{
		CGameLayer::getInstance()->setCarrotHp();
		this->runAction(RemoveSelf::create());
	}
}

void CMonster::update(float fDelta)
{
	float fPercent = (float)m_nCurHp / m_nTotalHp * 100;
	m_pHpBar->setPercentage((float)m_nCurHp / m_nTotalHp * 100);

	if (this->m_nCurHp <= 0)
	{
		//播放死亡动画
		CGameLayer::getInstance()->getAnimateMgr()->playAnimate(this->getPosition(), E_ANIMATE_MONSTERDEATH);

		//播放加钱动画
		Sprite *pSprite = Sprite::create();
		pSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(CDataMgr::getInstance()
			->getCardDtMgr()->getRewardImgByMoney(this->m_nReward)));
		pSprite->setPosition(this->getPositionX(),this->getPositionY()+20);
		Sequence *pSequ1 = Sequence::create(MoveBy::create(0.5, Vec2(0, 60)),RemoveSelf::create(),nullptr);
		pSprite->runAction(pSequ1);
		CGameLayer::getInstance()->addChild(pSprite, 10);
		
		//播放死亡音效
		SimpleAudioEngine::getInstance()->playEffect(m_sdeathMuic.c_str());

		//加上相应的金币
		CGameLayer::getInstance()->setAddGold(m_nReward);
		this->runAction(RemoveSelf::create());
	}
}


void CMonster::setHp(int nAck)
{
	m_nCurHp -= nAck;
}