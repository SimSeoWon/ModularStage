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

	//~ Begin IMissionPropComponent Interface
	virtual void SetGuid(const FGuid& InGuid) override;
	virtual FGuid GetGuid() const override;
	virtual void OnActivate() override;
	virtual void OnDeactivate() override;
	virtual void SetHexTileIndex(int32 InIndex) override { HexTileIndex = InIndex; }
	virtual int32 GetHexTileIndex() const override { return HexTileIndex; }
	//~ End IMissionPropComponent Interface

	virtual void PostInitProperties() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	/** 이 컴포넌트가 배치될 헥스 타일 인덱스 (0~6) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MissionProp | HexGrid")
	int32 HexTileIndex = 0;

private:
	/** A unique identifier for this component instance. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MissionProp", meta = (AllowPrivateAccess = "true"))
	FGuid ComponentGuid;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
