#pragma once
#include "cocos2d.h"
#include "Bullet.h"

USING_NS_CC;

class CBulletMgr:public Layer
{
public:
	CBulletMgr();
	~CBulletMgr();
	bool init();
	//四个属性分别武器（子弹）的类型，武器（子弹）的等级，子弹初始位置，子弹最终位置
	void addBullet(const int& nType,const int& nGrade,const Vec2& vStartPos, const Vec2& vEndPos);
	CREATE_FUNC(CBulletMgr);
private:

};
