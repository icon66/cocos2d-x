#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <vector>
#include "DataMgr.h"

using namespace cocos2d::ui;
using namespace  std;


USING_NS_CC;

class CMonster:public Sprite
{
public:
	CMonster();
	~CMonster();
	void move();
	void update(float fDelta);
	void setHp(int nAck);
	void setSpeed(int nReduce)
	{
		fSpeed -= nReduce;
	}
	void addSpeed(int nAdd)
	{
		fSpeed += nAdd;
	}
	int getType()
	{
		return m_nType;
	}
	bool getReduceSpeed()
	{
		return bIsReduceSpeed;
	}
	void setHasReduceSpeed(bool bHasReduce)
	{
		bIsReduceSpeed = bHasReduce;
	}
	static CMonster* createWithData(const int &nType);
	bool initWithData(const int &nType);
	CREATE_FUNC(CMonster);
private:
	vector<Vec2> m_vecPathPoint; //移动路径的点
	sMonsterDt *m_pMonsterDetailDt;//怪物全部的数据
	vector<string> vecMonsterMoveImg;
	Vec2 m_pNextPoint;
	ProgressTimer *m_pHpBar;
	Sprite *m_pHpBg;
	Animate *m_pAnimateMove;
	string m_sdeathMuic;
	float fSpeed;
	bool bIsNewMonster;
	bool bIsReduceSpeed;
	int nIndex; //拐点索引
	int m_nCurHp;
	int m_nTotalHp;
	int m_nType;
	int m_nReward;
};

