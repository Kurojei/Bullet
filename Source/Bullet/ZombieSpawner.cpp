#include "ZombieSpawner.h"

AZombieSpawner::AZombieSpawner()
{
	//How many players to determine max zombies on map etc;
}

void AZombieSpawner::BeginPlay()
{
	StartNewRound();
}

void AZombieSpawner::SpawnZombies()
{
	if (zombiesRemaining < maxZombiesOnMap)
	{
		if (spawnPoints.Num() > 0) 
		{
			AZombieSpawn* randSpawnPoint = spawnPoints[FMath::RandRange(0, spawnPoints.Num() - 1)];
			AZombie* zombie = GetWorld()->SpawnActor<AZombie>(zombieToSpawn, randSpawnPoint->GetActorLocation(), randSpawnPoint->GetActorRotation());
			zombie->onZombieDeath.AddDynamic(this, &AZombieSpawner::OnZombieDeath);
			amountToSpawn--;
			//zombie->health = zombieHealth;
			//zombie->speed = zombieSpeed;
		}
	}

	zombiesRemaining++;

	if (amountToSpawn != 0)
	{
		GetWorld()->GetTimerManager().SetTimer(zombieSpawnerTimer, this, &AZombieSpawner::SpawnZombies, timeBetweenZombieSpawns, false);
	}
}

void AZombieSpawner::OnZombieDeath()
{
	zombiesRemaining--;
	if (zombiesRemaining == 0)
	{
		//End round music
		StartNewRound();
	}
}

void AZombieSpawner::StartNewRound()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("New round starting..."));
	currentRound++;
	//UI Broadcast round number;

	//New round music;

	timeBetweenZombieSpawns = timeBetweenZombieSpawns * 0.95;

	//zombieHealth =
	//zombieSpeed =

	float multiplier = currentRound * roundMultiplierConstant;
	amountToSpawn = FMath::CeilToInt(multiplier * maxZombiesOnMap);

	GetWorld()->GetTimerManager().SetTimer(zombieSpawnerTimer, this, &AZombieSpawner::SpawnZombies, timeBetweenRounds, false);
}