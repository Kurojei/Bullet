#include "BaseCharacter.h"
#include "HealthComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	healthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	healthComponent->onDeath.AddDynamic(this, &ABaseCharacter::OnDeath);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::OnDeath()
{
	onCharacterDeath.Broadcast();
	Destroy();
}