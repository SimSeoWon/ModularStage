// Copyright Epic Games, Inc. All Rights Reserved.

#include "ModularStageCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Engine/LocalPlayer.h"
#include "Engine/DataTable.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Animation/AnimMontage.h"

#include "Table/CharacterActionTable.h"

AModularStageCharacter::AModularStageCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	bIsInActionState = false;
}

void AModularStageCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

#include "Table/CharacterActionTable.h"

void AModularStageCharacter::EnterActionState(FName StateID)
{
	// 1. 데이터 테이블 로드 (SoftObjectPtr)
	UDataTable* LoadedTable = ActionDataTable.LoadSynchronous();
	if (!LoadedTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActionDataTable is null or failed to load."));
		return;
	}

	// 2. 이미 상태 중이면 이전 상태 종료
	if (bIsInActionState)
	{
		ExitActionState();
	}

	// 3. 행(Row) 찾기
	FCharacterStateConfig* FoundConfig = LoadedTable->FindRow<FCharacterStateConfig>(StateID, TEXT("EnterActionState"));
	if (!FoundConfig)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find ActionState ID: %s"), *StateID.ToString());
		return;
	}

	CurrentStateConfig = FoundConfig;
	bIsInActionState = true;

	// 4. 시작 몽타주 재생
	if (CurrentStateConfig->StartMontage)
	{
		PlayAnimMontage(CurrentStateConfig->StartMontage);
	}

	// 5. IMC 추가
	if (CurrentStateConfig->StateIMC)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(CurrentStateConfig->StateIMC, CurrentStateConfig->IMCPriority);
			}
		}
	}
}

void AModularStageCharacter::ExitActionState()
{
	if (!bIsInActionState || !CurrentStateConfig) return;

	// 1. 종료 몽타주 재생
	if (CurrentStateConfig->EndMontage)
	{
		PlayAnimMontage(CurrentStateConfig->EndMontage);
	}

	// 2. IMC 제거
	if (CurrentStateConfig->StateIMC)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(CurrentStateConfig->StateIMC);
			}
		}
	}

	bIsInActionState = false;
	CurrentStateConfig = nullptr;
}
