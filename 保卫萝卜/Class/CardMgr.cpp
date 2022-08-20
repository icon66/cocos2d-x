#include "CardMgr.h"
#include "GameLayer.h"


CCardMgr::CCardMgr()
{
	bIsCanCreate = true;
	hasFind = false;
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

	//添加选中动画
	char szName[32] = {};
	Animation *pAnimation = Animation::create();
	for (int i = 1; i < 4; i++)
	{
		sprintf_s(szName, "select_0%d.png", i);
		SpriteFrame *pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szName);
		pAnimation->addSpriteFrame(pFrame);
	}
	pAnimation->setDelayPerUnit(0.1);
	Animate *pAnimate = Animate::create(pAnimation);

	m_pCurWeapon = CWeapon::create();
	m_pCurObstacle = CObstacle::create();
	m_pCurObstacle->retain();

	vecWeaponCardDt=CDataMgr::getInstance()->getLevelDtMgr()->getCurLevelWeapon();

	int nType = 0;
	for (int i=0;i<vecWeaponCardDt.size();++i)
	{
		CCard *pCard=CCard::create();
		pCard->setType(vecWeaponCardDt[i]);
		pCard->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(CDataMgr::getInstance()->getWeaponDtMgr()->getWeaponDetailDtByID(vecWeaponCardDt[i])->vecShopImg[1]));
		vecCard.push_back(pCard);
		this->addChild(vecCard[i]);
	}
	
	//选中框
	m_pCardRec = CCard::createWithData(E_TISHI_REC);
	m_pCardRec->setLocalZOrder(-1);
	m_pCardRec -> setVisible(false);
	this->addChild(m_pCardRec);
	m_pCardRec->runAction(RepeatForever::create(pAnimate));

	//禁止建塔图标
	m_pForbid = CCard::createWithData(E_TISHI_FORBID);
	m_pForbid->setVisible(false);
	this->addChild(m_pForbid);

	//攻击范围
	m_pAttcRangeCircle = CCard::create();
	m_pAttcRangeCircle->setVisible(false);
	this->addChild(m_pAttcRangeCircle);

	//更新卡片
	m_pUpgrade = CCard::create();
	m_pUpgrade->setVisible(false);
	this->addChild(m_pUpgrade);

	//出售卡片
	m_pSell = CCard::create();
	m_pSell->setVisible(false);
	this->addChild(m_pSell);

	//标记某个障碍物或者
	m_pBiaoji = CCard::createWithData(E_TISHI_BIAOJI);
	m_pBiaoji->setVisible(false);
	this->addChild(m_pBiaoji);

	//提示不可建塔
	CallFunc *pFun = CallFunc::create(CC_CALLBACK_0(CCardMgr::disappear, this));
	pSeq = Sequence::create(DelayTime::create(0.4), pFun, nullptr);
	pSeq->retain();
	
	pCache = SpriteFrameCache::getInstance();
	return true;
}

