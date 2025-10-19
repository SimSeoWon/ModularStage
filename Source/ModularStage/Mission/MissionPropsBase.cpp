#include "MissionPropsBase.h"

// Sets default values
AMissionPropsBase::AMissionPropsBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMissionPropsBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMissionPropsBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
