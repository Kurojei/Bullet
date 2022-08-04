#include "BaseWeapon.h"
#include "HealthComponent.h"
#include "BulletCharacter.h"
#include <DrawDebugHelpers.h>

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMesh"));
	RootComponent = mesh;
	fireRate = 60 / fireRate;
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	mesh->PlayAnimation(idle, true);
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
	if (!bIsFiring) {
		bIsFiring = true;
	}

	currentMagAmmo--;
	owner = Cast<ABulletCharacter>(GetOwner());
	owner->onAmmoChanged.Broadcast(currentMagAmmo, currentStockAmmo, gunName);
	mesh->PlayAnimation(fireAnim, false);

	UAnimMontage* armsFireAnimation = owner->bIsAiming ? armsFireAnimAim : armsFireAnim;
	owner->GetMesh()->GetAnimInstance()->Montage_Play(armsFireAnimation);
	  
	const FVector start = owner->cam->GetSocketLocation("None");
	const FVector end = owner->cam->GetForwardVector() * gunRange + start;
	FCollisionQueryParams params;
	params.AddIgnoredActor(owner);

	//Recoil
	owner->audioComponent->SetSound(fireSound);
	owner->audioComponent->Play();
	//Muzzle flash
	//Shell eject

	FHitResult outHit;
	if (GetWorld()->LineTraceSingleByChannel(outHit, start, end, ECC_Visibility, params))
	{
		if (AZombie* zombie = Cast<AZombie>(outHit.GetActor()))
		{
			zombie->GetHealthComponent()->ApplyDamage(owner, gunDamage);
		}
	}

	if (fullAuto)
	{
		GetWorld()->GetTimerManager().SetTimer(fullAutoHandle, this, &ABaseWeapon::Fire, fireRate, false);
	}
}

void ABaseWeapon::StopFire()
{
	bIsFiring = false;
	owner = Cast<ABulletCharacter>(GetOwner());
	owner->GetMesh()->GetAnimInstance()->Montage_SetNextSection(FName("Loop"), FName("Tail"));
	mesh->GetAnimInstance()->Montage_SetNextSection(FName("Loop"), FName("Tail"));
	GetWorld()->GetTimerManager().ClearTimer(fullAutoHandle);
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

		UAnimSequence* gunReloadAnim = currentMagAmmo > 0 ? gunReloadPartial : gunReloadEmpty;
		UAnimMontage* armsReloadAnim = currentMagAmmo > 0 ? armsReloadPartial : armsReloadEmpty;

		owner = Cast<ABulletCharacter>(GetOwner());
		owner->GetMesh()->GetAnimInstance()->Montage_Play(armsReloadAnim);
		mesh->PlayAnimation(gunReloadAnim, false);

		GetWorld()->GetTimerManager().SetTimer(reloadTimer, refreshAmmoUI, gunReloadAnim->GetPlayLength(), false);
	}
	else
	{
		//deadmans click? Broadcast out of ammo?
	}
}

void ABaseWeapon::StopReloading()
{
	bIsReloading = false;
	GetWorld()->GetTimerManager().ClearTimer(reloadTimer);
}
