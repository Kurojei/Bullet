#include "HealthComponent.h"
#include "BulletCharacter.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UHealthComponent::ApplyDamage(ACharacter* instigator, float amount)
{
	health = FMath::Clamp(health - amount, 0.f, maxHealth);
	if (health <= 0)
	{
		onDeath.Broadcast();
	}
}

