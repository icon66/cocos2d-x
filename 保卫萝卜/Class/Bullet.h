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
	vector<int> vecMonsterTag;//�����ǩ
	vector<int> vecObstacleTag;//�ϰ����ǩ
	sBulletDt *m_pBulletDetailDt; //�ӵ���ϸ����
	Animate *m_pMoveAnimate;//�ƶ�����
	Animate *m_pBoomAnimate;//��ײ��ը����
	int m_nAck;//�ӵ�������
	int m_nType;//�ӵ����ͣ�����������һ�£�
	int m_nReduceSpBuff;
	bool isRotate;//�ӵ��Ƿ���ת
};

