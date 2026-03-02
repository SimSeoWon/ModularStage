// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ModularStageCharacter.generated.h"

class UDataTable;
struct FCharacterStateConfig;

UCLASS(Blueprintable)
class AModularStageCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AModularStageCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** 특정 상호작용 상태로 진입합니다. */
	UFUNCTION(BlueprintCallable, Category = "ActionState")
	void EnterActionState(FName StateID);

	/** 현재 상호작용 상태를 해제합니다. */
	UFUNCTION(BlueprintCallable, Category = "ActionState")
	void ExitActionState();

	/** 현재 상호작용 상태 중인지 확인합니다. */
	UFUNCTION(BlueprintPure, Category = "ActionState")
	bool IsInActionState() const { return bIsInActionState; }

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

protected:
	/** 상호작용 상태 데이터 테이블 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionState")
	TSoftObjectPtr<UDataTable> ActionDataTable;

private:
	/** 현재 적용 중인 상태 설정 */
	const FCharacterStateConfig* CurrentStateConfig = nullptr;

	/** 현재 상태가 유효한지 여부 */
	bool bIsInActionState = false;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
};

