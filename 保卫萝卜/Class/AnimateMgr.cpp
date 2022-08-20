#include "AnimateMgr.h"
#include "GameLayer.h"

CAnimateMgr::CAnimateMgr()
{
	
}

CAnimateMgr::~CAnimateMgr()
{
}


//���������õ���Ӧ����
Animate * CAnimateMgr::getAnimateByID(const int& nType)
{
	for (int i=0;i<vecAnimate.size();++i)
	{
		if (vecAnimate[i]->getTag()==nType)
		{
			return vecAnimate[i];
		}
	}
	return nullptr;
}

Animate * CAnimateMgr::getBulletMoveAnimateByTypeAndGrade(const int & nType, const int & nGrade)
{
	vector<Animate*> vAnimate;
	vAnimate=mapBulletMoveAnimate[nType];
	for (int i = 0; i < vAnimate.size(); ++i)
	{
		if (vAnimate[nGrade]!=nullptr)
		{
			return vAnimate[nGrade];
		}
	}
	return nullptr;
}



bool CAnimateMgr::init()
{
	if (!Node::init())
	{
		return false;
	}

	char szName[32] = {};
	vector<string> vSpriteFrame;//��ž���֡ͼƬ��

	//�������ʱ����
	vSpriteFrame.clear();
	for (int i = 1; i <= 2; ++i)
	{
		sprintf_s(szName, "mcm0%d.png", i);
		vSpriteFrame.push_back(szName);
	}
	this->createAnimate(vSpriteFrame, 0.2, E_ANIMATE_CREATEMONSTER);

	//����������ը���ƶ���
	vSpriteFrame.clear();
	for (int i = 1; i <= 5; ++i)
	{
		sprintf_s(szName, "air0%d.png",i);
		vSpriteFrame.push_back(szName);
	}
	this->createAnimate(vSpriteFrame, 0.1, E_ANIMATE_MONSTERDEATH);

	//��������
	vSpriteFrame.clear();
	for (int i = 2; i <= 5; ++i)
	{
		sprintf_s(szName, "air3%d.png", i);
		vSpriteFrame.push_back(szName);
	}
	vSpriteFrame.push_back("air31.png");
	this->createAnimate(vSpriteFrame, 0.07, E_ANIMATE_UPGRADE);

	//�ϰ��ﱻ��Ƕ���
	vSpriteFrame.clear();
	Animation *pAnimationBiaoji = Animation::create();
	for (int i = 2; i <= 3; ++i)
	{
		sprintf_s(szName, "point0%d.png", i);
		vSpriteFrame.push_back(szName);
	}
	vSpriteFrame.push_back("point01.png");
	this->createAnimate(vSpriteFrame, 0.07, E_ANIMATE_BIOAJI);

	//��Ϸ��ʼ����
	vSpriteFrame.clear();
	for (int i = 1; i <= 3; ++i)
	{
		sprintf_s(szName, "countdown_0%d.png", i);
		vSpriteFrame.push_back(szName);
	}
	this->createAnimate(vSpriteFrame, 1, E_ANIMATE_GAMESTART);

	//�ܲ����Զ���
	vSpriteFrame.clear();
	for (int i = 1; i <= 5; ++i)
	{
		sprintf_s(szName, "air1%d.png", i);
		vSpriteFrame.push_back(szName);
	}
	this->createAnimate(vSpriteFrame,0.1,E_ANIMATE_EATCARROT);

	//�ܲ���������
	vSpriteFrame.clear();
	for (int i = 0; i <= 8; ++i)
	{
		sprintf_s(szName, "hlb1%d.png", i);
		vSpriteFrame.push_back(szName);
	}
	vSpriteFrame.push_back("hlb21.png");
	this->createAnimate(vSpriteFrame, 0.05, E_ANIMATE_CARROTSHAKE);

	//�����������
	vSpriteFrame.clear();
	Animation *pAnimationBiaoji1 = Animation::create();
	for (int i = 1; i <= 2; ++i)
	{
		sprintf_s(szName, "showupgrade0%d.png", i);
		vSpriteFrame.push_back(szName);
	}
	vSpriteFrame.push_back("showupgrade01.png");
	this->createAnimate(vSpriteFrame, 0.07, E_ANIMATE_WUQIBIAOJI);

	//ʺ���ٶ���
	vSpriteFrame.clear();
	for (int i = 1; i <= 2; ++i)
	{
		sprintf_s(szName, "PShit-1%d.png", i);
		vSpriteFrame.push_back(szName);
	}
	this->createAnimate(vSpriteFrame, 0.1, E_ANIMATE_SHIT);


	return true;
}


//���Ŷ����ӿ�
void CAnimateMgr::playAnimate(const Vec2& vPos, const int& nType)
{
	//��ָ��λ�ò���ָ���Ķ���
	Sprite *pSprite = Sprite::create();
	pSprite->setPosition(vPos);
	CGameLayer::getInstance()->addChild(pSprite,1);
	Sequence *pSeq = Sequence::create(getAnimateByID(nType),RemoveSelf::create(),nullptr);
	pSprite->runAction(pSeq);
}

//��Ӷ���
void CAnimateMgr::createAnimate(vector<string> vecSpriteFrame, float fDelayPerUnit, const int & nType)
{
	//ÿ���������������ñ�ǩ�ŵ�����������
	Animation *pAnimation = Animation::create();
	for (int i = 0; i < vecSpriteFrame.size(); ++i)
	{
		SpriteFrame *pSpriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(vecSpriteFrame[i]);
		pAnimation->addSpriteFrame(pSpriteFrame);
	}
	pAnimation->setDelayPerUnit(fDelayPerUnit);
	Animate *m_pAnimate = Animate::create(pAnimation);
	m_pAnimate->retain();
	m_pAnimate->setTag(nType);
	vecAnimate.push_back(m_pAnimate);
	
}
