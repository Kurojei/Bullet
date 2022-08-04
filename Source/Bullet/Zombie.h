#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Zombie.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnZombieDeath);

UCLASS()
class BULLET_API AZombie : public ACharacter
{
	GENERATED_BODY()

public:
	AZombie();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	class UHealthComponent* GetHealthComponent() { return healthComponent; };

	UFUNCTION()
	void OnDeath();

public:
	UPROPERTY(BlueprintAssignable)
	FOnZombieDeath onZombieDeath;

private:
	class UHealthComponent* healthComponent;
};
