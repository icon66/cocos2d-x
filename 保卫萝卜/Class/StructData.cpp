#include "StructData.h"
#include "json/rapidjson.h"
#include "json/document.h"

sResDt::sResDt()
{
}

void sResDt::LoadFile(const char *pFileName)
{
	string strPath = FileUtils::getInstance()->fullPathForFilename(pFileName);

	string strData = FileUtils::getInstance()->getStringFromFile(strPath);

	rapidjson::Document doc;

	doc.Parse<0>(strData.c_str());

	rapidjson::Value &vPng = doc["png"];

	for (int i = 0; i < vPng.Size(); i++)
	{
		vecPng.push_back(vPng[i].GetString());
	}

	rapidjson::Value& vPlist = doc["plist"];

	for (int i = 0; i < vPlist.Size(); i++)
	{
		vecPlist.push_back(vPlist[i].GetString());
	}
}

//地图数据的更新
void sResDt::UpdateFile(const int & nTheme)
{
	string strPath = FileUtils::getInstance()->fullPathForFilename("Data1/mapDt.json");
	string strData = FileUtils::getInstance()->getStringFromFile(strPath);

	rapidjson::Document doc;
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator(); //获取分配器

	doc.Parse<0>(strData.c_str());

	//doc[]
}

sMapDt::sMapDt()
{
	nThemeID = 0;
}



CMapDtMgr::CMapDtMgr()
{
}

CMapDtMgr::~CMapDtMgr()
{
}

void CMapDtMgr::LoadFile(const char * pFileName)
{
	string strPath = FileUtils::getInstance()->fullPathForFilename(pFileName);
	string strData = FileUtils::getInstance()->getStringFromFile(strPath);

	//解析json
	rapidjson::Document doc;
	doc.Parse<0>(strData.c_str());

	
	for (int i=0;i<doc.Size();++i)
	{
		sMapDt *pMapDt = new sMapDt;
		rapidjson::Value& pData = doc[i];
		pMapDt->nThemeID = pData["themeid"].GetInt();
		
		//主题图片
		rapidjson::Value& vlevelpng = pData["levelpng"];
		for (int j=0;j<vlevelpng.Size();++j)
		{
			pMapDt->vecLevelPng.push_back(vlevelpng[j].GetString());
		}
		//背景图
		rapidjson::Value& vCloudpng = pData["cloud"];
		for (int k=0;k<vCloudpng.Size();++k)
		{
			pMapDt->vecCloudPng.push_back(vCloudpng[k].GetString());
		}
		//主题音乐
		rapidjson::Value& sdata = pData["music"];
		pMapDt->sMusic = sdata.GetString();


		//是否锁定（0为锁定，1为已解锁）
		rapidjson::Value& nIsLock = pData["lock"];
		pMapDt->nLock = nIsLock.GetInt();

		vecMapDt.push_back(pMapDt);
	}
}

int CMapDtMgr::getCurThemeLockByID(const int& nID)
{
	for (int i=0;i<vecMapDt.size();++i)
	{
		if (vecMapDt[i]->nThemeID==nID)
		{
			return vecMapDt[i]->nLock;
		}
	}
}

std::vector<std::string> CMapDtMgr::getCurThemeMap()
{
	for (sMapDt *pMapDt : vecMapDt)
	{
		if (pMapDt->nThemeID == curTheme)
		{
			return pMapDt->vecLevelPng;
		}
	}
	return vector<string>();
}

vector<string> CMapDtMgr::getCurThemeBg()
{
	for (sMapDt *pMapDt:vecMapDt)
	{
		if (pMapDt->nThemeID == curTheme)
		{
			return pMapDt->vecCloudPng;
		}
	}
	return vector<string>();
}

string CMapDtMgr::getThemeMusic()
{
	for (sMapDt *pMapDt : vecMapDt)
	{
		if (pMapDt->nThemeID == curTheme)
		{
			return pMapDt->sMusic;
		}
	}
	return string();
}


CLevelDtMgr::CLevelDtMgr()
{
	nCurLevel = 1;
}

