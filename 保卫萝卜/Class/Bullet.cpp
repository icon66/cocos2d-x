#include "Bullet.h"
#include "Weapon.h"
#include "Card.h"
#include "GameLayer.h"

CBullet::CBullet()
:m_nAck(0)
{
	isRotate = false;
}

CBullet::~CBullet()
{
}

void CBullet::move(const Vec2& vEndPos)
{
	MoveTo *pMoveTo;
	MoveBy *pMoveby;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//（群攻子弹）树叶……
	if (this->m_nType==E_TOWER_TLEAF)
	{
		float fRatio = 0;
		Vec2 detailPos = this->getPosition() - vEndPos;
		if (abs(detailPos.x) > abs(detailPos.y))  //<45
		{
			fRatio = abs(detailPos.x) / abs(visibleSize.width);
		}
		else if (abs(detailPos.y) > abs(detailPos.x)) //>45
		{
			fRatio = abs(detailPos.y) / abs(visibleSize.height);
		}
		float fDistance = (detailPos/fRatio).getLength();
		pMoveby = MoveBy::create(fDistance/300, -Vec2(detailPos.x / (fRatio), detailPos.y / (fRatio)));
		
		Sequence *pSequ1 = Sequence::create(pMoveby, RemoveSelf::create(), nullptr);
		this->runAction(RepeatForever::create(RotateBy::create(0.5, 360)));//旋转
		this->runAction(pSequ1);
	
	}
	//（射线型子弹）闪电炮塔
	else if (this->m_nType == E_TOWER_TTHUNDER)
	{
		Vec2 detailPos = this->getPosition() - vEndPos;
		Sprite *pSprite = Sprite::createWithSpriteFrameName("PBall11.png");
		float flength = pSprite->getContentSize().width;//获得子弹的长度
		float fDistance = detailPos.length();
		float fRatio = flength / fDistance;
		this->setGlobalZOrder(102);
		this->setAnchorPoint(Vec2(0.5, 0));
		this->setScaleY(1.2-fRatio);
		this->setScaleX(0.5);
		Sequence *pSeq = Sequence::createWithTwoActions(DelayTime::create(0.2),RemoveSelf::create());
		this->runAction(pSeq);
	}
	//（单攻子弹）
	else
	{
		pMoveTo = MoveTo::create(0.2, vEndPos);
		if (this->m_nType==E_TOWER_TREDSTART)
		{
			this->runAction(RepeatForever::create(RotateBy::create(0.5, 360)));//旋转
			this->setGlobalZOrder(102);
		}
		Sequence *pSequ = Sequence::create(pMoveTo, RemoveSelf::create(),nullptr);
		this->runAction(pSequ);
	}

   	this->runAction(RepeatForever::create(m_pMoveAnimate));	
}

CBullet * CBullet::createWithData(const int & nType,const int& nGrade, const Vec2& vStartPos)
{
	CBullet *pBullet = new CBullet;
	if (pBullet&&pBullet->initWithData(nType,nGrade,vStartPos))
	{
		pBullet->autorelease();
		return pBullet;
	}
	CC_SAFE_DELETE(pBullet);
	return nullptr;
}

bool CBullet::initWithData(const int& nType,const int& nGrade, const Vec2& vStartPos)
{
	vecBulletImg = CDataMgr::getInstance()->getBulletDtMgr()->getBulletMoveImgByIdAndGrade(nType,nGrade);
	vecBulletBoomImg = CDataMgr::getInstance()->getBulletDtMgr()->getBulletBoomImgByID(nType);
	m_pBulletDetailDt = CDataMgr::getInstance()->getBulletDtMgr()->getBulletDetailDtById(nType);
	m_nType = nType;
	m_nReduceSpBuff = m_pBulletDetailDt->fSpeedBuff;
	m_nAck = m_pBulletDetailDt->nAck;

	
	/*Animate *pAnimate=CGameLayer::getInstance()->getAnimateMgr()->getBulletMoveAnimateByTypeAndGrade(nType,nGrade);
	if (pAnimate!=nullptr)
	{
		m_pMoveAnimate = pAnimate;
	}*/

	//创建子弹运动动画
	Animation *pAnimation = Animation::create();
	for (int i = 0; i < vecBulletImg.size(); ++i)
	{
		SpriteFrame* pSprite = SpriteFrameCache::getInstance()->getSpriteFrameByName(vecBulletImg[i]);
		pAnimation->addSpriteFrame(pSprite);
	}
	pAnimation->setDelayPerUnit(0.02);
	m_pMoveAnimate = Animate::create(pAnimation);
	m_pMoveAnimate->retain();

	//创建子弹爆炸动画
	Animation *pAnimationBoom = Animation::create();
	for (int i=0;i<vecBulletBoomImg.size();++i)
	{
		SpriteFrame* pSprite = SpriteFrameCache::getInstance()->getSpriteFrameByName(vecBulletBoomImg[i]);
		pAnimationBoom->addSpriteFrame(pSprite);
	}
	pAnimationBoom->setDelayPerUnit(0.06);
	m_pBoomAnimate = Animate::create(pAnimationBoom);
	m_pBoomAnimate->retain();

	if (!Sprite::initWithSpriteFrameName(vecBulletImg[0]))
	{
		return false;
	}

	return true;
}

bool CBullet::addMonsterTag(const int &nTag)
{
	for (int i=0;i<vecMonsterTag.size();++i)
	{
		if (vecMonsterTag[i]==nTag)
		{
			return false;
		}
	}

	vecMonsterTag.push_back(nTag);
	return true;		
}

bool CBullet::addObstacleTag(const int &nTag)
{
	for (int i = 0; i < vecObstacleTag.size(); ++i)
	{
		if (vecObstacleTag[i] == nTag)
		{
			return false;
		}
	}

	vecObstacleTag.push_back(nTag);
	return true;
}


