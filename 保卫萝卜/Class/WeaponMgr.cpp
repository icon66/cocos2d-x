#include "WeaponMgr.h"
#include "GameLayer.h"

CWeaponMgr::CWeaponMgr()
{
}

CWeaponMgr::~CWeaponMgr()
{
}

void CWeaponMgr::addWeapon(const int& nType,Vec2 vPos)
{
	CWeapon *pWeapon = CWeapon::createWithData(nType);
	pWeapon->setPosition(vPos);
	this->addChild(pWeapon);
	CGameLayer::getInstance()->judgeCanUpdate();
}

bool CWeaponMgr::init()
{
	if (!Node::init())
	{
		return false;
	}
	return true;
}


