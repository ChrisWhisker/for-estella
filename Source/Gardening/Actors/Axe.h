// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tool.h"
#include "Axe.generated.h"

UCLASS()
class GARDENING_API AAxe : public ATool
{
	GENERATED_BODY()
	
public:
	////////// FUNCTIONS //////////
	AAxe();

protected:
	////////// FUNCTIONS //////////
	virtual void BeginPlay() override;
};
