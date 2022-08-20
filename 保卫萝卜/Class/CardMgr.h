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
	vector<CCard*> vecCard; //������Ƭ
	CCard *m_pCardRec;
	CCard *m_pForbid;
	CCard *m_pAttcRangeCircle;//������ΧȦ;
	CCard *m_pUpgrade;//����
	CCard *m_pSell; //����
	CCard *m_pBiaoji;
	CWeapon *m_pCurWeapon;//��ǰ���е�����
	SpriteFrameCache *pCache;
	CObstacle *m_pCurObstacle;//ѡ�е��ϰ���
	Vector<CWeapon*> vecRangeWeapon;
	Vec2 m_CenterPos;
	Sequence *pSeq; //��ʾ��Ƭ��ʾ��ʧ�����У�
	bool bIsCanCreate;//�Ƿ��ܹ�����
	bool bIsBiaoji;
	bool hasFind;
};

