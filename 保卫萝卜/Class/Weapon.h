#pragma once
#include "cocos2d.h"
#include "DataMgr.h"
USING_NS_CC;

class CWeapon:public Sprite
{
public:
	CWeapon();
	~CWeapon();
	static CWeapon *createWithData(const int &nType);
	bool initWithData(const int &nType);
	Node *findMonster();
	void update(float fDelta);
	void shoot();
	int getAttackRange()
	{
		return nAttackRange;
	}
	int getGrade()
	{
		return m_nGrade;
	}
	int getWeaponType()
	{
		return m_nWeaponType;
	}
	void setState(const Vec2& vPos)
	{
		m_bSelect = true;
		m_SelectObstaclePos = vPos;
	
	}
	void resetState()
	{
		m_bSelect = false;
	}
	int getPrice()
	{
		return m_nCreatePrice;
	}
	int getUpgradePrice()
	{
		if (m_nGrade<3)
		{
			return m_pDetail->vecCreatePrice[m_nGrade];
		}
	}
	int getSellPrice()
	{
		return m_nSellPrice;
	}
	void upGrade();//武器升级处理
	CREATE_FUNC(CWeapon);
private:
	vector<string> m_vecWeaponImgDt;//武器的图片数据
	sWeaponDt * m_pDetail;//具体数据
	Vec2 m_SelectObstaclePos;//被标记障碍物坐标
	Sprite * m_pTray;//武器底座
	Animate *m_pShootAnimate;//射击动画
	int m_nWeaponType;//武器类型
	int nAttackRange;//攻击范围
	float m_fShootSpeed;
	int m_nGrade;//等级
	int m_nCreatePrice;//建造价格
	int m_nUpgradePrice;//升级价格(下一个等级建造所需价格，外部使用)
	int m_nSellPrice;//出售价格
	bool m_bSelect;//是否被选中
	string m_sMusic;
};