void CCardMgr::clickEvent(Vec2 &clickPos,int nGid)
{
	
	//第一次点击
	if (!this->isVisible())
	{
		//遍历怪物
		Vector<Node*> vMonster = CGameLayer::getInstance()->getMonsterMgr()->getChildren();
		Vector<Node*> vecWeapon = CGameLayer::getInstance()->getWeaponMgr()->getChildren();
		Vector<Node*> vecObstacle = CGameLayer::getInstance()->getObstacleMgr()->getChildren();
		for (int i = 0; i < vMonster.size(); ++i)
		{
			CMonster *pMos = static_cast<CMonster*>(vMonster.at(i));
			if (pMos->getBoundingBox().containsPoint(clickPos))
			{
				m_pCurObstacle->setReward(0);//设置当前障碍物为空
				SimpleAudioEngine::getInstance()->playEffect("Music/Items/ShootSelect.ogg");
				//选中怪物时将障碍物上的标记取消掉
				for (Node* pNode:vecObstacle)
				{
					static_cast<CObstacle*>(pNode)->getBiaoji()->setVisible(false);
				}
				for (Node *pNode:vMonster)
				{
					pNode->getChildByName("Biaoji")->setVisible(false);
				}
				//播放标记动画
				pMos->getChildByName("Biaoji")->setVisible(true);
				pMos->getChildByName("Biaoji")->runAction(RepeatForever::create(CGameLayer::getInstance()
					->getAnimateMgr()->getAnimateByID(E_ANIMATE_BIOAJI))->clone());
				for (int j=0;j<vecWeapon.size();++j)
				{
					CWeapon *pWea = static_cast<CWeapon*>(vecWeapon.at(j));
					if (pWea->getPosition().distance(pMos->getPosition())<pWea->getAttackRange()+20)
					{
						CCLOG("attack monster");
						pWea->resetState();
					}
					else if (j==vecWeapon.size()-1)//遍历到最后一个
					{
						return;  
					}
				}
			}
		}

	
		//没有路径层的物体
		if (nGid==0&&clickPos.y<=560)
		{
			Vec2 tiled = CGameLayer::getInstance()->getTiledByPos(clickPos);
			Vec2 curCenterPos = CGameLayer::getInstance()->getCenterPosByTiled(tiled);
			Vector<Node*> vecWeapon = CGameLayer::getInstance()->getWeaponMgr()->getChildren();
			//遍历武器
			for (Node *pNode : vecWeapon)
			{
				if (pNode->getBoundingBox().containsPoint(curCenterPos))
				{
					SimpleAudioEngine::getInstance()->playEffect("Music/Items/TowerSelect.ogg");
					m_pCurWeapon = static_cast<CWeapon*>(pNode);
					//根据武器范围设置范围图片
					m_pAttcRangeCircle->setSpriteFrame(pCache->getSpriteFrameByName(CDataMgr::getInstance()
						->getCardDtMgr()->getRangImgByRange(m_pCurWeapon->getAttackRange())));
					m_pCardRec->setPosition(curCenterPos);
					m_pAttcRangeCircle->setPosition(curCenterPos);
					//让升级相关的卡片显示
					this->upgrateCardShow();

					m_pUpgrade->setPosition(Vec2(m_pCardRec->getPositionX(), m_pCardRec->getPositionY() + m_pUpgrade->getContentSize().height));
					m_pSell->setPosition(Vec2(m_pCardRec->getPositionX(), m_pCardRec->getPositionY() - m_pUpgrade->getContentSize().height));
					bIsCanCreate = false;//是否可以建塔置为false;
					return;
				}
			}

			//遍历障碍物
			for (Node* pObstacle:vecObstacle)
			{
				if (pObstacle->getBoundingBox().containsPoint(curCenterPos))
				{
					m_pCurObstacle = static_cast<CObstacle*>(pObstacle);
					//m_pCurObstacle->setReward(static_cast<CObstacle*>(pObstacle)->getReward());
					
					//每次重新选中障碍物时，讲所有武器状态重置，避免同时对多个障碍物进行设计
					for (Node *pNode : vecWeapon)
					{
						static_cast<CWeapon*>(pNode)->resetState();
					}

					//判断是否在塔的攻击范围内
					Vector<Node*> vecWeapon = CGameLayer::getInstance()->getWeaponMgr()->getChildren();
					vecRangeWeapon.clear(); //每次当前障碍物周围武器的容器清空
					for(int i=0;i<vecWeapon.size();++i)
					{
						CWeapon*pWeapon = static_cast<CWeapon*>(vecWeapon.at(i));
						
						if (pWeapon->getPosition().distance(m_pCurObstacle->getPosition())<static_cast<CWeapon*>(pWeapon)->getAttackRange()+40)
						{
							SimpleAudioEngine::getInstance()->playEffect("Music/Items/ShootSelect.ogg");
							vecRangeWeapon.pushBack(pWeapon);
							//当前障碍物设置有非空
							//m_pCurObstacle = static_cast<CObstacle*>(pObstacle);
							m_pCurObstacle->getBiaoji()->setVisible(true);
						
							for (Node *pNode:vMonster)
							{
								static_cast<CMonster*>(pNode)->getChildByName("Biaoji")->stopAllActions();
								static_cast<CMonster*>(pNode)->getChildByName("Biaoji")->setVisible(false);
							}
							
							//非当前选中的障碍物的取消标记
							for (Node *pNode:vecObstacle)
							{
								if (static_cast<CObstacle*>(pNode) != m_pCurObstacle)
								{
									static_cast<CObstacle*>(pNode)->getBiaoji()->setVisible(false);
								}
							}
							
								//某些塔不能旋转
								CCLOG("ACCACK OBSTANCLE");
								Vec2 detailPos = m_pCurObstacle->getPosition() - pWeapon->getPosition();//向量差
								Point normalizedVector = ccpNormalize(detailPos);
								float fRadians = atan2(normalizedVector.y, -normalizedVector.x);
								float fDegree = CC_RADIANS_TO_DEGREES(fRadians);

								float rotateSpeed = 2 * M_PI;
								float rotate_1rad_time = 1 / rotateSpeed;
								float rotateDuration = fabs(fRadians * rotate_1rad_time);
								

								//播放选中动画		
								for (int j=0;j<vecRangeWeapon.size();++j)
								{
									if (pWeapon->getWeaponType() != E_TOWER_TSHIT && pWeapon->getWeaponType() != E_TOWER_TLEAF
										&& pWeapon->getWeaponType() != E_TOWER_TREDSTART && pWeapon->getWeaponType() != E_TOWER_TTHUNDER)
									{
										vecRangeWeapon.at(j)->runAction(RotateTo::create(rotateDuration, fDegree - 90));
										vecRangeWeapon.at(j)->getChildByName("tray")->runAction(RotateTo::create(rotateDuration, -(fDegree - 90)));
									}
									vecRangeWeapon.at(j)->setState(m_pCurObstacle->getPosition());
								}
							
								m_pCurObstacle->getBiaoji()->runAction(RepeatForever::create(CGameLayer::getInstance()
									->getAnimateMgr()->getAnimateByID(E_ANIMATE_BIOAJI)));
								
						}
						else if (i==vecWeapon.size()-1)
						{
							//播放不能创建塔音乐
							SimpleAudioEngine::getInstance()->playEffect("Music/Items/SelectFault.ogg", false);
							m_pForbid->setPosition(curCenterPos);
							for (int i = 0; i < vecCard.size(); ++i)
							{
								vecCard[i]->setVisible(false);
							}
							this->setVisible(true);
							m_pForbid->setVisible(true);
							m_pForbid->runAction(pSeq->clone());	
						}
					}
					return;
				}	
			}
			m_CenterPos = CGameLayer::getInstance()->getCenterPosByTiled(tiled);
			//能够显示
		
			this->setVisible(true);
			m_pCardRec->setVisible(true);
			m_pCardRec->setPosition(m_CenterPos);
			for (int i = 0; i < vecCard.size(); ++i)
			{
				vecCard[i]->setVisible(true);
				vecCard[i]->setPosition(Vec2(m_pCardRec->getPositionX()+(m_pCardRec->getContentSize().width / 2 + vecCard[0]->getContentSize().width / 2)*i,
					m_pCardRec->getPositionY() + m_pCardRec->getContentSize().height / 2 + vecCard[0]->getContentSize().height / 2));
			}
			SimpleAudioEngine::getInstance()->playEffect("Music/Items/TowerSelect.ogg");
		}
		//在路径区域建塔
		else
		{
			//播放不能创建塔音乐
			SimpleAudioEngine::getInstance()->playEffect("Music/Items/SelectFault.ogg", false);
			Vec2 tiled = CGameLayer::getInstance()->getTiledByPos(clickPos);
			Vec2 curCenterPos = CGameLayer::getInstance()->getCenterPosByTiled(tiled);
			Vector<Node*> vecWeapon = CGameLayer::getInstance()->getWeaponMgr()->getChildren();
			m_pForbid->setPosition(curCenterPos);
			this->setVisible(true);
			m_pForbid->setVisible(true);
			this->runAction(pSeq->clone());
		}
		
	}
	//第二次点击
	else
	{
		//升级卡片显示出来后，不能点到建塔卡片
		if (bIsCanCreate)
		{
			//判断点到的是那张武器卡片，进行相应的建造
			for (int i = 0; i < vecCard.size(); ++i)
			{
				if (vecCard[i]->getBoundingBox().containsPoint(clickPos))
				{
					if (CGameLayer::getInstance()->getGold()>=CDataMgr::getInstance()->getWeaponDtMgr()->getWeaponDetailDtByID(vecCard[i]->getnType())->vecCreatePrice[0])
					{
						SimpleAudioEngine::getInstance()->playEffect("Music/Items/TowerBulid.ogg");
						CGameLayer::getInstance()->getWeaponMgr()->addWeapon(vecCard[i]->getnType(), m_CenterPos);
					}	
				}
			}
		}
		//不能建塔的时候判断是否进行升级
		else
		{
			if (m_pUpgrade->getBoundingBox().containsPoint(clickPos))
			{
				//金钱足够的话尽进行升级
				if (CGameLayer::getInstance()->getGold()>=m_pCurWeapon->getUpgradePrice())
				{
					m_pCurWeapon->upGrade();
				}
				
			}
			else if (m_pSell->getBoundingBox().containsPoint(clickPos))
			{
				SimpleAudioEngine::getInstance()->playEffect("Music/Items/TowerSell.ogg");
				int num = m_pCurWeapon->getSellPrice();
				CGameLayer::getInstance()->setAddGold(m_pCurWeapon->getSellPrice());
				m_pCurWeapon->runAction(RemoveSelf::create());
				CCLOG("REMOVE");
			}
		}
		for (int i=0;i<vecCard.size();++i)
		{
			vecCard[i]->setVisible(false);
		}
		m_pCardRec->setVisible(false);
		m_pUpgrade->setVisible(false);
		m_pAttcRangeCircle->setVisible(false);
		m_pSell->setVisible(false);
		bIsCanCreate = true;
		this->setVisible(false);	
		SimpleAudioEngine::getInstance()->playEffect("Music/Items/TowerDeselect.ogg", false);
	}
}

