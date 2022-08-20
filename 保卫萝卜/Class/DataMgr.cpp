#include "DataMgr.h"

CDataMgr* CDataMgr::DataMgr = nullptr;
CDataMgr::CDataMgr()
{
	//图片资源资源
	m_pRes = new sResDt;
	m_pRes->LoadFile("Data1/resDt.json");

	//地图数据
	m_pMapDtMgr = new CMapDtMgr;
	m_pMapDtMgr->LoadFile("Data1/mapDt.json");

	//关卡数据
	m_pLevelDtMgr = new CLevelDtMgr;
	m_pLevelDtMgr->LoadFile("Data1/levelDt.json");

	//武器数据
	m_pWeaponDtMgr = new CWeaponDtMgr;
	m_pWeaponDtMgr->LoadFile("Data1/weaponDt.json");

	//子弹数据
	m_pBulletDtMgr = new CBulletDtMgr;
	m_pBulletDtMgr->LoadFile("Data1/bulletDt.json");

	//卡片数据
	m_pCardMgr = new CCardDtMgr;
	m_pCardMgr->LoadFile("Data1/cardDt.json");

	//障碍物数据
	m_pObstacleDtMgr = new CObstacleDtMgr;
	m_pObstacleDtMgr->LoadFile("Data1/obstacleDt.json");

	//怪物数据
	m_pMonsterDtMgr = new CMonsterDtMgr;
	m_pMonsterDtMgr->LoadFile("Data1/monsterDt.json");
}

CDataMgr::~CDataMgr()
{
}

CDataMgr * CDataMgr::getInstance()
{
	//DataMgr = nullptr;
	if (DataMgr==nullptr)
	{
		DataMgr = new CDataMgr;
	}
	return DataMgr;
}

void CDataMgr::levelDtUpdate()
{
	
}


