#pragma once
#include <string>
#include "cocos2d.h"
#include "DataMgr.h"

using namespace std;
USING_NS_CC;

class CObstacle:public Sprite
{
public:
	CObstacle();
	~CObstacle();
	bool initWithData(Vec2& vPos,string ntype);
	static CObstacle* createWithData(Vec2& vPos,string ntype);
	void update(float fDelta);
	Sprite *getBiaoji();
	void setHp(int nAck);
	void showThenDisappear();
	void checkAllClear();
	void show();
	bool init();
	void disappear();
	void stopAction();
	int getReward()
	{
		return m_nReward;
	}
	void setReward(int nNum)
	{
		m_nReward = nNum;
	}
	CREATE_FUNC(CObstacle);
private:
	sObstacleDt * pData;
	Sprite * m_pBiaoji;
	ProgressTimer * m_pHpBar;
	Sprite *pHpBg;
	int m_nCurHp;
	int m_nTotalHp;
	int m_nReward;
};
