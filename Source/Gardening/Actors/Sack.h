// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tool.h"
#include "Sack.generated.h"

UCLASS()
class GARDENING_API ASack : public ATool
{
	GENERATED_BODY()

public:
	////////// FUNCTIONS //////////
	ASack();

protected:
	////////// FUNCTIONS //////////
	virtual void BeginPlay() override;
};
