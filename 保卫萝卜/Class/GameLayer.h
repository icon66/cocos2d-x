#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <vector>
#include "MonsterMgr.h"
#include "ObstacleMgr.h"
#include "CardMgr.h"
#include "WeaponMgr.h"
#include "AnimateMgr.h"
#include "BulletMgr.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
using namespace cocos2d::ui;
using namespace std;

USING_NS_CC;

class CGameLayer:public Layer
{
public:
	CGameLayer();
	~CGameLayer();
	static Scene * createScene();
	virtual bool init();
	void addTouch();
	void btnCallBack(Ref *pSender); //按钮回调
	Vec2 getTiledByPos(const Vec2& pos); //得到格子坐标
	Vec2 getCenterPosByTiled(const Vec2& tiled); //获得格子中心坐标
	vector<Vec2> getPathPoint(); //获得怪物所走路径拐点
	vector<Vec2> getObstaclePoint(); //获得障碍物位置信息
	void setBuyGold(int nPrice); //买完武器金币数量减少
	void setAddGold(int nPrice);
	void judgeCanUpdate();
	void setCurWaveString(); //更新当前波数
	void startCreateMonster();
	void showWinMenu();//显示胜利界面
	int getGold();
	void BufferTimeOver(CMonster* pMos);
	void setCarrotHp();
	void setMenuString(); //设置菜单的标签内容
	void playMusic(); //倒计时音乐
	void setCloseTouch(bool isClose);//是否关闭触摸
	void startend();
	void getBlock(); //得到空地
	void setAllObstacleClear(bool isClear);
	void startdisappear();
	void updateGoldSting();//更新金币显示
	void playBoom(const Vec2& vPos, Animate* pAnimate);
	void update(float fDelta);
	void playStartAnimate();
	void collision();
	static CGameLayer *getInstance();
	void destoryInstance();
	CWeaponMgr *getWeaponMgr()
	{
		return m_pWeaponMgr;
	}
	CCardMgr *getCardMgr()
	{
		return m_pCarMgr;
	}
	CMonterMgr *getMonsterMgr()
	{
		return m_pMonsterMgr;
	}
	CBulletMgr * CGameLayer::getBulletMgr()
	{
		return m_pBulletMgr;
	}
	CAnimateMgr *getAnimateMgr()
	{
		return m_pAnimateMgr;
	}
	CObstacleMgr *getObstacleMgr()
	{
		return m_pObstacleMgr;
	}
	CREATE_FUNC(CGameLayer);
private:
	static CGameLayer *m_pInstance;
	TMXTiledMap * m_pTiledMap; //格子地图(TMX)
	TMXLayer * m_pPathLayer; //路径层
	vector<Vec2> m_VecPathPoint; //怪物行走路径节点坐标
	vector<Vec2> m_VecObstacle; //障碍物
	vector<Vec2> m_VecpBlock;  //空地的点（可建塔）
	CMonterMgr *m_pMonsterMgr; //怪物管理者
	CObstacleMgr *m_pObstacleMgr; //障碍物管理者
	CCardMgr *m_pCarMgr; //卡片管理者
	CWeaponMgr *m_pWeaponMgr; //武器管理者
	CBulletMgr *m_pBulletMgr;//子弹管理者
	CAnimateMgr *m_pAnimateMgr;//动画管理者
	Sprite *m_pCarrot; //萝卜精灵
	Sprite *m_pStartPoint; //起始点的牌
	Sprite *pWaveBg; 
	Sprite *pPauseBg;
	MenuItemImage *m_pSpeed;
	MenuItemImage *m_pPause;
	MenuItemImage *m_pMenu;
	MenuItemImage *m_pContinue;
	MenuItemImage *m_pRestart;     //菜单按钮
	MenuItemImage *m_pSelect;
	MenuItemImage *m_pOverSelect;
	MenuItemImage *m_pOverRestart;
	MenuItemImage *m_pPassRestart;
	MenuItemImage *m_pPassContinue;
	LayerColor *m_pMenuLayer; //菜单层
	LayerColor *m_pGameOverLayer; //游戏结束
	LayerColor *m_pPassLayer; //过关
	Label *m_pTotalWave;
	Label *m_pCurWave;
	Label *pJituiWave; //击退了几波
	Label *m_pTotal;
	Label *pCurLevel;
	Node *m_pCurMonster;
	Label *GoldLabel;
	Sprite *pStartSprite; //箭头（开场动画）
	Sprite *m_pCarrotHpSprite;
	bool stopCreateMonster;
	bool m_bIsCloseTouch;
	bool m_bIsSpeeed; //是否加速
	bool m_bIsPause; //是否暂停
	bool m_bIsOpenMenu; //是否打开菜单
	bool m_bIsPass;
	bool m_bIsClearAllObstacle; //是否所有障碍物都被清除
	int m_nGold; //金币
	int m_nCarrotHp; //萝卜血量
	int m_nAnimateTimeCount; 
	int m_nTotalWave; //总波数
	int m_nCurWave; //当前波数
	int m_nTimeCount; //计时（用于）;
};

