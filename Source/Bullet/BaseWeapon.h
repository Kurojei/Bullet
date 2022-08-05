#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include <Animation/BlendSpace1D.h>
#include <Sound/SoundCue.h>
#include "Zombie.h"
#include "BaseWeapon.generated.h"

UCLASS()
class BULLET_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

//Functions
public:
	ABaseWeapon();
	void Fire();
	void Reload();
	void StopFire();
	void StopReloading();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

//Variables
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* mesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimSequence* idle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimSequence* fireAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* armsFireAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimSequence* gunReloadPartial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimSequence* gunReloadEmpty;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* armsReloadPartial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* armsReloadEmpty;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* unequip;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName gunName = "";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBlendSpace1D* notAiming;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool fullAuto = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float fireRate = 500;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float maxStockAmmo = 90;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float maxMagAmmo = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float gunRange = 20000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float gunDamage = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundCue* fireSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector gunLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FRotator gunRotation;

	class ABulletCharacter* owner;

	bool bIsFiring = false;
	bool bIsReloading = false;
	float currentStockAmmo = 90;
	float currentMagAmmo = 30;

private:
	FTimerHandle fullAutoHandle;
	FTimerHandle reloadTimer;
};
