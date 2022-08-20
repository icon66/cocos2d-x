#pragma once
#include "cocos2d.h"
#include "Obstacle.h"
#include "DataMgr.h"
#include <vector>
using namespace std;
USING_NS_CC;

class CObstacleMgr:public Layer
{
public:
	CObstacleMgr();
	~CObstacleMgr();
	static CObstacleMgr* createWithData(vector<Vec2> vecPoint);
	bool initWithData(vector<Vec2> vecPoint);
private:
	vector<Vec2> m_VecObstacle;
	vector<string> m_vecLevelObject;
};



