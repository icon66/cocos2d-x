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
	//�ĸ����Էֱ��������ӵ��������ͣ��������ӵ����ĵȼ����ӵ���ʼλ�ã��ӵ�����λ��
	void addBullet(const int& nType,const int& nGrade,const Vec2& vStartPos, const Vec2& vEndPos);
	CREATE_FUNC(CBulletMgr);
private:

};
