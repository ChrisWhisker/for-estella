// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GardeningCharacterHelper.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GARDENING_API UGardeningCharacterHelper : public UActorComponent
{
	GENERATED_BODY()

public:
	UGardeningCharacterHelper();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	void UseTool();
	void SwitchTool(int32 NewToolIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ActiveTool = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FString> Tools;

protected:
	virtual void BeginPlay() override;
	bool Trace(FHitResult& Hit, FVector& ShotDirection);
	

private:
	UPROPERTY(EditAnywhere)
	float MaxRange = 500.f;
	
	UPROPERTY(EditAnywhere, /*Category = Players,*/ meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APlant> PlantBpClass;
};
