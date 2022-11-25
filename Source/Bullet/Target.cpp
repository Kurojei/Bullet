#include "Target.h"

ATarget::ATarget()
{
	PrimaryActorTick.bCanEverTick = true;

	target = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Target"));
	target->SetupAttachment(RootComponent);
}

void ATarget::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ATarget::Hit()
{
	target->PlayAnimation(targetDown, false);

	float delay = FMath::FRandRange(minDelay, maxDelay);

	FTimerHandle targetHandle;
	GetWorld()->GetTimerManager().SetTimer(targetHandle, this, &ATarget::Activate, delay, false);
}

void ATarget::Activate()
{
	target->PlayAnimation(targetUp, false);
}
