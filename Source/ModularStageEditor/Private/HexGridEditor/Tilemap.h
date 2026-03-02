#pragma once



#include <vector>
#include <unordered_map>


// TileMap
enum class ETileVersionTag : int32
{
	LastVersion = 106,
};

extern const int32 MAX_ZONE_LAYER;

extern uint32 MakeVersionCode(int32 Ver);

const float TILE_RESOLUTION = 50.f;

struct FTileHeader
{
	uint32 Version;
	int32 TileDataSize;
	int32 ZonePaletteCount;
	int32 PCTerritoryPaletteCount;
	int32 NPCTerritoryPaletteCount;
	int32 TriggerPaletteCount;

	FTileHeader()
		: Version(0)
		, TileDataSize(0)
		, ZonePaletteCount(0)
		, PCTerritoryPaletteCount(0)
		, NPCTerritoryPaletteCount(0)
		, TriggerPaletteCount(0)
	{
	}
};


struct FTilePalette
{
	std::vector<uint32> IDs;

	FTilePalette()
	{
	}

	void Resize(int32 newSize)
	{
		std::vector<uint32> NewContainer(newSize);
		IDs = NewContainer;
	}
};


struct FTileAreaInfo
{
	FTileHeader Header;
	int32 TileWidth;
	int32 TileHeight;
	float OriginTileX;
	float OriginTileY;
	uint8* ZoneDatas;
	uint8* PCTerritoryDatas;
	uint16* NPCTerritoryDatas;
	uint8* TriggerDatas;
	FTilePalette* ZonePalettes;
	FTilePalette* PCTerritoryPalettes;
	FTilePalette* NPCTerritoryPalettes;
	FTilePalette* TriggerPalettes;

	FTileAreaInfo()
		: Header()
		, TileWidth(0)
		, TileHeight(0)
		, OriginTileX(0)
		, OriginTileY(0)
		, ZoneDatas(nullptr)
		, PCTerritoryDatas(nullptr)
		, NPCTerritoryDatas(nullptr)
		, TriggerDatas(nullptr)
		, ZonePalettes(nullptr)
		, PCTerritoryPalettes(nullptr)
		, NPCTerritoryPalettes(nullptr)
		, TriggerPalettes(nullptr)
	{
	}

	~FTileAreaInfo()
	{
		if (ZoneDatas != nullptr)
			delete[] ZoneDatas;
		if (PCTerritoryDatas != nullptr)
			delete[] PCTerritoryDatas;
		if (NPCTerritoryDatas != nullptr)
			delete[] NPCTerritoryDatas;
		if (TriggerDatas != nullptr)
			delete[] TriggerDatas;

		if (ZonePalettes != nullptr)
			delete[] ZonePalettes;
		if (PCTerritoryPalettes != nullptr)
			delete[] PCTerritoryPalettes;
		if (NPCTerritoryPalettes != nullptr)
			delete[] NPCTerritoryPalettes;
		if (TriggerPalettes != nullptr)
			delete[] TriggerPalettes;
	}

	int32 ConvertPositionToTileIndex(const float* Position) const;
	void ConvertTileIndexToPosition(const int32 tileIndex, float* outputPosition) const;

	bool Write(const char* Filename);
	bool Read(const char* Filename);

	bool CheckValidTileIndex(int32 TileIndex);
	const std::vector<uint32>* GetZoneInfo(const float* Position);
	const std::vector<uint32>* GetPCTerritoryInfo(const float* Position);
	const std::vector<uint32>* GetNPCTerritoryInfo(const float* Position);
	const std::vector<uint32>* GetTriggerInfo(const float* Position);
};