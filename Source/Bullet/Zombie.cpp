#include "Zombie.h"
#include "HealthComponent.h"

AZombie::AZombie()
{
	PrimaryActorTick.bCanEverTick = true;

	healthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	healthComponent->onDeath.AddDynamic(this, &AZombie::OnDeath);
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

void AZombie::OnDeath()
{
	onZombieDeath.Broadcast();
	Destroy();
}