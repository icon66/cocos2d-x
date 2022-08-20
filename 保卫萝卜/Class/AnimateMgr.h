#pragma once
#include "cocos2d.h"
#include <string>
using namespace std;
USING_NS_CC;

enum
{
	E_ANIMATE_MONSTERDEATH, //��������
	E_ANIMATE_CREATEMONSTER,//�������ﶯ��
	E_ANIMATE_UPGRADE, //����
	E_ANIMATE_BIOAJI, //�ϰ�����
	E_ANIMATE_GAMESTART, //��Ϸ��ʼ
	E_ANIMATE_EATCARROT, //�ܲ�����
	E_ANIMATE_CARROTSHAKE, //�ܲ�����
	E_ANIMATE_WUQIBIAOJI, //�������������
	E_ANIMATE_SHIT, 
};
class CAnimateMgr:public Node
{
public:
	CAnimateMgr();
	~CAnimateMgr();
	bool init();
	CREATE_FUNC(CAnimateMgr);
	Animate *getAnimateByID(const int& nType); //�õ�����
	void playAnimate(const Vec2& vPos,const int& nType);//���Ŷ���
	void createAnimate(vector<string> vecSpriteFrame, float fDelayPerUnit, const int & nType);//�½�����

	Animate *getBulletMoveAnimateByTypeAndGrade(const int& nType, const int& nGrade);//�õ���Ӧ���ͺ͵ȼ����ӵ��Ķ���
private:
	vector<Animate*> vecAnimate;//���һЩ��������������
	map<int, vector<Animate*>> mapBulletMoveAnimate;//�ӵ��ƶ�����
	map<int, vector<Animate*>> mapBulletBoomAnimate;//�ӵ���ը����
};

