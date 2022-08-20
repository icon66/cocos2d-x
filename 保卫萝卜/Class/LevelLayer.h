#pragma once
#include "cocos2d.h"
#include "DataMgr.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

using namespace cocos2d::ui;
USING_NS_CC;

class CLevelLayer:public Layer
{
public:
	CLevelLayer();
	~CLevelLayer();
	static Scene *createScene();
	virtual bool init();
	void btnCallBack(Ref *pSender);
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
	bool judgeLock(const int& nLevel);
	void update(float fDelta);
	CREATE_FUNC(CLevelLayer);
private:
	MenuItemImage * m_pBtnBack;
	MenuItemImage * m_pBtnHelp;
	MenuItemImage * m_pBtnStart;
	PageView *pPageView;
	Sprite *m_pWave;  //���������飩
	Vec2 m_StartPos;
	int m_nCurLevelWave;
	vector<int> vecLockLevelIndex;
	vector<int> m_vecLevelWeapon; //��ǰ�ؿ�����������
	vector<string> m_vecLevelPng; //��ǰ�ؿ��ĵ�ͼ
	vector<string> m_vecBg;      //��ǰ���ⱳ��
	vector<Sprite*> m_vecWeapon; //��ǰ�ؿ�������
	float startPos;
};
