// Fill out your copyright notice in the Description page of Project Settings.


#include "GardeningCharacterHelper.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Gardening/Actors/Plant.h"
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
		UE_LOG(LogTemp, Error, TEXT("Player controller isn't found on the character helper."));
		return;
	}

	if (!WateringSound)
	{
		UE_LOG(LogTemp, Error, TEXT("Watering sound not set on Character Helper"));
	}

	if (!WateringParticle)
	{
		UE_LOG(LogTemp, Error, TEXT("Watering particle not set on Character Helper"));
	}
}

void UGardeningCharacterHelper::SwitchTool()
{
	ActiveTool = (ActiveTool + 1) % Tools.Num();
}

void UGardeningCharacterHelper::UseTool()
{
	FHitResult Hit;
	const bool bTraceSucceeded = Trace(Hit);

	if (!bTraceSucceeded) { return; }

	AActor* HitActor = Hit.GetActor();
	if (!HitActor) { return; }

	if (Tools[ActiveTool] == Tool_Seeds && SeedCount > 0)
	{
		PlantSeed(Hit);
	}
	else if (Tools[ActiveTool] == Tool_Axe)
	{
		UseAxe();
	}
}

void UGardeningCharacterHelper::PlantSeed(const FHitResult Hit)
{
	if (!Hit.GetActor()->GetClass()->GetName().Contains(TEXT("Landscape"))) { return; }

	const float PlantYaw = FMath::RandRange(0.f, 360.f);
	const FRotator PlantingDirection = FRotator(0.f, PlantYaw, 0.f);
	AActor* Spawned = GetWorld()->SpawnActor<APlant>(PlantBpClass, Hit.Location, PlantingDirection);
	APlant* SpawnedPlant = Cast<APlant>(Spawned);

	if (SpawnedPlant)
	{
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
	WateringSoundComponent = UGameplayStatics::SpawnSoundAttached(WateringSound, WateringTrigger);
	WateringParticleComponent = UGameplayStatics::SpawnEmitterAttached(WateringParticle, WaterSpawnPoint);
}

void UGardeningCharacterHelper::StopWatering()
{
	for (APlant* WateredPlant : WateredPlants)
	{
		WateredPlant->StopGrowing();
	}

	WateredPlants.Empty();

	if (WateringSoundComponent)
	{
		WateringSoundComponent->StopDelayed(0.5f);
	}

	if (WateringParticleComponent)
	{
		WateringParticleComponent->Deactivate();
	}
}

void UGardeningCharacterHelper::WaterPlant(APlant* PlantToWater)
{
	PlantToWater->StartGrowing();
	WateredPlants.Add(PlantToWater);
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
	const FVector EndLocation = StartLocation + Rotation.Vector() * MaxRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(OUT Hit, StartLocation, EndLocation,
	                                            ECollisionChannel::ECC_GameTraceChannel1,
	                                            Params);
}

int32 UGardeningCharacterHelper::GetMaxSeeds() const
{
	return MaxSeeds;
}
