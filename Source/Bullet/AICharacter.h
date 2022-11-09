#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.h"
#include "AICharacter.generated.h"

UCLASS()
class BULLET_API AAICharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AAICharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
