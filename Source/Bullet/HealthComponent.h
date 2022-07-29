#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BULLET_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();
	void ApplyDamage(ACharacter* instigator, float amount);

	virtual void OnDeath(ACharacter* characterToKill);

private:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(BlueprintAssignable)
		FOnDeath onDeath;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float health = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float maxHealth = 100.f;

private:

};