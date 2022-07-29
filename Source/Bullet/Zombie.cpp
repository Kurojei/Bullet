#include "Zombie.h"

AZombie::AZombie()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AZombie::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

