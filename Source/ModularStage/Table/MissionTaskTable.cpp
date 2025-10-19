#include "MissionTaskTable.h"


FMissionTaskInfo::FMissionTaskInfo()
{
	Step = 0; //���൵
	Title = TEXT(""); // Ÿ��Ʋ
	Desc = TEXT(""); // ����
}

FMissionTaskInfo::FMissionTaskInfo(const FMissionTaskInfo& obj)
{
	Step = obj.Step; //���൵
	Title = obj.Title; // Ÿ��Ʋ
	Desc = obj.Desc; // ����

	IntegerParamList.Reset();
	IntegerParamList.Reserve(obj.IntegerParamList.Num());
	for (auto iter : obj.IntegerParamList)
	{
		IntegerParamList.Add(iter);
	}
}