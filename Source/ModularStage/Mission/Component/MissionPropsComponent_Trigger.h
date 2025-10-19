#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "MissionPropComponent.h"
#include "MissionPropsComponent_Trigger.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MODULARSTAGE_API UMissionPropsComponent_Trigger : public UBoxComponent, public IMissionPropComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMissionPropsComponent_Trigger();

	virtual void OnActivate() override;
	virtual void OnDeactivate() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
