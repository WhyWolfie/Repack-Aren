#ifndef _MMATCHMAP_H
#define _MMATCHMAP_H

#include "MMatchGlobal.h"


enum MMATCH_MAP 
{
	MMATCH_MAP_MANSION			= 0,
	MMATCH_MAP_PRISON			= 1,
	MMATCH_MAP_STATION			= 2,
	MMATCH_MAP_PRISON_II		= 3,
	MMATCH_MAP_BATTLE_ARENA		= 4,
	MMATCH_MAP_TOWN				= 5,
	MMATCH_MAP_DUNGEON			= 6,
	MMATCH_MAP_RUIN				= 7,
	MMATCH_MAP_ISLAND			= 8,
	MMATCH_MAP_GARDEN			= 9,
	MMATCH_MAP_CASTLE			= 10,
	MMATCH_MAP_FACTORY			= 11,
	MMATCH_MAP_PORT				= 12,
	MMATCH_MAP_LOST_SHRINE		= 13,
	MMATCH_MAP_STAIRWAY			= 14,
	MMATCH_MAP_SNOWTOWN			= 15,
	MMATCH_MAP_HALL				= 16,
	MMATCH_MAP_CATACOMB			= 17,
	MMATCH_MAP_JAIL				= 18,
	MMATCH_MAP_SHOWERROOM		= 19,
	MMATCH_MAP_HIGH_HAVEN		= 20,
	MMATCH_MAP_CITADEL			= 21,
	MMATCH_MAP_RELAYMAP			= 22,
	MMATCH_MAP_HALLOWEEN_TOWN	= 23,
	MMATCH_MAP_WEAPON_SHOP		= 24,
	MMATCH_MAP_SkillMapV1		= 25,
	MMATCH_MAP_SkillMapV2		= 26,
	MMATCH_MAP_SkillMapV3		= 27,
	MMATCH_MAP_SkillMapV4		= 28,
	MMATCH_MAP_SkillMapV5		= 29,
	MMATCH_MAP_SkillMapV6		= 30,
	MMATCH_MAP_SkillMapV7		= 31,	
	MMATCH_MAP_Bunker			= 32,
	MMATCH_MAP_Coliseo			= 33,
	MMATCH_MAP_DodgeBall		= 34,
	MMATCH_MAP_Hot_Summer		= 35,
	MMATCH_MAP_Iceworld			= 36,
	MMATCH_MAP_lostHospital		= 37,
	MMATCH_MAP_Old_Mansion		= 38,
	MMATCH_MAP_Panic_Room		= 39,
	MMATCH_MAP_Resurrected		= 40,
	MMATCH_MAP_Skyline			= 41,
	MMATCH_MAP_TheOffice		= 42,	
	MMATCH_MAP_TheTunnel		= 43,
	MMATCH_MAP_Tower			= 44,	
	MMATCH_MAP_Toy_Room			= 45,	
	MMATCH_MAP_Vice				= 46,
	MMATCH_MAP_AngryBirds_WH		= 47,
	MMATCH_MAP_House_Simpsons		= 48,
	MMATCH_MAP_Shadow_Mansion		= 49,
	MMATCH_MAP_Cs_assaultV2			= 50,
	MMATCH_MAP_Cs_assaultV3			= 51,
	MMATCH_MAP_WeaponShop_ACT		= 52,
	MMATCH_MAP_brokenreed			= 53,
	MMATCH_MAP_1vs1					= 54,
	MMATCH_MAP_de_dust2				= 55,
	MMATCH_MAP_Mario_Castle			= 56,
	MMATCH_MAP_Athena				= 57,
	MMATCH_MAP_DBZ					= 58,
	MMATCH_MAP_Dojo					= 59,
	MMATCH_MAP_dusterskilltrail_v4	= 60,
	MMATCH_MAP_Future_Duel			= 61,
	MMATCH_MAP_Halloween_Town		= 62,
	MMATCH_MAP_Superflip			= 63,
	MMATCH_MAP_WeaponShop			= 64,
	MMATCH_MAP_DussaPoRace			= 65,
	
	MMATCH_MAP_MAX = 100,
};

#define MMATCH_MAP_COUNT	MMATCH_MAP_MAX			// ��ü �� ����

#define MMATCH_MAPNAME_RELAYMAP				"RelayMap"


class MMapDesc
{
private:
	const struct MapInfo
	{
		int			nMapID;							// map id
		char		szMapName[MAPNAME_LENGTH];		// �� �̸�
		char		szMapImageName[MAPNAME_LENGTH];	// �� �̹��� �̸�
		char		szBannerName[MAPNAME_LENGTH];	// ���� �̸�
		float		fExpRatio;						// ����ġ �����
		int			nMaxPlayers;					// �ִ� �ο�
		bool		bOnlyDuelMap;					// ���� ����
		bool		bIsCTFMap;
		bool		bIsSWRMap;
	};


	// data
	MapInfo	m_MapVectors[MMATCH_MAP_COUNT];
	MMapDesc();
public:
	~MMapDesc() { }
	
	static MMapDesc* GetInstance();

	bool Initialize(const char* szFileName);
	bool Initialize(MZFileSystem* pfs, const char* szFileName);
	bool MIsCorrectMap(const int nMapID);
	

 
	bool IsMapOnlyDuel( const int nMapID);
	bool IsCTFMap( const int nMapID);
	bool IsSWRMap( const int nMapID);
	int GetMapID( const int nMapID);
	const char* GetMapName(const int nMapID);
	const char* GetMapImageName(const char* szMapName);
	const char* GetBannerName(const char* szMapName);
	float GetExpRatio( const int nMapID); 
	int GetMaxPlayers( const int nMapID);

	int GetMapCount(){ return MMATCH_MAP_COUNT; }
};


inline MMapDesc* MGetMapDescMgr() 
{ 
	return MMapDesc::GetInstance();
}



#endif