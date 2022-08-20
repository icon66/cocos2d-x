#pragma once
#include "cocos2d.h"
#include "DataMgr.h"
#include <vector>
using namespace std;

USING_NS_CC;

class CBullet:public Sprite
{
public:
	CBullet();
	~CBullet();
	void move(const Vec2& vEndPos);
	static CBullet *createWithData(const int& nType,const int& nAck,const Vec2& vStartPos);
	bool initWithData(const int& nType,const int& nAck,const Vec2& vStartPos);
	void setType(int nType)
	{
		m_nType = nType;
	}
	int getType()
	{
		return m_nType;
	}
	Animate *getAnimate()
	{
		return m_pBoomAnimate;
	}
	int getAck()
	{
		return m_nAck;
	}
	int getSpeedBuff()
	{
		return m_nReduceSpBuff;
	}
	bool addMonsterTag(const int &nTag);
	bool addObstacleTag(const int &nTag);
private:
	vector<string> vecBulletImg;
	vector<string> vecBulletBoomImg;
	vector<int> vecMonsterTag;//怪物标签
	vector<int> vecObstacleTag;//障碍物标签
	sBulletDt *m_pBulletDetailDt; //子弹详细数据
	Animate *m_pMoveAnimate;//移动动画
	Animate *m_pBoomAnimate;//碰撞爆炸动画
	int m_nAck;//子弹攻击力
	int m_nType;//子弹类型（和武器类型一致）
	int m_nReduceSpBuff;
	bool isRotate;//子弹是否旋转
};

