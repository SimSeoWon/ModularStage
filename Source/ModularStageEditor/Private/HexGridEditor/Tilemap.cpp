#include "Tilemap.h"


uint32 MakeVersionCode(int32 Ver)
{
	static const char Word[6] = { 'P', 'R', 'T', 'I', 'L', 'E' };

	uint32 Result = Ver & 0xff;

#define WORD_CODE(C) (C & 0xf)
	for (int i = 0; i < 6; ++i)
		Result |= WORD_CODE(Word[i]) << (8 + (i * 4));
#undef WORD_CODE

	return Result;
}

int32 FTileAreaInfo::ConvertPositionToTileIndex(const float* Position) const
{
	if (Position == nullptr)
		return -1;

	// 타일인덱스를 구하자. / cm -> m 단위 변환
	int32 X = -(int32)(ceil((Position[0] - OriginTileX) / TILE_RESOLUTION));
	int32 Z = -(int32)(floor((Position[2] - OriginTileY) / TILE_RESOLUTION));

	if (X < 0 || Z < 0)
		return -1;

	return (Z * TileWidth) + X;
}

void FTileAreaInfo::ConvertTileIndexToPosition(const int32 tileIndex, float* outputPosition) const
{
	if (outputPosition == nullptr)
		return;

	outputPosition[0] = 0;
	outputPosition[1] = 0;
	outputPosition[2] = 0;

	if (tileIndex < 0 || tileIndex >= (TileWidth * TileHeight))
		return;

	outputPosition[0] = (-(tileIndex % TileWidth) * TILE_RESOLUTION) + OriginTileX;
	outputPosition[2] = (-(tileIndex / TileWidth) * TILE_RESOLUTION) + OriginTileY;
}

bool FTileAreaInfo::Write(const char* Filename)
{
#if PLATFORM_WINDOWS
	if (Filename == nullptr || Filename[0] == '\0')
		return false;

	FILE* fp = nullptr;
	fopen_s(&fp, Filename, "wb");
	if (fp == nullptr)
		return false;

	// 헤더 저장
	fwrite(&Header, sizeof(FTileHeader), 1, fp);
	// 타일 데이터 저장
	fwrite(&TileWidth, sizeof(int32), 1, fp);
	fwrite(&TileHeight, sizeof(int32), 1, fp);
	fwrite(&OriginTileX, sizeof(int32), 1, fp);
	fwrite(&OriginTileY, sizeof(int32), 1, fp);
	fwrite(ZoneDatas, sizeof(uint8), Header.TileDataSize, fp);
	fwrite(PCTerritoryDatas, sizeof(uint8), Header.TileDataSize, fp);
	fwrite(NPCTerritoryDatas, sizeof(uint16), Header.TileDataSize, fp);
	fwrite(TriggerDatas, sizeof(uint8), Header.TileDataSize, fp);

	// 타일 팔레트 저장
	for (int32 i = 0; i < Header.ZonePaletteCount; ++i)
	{
		int32 size = (int32)ZonePalettes[i].IDs.size();
		fwrite(&size, sizeof(int32), 1, fp);
		for (int32 j = 0; j < size; ++j)
			fwrite(&ZonePalettes[i].IDs[j], sizeof(uint32), 1, fp);
	}
	for (int32 i = 0; i < Header.PCTerritoryPaletteCount; ++i)
	{
		int32 size = (int32)PCTerritoryPalettes[i].IDs.size();
		fwrite(&size, sizeof(int32), 1, fp);
		for (int32 j = 0; j < size; ++j)
			fwrite(&PCTerritoryPalettes[i].IDs[j], sizeof(uint32), 1, fp);
	}
	for (int32 i = 0; i < Header.NPCTerritoryPaletteCount; ++i)
	{
		int32 size = (int32)NPCTerritoryPalettes[i].IDs.size();
		fwrite(&size, sizeof(int32), 1, fp);
		for (int32 j = 0; j < size; ++j)
			fwrite(&NPCTerritoryPalettes[i].IDs[j], sizeof(uint32), 1, fp);
	}
	for (int32 i = 0; i < Header.TriggerPaletteCount; ++i)
	{
		int32 size = (int32)TriggerPalettes[i].IDs.size();
		fwrite(&size, sizeof(int32), 1, fp);
		for (int32 j = 0; j < size; ++j)
			fwrite(&TriggerPalettes[i].IDs[j], sizeof(uint32), 1, fp);
	}

	fclose(fp);
#endif
	return true;
}


