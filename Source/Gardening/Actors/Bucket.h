// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tool.h"
#include "Bucket.generated.h"

UCLASS()
class GARDENING_API ABucket : public ATool
{
	GENERATED_BODY()

public:
	////////// FUNCTIONS //////////
	ABucket();

protected:
	////////// FUNCTIONS //////////
	virtual void BeginPlay() override;
};
