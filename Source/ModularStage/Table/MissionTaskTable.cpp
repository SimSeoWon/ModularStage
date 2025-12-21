#include "MissionTaskTable.h"


FMissionTaskInfo::FMissionTaskInfo()
{
	Step = 0;
	Title = TEXT("");
	Desc = TEXT("");
}

FMissionTaskInfo::FMissionTaskInfo(const FMissionTaskInfo& obj)
{
	Step = obj.Step;
	Title = obj.Title;
	Desc = obj.Desc;

	IntegerParamList.Reset();
	IntegerParamList.Reserve(obj.IntegerParamList.Num());
	for (auto iter : obj.IntegerParamList)
	{
		IntegerParamList.Add(iter);
	}
}