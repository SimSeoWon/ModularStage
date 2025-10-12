// Fill out your copyright notice in the Description page of Project Settings.


#include "EUW_InGameTask.h"
#include "EditorView_InGameTask.h"

void UEUW_InGameTask::Run()
{
	Super::Run();

	if (false == IsValid(Edit_InGameTask))
		return;

	Edit_InGameTask->Run();
}
