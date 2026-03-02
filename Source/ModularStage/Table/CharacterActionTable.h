#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterActionTable.generated.h"

class UInputMappingContext;
class UAnimMontage;

/**
 * 캐릭터의 상호작용 상태(기믹 조작, 물건 들기 등)를 정의하는 데이터 구조체
 */
USTRUCT(BlueprintType)
struct FCharacterStateConfig : public FTableRowBase
{
	GENERATED_BODY()

public:
	/** 상태 이름 (예: Boarding, Carrying) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FName StateID;

	/** 진입 시 재생할 시작 몽타주 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UAnimMontage> StartMontage;

	/** 해제 시 재생할 종료 몽타주 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UAnimMontage> EndMontage;

	/** 상태 진입 시 추가할 입력 매핑 컨텍스트 (IMC) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> StateIMC;

	/** IMC 우선순위 (기존 IMC보다 높아야 덮어씌움) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	int32 IMCPriority = 10;
};
