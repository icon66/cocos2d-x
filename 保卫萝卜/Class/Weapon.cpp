#include "Weapon.h"
#include "GameLayer.h"
#include "Card.h"

CWeapon::CWeapon()
:m_nWeaponType(0)
{
	nAttackRange = 0;
	m_nGrade = 1;
	m_bSelect = false;
}

CWeapon::~CWeapon()
{
}

CWeapon * CWeapon::createWithData(const int & nType)
{
	CWeapon *pWeapon = new CWeapon;
	if (pWeapon&&pWeapon->initWithData(nType))
	{
		pWeapon->autorelease();
		return pWeapon;
	}
	CC_SAFE_DELETE(pWeapon);
	return nullptr;
}



bool CWeapon::initWithData(const int &nType)
{
	m_pTray = Sprite::create();
	m_nWeaponType = nType;
	Sprite *Biaoji = Sprite::createWithSpriteFrameName("showupgrade01.png");
	m_vecWeaponImgDt = CDataMgr::getInstance()->getWeaponDtMgr()->getWeaponImgDtByIdAndGrade(nType, m_nGrade);
	m_pDetail = CDataMgr::getInstance()->getWeaponDtMgr()->getWeaponDetailDtByID(nType);
	m_sMusic = m_pDetail->sMusic;
	m_fShootSpeed = CDataMgr::getInstance()->getWeaponDtMgr()->getWeaponShootSpeedDtByTypeAndGrade(nType, m_nGrade);
	nAttackRange = CDataMgr::getInstance()->getWeaponDtMgr()->getWeaponRangeDtByTypeAndGrade(nType, m_nGrade);
	m_nCreatePrice = m_pDetail->vecCreatePrice[m_nGrade - 1];
	m_nSellPrice = m_pDetail->vecSellPrice[m_nGrade - 1];
	CGameLayer::getInstance()->setBuyGold(this->getPrice());
	
	
	//创建武器动画
	Animation *pAnimation = Animation::create();
	for (int i=1;i<m_vecWeaponImgDt.size()-1;++i)
	{
		SpriteFrame* pSprite = SpriteFrameCache::getInstance()->getSpriteFrameByName(m_vecWeaponImgDt[i]);
		pAnimation->addSpriteFrame(pSprite);
	}
	SpriteFrame* pSprite = SpriteFrameCache::getInstance()->getSpriteFrameByName(m_vecWeaponImgDt[0]);
	pAnimation->addSpriteFrame(pSprite);
	pAnimation->setDelayPerUnit(m_fShootSpeed/10);
	m_pShootAnimate = Animate::create(pAnimation);
	m_pShootAnimate->retain();
	
	if (!Sprite::initWithSpriteFrameName(m_vecWeaponImgDt[0]))
	{
		return false;
	}
	
	m_pTray->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(m_vecWeaponImgDt.back()));
	m_pTray->setPosition(Vec2(this->getContentSize().width/2,this->getContentSize().height/2));

	Biaoji->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height+10));
	this->addChild(m_pTray,-1,"tray");
	m_pTray->addChild(Biaoji,1,"biaoji");
	Biaoji->setVisible(false);
	this->schedule(CC_CALLBACK_0(CWeapon::shoot, this), m_fShootSpeed, "shoot");
	this->scheduleUpdate();
	return true;
}

Node * CWeapon::findMonster()
{
	Vector<Node*> vecNode = CGameLayer::getInstance()->getMonsterMgr()->getChildren();
	for (Node *pNode:vecNode)
	{
		if (this->m_bSelect)
		{
			continue;
		}
		if (this->getPosition().distance(pNode->getPosition())<=nAttackRange+20)
		{
			return pNode;
		}
	}
	return nullptr;
}

void CWeapon::update(float fDelta)
{
	if (nullptr==findMonster())
	{
		return;
	}
	
	Vec2 startPos = this->getPosition();
	Vec2 endPos = this->findMonster()->getPosition();
	Vec2 pVec = endPos - startPos;
	float fRotateRadians = pVec.getAngle();
	float fRotateDegrees = CC_RADIANS_TO_DEGREES(fRotateRadians);
	//大便不能旋转（底座也不能动）
	if (m_nWeaponType!=E_TOWER_TSHIT&&m_nWeaponType!=E_TOWER_TLEAF
		&& m_nWeaponType != E_TOWER_TREDSTART && m_nWeaponType != E_TOWER_TTHUNDER)
	{
		this->setRotation(-(fRotateDegrees-90));
		m_pTray->setRotation((fRotateDegrees - 90));
	}
}

void CWeapon::shoot()
{
	if (this->findMonster()!=nullptr||m_bSelect)
	{
		float offset = 0;
		if (this->m_nWeaponType==E_TOWER_TTHUNDER)
		{
			offset = 20;
		}
		this->runAction(m_pShootAnimate);
		SimpleAudioEngine::getInstance()->playEffect(m_sMusic.c_str(), false);

		Vec2 endPos;
		if (m_bSelect)
		{
			endPos = m_SelectObstaclePos;
		}
		else
		{
			endPos = this->findMonster()->getPosition();
		}
		
		CGameLayer::getInstance()->getBulletMgr()->addBullet(m_nWeaponType, this->getGrade(),
			Vec2(this->getPositionX(),this->getPositionY()+offset), endPos);
	}
	
}

void CWeapon::upGrade()
{
	//
	if (m_nGrade<3)
	{
		m_nGrade++;
		m_pTray->removeFromParent();
		//调用初始化函数，达到升级的目的
		this->initWithData(m_nWeaponType);
		//播放动画和音效
		CGameLayer::getInstance()->getAnimateMgr()->playAnimate(this->getPosition(), E_ANIMATE_UPGRADE);
		SimpleAudioEngine::getInstance()->playEffect("Music/Items/TowerUpdata.ogg");
	}	
}
