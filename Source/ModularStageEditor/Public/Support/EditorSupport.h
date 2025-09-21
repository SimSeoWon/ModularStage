// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Editor/UnrealEdTypes.h"
#include "EditorUtilityWidget.h"

/**
 *
 */
class FEditorSupport
{
public:
	static UEditorUtilityWidget* TryLoad(FString inPath, FName& inTabID);
	
	template<typename T>
	static T* ShowEditorUtilityWidget(FString inPath, FName& inTabID)
	{
		UEditorUtilityWidget* editorWidget = FEditorSupport::TryLoad(inPath, inTabID);
		if (false == IsValid(editorWidget))
			return nullptr;
		
		T* editor = Cast<T>(editorWidget);
		if (false == IsValid(editor))
			return nullptr;

		return editor;
	}



protected:
	
};