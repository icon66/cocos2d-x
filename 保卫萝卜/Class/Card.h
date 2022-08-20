#pragma once

#include "cocos2d.h"

enum 
{
	E_TISHI_REC,
	E_TISHI_BIAOJI,
	E_TISHI_FORBID,
	E_TISHI_UPGRADE,
	E_TISHI_UPGRADECH,
	E_TISHI_REMOVE,
	E_TOWER_TBOTTLE=1001,
	E_TOWER_TSHIT=1002,
	E_TOWER_TLEAF=1003,
	E_TOWER_TTHUNDER,
	E_TOWER_TREDSTART,
	E_TOWER_TFIRE,
};

USING_NS_CC;

class CCard:public Sprite
{
public:
	CCard();
	~CCard();
	static CCard* createWithData(const int &nType);
	bool initWithData(const int &nType);
	void setType(const int& nType)
	{
		m_nType = nType;
	}
	int getnType()
	{
		return m_nType;
	}
	CREATE_FUNC(CCard);
private:
	int m_nType;
};
