#include "BaseWeapon.h"
#include "HealthComponent.h"
#include "BulletCharacter.h"
#include <DrawDebugHelpers.h>

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMesh"));
	RootComponent = mesh;
	roundsPerMinute = 60 / fireRate;
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	mesh->PlayAnimation(armsIdle, true);
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
	if (!owner->bIsFiring) {
		owner->bIsFiring = true;
	}

	currentMagAmmo--;

	owner->onAmmoChanged.Broadcast(currentMagAmmo, currentStockAmmo, gunName);
	mesh->PlayAnimation(gunFire, false);

	UAnimMontage* gunFireAnim = owner->bIsAiming ? armsFireAim : armsFire;
	owner->GetMesh()->GetAnimInstance()->Montage_Play(gunFireAnim);
	  
	const FVector start = owner->cam->GetSocketLocation("None");
	const FVector end = owner->cam->GetForwardVector() * gunRange + start;
	FCollisionQueryParams params;
	params.AddIgnoredActor(owner);

	owner->audioComponent->SetSound(fireSound);
	owner->audioComponent->Play();

	//Recoil
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
		GetWorld()->GetTimerManager().SetTimer(fullAutoHandle, this, &ABaseWeapon::Fire, roundsPerMinute, false);
	}
}

void ABaseWeapon::StopFire()
{
	owner->bIsFiring = false;
	owner = Cast<ABulletCharacter>(GetOwner());
	owner->GetMesh()->GetAnimInstance()->StopAllMontages(0.1);
	//owner->GetMesh()->GetAnimInstance()->Montage_SetNextSection(FName("Loop"), FName("Tail"));
	//mesh->GetAnimInstance()->Montage_SetNextSection(FName("Loop"), FName("Tail"));
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

		//owner->audioComponent->SetSound(reloadSound);
		//owner->audioComponent->Play();

		owner = Cast<ABulletCharacter>(GetOwner());
		owner->GetMesh()->GetAnimInstance()->Montage_Play(armsReloadAnim);
		mesh->PlayAnimation(gunReloadAnim, false);

		GetWorld()->GetTimerManager().SetTimer(reloadTimer, refreshAmmoUI, gunReloadAnim->GetPlayLength(), false);
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