CLevelDtMgr::~CLevelDtMgr()
{
}

void CLevelDtMgr::LoadFile(const char * pFileName)
{
	string strPath = FileUtils::getInstance()->fullPathForFilename(pFileName);
	string strData = FileUtils::getInstance()->getStringFromFile(strPath);

	rapidjson::Document doc;
	doc.Parse<0>(strData.c_str());

	for (int i=0;i<doc.Size();++i)
	{
		rapidjson::Value &vData = doc[i];
		sLevelDt* pData = new sLevelDt;
		pData->nTheme = vData["theme"].GetInt();

		//获得TiledMap
		rapidjson::Value& vTmx = vData["TiledMap"];
		for (int n=0;n<vTmx.Size();++n)
		{
			pData->vecTiledMap.push_back(vTmx[n].GetString());
		}
		//获得Wave数
		rapidjson::Value& vWave = vData["wave"];
		for (int m=0;m<vWave.Size();++m)
		{
			pData->vecWaveNum.push_back(vWave[m].GetInt());
		}
		//关卡拥有的武器
		rapidjson::Value& vLevelWeapon = vData["weapon"];
		for (int j=0;j<vLevelWeapon.Size();++j)
		{
			vector<int> vWid;
			rapidjson::Value& vWeaponPng = vLevelWeapon[j];
			for (int k=0;k<vWeaponPng.Size();++k)
			{
				vWid.push_back(vWeaponPng[k].GetInt());
			}
			pData->mapWeaponId.insert(pData->mapWeaponId.begin(), pair<int,vector<int>>(j, vWid));
		}

		//达成荣誉
		rapidjson::Value& vHonor = vData["honor"];
		for (int j = 0; j < vHonor.Size(); ++j)
		{
			vector<int> vHon;
			rapidjson::Value& vDetail = vHonor[j];
			for (int k=0;k<vDetail.Size();++k)
			{
				vHon.push_back(vDetail[k].GetInt());
			}
			pData->mapHonor.insert(pData->mapHonor.begin(), pair<int, vector<int>>(j, vHon));
		}

		//锁定
		rapidjson::Value& vLock = vData["lock"];
		for (int j=0;j<vLock.Size();++j)
		{
			pData->vecLock.push_back(vLock[j].GetInt());
		}


		//关卡障碍物
		rapidjson::Value& vLevelObstacle = vData["obstacle"];
		for (int j = 0; j < vLevelObstacle.Size(); ++j)
		{
			vector<string> vPng;
			rapidjson::Value& vObstaclePng = vLevelObstacle[j];
			for (int k = 0; k < vObstaclePng.Size(); ++k)
			{
				vPng.push_back(vObstaclePng[k].GetString());
			}
			pData->mapObstaclePng.insert(pData->mapObstaclePng.begin(), pair<int, vector<string>>(j, vPng));
		}

		//当前关卡怪物
		rapidjson::Value& vLevelMonster = vData["monsterid"];
		for (int l=0;l<vLevelMonster.Size();++l)
		{
			vector<int> vMonsterid;
			rapidjson::Value& vMonster = vLevelMonster[l];
			for (int o=0;o<vMonster.Size();++o)
			{
				vMonsterid.push_back(vMonster[o].GetInt());
			}
			pData->mapMonsterId.insert(pData->mapMonsterId.begin(), pair<int,vector<int>>(l,vMonsterid));
		}

		//每一波怪物数量
		rapidjson::Value& vWaveCount = vData["wavemonstercount"];
		for (int p=0;p<vWaveCount.Size();++p)
		{
			vector<int> vecCount;
			rapidjson::Value& vCount = vWaveCount[p];
			for (int b=0;b<vCount.Size();++b)
			{
				vecCount.push_back(vCount[b].GetInt());
			}
			pData->mapWaveCount.insert(pData->mapWaveCount.begin(), pair<int, vector<int>>(p,vecCount));
		}

		//关卡初始金币
		rapidjson::Value& vinitgold = vData["initgold"];
		for (int y=0;y<vinitgold.Size();++y)
		{
			pData->vecInitGold.push_back(vinitgold[y].GetInt());
		}

		//通过关卡数量
		rapidjson::Value& nPassCount = vData["passLevelCount"];
		pData->nPassLevelCount = nPassCount.GetInt();

		vecLevelDt.push_back(pData);
	}
}

