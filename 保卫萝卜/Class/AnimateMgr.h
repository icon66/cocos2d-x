#pragma once
#include "cocos2d.h"
#include <string>
using namespace std;
USING_NS_CC;

enum
{
	E_ANIMATE_MONSTERDEATH, //怪物死亡
	E_ANIMATE_CREATEMONSTER,//产生怪物动画
	E_ANIMATE_UPGRADE, //升级
	E_ANIMATE_BIOAJI, //障碍物标记
	E_ANIMATE_GAMESTART, //游戏开始
	E_ANIMATE_EATCARROT, //萝卜被吃
	E_ANIMATE_CARROTSHAKE, //萝卜颤抖
	E_ANIMATE_WUQIBIAOJI, //武器可升级标记
	E_ANIMATE_SHIT, 
};
class CAnimateMgr:public Node
{
public:
	CAnimateMgr();
	~CAnimateMgr();
	bool init();
	CREATE_FUNC(CAnimateMgr);
	Animate *getAnimateByID(const int& nType); //得到动画
	void playAnimate(const Vec2& vPos,const int& nType);//播放动画
	void createAnimate(vector<string> vecSpriteFrame, float fDelayPerUnit, const int & nType);//新建动画

	Animate *getBulletMoveAnimateByTypeAndGrade(const int& nType, const int& nGrade);//得到对应类型和等级的子弹的动画
private:
	vector<Animate*> vecAnimate;//存放一些孤立动画的容器
	map<int, vector<Animate*>> mapBulletMoveAnimate;//子弹移动动画
	map<int, vector<Animate*>> mapBulletBoomAnimate;//子弹爆炸动画
};

