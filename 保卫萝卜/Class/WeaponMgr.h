#pragma once
#include "cocos2d.h"
#include "Weapon.h"

USING_NS_CC;

class CWeaponMgr :public Node
{
public:
	CWeaponMgr();
	~CWeaponMgr();
	void addWeapon(const int& nType,Vec2 vPos);
	bool init();
	CREATE_FUNC(CWeaponMgr);
private:

};

