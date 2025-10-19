#pragma once

UENUM()
enum class EInGameTaskType : uint8 /**인게임 태스크 타입*/
{
	NONE = 0,
	MISSION,		// 미션 추가.
	SPAWN_PLAYER,	// 플레이어 스폰
	SPAWN_MONSTER,	// 몬스터 스폰
	MARCH_ON,		// 액터 이동
	MAX,
};

UENUM()
enum class EInGameSpawnType :uint8
{
	NONE = 0,
	MONSTER,
	DIALOG,
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

