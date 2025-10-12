
#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Editor.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "EditorWidgetBase.generated.h"

class FName;

UCLASS(Abstract)
class MODULARSTAGEEDITOR_API UEditorWidgetBase : public UEditorUtilityWidget
{
    GENERATED_BODY()

public:
    // Default implementation for menu name.
    static FName GetMenuName() { return FName(TEXT("Default Editor")); }
    
    // Default implementation for menu display name. Child classes should hide this with their own implementation.
    static FText GetMenuDisplayName() { return FText::FromString(TEXT("Default Editor")); }

    // Default implementation for menu tooltip.
    static FText GetMenuTooltip() { return FText::FromString(TEXT("Default Editor")); }

    // Default implementation for the widget blueprint path. Child classes should hide this.
    static FString GetWidgetBlueprintPath() { return FString(); }

    /**
     * Shows the editor utility widget.
     * This function finds the blueprint specified by the static GetWidgetBlueprintPath function,
     * loads it, and then opens it in a new or existing editor tab.
     * @return A pointer to the created widget instance.
     */
    template<typename TEditorWidget>
    static TEditorWidget* ShowEditorUtilityWidget()
    {
        const FString WidgetBlueprintPath = TEditorWidget::GetWidgetBlueprintPath();
        if (WidgetBlueprintPath.IsEmpty())
        {
            UE_LOG(LogTemp, Error, TEXT("WidgetBlueprintPath is empty for class %s"), *TEditorWidget::StaticClass()->GetName());
            return nullptr;
        }

        FSoftObjectPath ObjectPath;
        ObjectPath.SetPath(WidgetBlueprintPath);

        UObject* object = ObjectPath.TryLoad();
        if (false == IsValid(object))
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load widget blueprint: %s"), *WidgetBlueprintPath);
            return nullptr;
        }

        UEditorUtilityWidgetBlueprint* EditorWidgetBlueprint = Cast<UEditorUtilityWidgetBlueprint>(object);
        if (false == IsValid(EditorWidgetBlueprint))
        {
            UE_LOG(LogTemp, Error, TEXT("Loaded object is not a UEditorUtilityWidgetBlueprint: %s"), *WidgetBlueprintPath);
            return nullptr;
        }

        UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
        if (false == IsValid(EditorUtilitySubsystem))
        {
            UE_LOG(LogTemp, Error, TEXT("Could not get EditorUtilitySubsystem."));
            return nullptr;
        }

        FName TabID;
        EditorUtilitySubsystem->SpawnAndRegisterTabAndGetID(EditorWidgetBlueprint, TabID);

        UEditorUtilityWidget* CreatedWidget = EditorWidgetBlueprint->GetCreatedWidget();
        return Cast<TEditorWidget>(CreatedWidget);
    }
};
