#pragma once
#include "cocos2d.h"
#include "Card.h"
#include "WeaponMgr.h"
#include "DataMgr.h"
#include "Obstacle.h"

USING_NS_CC;

class CCardMgr:public Node
{
public:
	CCardMgr();
	~CCardMgr();
	void clickEvent(Vec2 &clickPos,int nGid);
	void disappear();
	void upgrateCardShow();
	void cardUpdate();
	vector<CCard*> getvecCard()
	{
		return vecCard;
	}
	bool getCanCreate() {
		return bIsCanCreate;
	}
	CObstacle *getCurObstacle()
	{
		return m_pCurObstacle;
	}
	bool init();
	CREATE_FUNC(CCardMgr);
private:
	vector<int> vecWeaponCardDt;
	vector<CCard*> vecCard; //武器卡片
	CCard *m_pCardRec;
	CCard *m_pForbid;
	CCard *m_pAttcRangeCircle;//攻击范围圈;
	CCard *m_pUpgrade;//升级
	CCard *m_pSell; //出售
	CCard *m_pBiaoji;
	CWeapon *m_pCurWeapon;//当前点中的武器
	SpriteFrameCache *pCache;
	CObstacle *m_pCurObstacle;//选中的障碍物
	Vector<CWeapon*> vecRangeWeapon;
	Vec2 m_CenterPos;
	Sequence *pSeq; //提示卡片显示消失（序列）
	bool bIsCanCreate;//是否能够建塔
	bool bIsBiaoji;
	bool hasFind;
};

