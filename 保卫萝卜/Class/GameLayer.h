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
	void btnCallBack(Ref *pSender); //��ť�ص�
	Vec2 getTiledByPos(const Vec2& pos); //�õ���������
	Vec2 getCenterPosByTiled(const Vec2& tiled); //��ø�����������
	vector<Vec2> getPathPoint(); //��ù�������·���յ�
	vector<Vec2> getObstaclePoint(); //����ϰ���λ����Ϣ
	void setBuyGold(int nPrice); //�������������������
	void setAddGold(int nPrice);
	void judgeCanUpdate();
	void setCurWaveString(); //���µ�ǰ����
	void startCreateMonster();
	void showWinMenu();//��ʾʤ������
	int getGold();
	void BufferTimeOver(CMonster* pMos);
	void setCarrotHp();
	void setMenuString(); //���ò˵��ı�ǩ����
	void playMusic(); //����ʱ����
	void setCloseTouch(bool isClose);//�Ƿ�رմ���
	void startend();
	void getBlock(); //�õ��յ�
	void setAllObstacleClear(bool isClear);
	void startdisappear();
	void updateGoldSting();//���½����ʾ
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
	TMXTiledMap * m_pTiledMap; //���ӵ�ͼ(TMX)
	TMXLayer * m_pPathLayer; //·����
	vector<Vec2> m_VecPathPoint; //��������·���ڵ�����
	vector<Vec2> m_VecObstacle; //�ϰ���
	vector<Vec2> m_VecpBlock;  //�յصĵ㣨�ɽ�����
	CMonterMgr *m_pMonsterMgr; //���������
	CObstacleMgr *m_pObstacleMgr; //�ϰ��������
	CCardMgr *m_pCarMgr; //��Ƭ������
	CWeaponMgr *m_pWeaponMgr; //����������
	CBulletMgr *m_pBulletMgr;//�ӵ�������
	CAnimateMgr *m_pAnimateMgr;//����������
	Sprite *m_pCarrot; //�ܲ�����
	Sprite *m_pStartPoint; //��ʼ�����
	Sprite *pWaveBg; 
	Sprite *pPauseBg;
	MenuItemImage *m_pSpeed;
	MenuItemImage *m_pPause;
	MenuItemImage *m_pMenu;
	MenuItemImage *m_pContinue;
	MenuItemImage *m_pRestart;     //�˵���ť
	MenuItemImage *m_pSelect;
	MenuItemImage *m_pOverSelect;
	MenuItemImage *m_pOverRestart;
	MenuItemImage *m_pPassRestart;
	MenuItemImage *m_pPassContinue;
	LayerColor *m_pMenuLayer; //�˵���
	LayerColor *m_pGameOverLayer; //��Ϸ����
	LayerColor *m_pPassLayer; //����
	Label *m_pTotalWave;
	Label *m_pCurWave;
	Label *pJituiWave; //�����˼���
	Label *m_pTotal;
	Label *pCurLevel;
	Node *m_pCurMonster;
	Label *GoldLabel;
	Sprite *pStartSprite; //��ͷ������������
	Sprite *m_pCarrotHpSprite;
	bool stopCreateMonster;
	bool m_bIsCloseTouch;
	bool m_bIsSpeeed; //�Ƿ����
	bool m_bIsPause; //�Ƿ���ͣ
	bool m_bIsOpenMenu; //�Ƿ�򿪲˵�
	bool m_bIsPass;
	bool m_bIsClearAllObstacle; //�Ƿ������ϰ��ﶼ�����
	int m_nGold; //���
	int m_nCarrotHp; //�ܲ�Ѫ��
	int m_nAnimateTimeCount; 
	int m_nTotalWave; //�ܲ���
	int m_nCurWave; //��ǰ����
	int m_nTimeCount; //��ʱ�����ڣ�;
};

