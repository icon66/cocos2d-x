#include "MonsterMgr.h"
#include "Monster.h"
#include "GameLayer.h"
#include <time.h>
#include "LoadingLayer.h"

CMonterMgr::CMonterMgr()
{
	m_nTagCount = 0;
	m_nCurWave = 1;
	m_nMonsterCount = 0;
	bBegin = false;
}

CMonterMgr::~CMonterMgr()
{

}

bool CMonterMgr::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//��ǰ�ؿ����ܳ��ֵĹ�������
	vecCurLevelMonster = CDataMgr::getInstance()->getLevelDtMgr()->getCurLevelMonsterType();
	//��ǰ��������������
	vecCurLevelWaveMonsterCount = CDataMgr::getInstance()->getLevelDtMgr()->getCurLevelWaveMonsterCount();
	//��ǰһ���Ĺ�������
	m_nCurWaveMosCount = vecCurLevelWaveMonsterCount[m_nCurWave - 1];
	
	this->schedule(CC_CALLBACK_0(CMonterMgr::addMonster, this), 2.0f, "addMonster");

	this->unscheduleUpdate();
	return true;
}

void CMonterMgr::addMonster()
{
	if (!bBegin)
	{
		return;
	}
	//���Ų������ﶯ��
	CGameLayer::getInstance()->getAnimateMgr()->playAnimate
	(CGameLayer::getInstance()->getPathPoint()[0], E_ANIMATE_CREATEMONSTER);

	//���Ų�����������
	SimpleAudioEngine::getInstance()->playEffect("Music/Items/MC.ogg");

	srand(unsigned(time(0)));
	int monsterTypeIndex = rand() % vecCurLevelMonster.size();
	CMonster *pMonster = CMonster::createWithData(vecCurLevelMonster[monsterTypeIndex]);

	m_nMonsterCount++;
	if (m_nMonsterCount==m_nCurWaveMosCount)
	{
		
		CallFunc *pFun = CallFunc::create(CC_CALLBACK_0(CMonterMgr::waitForNextWave, this));
		this->runAction(pFun);
	}

	//ÿ���������ñ�ǩ
	m_nTagCount++;
	pMonster->setTag(m_nTagCount);
	this->addChild(pMonster);
}

void CMonterMgr::waitForNextWave()
{
	Sequence *pSeq = Sequence::create(CallFunc::create(CC_CALLBACK_0(CMonterMgr::stopSchedule, this)), 
		CallFunc::create(CC_CALLBACK_0(CMonterMgr::resumeSchedule,this)),nullptr);
	this->runAction(pSeq);
	
	
}

void CMonterMgr::stopSchedule()
{
	this->unschedule("addMonster");
}


void CMonterMgr::resumeSchedule()
{
	
	this->scheduleUpdate();
	m_nMonsterCount = 0;//ÿһ�����＼����Ϻ�����

}

void CMonterMgr::update(float fDelta)
{
	Vector<Node*> vecMonster = this->getChildren();
	if (vecMonster.size()==0)
	{
		m_nCurWave++;
		int lastWave = CDataMgr::getInstance()->getLevelDtMgr()->getCurLevelWave() + 1;
		
		if (m_nCurWave<CDataMgr::getInstance()->getLevelDtMgr()->getCurLevelWave()+1)
		{
			CGameLayer::getInstance()->setCurWaveString();
			//����init�������ݣ�������ӹ���ӿ�
			this->init(); 
			//����������һ��,���Ŷ���������
			if (m_nCurWave == lastWave - 1)
			{
				Size visibleSize = Director::getInstance()->getWinSize();
				SimpleAudioEngine::getInstance()->playEffect("Music/Items/Finalwave.ogg");
				Sprite *pFinal = Sprite::createWithSpriteFrameName("finalwave.png");
				pFinal->setScale(0.1);
				pFinal->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
				Sequence *pSeq = Sequence::create(ScaleBy::create(1 ,15), DelayTime::create(1.0),RemoveSelf::create(), nullptr);
				pFinal->runAction(pSeq);
				CGameLayer::getInstance()->addChild(pFinal);
				
			}
		}
		else if (m_nCurWave== lastWave)
		{
			SimpleAudioEngine::getInstance()->playEffect("Music/Items/Perfect.ogg");
			CGameLayer::getInstance()->showWinMenu();
		}
	}
}