//修改数据
void CLevelDtMgr::UpdateFile(int nTheme, int nLevel, vector<int> vecHonor)
{
	//内存数据覆盖
	vecLevelDt.at(nTheme-1)->mapHonor.at(nLevel-1) = vecHonor;
	vecLevelDt.at(nTheme-1)->vecLock.at(nLevel)=1;
	vecLevelDt.at(nTheme - 1)->nPassLevelCount++;

	//对文件的覆盖存储
	string strPath = FileUtils::getInstance()->fullPathForFilename("Data1/levelDt.json");
	string strData = FileUtils::getInstance()->getStringFromFile(strPath);

	rapidjson::Document doc;
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator(); //获得分配器
	doc.Parse<0>(strData.c_str());

	//如果现获得荣誉1大于数据中的荣誉1，则更新
	if (vecHonor[0]>doc[nTheme - 1]["honor"][nLevel - 1][0].GetInt())
	{
		doc[nTheme - 1]["honor"][nLevel - 1][0].SetInt(vecHonor[0]);
	}
	//如果现获得荣誉2大于数据中的荣誉1，则更新
	if (vecHonor[1]>doc[nTheme - 1]["honor"][nLevel - 1][0].GetInt())
	{
		doc[nTheme - 1]["honor"][nLevel - 1][1].SetInt(vecHonor[1]);
	}
	//解锁下一关
	doc[nTheme - 1]["lock"][nLevel].SetInt(1);
	//通过关卡数+1
	doc[nTheme - 1]["passLevelCount"].SetInt(doc[nTheme-1]["passLevelCount"].GetInt()+1);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
	doc.Accept(write);

	//写文件
	FILE* pFile = fopen(strPath.c_str(), "wb");
	if (NULL != pFile)
	{
		fputs(buffer.GetString(), pFile);
		fclose(pFile);
	}
}

int CLevelDtMgr::getCurLevelWave()
{
	for (sLevelDt* pLevelDt : vecLevelDt)
	{
		if (pLevelDt->nTheme == nCurTheme)
		{
			return pLevelDt->vecWaveNum[nCurLevel-1];
		}
	}
	return 0;
}

vector<int> CLevelDtMgr::getCurLevelWeapon()
{
	for (sLevelDt* pLevelDt:vecLevelDt)
	{
		if (pLevelDt->nTheme==nCurTheme)
		{
			return pLevelDt->mapWeaponId[nCurLevel-1];
		}
	}
	return vector<int>();
}

vector<string> CLevelDtMgr::getCurLevelObstacle()
{
	for (sLevelDt* pLevelDt : vecLevelDt)
	{
		if (pLevelDt->nTheme == nCurTheme)
		{
			return pLevelDt->mapObstaclePng[nCurLevel - 1];
		}
	}
	return vector<string>();
}

std::vector<int> CLevelDtMgr::getCurLevelMonsterType()
{
	for (sLevelDt *pLevelDt:vecLevelDt)
	{
		if (pLevelDt->nTheme==nCurTheme)
		{
			return pLevelDt->mapMonsterId[nCurLevel - 1];
		}
	}
	return vector<int>();
}

std::vector<int> CLevelDtMgr::getCurLevelWaveMonsterCount()
{
	for (sLevelDt *pLevelDt : vecLevelDt)
	{
		if (pLevelDt->nTheme == nCurTheme)
		{
			return pLevelDt->mapWaveCount[nCurLevel - 1];
		}
	}
	return vector<int>();
}

vector<int> CLevelDtMgr::getCurLevelHonorByID(const int& nID)
{
	for (sLevelDt *pLevelDt:vecLevelDt)
	{
		if (pLevelDt->nTheme==nCurTheme)
		{
			return pLevelDt->mapHonor[nID];
		}
	}
	return vector<int>();
}