void CCardMgr::disappear()
{
	this->setVisible(false);
	m_pForbid->setVisible(false);
}

void CCardMgr::upgrateCardShow()
{
	m_pAttcRangeCircle->setVisible(true);
	m_pCardRec->setVisible(true);
	m_pSell->setVisible(true);
	m_pUpgrade->setVisible(true);
	this->setVisible(true);
	cardUpdate();

}

//卡片更新
void CCardMgr::cardUpdate()
{
	if (m_pCurWeapon->getGrade() < 3)
	{
		int flag;
		int num = CGameLayer::getInstance()->getGold();
		if (num >= m_pCurWeapon->getUpgradePrice())
		{
			flag = 1;
		}
		else
		{
			flag = -1;
		}
		m_pUpgrade->setSpriteFrame(pCache->getSpriteFrameByName(CDataMgr::getInstance()->getCardDtMgr()
			->getUpgradeImgByMoney(m_pCurWeapon->getUpgradePrice()*flag)));

	}
	else
	{
		m_pUpgrade->setSpriteFrame(pCache->getSpriteFrameByName("upgrade_0_CN.png"));
	}


	m_pSell->setSpriteFrame(pCache->getSpriteFrameByName(CDataMgr::getInstance()->getCardDtMgr()
		->getSellImgByMoney(m_pCurWeapon->getSellPrice())));
}


