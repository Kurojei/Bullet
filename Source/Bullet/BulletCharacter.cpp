#include "BulletCharacter.h"
#include "HealthComponent.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>

ABulletCharacter::ABulletCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(GetMesh(), FName("CameraSocket"));
	springArm->TargetArmLength = 0;

	cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Cam"));
	cam->SetupAttachment(springArm, FName("SpringEndpoint"));

	audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	audioComponent->SetupAttachment(RootComponent);

	GetMesh()->SetupAttachment(RootComponent);

	healthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	bulletSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("BulletSpawnPoint"));
	bulletSpawnPoint->SetupAttachment(GetMesh());
}

void ABulletCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ABulletCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void InitializeInputBindings()
{
	static bool bBindingsAdded = false;
	if (!bBindingsAdded)
	{
		bBindingsAdded = true;

		//Axis
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::W, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::S, -1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::Gamepad_LeftY, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::A, -1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::D, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::Gamepad_LeftX, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("LookRight", EKeys::Gamepad_RightX, 0.7f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("LookRight", EKeys::MouseX, 0.7f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("LookUp", EKeys::Gamepad_RightY, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("LookUp", EKeys::MouseY, -1.f));

		//Actions
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Jump", EKeys::Gamepad_FaceButton_Bottom));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Jump", EKeys::SpaceBar));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Reload", EKeys::Gamepad_FaceButton_Left));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Reload", EKeys::R));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Fire", EKeys::Gamepad_RightTrigger));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Fire", EKeys::LeftMouseButton));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("ToggleSprint", EKeys::LeftShift));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("ToggleSprint", EKeys::Gamepad_LeftThumbstick));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("ToggleADS", EKeys::RightMouseButton));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("ToggleADS", EKeys::Gamepad_LeftTrigger));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("SwapGun", EKeys::Gamepad_FaceButton_Top));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("SwapGun", EKeys::MouseScrollUp));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("SwapGun", EKeys::MouseScrollDown));
	}
}

void ABulletCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InitializeInputBindings();

	//Axis
	PlayerInputComponent->BindAxis("MoveForward", this, &ABulletCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABulletCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookRight", this, &ABulletCharacter::LookRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ABulletCharacter::LookUp);

	//Actions
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABulletCharacter::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABulletCharacter::StopFire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ABulletCharacter::Reload);
	PlayerInputComponent->BindAction("ToggleSprint", IE_Pressed, this, &ABulletCharacter::StartSprint);
	PlayerInputComponent->BindAction("ToggleSprint", IE_Released, this, &ABulletCharacter::StopSprint);
	PlayerInputComponent->BindAction("ToggleADS", IE_Pressed, this, &ABulletCharacter::ToggleADS);
	PlayerInputComponent->BindAction("ToggleADS", IE_Released, this, &ABulletCharacter::ToggleADS);
	PlayerInputComponent->BindAction("SwapGun", IE_Pressed, this, &ABulletCharacter::SwapWeapon);
}

void ABulletCharacter::ToggleADS()
{
	//TODO Slow aim speed side to side
	if (bIsSprinting)
	{
		StopSprint();
	}
	bIsAiming = !bIsAiming;
	onAiming.Broadcast();
	//GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? 150.f : 300.f;
	CameraFovLerp();
}

void ABulletCharacter::StartSprint()
{
	//TODO Only sprint forwards
	if (!currentWeapon) return;
	if (bIsAiming) return;
	if (bIsFiring) return;
	if (currentWeapon->bIsReloading) return;
	bIsSprinting = true;
	GetMesh()->GetAnimInstance()->StopAllMontages(0);
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void ABulletCharacter::StopSprint()
{
	if (!currentWeapon) return;
	if (bIsAiming) return;
	if (currentWeapon->bIsReloading) return;
	bIsSprinting = false;
	GetMesh()->GetAnimInstance()->StopAllMontages(0);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void ABulletCharacter::Fire()
{
	if (!currentWeapon) return;
	if (currentWeapon->bIsReloading) return;
	if(bIsFiring) return;
	if (bIsSprinting)
	{
		StopSprint();
	}
	//GetCharacterMovement()->MaxWalkSpeed = 150.f; // only slow if has bullets
	currentWeapon->Fire();
}

void ABulletCharacter::StopFire()
{
	if (!currentWeapon) return;
	if (currentWeapon->bIsReloading) return;
	//GetCharacterMovement()->MaxWalkSpeed = 300.f;
	if (currentWeapon->fullAuto)
	{
		currentWeapon->StopFire();
	}
}

void ABulletCharacter::Reload()
{
	if (!currentWeapon) return;
	StopSprint();
	currentWeapon->Reload();
}

void ABulletCharacter::SwapWeapon()
{
	if (!currentWeapon->unequip || !currentWeapon->equip) return;
	if (!currentWeapon) return;
	if (bIsSwapping) return;

	//FTimerHandle swapTimer;
	//GetMesh()->GetAnimInstance()->Montage_Play(currentWeapon->unequip);
	//
	//auto swapAnim = [=]() {
	//	//currentWeapon->SetOwner(this);
	//	currentWeapon = secondWeapon;
	//	onGunChanged.Broadcast();
	//	onAmmoChanged.Broadcast(currentWeapon->currentMagAmmo, currentWeapon->currentStockAmmo, currentWeapon->gunName);
	//	GetMesh()->GetAnimInstance()->Montage_Play(currentWeapon->equip);
	//
	//	bIsSwapping = false;
	//};
	//
	//if (secondWeapon) //Is there a next gun?
	//{
	//	bIsSwapping = true;
	//	GetWorld()->GetTimerManager().SetTimer(swapTimer, swapAnim, currentWeapon->unequip->GetPlayLength(), false);
	//}
	//else
	//{
	//	bIsSwapping = true;
	//	currentWeaponIndex = 0;
	//	GetWorld()->GetTimerManager().SetTimer(swapTimer, swapAnim, 0.7f, false);
	//}
}

void ABulletCharacter::MoveForward(float value)
{
	FRotator const ControlSpaceRot = Controller->GetControlRotation();
	AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X), value);
}

void ABulletCharacter::MoveRight(float value)
{
	FRotator const ControlSpaceRot = Controller->GetControlRotation();
	AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), value);
}

void ABulletCharacter::LookRight(float value)
{
	AddControllerYawInput(value * turnRate * GetWorld()->GetDeltaSeconds());
}

void ABulletCharacter::LookUp(float value)
{
	GetMesh()->AddRelativeRotation(FRotator(-value * lookRate * GetWorld()->GetDeltaSeconds(), 0, 0));
}