int CLevelDtMgr::getPassLevelCountByTheme(const int& nID)
{
	for (sLevelDt *pLevelDt : vecLevelDt)
	{
		if (pLevelDt->nTheme == nID)
		{
			return pLevelDt->nPassLevelCount;
		}
	}
	return 0;
}

int CLevelDtMgr::getCurLevelLockByID(const int& nID)
{
	for (sLevelDt *pLevelDt : vecLevelDt)
	{
		if (pLevelDt->nTheme == nCurTheme)
		{
			return pLevelDt->vecLock[nID];
		}
	}
	return 0;
}



std::string CLevelDtMgr::getCurTiledMap()
{
	for (sLevelDt* pLevelDt : vecLevelDt)
	{
		if (pLevelDt->nTheme == nCurTheme)
		{
			return pLevelDt->vecTiledMap[nCurLevel - 1];
		}
	}
	return string();
}

int CLevelDtMgr::getCurLevelInitGold()
{
	for (sLevelDt* pLevelDt : vecLevelDt)
	{
		if (pLevelDt->nTheme == nCurTheme)
		{
			return pLevelDt->vecInitGold[nCurLevel - 1];
		}
	}
	return 0;
}

sLevelDt::sLevelDt()
{
	nTheme = 1;
}

sWeaponDt::sWeaponDt()
{

}


CWeaponDtMgr::CWeaponDtMgr()
{
}

CWeaponDtMgr::~CWeaponDtMgr()
{
}

void CWeaponDtMgr::LoadFile(const char * pFileName)
{
	string strPath = FileUtils::getInstance()->fullPathForFilename(pFileName);
	string strData = FileUtils::getInstance()->getStringFromFile(strPath);

	rapidjson::Document doc;
	doc.Parse<0>(strData.c_str());

	for (int i=0;i<doc.Size();++i)
	{
		rapidjson::Value& vData = doc[i];
		sWeaponDt *pData = new sWeaponDt;
		pData->nID = vData["weaponID"].GetInt();
		//攻击范围
		rapidjson::Value &vRange = vData["attackrange"];
		for (int v = 0; v < vRange.Size(); ++v)
		{
			pData->vecAttackRange.push_back(vRange[v].GetInt());
		}
		//攻击速度
		rapidjson::Value& vShootSpeed = vData["shootSpeed"];
		for (int f=0;f<vShootSpeed.Size();++f)
		{
			pData->vecShootSpeed.push_back(vShootSpeed[f].GetDouble());
		}
		//武器图标
		rapidjson::Value& vIcon = vData["icon"];
		pData->sIcon = vIcon.GetString();

		//武器购买的图标
		rapidjson::Value& vShopImg = vData["shopimg"];
		for (int f=0;f<vShopImg.Size();++f)
		{
			pData->vecShopImg.push_back(vShopImg[f].GetString());
		}

		//动画图片
		rapidjson::Value& vImgData = vData["actionimg"];
		for (int j=0;j<vImgData.Size();++j)
		{
			vector<string> vImg;
			rapidjson::Value & vDetailImgData = vImgData[j];
			for (int m=0;m<vDetailImgData.Size();++m)
			{
				vImg.push_back(vDetailImgData[m].GetString());
			}
			pData->mapImg.insert(pData->mapImg.begin(), pair<int, vector<string>>(j,vImg));
		}

		//射击音乐
		rapidjson::Value& vMusic = vData["music"];
		for (int n=0;n<vMusic.Size();++n)
		{
			pData->sMusic = vMusic[0].GetString();
		}

		//建造价格
		rapidjson::Value& vCreatePrice = vData["createprice"];
		for (int n=0;n<vCreatePrice.Size();++n)
		{
			pData->vecCreatePrice.push_back(vCreatePrice[n].GetInt());
		}

		//出售价格
		rapidjson::Value& vSellPrice = vData["sellprice"];
		for (int n = 0; n < vSellPrice.Size(); ++n)
		{
			pData->vecSellPrice.push_back(vSellPrice[n].GetInt());
		}
		
		vecWeaponDt.push_back(pData);
	}
	
}

