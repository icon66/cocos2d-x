#pragma once
#include "cocos2d.h"
#include "DataMgr.h"
USING_NS_CC;

class CWeapon:public Sprite
{
public:
	CWeapon();
	~CWeapon();
	static CWeapon *createWithData(const int &nType);
	bool initWithData(const int &nType);
	Node *findMonster();
	void update(float fDelta);
	void shoot();
	int getAttackRange()
	{
		return nAttackRange;
	}
	int getGrade()
	{
		return m_nGrade;
	}
	int getWeaponType()
	{
		return m_nWeaponType;
	}
	void setState(const Vec2& vPos)
	{
		m_bSelect = true;
		m_SelectObstaclePos = vPos;
	
	}
	void resetState()
	{
		m_bSelect = false;
	}
	int getPrice()
	{
		return m_nCreatePrice;
	}
	int getUpgradePrice()
	{
		if (m_nGrade<3)
		{
			return m_pDetail->vecCreatePrice[m_nGrade];
		}
	}
	int getSellPrice()
	{
		return m_nSellPrice;
	}
	void upGrade();//������������
	CREATE_FUNC(CWeapon);
private:
	vector<string> m_vecWeaponImgDt;//������ͼƬ����
	sWeaponDt * m_pDetail;//��������
	Vec2 m_SelectObstaclePos;//������ϰ�������
	Sprite * m_pTray;//��������
	Animate *m_pShootAnimate;//�������
	int m_nWeaponType;//��������
	int nAttackRange;//������Χ
	float m_fShootSpeed;
	int m_nGrade;//�ȼ�
	int m_nCreatePrice;//����۸�
	int m_nUpgradePrice;//�����۸�(��һ���ȼ���������۸��ⲿʹ��)
	int m_nSellPrice;//���ۼ۸�
	bool m_bSelect;//�Ƿ�ѡ��
	string m_sMusic;
};
