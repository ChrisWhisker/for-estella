// Fill out your copyright notice in the Description page of Project Settings.


#include "GardeningCharacterHelper.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Gardening/Actors/Axe.h"
#include "Gardening/Actors/Bucket.h"
#include "Gardening/Actors/Plant.h"
#include "Gardening/Actors/Sack.h"
#include "Gardening/GardeningPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

UGardeningCharacterHelper::UGardeningCharacterHelper()
{
	PrimaryComponentTick.bCanEverTick = false;

	SeedCount = MaxSeeds;
}

void UGardeningCharacterHelper::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* FirstPlayerController = GetWorld()->GetFirstPlayerController();
	GardeningPlayerController = Cast<AGardeningPlayerController>(FirstPlayerController);
	if (!GardeningPlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("Player controller isn't found on the character."));
		return;
	}

	if (!PlantBlueprintClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Plant Blueprint Class is not set on the character."));
	}

	if (!WaterSound)
	{
		UE_LOG(LogTemp, Error, TEXT("Watering sound not set on the character"));
	}

	if (!WaterParticle)
	{
		UE_LOG(LogTemp, Error, TEXT("Watering particle not set on the character"));
	}
}

void UGardeningCharacterHelper::SwitchTool()
{
	ActiveTool = (ActiveTool + 1) % Tools.Num();

	if (Tools[ActiveTool] == Tool_Seeds)
	{
		Sack->SetActorHiddenInGame(false);
		Bucket->SetActorHiddenInGame(true);
		Axe->SetActorHiddenInGame(true);
	}
	else if (Tools[ActiveTool] == Tool_Water)
	{
		Sack->SetActorHiddenInGame(true);
		Bucket->SetActorHiddenInGame(false);
		Axe->SetActorHiddenInGame(true);
	}
	else if (Tools[ActiveTool] == Tool_Axe)
	{
		Sack->SetActorHiddenInGame(true);
		Bucket->SetActorHiddenInGame(true);
		Axe->SetActorHiddenInGame(false);
	}
}

void UGardeningCharacterHelper::FirePressed()
{
	if (Tools[ActiveTool] == Tool_Seeds && SeedCount > 0)
	{
		FHitResult Hit;
		const bool bTraceSucceeded = Trace(Hit);
		if (!bTraceSucceeded) { return; }
		AActor* HitActor = Hit.GetActor();
		if (!HitActor) { return; }
		PlantSeed(Hit);
	}
	else if (Tools[ActiveTool] == Tool_Water)
	{
		UE_LOG(LogTemp, Warning, TEXT("Watering"));
		bIsPourWaterHeld = true;
		StartWatering();
	}
	else if (Tools[ActiveTool] == Tool_Axe)
	{
		UseAxe();
	}
}

void UGardeningCharacterHelper::FireReleased()
{
	bIsPourWaterHeld = false;
	StopWatering();
}

void UGardeningCharacterHelper::PlantSeed(const FHitResult Hit)
{
	if (!Hit.GetActor()->GetClass()->GetName().Contains(TEXT("Landscape"))) { return; }

	const float PlantYaw = FMath::RandRange(0.f, 360.f);
	const FRotator PlantingDirection = FRotator(0.f, PlantYaw, 0.f);
	AActor* Spawned = GetWorld()->SpawnActor<APlant>(PlantBlueprintClass, Hit.Location, PlantingDirection);
	APlant* SpawnedPlant = Cast<APlant>(Spawned);

	if (SpawnedPlant)
	{
		SpawnedPlant->CharacterHelper = this;
		SeedCount--;
	}
}

void UGardeningCharacterHelper::StartWatering()
{
	TArray<AActor*> PlantsInTrigger;

	WateringTrigger->GetOverlappingActors(PlantsInTrigger, TSubclassOf<APlant>());

	for (AActor* PlantActor : PlantsInTrigger)
	{
		APlant* Plant = Cast<APlant>(PlantActor);
		if (!Plant) { continue; }

		WaterPlant(Plant);
	}

	WaterSoundComponent = UGameplayStatics::SpawnSoundAttached(WaterSound, WateringTrigger);
	WaterSoundComponent->Stop();
	WaterSoundComponent->FadeIn(WaterSoundFadeInSeconds, 1.f);
	WateringParticleComponent = UGameplayStatics::SpawnEmitterAttached(WaterParticle, WaterSpawnPoint);
}

void UGardeningCharacterHelper::StopWatering()
{
	for (APlant* WateredPlant : WateredPlants)
	{
		WateredPlant->StopGrowing();
	}

	WateredPlants.Empty();

	if (WaterSoundComponent)
	{
		WaterSoundComponent->FadeOut(WaterSoundFadeOutSeconds, 0.f);
	}

	if (WateringParticleComponent)
	{
		WateringParticleComponent->Deactivate();
	}
}

void UGardeningCharacterHelper::StopWateringOnePlant(APlant* Plant)
{
	Plant->StopGrowing();
	WateredPlants.Remove(Plant);
}

void UGardeningCharacterHelper::WaterPlant(APlant* PlantToWater)
{
	PlantToWater->StartGrowing();

	if (!WateredPlants.Contains(PlantToWater))
	{
		WateredPlants.Add(PlantToWater);
	}
}

void UGardeningCharacterHelper::UseAxe() const
{
	UE_LOG(LogTemp, Warning, TEXT("Axe used."));
}

bool UGardeningCharacterHelper::Trace(FHitResult& Hit) const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) { return false; }

	AController* OwnerController = OwnerPawn->GetController();
	if (!OwnerController) { return false; }

	FVector StartLocation;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(OUT StartLocation, OUT Rotation);
	const FVector EndLocation = StartLocation + Rotation.Vector() * MaxTraceRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(OUT Hit, StartLocation, EndLocation,
	                                            ECollisionChannel::ECC_GameTraceChannel1,
	                                            Params);
}

void UGardeningCharacterHelper::AddFeetToGardenHeight(const float Height)
{
	GardenHeightFeet += Height;
}

void UGardeningCharacterHelper::SubtractFeetFromGardenHeight(const float Height)
{
	GardenHeightFeet -= Height;
}