vector<string> CWeaponDtMgr::getWeaponImgDtByIdAndGrade(const int & nid, const int& ngrade)
{
	for (sWeaponDt *pData : vecWeaponDt)
	{
		if (pData->nID == nid)
		{
			return pData->mapImg[ngrade-1];
		}
	}
	return vector<string>();
}

sWeaponDt * CWeaponDtMgr::getWeaponDetailDtByID(const int & nid)
{
	for (sWeaponDt *pData : vecWeaponDt)
	{
		if (pData->nID == nid)
		{
			return pData;
		}
	}
	return nullptr;
}


int CWeaponDtMgr::getWeaponRangeDtByTypeAndGrade(const int& nType,const int& nGrade)
{
	for (sWeaponDt *pData: vecWeaponDt)
	{
		if (pData->nID==nType)
		{
			return pData->vecAttackRange[nGrade - 1];
		}
	}
	return 0;
}

float CWeaponDtMgr::getWeaponShootSpeedDtByTypeAndGrade(const int& nType, const int& nGrade)
{
	for (sWeaponDt *pData : vecWeaponDt)
	{
		if (pData->nID == nType)
		{
			return pData->vecShootSpeed[nGrade - 1];
		}
	}
	return 0;
}

CBulletDtMgr::CBulletDtMgr()
{
}

CBulletDtMgr::~CBulletDtMgr()
{
}

void CBulletDtMgr::LoadFile(const char * pFileName)
{
	string strPath = FileUtils::getInstance()->fullPathForFilename(pFileName);
	string strData = FileUtils::getInstance()->getStringFromFile(strPath);

	rapidjson:: Document doc;
	doc.Parse<0>(strData.c_str());

	for (int i=0;i<doc.Size();++i)
	{
		rapidjson::Value &vData = doc[i];
		sBulletDt *pData = new sBulletDt;
		pData->nID = vData["bulletID"].GetInt();
		pData->nAck = vData["nAck"].GetInt();
		pData->fSpeedBuff = vData["speedbuff"].GetDouble();
		pData->bIsRotate = vData["bRotate"].GetInt();
		//子弹三种移动图片
		rapidjson::Value& vMoveData = vData["moveimg"];
		for (int j=0;j<vMoveData.Size();++j)
		{
			vector<string> vImg;
			rapidjson::Value& vMoveImg= vMoveData[j];
			for (int m=0;m<vMoveImg.Size();++m)
			{
				vImg.push_back(vMoveImg[m].GetString());
			}
			pData->mapMoveImg.insert(pData->mapMoveImg.begin(), pair<int, vector<string>>(j, vImg));
		}
		//爆炸图片
		rapidjson::Value& vBoomData = vData["boomImg"];
		for (int k=0;k<vBoomData.Size();++k)
		{
			pData->vecBoomImg.push_back(vBoomData[k].GetString());
		}
		vecBulletDt.push_back(pData);
	}

}

vector<string> CBulletDtMgr::getBulletMoveImgByIdAndGrade(const int & nid,const int& nGrade)
{
	for (sBulletDt *pData : vecBulletDt)
	{
		if (pData->nID == nid)
		{
			return pData->mapMoveImg[nGrade-1];//返回对应等级
		}
	}
	return vector<string>();
}

std::vector<std::string> CBulletDtMgr::getBulletBoomImgByID(const int& nid)
{
	for (sBulletDt *pData : vecBulletDt)
	{
		if (pData->nID == nid)
		{
			return pData->vecBoomImg;
		}
	}
	return vector<string>();
}

sBulletDt * CBulletDtMgr::getBulletDetailDtById(const int& nid)
{
	for (sBulletDt *pData:vecBulletDt)
	{
		if (pData->nID==nid)
		{
			return pData;
		}
	}
	return nullptr;
}


