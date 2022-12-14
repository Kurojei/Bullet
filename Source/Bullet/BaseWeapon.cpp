#include "BaseWeapon.h"
#include "HealthComponent.h"
#include "BulletCharacter.h"
#include <DrawDebugHelpers.h>
#include "Kismet/GameplayStatics.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	gunMain = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMain"));
	gunMain->SetupAttachment(RootComponent);

	slider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Slider"));
	slider->SetupAttachment(RootComponent);

	magazine = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Magazine"));
	magazine->SetupAttachment(RootComponent);

	audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	audioComponent->SetupAttachment(RootComponent);
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWeapon::Fire()
{
	if (currentMagAmmo == 0) {
		Reload();
		return;
	}

	owner = Cast<ABulletCharacter>(GetOwner());
	//if (owner->bIsFiring) return;
	if (!owner->bIsFiring) {
		owner->bIsFiring = true;
	}

	currentMagAmmo--;

	owner->onAmmoChanged.Broadcast(currentMagAmmo, currentStockAmmo, gunName);

	UAnimMontage* gunFireAnim = owner->bIsAiming ? fireAim : fire;
	owner->GetMesh()->GetAnimInstance()->Montage_Play(gunFireAnim);
	  
	const FVector start = owner->cam->GetSocketLocation("None");
	const FVector end = owner->cam->GetForwardVector() * gunRange + start;
	FCollisionQueryParams params;
	params.AddIgnoredActor(owner);

	//Recoil

	
	FHitResult outHit;
	if (GetWorld()->LineTraceSingleByChannel(outHit, start, end, ECC_Visibility, params))
	{
		if (ABaseCharacter* character = Cast<ABaseCharacter>(outHit.GetActor()))
		{
			character->GetHealthComponent()->ApplyDamage(owner, gunDamage);
			audioComponent->SetSound(hitmarker);
			audioComponent->Play();
			onCharacterHit.Broadcast();
		}
		else if (ATarget* target = Cast<ATarget>(outHit.GetActor()))
		{
			if (!target->bHit)
			{
				target->Hit();
				audioComponent->SetSound(hitmarker);
				audioComponent->Play();
				onCharacterHit.Broadcast();
			}
		}
	}

	//Spawn bullet decal
	float decalSize = FMath::FRandRange(1.5f, 7.f);
	UGameplayStatics::SpawnDecalAtLocation(GetWorld(), bulletDecal, FVector(decalSize, decalSize, decalSize), outHit.Location, outHit.ImpactNormal.Rotation() * -1, 100.f);


	if (fullAuto)
	{
		GetWorld()->GetTimerManager().SetTimer(fullAutoHandle, this, &ABaseWeapon::Fire, fireRate, false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(singleShotHandle, this, &ABaseWeapon::StopFire, fireRate, false);
	}
}

void ABaseWeapon::StopFire()
{
	owner->bIsFiring = false;
	owner = Cast<ABulletCharacter>(GetOwner());
	owner->GetMesh()->GetAnimInstance()->StopAllMontages(0.1);
	GetWorld()->GetTimerManager().ClearTimer(fullAutoHandle);
	GetWorld()->GetTimerManager().ClearTimer(singleShotHandle);
}

void ABaseWeapon::Reload()
{
	if (currentStockAmmo > 0 && currentMagAmmo < maxMagAmmo)
	{
		StopFire();
		bIsReloading = true;

		auto refreshAmmoUI = [=]()
		{
			int amountToAdd = maxMagAmmo - currentMagAmmo;
			if (currentStockAmmo >= amountToAdd)
			{
				currentStockAmmo -= amountToAdd;
				currentMagAmmo = maxMagAmmo;
				owner->onAmmoChanged.Broadcast(currentMagAmmo, currentStockAmmo, gunName);
			}
			else
			{
				currentMagAmmo += currentStockAmmo;
				currentStockAmmo = 0;
			}
			StopReloading();
		};

		UAnimMontage* reloadAnim = currentMagAmmo > 0 ? reloadPartial : reloadEmpty;

		owner = Cast<ABulletCharacter>(GetOwner());
		owner->GetMesh()->GetAnimInstance()->Montage_Play(reloadAnim);

		GetWorld()->GetTimerManager().SetTimer(reloadTimer, refreshAmmoUI, reloadAnim->GetPlayLength(), false);
	}
	else
	{
		//owner->audioComponent->SetSound(emptySound);
		//owner->audioComponent->Play();
		
		//Broadcast out of ammo?
	}
}

void ABaseWeapon::StopReloading()
{
	bIsReloading = false;
	GetWorld()->GetTimerManager().ClearTimer(reloadTimer);
}
