#include "DataMgr.h"

CDataMgr* CDataMgr::DataMgr = nullptr;
CDataMgr::CDataMgr()
{
	//ͼƬ��Դ��Դ
	m_pRes = new sResDt;
	m_pRes->LoadFile("Data1/resDt.json");

	//��ͼ����
	m_pMapDtMgr = new CMapDtMgr;
	m_pMapDtMgr->LoadFile("Data1/mapDt.json");

	//�ؿ�����
	m_pLevelDtMgr = new CLevelDtMgr;
	m_pLevelDtMgr->LoadFile("Data1/levelDt.json");

	//��������
	m_pWeaponDtMgr = new CWeaponDtMgr;
	m_pWeaponDtMgr->LoadFile("Data1/weaponDt.json");

	//�ӵ�����
	m_pBulletDtMgr = new CBulletDtMgr;
	m_pBulletDtMgr->LoadFile("Data1/bulletDt.json");

	//��Ƭ����
	m_pCardMgr = new CCardDtMgr;
	m_pCardMgr->LoadFile("Data1/cardDt.json");

	//�ϰ�������
	m_pObstacleDtMgr = new CObstacleDtMgr;
	m_pObstacleDtMgr->LoadFile("Data1/obstacleDt.json");

	//��������
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


