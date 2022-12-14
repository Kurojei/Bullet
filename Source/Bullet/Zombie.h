#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.h"
#include "Zombie.generated.h"

UCLASS()
class BULLET_API AZombie : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AZombie();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
