#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDeath);

UCLASS()
class BULLET_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

public:	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	class UHealthComponent* GetHealthComponent() { return healthComponent; };

	UFUNCTION()
	void OnDeath();

public:
	UPROPERTY(BlueprintAssignable)
	FOnCharacterDeath onCharacterDeath;

private:
	class UHealthComponent* healthComponent;
};
