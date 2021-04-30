// Fill out your copyright notice in the Description page of Project Settings.


#include "Bucket.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ABucket::ABucket()
{
	PrimaryActorTick.bCanEverTick = false;

	WaterSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Water Spawn Point"));
	WaterSpawnPoint->SetupAttachment(Mesh);
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
	WateringParticleComponent = UGameplayStatics::SpawnEmitterAttached(WaterParticle, WaterSpawnPoint);
}

void ABucket::StopPouring()
{
	if (WaterSoundComponent)
	{
		WaterSoundComponent->FadeOut(WaterSoundFadeOutSeconds, 0.f);
	}
	
	if (WateringParticleComponent)
	{
		WateringParticleComponent->Deactivate();
	}
}
