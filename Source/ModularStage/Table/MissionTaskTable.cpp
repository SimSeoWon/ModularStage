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
	Type = obj.Type;
	ParentID = obj.ParentID;
	SerializeID = obj.SerializeID;
	ObjectiveType = obj.ObjectiveType;

	TaskDetail = obj.TaskDetail;
}