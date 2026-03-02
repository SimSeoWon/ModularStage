
#include "EditorWidgetBase.h"
#include "Misc/CoreDelegates.h" // 필수
#include "Delegates/IDelegateInstance.h"
// Intentionally left blank. 
// All template and static functions are defined in the header for now.
void UEditorWidgetBase::NativeConstruct()
{
    Super::NativeConstruct();

    // "에디터 꺼질 때 저 좀 불러주세요" (직접 등록)
    if (!OnExitHandle.IsValid())
    {
        OnExitHandle = FCoreDelegates::OnPreExit.AddUObject(this, &UEditorWidgetBase::OnEditorExit);
    }
}

void UEditorWidgetBase::BeginDestroy()
{
    // 혹시 에디터 종료가 아니라 그냥 탭만 닫은 거라면, 
    // 등록해둔 델리게이트를 취소해야 합니다.
    if (OnExitHandle.IsValid())
    {
        FCoreDelegates::OnPreExit.Remove(OnExitHandle);
        OnExitHandle.Reset();
    }

    // 탭만 닫을 때를 대비해 여기서도 정리 시도
    OnEditorExit();

    Super::BeginDestroy();
}

void UEditorWidgetBase::OnEditorExit()
{
    OnEditorExit_Implement();
}

void UEditorWidgetBase::OnEditorExit_Implement() 
{

}