#pragma once
#include "StructData.h"
class CDataMgr
{
public:
	CDataMgr();
	~CDataMgr();
	sResDt *getResDt()
	{
		return m_pRes;
	}
	CMapDtMgr *getMapDtMgr()
	{
		return m_pMapDtMgr;
	}
	CLevelDtMgr *getLevelDtMgr()
	{
		return m_pLevelDtMgr;
	}
	CWeaponDtMgr *getWeaponDtMgr()
	{
		return m_pWeaponDtMgr;
	}
	CBulletDtMgr *getBulletDtMgr()
	{
		return m_pBulletDtMgr;
	}
	CCardDtMgr *getCardDtMgr()
	{
		return m_pCardMgr;
	}
	CObstacleDtMgr *getCObstacleDtMgr()
	{
		return m_pObstacleDtMgr;
	}
	CMonsterDtMgr *getCMonsterDtMgr()
	{
		return m_pMonsterDtMgr;
	}
	static CDataMgr *getInstance();
	void levelDtUpdate();
private:
	static CDataMgr *DataMgr;
	sResDt * m_pRes;
	CMapDtMgr *m_pMapDtMgr;
	CLevelDtMgr *m_pLevelDtMgr;
	CWeaponDtMgr *m_pWeaponDtMgr;
	CBulletDtMgr *m_pBulletDtMgr;
	CCardDtMgr* m_pCardMgr;
	CObstacleDtMgr *m_pObstacleDtMgr;
	CMonsterDtMgr *m_pMonsterDtMgr;
};