sBulletDt::sBulletDt()
{

}

CCardDtMgr::CCardDtMgr()
{
}

CCardDtMgr::~CCardDtMgr()
{
}

void CCardDtMgr::LoadFile(const char * pFileName)
{
	string strPath = FileUtils::getInstance()->fullPathForFilename(pFileName);
	string strData = FileUtils::getInstance()->getStringFromFile(strPath);

	rapidjson::Document doc;
	doc.Parse<0>(strData.c_str());
	sCardDt* pCard = new sCardDt;
	//范围卡片
	rapidjson::Value& vRangeData = doc["range"];
	for (int i=0;i<vRangeData.Size();++i)
	{
		pCard->vecRange.push_back(vRangeData[i].GetInt());
	}
	rapidjson::Value &vRangePng = doc["rangeimg"];
	for (int j=0;j<vRangePng.Size();++j)
	{
		pCard->vecRangeImg.push_back(vRangePng[j].GetString());
	}
	
	//奖励金币卡片
	rapidjson::Value &vRewardData=doc["rewardgold"];
	for (int i=0;i<vRewardData.Size();++i)
	{
		pCard->vecRewardGold.push_back(vRewardData[i].GetInt());
	}
	rapidjson::Value &vRewardImg = doc["rewardgoldimg"];
	for (int j=0;j<vRewardImg.Size();++j)
	{
		pCard->vecRewardGoldImg.push_back(vRewardImg[j].GetString());
	}

	//升级所需金币卡片
	rapidjson::Value &vUpgradeData = doc["upgradegold"];
	for (int i = 0; i < vUpgradeData.Size(); ++i)
	{
		pCard->vecUpgradeGold.push_back(vUpgradeData[i].GetInt());
	}
	rapidjson::Value &vUpgradeImg = doc["upgradegoldimg"];
	for (int j = 0; j < vUpgradeImg.Size(); ++j)
	{
		pCard->vecUpgradeGoldImg.push_back(vUpgradeImg[j].GetString());
	}

	//出售所值金币卡片

	rapidjson::Value &vSellData = doc["sellgold"];
	for (int i = 0; i < vSellData.Size(); ++i)
	{
		pCard->vecSellGold.push_back(vSellData[i].GetInt());
	}
	rapidjson::Value &vSellImg = doc["sellgoldimg"];
	for (int j = 0; j < vSellImg.Size(); ++j)
	{
		pCard->vecSellGoldImg.push_back(vSellImg[j].GetString());
	}

	//标签卡片

	rapidjson::Value &vMarkData = doc["bookmark"];
	for (int i = 0; i < vMarkData.Size(); ++i)
	{
		pCard->vecBookMark.push_back(vMarkData[i].GetInt());
	}
	rapidjson::Value &vMarkImg = doc["bookmarkimg"];
	for (int j = 0; j < vMarkImg.Size(); ++j)
	{
		pCard->vecBookMarkImg.push_back(vMarkImg[j].GetString());
	}

	//放到Map容器中
	for (int i=0;i<vRangeData.Size();++i)
	{
		mapRangeImg.insert(mapRangeImg.begin(), pair<int, string>(pCard->vecRange[i],pCard->vecRangeImg[i]));
	}
	for (int i = 0; i < vRewardData.Size(); ++i)
	{
		mapRewardImg.insert(mapRewardImg.begin(), pair<int, string>(pCard->vecRewardGold[i], pCard->vecRewardGoldImg[i]));
	}
	for (int i = 0; i < vUpgradeData.Size(); ++i)
	{
		mapUpgradeImg.insert(mapUpgradeImg.begin(), pair<int, string>(pCard->vecUpgradeGold[i], pCard->vecUpgradeGoldImg[i]));
	}
	for (int i = 0; i < vSellData.Size(); ++i)
	{
		mapSellImg.insert(mapSellImg.begin(), pair<int, string>(pCard->vecSellGold[i], pCard->vecSellGoldImg[i]));
	}
	for (int i = 0; i < vMarkData.Size(); ++i)
	{
		mapMarkImg.insert(mapMarkImg.begin(), pair<int, string>(pCard->vecBookMark[i], pCard->vecBookMarkImg[i]));
	}

}

