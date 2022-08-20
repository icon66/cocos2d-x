#include "CCardMgr.h"
#include "Card.h"

CCardMgr::CCardMgr()
{
}

CCardMgr::~CCardMgr()
{
}

bool CCardMgr::init()
{
	if (!Node::init())
	{
		return false;
	}
	
	return true;
}
