#pragma once

UENUM()
enum class EInGameTaskType : uint8
{
	NONE = 0,
	MISSION,		
	SPAWN_PLAYER,	
	SPAWN_MONSTER,	
	MARCH_ON,		
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

