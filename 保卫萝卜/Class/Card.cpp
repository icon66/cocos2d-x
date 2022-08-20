#include "Card.h"

CCard::CCard()
{
}

CCard::~CCard()
{
}

CCard * CCard::createWithData(const int &nType)
{
	
	CCard *pCard = new CCard;
	if (pCard!=nullptr&&pCard->initWithData(nType))
	{
		pCard -> autorelease();
		return pCard;
	}
	else
	{
		CC_SAFE_DELETE(pCard);
		return nullptr;
	}
}

bool CCard::initWithData(const int &nType)
{
	m_nType = nType;
	char szName[32] = {};
	switch (nType)
	{
	case E_TOWER_TBOTTLE:
		sprintf_s(szName, "Bottle01.png");
		break;
	case E_TOWER_TFIRE:
		sprintf_s(szName, "FBottle01.png");
		break;
	case E_TOWER_TSHIT:
		sprintf_s(szName, "Shit01.png");
		break;
	case E_TOWER_TTHUNDER:
		sprintf_s(szName, "Ball01.png");
		break;
	case E_TOWER_TLEAF:
		sprintf_s(szName, "Fan01.png");
		break;
	case E_TOWER_TREDSTART:
		sprintf_s(szName, "Star01.png");
		break;
	case E_TISHI_REC:
		sprintf_s(szName, "select_04.png");

	case E_TISHI_FORBID:
		sprintf_s(szName, "forbidden.png");
		break;
	case E_TISHI_UPGRADE:
		sprintf_s(szName, "range_120.png");
		break;
	case E_TISHI_UPGRADECH:
		sprintf_s(szName, "upgrade_-480.png");
		break;
	case E_TISHI_REMOVE:
		sprintf_s(szName, "sell_224.png");
		break;
	case E_TISHI_BIAOJI:
		sprintf_s(szName, "point01.png");
		break;
	default:
		break;
	}

	if (!Sprite::initWithSpriteFrameName(szName))
	{
		return false;
	}

	return true;
}