bool FTileAreaInfo::Read(const char* Filename)
{
#if PLATFORM_WINDOWS
	if (Filename == nullptr || Filename[0] == '\0')
		return false;

	FILE* fp = nullptr;
	fopen_s(&fp, Filename, "rb");
	if (fp == nullptr)
		return false;

	// 헤더 읽기
	fread(&Header, sizeof(FTileHeader), 1, fp);

	// 헤더 버전 체크
	if (Header.Version != MakeVersionCode((int32)ETileVersionTag::LastVersion))
	{
		// 버전이 일치하지 않는다. 읽기 실패
		fclose(fp);
		return false;
	}

	// 타일 데이터 읽기
	fread(&TileWidth, sizeof(int32), 1, fp);
	fread(&TileHeight, sizeof(int32), 1, fp);
	fread(&OriginTileX, sizeof(int32), 1, fp);
	fread(&OriginTileY, sizeof(int32), 1, fp);

	ZoneDatas = new uint8[Header.TileDataSize];
	if (ZoneDatas == nullptr)
	{
		fclose(fp);
		return false;
	}
	fread(ZoneDatas, sizeof(uint8), Header.TileDataSize, fp);

	PCTerritoryDatas = new uint8[Header.TileDataSize];
	if (PCTerritoryDatas == nullptr)
	{
		fclose(fp);
		return false;
	}
	fread(PCTerritoryDatas, sizeof(uint8), Header.TileDataSize, fp);

	NPCTerritoryDatas = new uint16[Header.TileDataSize];
	if (NPCTerritoryDatas == nullptr)
	{
		fclose(fp);
		return false;
	}
	fread(NPCTerritoryDatas, sizeof(uint16), Header.TileDataSize, fp);

	TriggerDatas = new uint8[Header.TileDataSize];
	if (TriggerDatas == nullptr)
	{
		fclose(fp);
		return false;
	}
	fread(TriggerDatas, sizeof(uint8), Header.TileDataSize, fp);


	// 타일 팔레트 읽기
	ZonePalettes = new FTilePalette[Header.ZonePaletteCount];
	if (ZonePalettes == nullptr)
	{
		fclose(fp);
		return false;
	}

	PCTerritoryPalettes = new FTilePalette[Header.PCTerritoryPaletteCount];
	if (PCTerritoryPalettes == nullptr)
	{
		fclose(fp);
		return false;
	}

	NPCTerritoryPalettes = new FTilePalette[Header.NPCTerritoryPaletteCount];
	if (NPCTerritoryPalettes == nullptr)
	{
		fclose(fp);
		return false;
	}

	TriggerPalettes = new FTilePalette[Header.TriggerPaletteCount];
	if (TriggerPalettes == nullptr)
	{
		fclose(fp);
		return false;
	}


	for (int32 i = 0; i < Header.ZonePaletteCount; ++i)
	{
		int32 size = 0;
		uint32 value = 0;
		fread(&size, sizeof(int32), 1, fp);
		ZonePalettes[i].Resize(size);
		for (int32 j = 0; j < size; ++j)
		{
			fread(&value, sizeof(uint32), 1, fp);
			ZonePalettes[i].IDs[j] = value;
		}
	}
	for (int32 i = 0; i < Header.PCTerritoryPaletteCount; ++i)
	{
		int32 size = 0;
		uint32 value = 0;
		fread(&size, sizeof(int32), 1, fp);
		PCTerritoryPalettes[i].Resize(size);
		for (int32 j = 0; j < size; ++j)
		{
			fread(&value, sizeof(uint32), 1, fp);
			PCTerritoryPalettes[i].IDs[j] = value;
		}
	}
	for (int32 i = 0; i < Header.NPCTerritoryPaletteCount; ++i)
	{
		int32 size = 0;
		uint32 value = 0;
		fread(&size, sizeof(int32), 1, fp);
		NPCTerritoryPalettes[i].Resize(size);
		for (int32 j = 0; j < size; ++j)
		{
			fread(&value, sizeof(uint32), 1, fp);
			NPCTerritoryPalettes[i].IDs[j] = value;
		}
	}
	for (int32 i = 0; i < Header.TriggerPaletteCount; ++i)
	{
		int32 size = 0;
		uint32 value = 0;
		fread(&size, sizeof(int32), 1, fp);
		TriggerPalettes[i].Resize(size);
		for (int32 j = 0; j < size; ++j)
		{
			fread(&value, sizeof(uint32), 1, fp);
			TriggerPalettes[i].IDs[j] = value;
		}
	}

	fclose(fp);
#endif
	return true;
}

bool FTileAreaInfo::CheckValidTileIndex(int32 TileIndex)
{
	if (Header.TileDataSize == 0)
	{
		return false;
	}

	if (TileIndex < 0 || TileIndex >= Header.TileDataSize)
	{
		return false;
	}

	return true;
}

#define GET_TILE_PALETTE_IDS(Name) \
	if (!CheckValidTileIndex(TileIndex)) \
	{ \
		return nullptr; \
	} \
	\
	if (Header.Name##PaletteCount == 0 || \
		Name##Datas == nullptr || Name##Palettes == nullptr) \
	{ \
		return nullptr; \
	} \
	\
	if (Name##Datas[TileIndex] >= Header.Name##PaletteCount) \
	{ \
		return nullptr; \
	} \
	\
	int32 PaletteIndex = Name##Datas[TileIndex]; \
	const FTilePalette& Palette = Name##Palettes[PaletteIndex]; \
	return &(Palette.IDs); 


const std::vector<uint32>* FTileAreaInfo::GetZoneInfo(const float* Position)
{
	int32 TileIndex = ConvertPositionToTileIndex(Position);
	GET_TILE_PALETTE_IDS(Zone);
}

const std::vector<uint32>* FTileAreaInfo::GetPCTerritoryInfo(const float* Position)
{
	int32 TileIndex = ConvertPositionToTileIndex(Position);
	GET_TILE_PALETTE_IDS(PCTerritory);
}

const std::vector<uint32>* FTileAreaInfo::GetNPCTerritoryInfo(const float* Position)
{
	int32 TileIndex = ConvertPositionToTileIndex(Position);
	GET_TILE_PALETTE_IDS(NPCTerritory);
}


const std::vector<uint32>* FTileAreaInfo::GetTriggerInfo(const float* Position)
{
	int32 TileIndex = ConvertPositionToTileIndex(Position);
	GET_TILE_PALETTE_IDS(Trigger);
}