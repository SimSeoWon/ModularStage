#include "MissionPropsComponent_Trigger.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

//#include "MissionManager.h"

// Sets default values for this component's properties
UMissionPropsComponent_Trigger::UMissionPropsComponent_Trigger()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	OnComponentBeginOverlap.AddDynamic(this, &UMissionPropsComponent_Trigger::OnOverlapBegin);
}

void UMissionPropsComponent_Trigger::OnActivate() 
{

}

void UMissionPropsComponent_Trigger::OnDeactivate()
{

}


// Called when the game starts
void UMissionPropsComponent_Trigger::BeginPlay()
{
	Super::BeginPlay();

	// ...	
}


// Called every frame
void UMissionPropsComponent_Trigger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMissionPropsComponent_Trigger::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
    if (OtherActor == PlayerCharacter)
    {
        if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
        {
            //if (UMissionManager* MissionManager = GameInstance->GetSubsystem<UMissionManager>())
            //{
            //    MissionManager->OnPlayerEnterMissionTrigger();
            //}
        }
    }
}
