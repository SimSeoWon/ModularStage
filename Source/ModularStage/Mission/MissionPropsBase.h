#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MissionPropsBase.generated.h"

UCLASS()
class MODULARSTAGE_API AMissionPropsBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMissionPropsBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
