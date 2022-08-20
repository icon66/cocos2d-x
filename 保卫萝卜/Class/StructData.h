#pragma once
#include "string"
#include <vector>
#include <map>
#include "cocos2d.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

USING_NS_CC;

using namespace std;

struct sResDt
{
	sResDt();
	vector<string> vecPng;
	vector<string> vecPlist;
	void LoadFile(const char *pFileName);
	void UpdateFile(const int& nTheme);
};


//地图数据（主题）
struct sMapDt
{
	sMapDt();
	int nThemeID;
	vector<string> vecLevelPng;
	vector<string> vecCloudPng;
	string sMusic;
	int nLock;
};

class CMapDtMgr
{
public:
	CMapDtMgr();
	~CMapDtMgr();
	void LoadFile(const char* pFileName);
	void setCurTheme(int nIndex)
	{
		curTheme = nIndex;
	}
	int getCurTheme()
	{
		return curTheme;
	}
	int getCurThemeLockByID(const int& nID);
	vector<string> getCurThemeMap();
	vector<string> getCurThemeBg();
	string getThemeMusic();
private:
	vector<sMapDt*> vecMapDt;
	int curTheme;
};

//关卡数据
struct sLevelDt
{
	sLevelDt();
	int nTheme;
	map<int,vector<int>> mapWeaponId;
	map<int, vector<string>> mapObstaclePng;
	map<int, vector<int>> mapMonsterId;
	map<int, vector<int>>mapWaveCount;
	map<int, vector<int>> mapHonor;
	vector<int> vecWaveNum;
	vector<int> vecInitGold;
	vector<int> vecLock;
	vector<string> vecTiledMap;
	int nPassLevelCount;
};

class CLevelDtMgr
{
public:
	CLevelDtMgr();
	~CLevelDtMgr();
	void LoadFile(const char *pFileName);
	void UpdateFile(int nTheme,int nLevel,vector<int> vecHonor);
	void setCurTheme(int nIndex)
	{
		nCurTheme = nIndex;
	}
	void setCurLevel(int nIndex)
	{
		nCurLevel = nIndex;
	}
	int getCurLevel()
	{
		return nCurLevel;
	}
	int getCurLevelWave();
	vector<int> getCurLevelWeapon(); //获得当前关卡的武器0

	vector<string> getCurLevelObstacle(); //得到当前关卡的障碍物
	vector<int> getCurLevelMonsterType(); //得到当前关卡怪物的类型
	vector<int> getCurLevelWaveMonsterCount(); //得到当前关卡每一波怪物的数量
	vector<int> getCurLevelHonorByID(const int& nID);//获得当前关卡的荣誉
	int getPassLevelCountByTheme(const int&nID); //获得已过关卡的数量
	int getCurLevelLockByID(const int& nID); //得到当前关卡是否锁定
	int getCurLevelInitGold(); //得到当前关卡初始金币
	string getCurTiledMap();
private:
	vector<sLevelDt*> vecLevelDt;
	int nCurTheme;
	int nCurLevel;
};


//武器数据
struct sWeaponDt
{
	sWeaponDt();
	int nID;
	vector<int> vecAttackRange;
	vector<float> vecShootSpeed;
	vector<string> vecShopImg;
	map<int,vector<string>> mapImg;
	vector <int> vecCreatePrice;
	vector <int> vecSellPrice;
	string sMusic;
	string sIcon;
};

class CWeaponDtMgr
{
public:
	CWeaponDtMgr();
	~CWeaponDtMgr();
	void LoadFile(const char* pFileName);
	vector<string> getWeaponImgDtByIdAndGrade(const int& nid,const int& ngrade);
	sWeaponDt *getWeaponDetailDtByID(const int &nid);
	int getWeaponRangeDtByTypeAndGrade(const int& nType, const int& nGrade);
	float getWeaponShootSpeedDtByTypeAndGrade(const int& nType, const int& nGrade);
private:
	vector<sWeaponDt*> vecWeaponDt;
};


//子弹数据
struct sBulletDt 
{
	sBulletDt();
	int nID;
	int nAck;
	int bIsRotate;
	float fSpeedBuff;
	map<int, vector<string>> mapMoveImg;
	vector<string> vecBoomImg;
};

class CBulletDtMgr
{
public:
	CBulletDtMgr();
	~CBulletDtMgr();
	void LoadFile(const char *pFileName);
	vector<string> getBulletMoveImgByIdAndGrade(const int& nid,const int& nGrade);
	vector<string> getBulletBoomImgByID(const int& nid);
	sBulletDt *getBulletDetailDtById(const int& nid);
private:
	vector<sBulletDt*> vecBulletDt;
};


//卡片数据
struct sCardDt
{
	sCardDt();
	//范围卡片
	vector<int> vecRange;
	vector<string> vecRangeImg;
	//奖励金币卡片
	vector<int> vecRewardGold;
	vector<string> vecRewardGoldImg;
	//升级金币卡片
	vector<int> vecUpgradeGold;
	vector<string> vecUpgradeGoldImg;
	//出售金币卡片
	vector<int> vecSellGold;
	vector<string> vecSellGoldImg;

	vector<int> vecBookMark;
	vector<string> vecBookMarkImg;
	
};

class CCardDtMgr
{
public:
	CCardDtMgr();
	~CCardDtMgr();
	void LoadFile(const char *pFileName);
	string getRangImgByRange(int nRange);//拿到对应范围的图片
	string getRewardImgByMoney(int nMoney);
	string getUpgradeImgByMoney(int nMoney);
	string getSellImgByMoney(int nMoney);
	string getMarkImgByHasPass(int nHaspass);
private:
	map<int, string> mapRangeImg;
	map<int, string> mapRewardImg;
	map<int, string> mapUpgradeImg;
	map<int, string> mapSellImg;
	map<int, string> mapMarkImg;
};



//障碍物数据
struct sObstacleDt
{
	sObstacleDt();
	int type;
	int hp;
	int nReward;
};

class CObstacleDtMgr
{
public:
	CObstacleDtMgr();
	~CObstacleDtMgr();
	void LoadFile(const char*pFileName);
	sObstacleDt* getObstacleDtByType(const int& nty);
private:
	vector<sObstacleDt*> vecObstacleDt;
};


//怪物数据
struct sMonsterDt
{
	sMonsterDt();
	int nID;
	int nHp;
	int nSpeed;
	int nReward;
	vector<string> vecMoveImg;
	string deathMusic;
};

class CMonsterDtMgr
{
public:
	CMonsterDtMgr();
	~CMonsterDtMgr();
	void LoadFile(const char* pFileName);
	sMonsterDt *getMonsterDtByID(const int& nType);
	vector<string> getMonsterImgByID(const int& nType);
private:
	vector<sMonsterDt*> vecMonsterDt;
};