std::string CCardDtMgr::getRangImgByRange(int nRange)
{
	return mapRangeImg[nRange];
}

string CCardDtMgr::getRewardImgByMoney(int nMoney)
{
	return mapRewardImg[nMoney];
}

std::string CCardDtMgr::getUpgradeImgByMoney(int nMoney)
{
	return mapUpgradeImg[nMoney];
}

std::string CCardDtMgr::getSellImgByMoney(int nMoney)
{
	return mapSellImg[nMoney];
}

string CCardDtMgr::getMarkImgByHasPass(int nHaspass)
{
	return mapMarkImg[nHaspass];
}

sCardDt::sCardDt()
{

}


CObstacleDtMgr::CObstacleDtMgr()
{
}

CObstacleDtMgr::~CObstacleDtMgr()
{
}

sObstacleDt::sObstacleDt()
{
}

void CObstacleDtMgr::LoadFile(const char * pFileName)
{
	string strPath = FileUtils::getInstance()->fullPathForFilename(pFileName);
	string strData = FileUtils::getInstance()->getStringFromFile(strPath);

	rapidjson::Document doc;
	doc.Parse<0>(strData.c_str());
	
	for (int i=0;i<doc.Size();++i)
	{
		rapidjson::Value &vData = doc[i];
		sObstacleDt *pData = new sObstacleDt;
		pData->type = vData["obstacletype"].GetInt();
		pData->hp = vData["hp"].GetInt();
		pData->nReward = vData["reward"].GetInt();
		vecObstacleDt.push_back(pData);
	}
	
}

sObstacleDt* CObstacleDtMgr::getObstacleDtByType(const int& ntype)
{
	for (sObstacleDt *pData:vecObstacleDt)
	{
		if (pData->type==ntype)
		{
			return pData;
		}
	}
	return 0;
}


CMonsterDtMgr::CMonsterDtMgr()
{
}

CMonsterDtMgr::~CMonsterDtMgr()
{
}

void CMonsterDtMgr::LoadFile(const char* pFileName)
{
	string strPath = FileUtils::getInstance()->fullPathForFilename(pFileName);
	string strData = FileUtils::getInstance()->getStringFromFile(strPath);

	rapidjson::Document doc;
	doc.Parse<0>(strData.c_str());

	for (int i=0;i<doc.Size();++i)
	{
		rapidjson::Value& vData = doc[i];
		sMonsterDt *pData = new sMonsterDt;
		pData->nID = vData["monsterid"].GetInt();
		pData->nHp = vData["hp"].GetInt();
		pData->nSpeed = vData["speed"].GetInt();

		rapidjson::Value& vMoveImg = vData["moveImg"];
		for (int j=0;j<vMoveImg.Size();++j)
		{
			pData->vecMoveImg.push_back(vMoveImg[j].GetString());
		}

		rapidjson::Value& vMusic = vData["deathmusic"];
		for (int n = 0; n < vMusic.Size(); ++n)
		{
			pData->deathMusic = vMusic[0].GetString();
		}
	
		

		pData->nReward = vData["reward"].GetInt();

		vecMonsterDt.push_back(pData);
	}
	
}

sMonsterDt * CMonsterDtMgr::getMonsterDtByID(const int & nType)
{
	for (sMonsterDt *pMonster:vecMonsterDt)
	{
		if (pMonster->nID==nType)
		{
			return pMonster;
		}
	}
	return nullptr;
}

std::vector<std::string> CMonsterDtMgr::getMonsterImgByID(const int& nType)
{
	for (sMonsterDt *pMoster:vecMonsterDt)
	{
		if (pMoster->nID == nType)
		{
			return pMoster->vecMoveImg;
		}
	}
	return vector<string>();
}

sMonsterDt::sMonsterDt()
{
	 nID = 0;
	 nHp = 0;
	 nSpeed = 0;
}
