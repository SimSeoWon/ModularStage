#include "MissionTaskTable.h"


FMissionTaskInfo::FMissionTaskInfo()
{
	Step = 0; //진행도
	Title = TEXT(""); // 타이틀
	Desc = TEXT(""); // 설명
}

FMissionTaskInfo::FMissionTaskInfo(const FMissionTaskInfo& obj)
{
	Step = obj.Step; //진행도
	Title = obj.Title; // 타이틀
	Desc = obj.Desc; // 설명

	IntegerParamList.Reset();
	IntegerParamList.Reserve(obj.IntegerParamList.Num());
	for (auto iter : obj.IntegerParamList)
	{
		IntegerParamList.Add(iter);
	}
}