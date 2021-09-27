// Copyright 2021 Chris Worcester All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tool.generated.h"

UCLASS()
class GARDENING_API ATool : public AActor
{
	GENERATED_BODY()
	
public:	
	////////// FUNCTIONS //////////
	ATool();

	protected:
	////////// FUNCTIONS //////////
	virtual void BeginPlay() override;
	
	////////// PROPERTIES //////////
	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;
};
