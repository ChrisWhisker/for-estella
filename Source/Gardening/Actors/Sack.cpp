// Fill out your copyright notice in the Description page of Project Settings.


#include "Sack.h"

ASack::ASack()
{
	PrimaryActorTick.bCanEverTick = false;

	// Put the sack in the character's hand
	const FVector MeshTranslation = FVector(-24.868, -2.845, 18.793);
	const FRotator MeshRotation = FRotator(-60, 180, -190);
	const FVector MeshScale = FVector(1.13, 1.13, 1.695);
	Mesh->SetRelativeTransform(FTransform(MeshRotation, MeshTranslation, MeshScale), false);
}

void ASack::BeginPlay()
{
	Super::BeginPlay();
}
