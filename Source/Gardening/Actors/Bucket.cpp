// Fill out your copyright notice in the Description page of Project Settings.


#include "Bucket.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ABucket::ABucket()
{
	PrimaryActorTick.bCanEverTick = false;

	// Put the bucket in the character's hand
	const FVector MeshTranslation = FVector(-2.736, -62.003, -4.812);
	const FRotator MeshRotation = FRotator(-76.107, -227.184, -45.421);
	const FVector MeshScale = FVector(0.5, 0.5, 0.5);
	Mesh->SetRelativeTransform(FTransform(MeshRotation, MeshTranslation, MeshScale), false);

	// Place water spawn point
	WaterSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Water Spawn Point"));
	WaterSpawnPoint->SetupAttachment(Mesh);
	const FVector WaterSpawnTranslation = FVector(0, 0, -400);
	const FRotator WaterSpawnRotation = FRotator(0, 0, 0);
	const FVector WaterSpawnScale = FVector(0.67, 0.67, 0.67);
	WaterSpawnPoint->SetRelativeTransform(FTransform(WaterSpawnRotation, WaterSpawnTranslation, WaterSpawnScale),
	                                      false);
}

void ABucket::BeginPlay()
{
	Super::BeginPlay();

	if (!WaterSound)
	{
		UE_LOG(LogTemp, Error, TEXT("Watering sound not set on the character"));
		return;
	}

	if (!WaterParticle)
	{
		UE_LOG(LogTemp, Error, TEXT("Watering particle not set on the character"));
		return;
	}
}

void ABucket::StartPouring()
{
	WaterSoundComponent = UGameplayStatics::SpawnSoundAttached(WaterSound, Mesh);
	WaterSoundComponent->Stop();
	WaterSoundComponent->FadeIn(WaterSoundFadeInSeconds, 1.f);
	WaterParticleComponent = UGameplayStatics::SpawnEmitterAttached(WaterParticle, WaterSpawnPoint);
	WaterParticleComponent->SetTranslucentSortPriority(-1);
}

void ABucket::StopPouring()
{
	if (WaterSoundComponent)
	{
		WaterSoundComponent->FadeOut(WaterSoundFadeOutSeconds, 0.f);
	}

	if (WaterParticleComponent)
	{
		WaterParticleComponent->Deactivate();
	}
}
