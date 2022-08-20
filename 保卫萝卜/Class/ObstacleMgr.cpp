#include "ObstacleMgr.h"
#include "GameLayer.h"


CObstacleMgr::CObstacleMgr()
{
}

CObstacleMgr::~CObstacleMgr()
{
}


CObstacleMgr * CObstacleMgr::createWithData(vector<Vec2> vecPoint)
{
	CObstacleMgr *pMgr = new CObstacleMgr;
	if (pMgr&&pMgr->initWithData(vecPoint))
	{
		pMgr->autorelease();
		return pMgr;
	}
	else
	{
		CC_SAFE_DELETE(pMgr);
		return nullptr;
	}
}

bool CObstacleMgr::initWithData(vector<Vec2> vecPoint)
{
	if (!Layer::init())
	{
		return false;
	}

	m_VecObstacle = vecPoint;
	int nVecSize = m_VecObstacle.size();
	
	m_vecLevelObject = CDataMgr::getInstance()->getLevelDtMgr()->getCurLevelObstacle();
	for (int i = 0; i < m_vecLevelObject.size(); ++i)
	{
		CObstacle *pObstacle = CObstacle::createWithData(m_VecObstacle[i], m_vecLevelObject[i]);
		pObstacle->setTag(i);
		this->addChild(pObstacle);
	}
	return true;
}
