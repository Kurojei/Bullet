#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Target.generated.h"

UCLASS()
class BULLET_API ATarget : public AActor
{
	GENERATED_BODY()
	
public:	
	ATarget();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void Hit();
	void Activate();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* target;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimSequence* targetDown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimSequence* targetUp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float minDelay = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float maxDelay = 15.f;
};
