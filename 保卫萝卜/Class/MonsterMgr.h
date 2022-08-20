#pragma once
#include "cocos2d.h"
#include "Monster.h"
USING_NS_CC;

class CMonterMgr:public Layer
{
public:
	CMonterMgr();
	~CMonterMgr();
	bool init();
	CREATE_FUNC(CMonterMgr);
	void addMonster();
	void waitForNextWave();
	void stopSchedule();
	void resumeSchedule();
	void setbBegin()
	{
		bBegin = true;
	}
	int getCurWave()
	{
		return m_nCurWave;
	}
	void update(float fDelta);
private:
	int m_nTagCount;
	int m_nMonsterCount;
	int m_nCurWave;
	int m_nCurWaveMosCount;

	bool bBegin;
	vector<int> vecCurLevelMonster;
	vector<int> vecCurLevelWaveMonsterCount;
};
