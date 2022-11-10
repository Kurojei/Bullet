#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "BaseWeapon.h"
#include <Components/AudioComponent.h>
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "BulletCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAmmoChanged, float, currentMagAmmo, float, currentStockAmmo, FName, gunName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGunChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAiming);

UCLASS()
class BULLET_API ABulletCharacter : public ACharacter
{
	GENERATED_BODY()

//Functions
public:
	ABulletCharacter();
	virtual void BeginPlay() override;

	void Fire();
	void StopFire();
	void Reload();
	class UHealthComponent* GetHealthComponent() { return healthComponent; };

private:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void MoveForward(float value);
	void MoveRight(float value);
	void LookRight(float value);
	void LookUp(float value);
	void ToggleADS();
	void SwapWeapon();
	void StartSprint();
	void StopSprint();

//Variables
public:
	UPROPERTY(BlueprintAssignable)
	FOnAmmoChanged onAmmoChanged;

	UPROPERTY(BlueprintAssignable)
	FOnGunChanged onGunChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAiming onAiming;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCameraShakeBase* sprintShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float turnRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float lookRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCameraComponent* cam;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ABaseWeapon* currentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<ABaseWeapon*> guns;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USpringArmComponent* minimapArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneCaptureComponent2D* minimapCapture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTextureRenderTarget2D* minimap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsSprinting = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsAiming = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsFiring = false;

	UFUNCTION(BlueprintImplementableEvent)
	void CameraFovLerp();

	UFUNCTION(BlueprintImplementableEvent)
	void ToggleCameraShake();

	int currentWeaponIndex = 0;
	bool bIsSwapping = false;
	UAudioComponent* audioComponent;

private:
	class UHealthComponent* healthComponent;
};

