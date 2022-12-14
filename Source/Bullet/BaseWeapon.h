#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include <Animation/BlendSpace1D.h>
#include <Sound/SoundCue.h>
#include <Components/AudioComponent.h>
#include "Zombie.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "BaseCharacter.h"
#include "Target.h"
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
	USkeletalMesh* armMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UClass* gunAnimBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* gunMain;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* slider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* magazine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMaterialInterface* bulletDecal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* gunSilouhette;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName gunName = "";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* reloadPartial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* reloadEmpty;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* fire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* fireAim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* equip;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* unequip;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBlendSpace1D* aiming;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBlendSpace1D* notAiming;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundCue* hitmarker;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool fullAuto = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float fireRate = 0.2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float maxStockAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float maxMagAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float gunRange = 20000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float gunDamage = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float currentStockAmmo = 90;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float currentMagAmmo = 30;

	class ABulletCharacter* owner;
	bool bIsReloading = false;

private:
	FTimerHandle fullAutoHandle;
	FTimerHandle singleShotHandle;
	FTimerHandle reloadTimer;
	UAudioComponent* audioComponent;
};
