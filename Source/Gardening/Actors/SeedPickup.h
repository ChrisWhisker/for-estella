// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SeedPickup.generated.h"

class UCapsuleComponent;
UCLASS()
class GARDENING_API ASeedPickup : public AActor
{
	GENERATED_BODY()

public:
	ASeedPickup();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;

	void ResetPickup();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* TrunkMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LeavesMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCapsuleComponent* Trigger;

	UPROPERTY()
	class UGardeningCharacterHelper* Helper;

	bool bIsActive = true;

	UPROPERTY(EditAnywhere)
	float ResetDelay = 5.f;

	FTimerHandle ResetTimer;

	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* Rustle;
};
