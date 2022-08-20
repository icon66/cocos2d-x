#include "BulletMgr.h"
#include "Card.h"

CBulletMgr::CBulletMgr()
{
}

CBulletMgr::~CBulletMgr()
{
}

bool CBulletMgr::init()
{
	if (!Layer::init())
	{
		return false;
	}
	return true;
}

void CBulletMgr::addBullet(const int & nType,const int& nGrade, const Vec2& vStartPos, const Vec2& vEndPos)
{
	
	CBullet* pBullet = CBullet::createWithData(nType, nGrade,vStartPos);
	//pBullet->setType(nType);

	//×Óµ¯µÄÐý×ª
	Vec2 pVec = vStartPos - vEndPos;
	float fAngle = pVec.getAngle();
	float fDegress = CC_RADIANS_TO_DEGREES(fAngle);
	pBullet->setRotation(-90 - fDegress);
	pBullet->setPosition(vStartPos);
	pBullet->move(vEndPos);
	this-> addChild(pBullet);
	
}

