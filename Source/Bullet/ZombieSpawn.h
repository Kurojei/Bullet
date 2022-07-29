#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieSpawn.generated.h"

UCLASS()
class BULLET_API AZombieSpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	AZombieSpawn();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:	

};
