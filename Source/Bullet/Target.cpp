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
	bHit = true;
	target->PlayAnimation(targetDown, false);

	float delay = FMath::FRandRange(minDelay, maxDelay);

	FTimerHandle targetHandle;
	GetWorld()->GetTimerManager().SetTimer(targetHandle, this, &ATarget::Activate, delay, false);
}

void ATarget::Activate()
{
	bHit = false;
	target->PlayAnimation(targetUp, false);
}
