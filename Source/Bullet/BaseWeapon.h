#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include <Animation/BlendSpace1D.h>
#include <Sound/SoundCue.h>
#include "Zombie.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "BaseCharacter.h"
#include "BaseWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterHit);

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
	UPROPERTY(BlueprintAssignable)
	FOnCharacterHit onCharacterHit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* mesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* gunSilouhette;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName gunName = "";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimSequence* gunFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimSequence* gunReloadPartial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimSequence* gunReloadEmpty;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimSequence* armsIdle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* armsFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* armsFireAim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* armsReloadPartial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* armsReloadEmpty;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* unequip;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBlendSpace1D* aiming;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBlendSpace1D* notAiming;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UNiagaraSystem* muzzleFlash;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UNiagaraSystem* shellEject;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundCue* fireSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundCue* reloadSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundCue* emptySound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundCue* swapSound;

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
	FVector gunLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FRotator gunRotation;

	class ABulletCharacter* owner;
	bool bIsReloading = false;
	float currentStockAmmo = 90;
	float currentMagAmmo = 30;
	float roundsPerMinute = 0;

private:
	FTimerHandle fullAutoHandle;
	FTimerHandle reloadTimer;
};
