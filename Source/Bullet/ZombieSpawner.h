#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Zombie.h"
#include "ZombieSpawn.h"
#include "ZombieSpawner.generated.h"

UCLASS()
class BULLET_API AZombieSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AZombieSpawner();
	virtual void BeginPlay() override;
	void SpawnZombies();
	void OnZombieDeath();
	void StartNewRound();

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AZombie> zombieToSpawn;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TArray<AZombieSpawn*> spawnPoints;

	int amountToSpawn = 0;
	int zombiesRemaining = 0;
	int maxZombiesOnMap = 24; //+ 6 for additional player
	int currentRound = 0;
	float roundMultiplierConstant = 0.15;
	int timeBetweenRounds = 10;
	float zombieSpeed = 100.f;
	float zombieHealth = 150.f;
	float timeBetweenZombieSpawns = 2.f;
	FTimerHandle zombieSpawnerTimer;
};
