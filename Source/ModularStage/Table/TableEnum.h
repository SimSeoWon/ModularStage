#pragma once

UENUM()
enum class EInGameTaskType : uint8
{
	NONE = 0,
	MISSION,
	SPAWN_OBJECT,
	SPAWN_MONSTER,

	// --- 확장 예정 ---
	PLAY_SEQUENCE,	// 시퀀스 플레이
	PLAY_DIALOG,	// 다이얼로그 플레이

	MAX,
};

UENUM()
enum class EMissionObjectiveType : uint8
{
	None,
	Destination,	// 목적지 도달
	Annihilation,	// 적 섬멸
	Escort,			// 호위 미션 (확장 예정)
	Defense,		// 디펜스 미션 (확장 예정)
};

UENUM()
enum class EInGameSpawnType :uint8
{
	NONE = 0,
	MONSTER,
	OBJECT,
	MAX,
};

UENUM()
enum class ETaskExecutorResult : uint8
{
	NONE,
	Complete,
	Fail,
	Progress,
	Max,
};

UENUM()
enum class EMissionType : uint8
{
	Main,	// 순차적(직렬) 실행
	Sub,	// 즉시(병렬) 실행
};

UENUM()
enum class EMissionExecutorState : uint8
{
	None,
	Ready,
	Running,
	Completed,
};

UENUM(BlueprintType)
enum class EHexagonTileType : uint8
{
	NONE = 0,
	Empty,
	Plain,
	Marsh,
	Water,
	MAX,
};

UENUM(BlueprintType)
enum class EHexagonTileHeight : uint8
{
	NONE = 0,
	Underground, // 지하
	SeaLevel, // 해수면
	Ground_Zero, // 지표면_0 레벨
	Ground_One, // 지표면_1 레벨
	MAX,
};
