// Fill out your copyright notice in the Description page of Project Settings.


#include "GardeningCharacterHelper.h"
#include "Blueprint/UserWidget.h"
#include "Gardening/Actors/Plant.h"
#include "Gardening/GardeningPlayerController.h"
#include "Kismet/GameplayStatics.h"
// #include "DrawDebugHelpers.h"

UGardeningCharacterHelper::UGardeningCharacterHelper()
{
	PrimaryComponentTick.bCanEverTick = true;

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
}

void UGardeningCharacterHelper::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (WateredPlant)
	{
		FHitResult Hit;
		const bool bTraceSucceeded = Trace(OUT Hit);
		if (!bTraceSucceeded) { return; }

		if (!Cast<APlant>(Hit.GetActor()))
		{
			StopUsingTool();
		}
	}
}

void UGardeningCharacterHelper::SwitchTool(const int32 NewToolIndex)
{
	if (NewToolIndex == ActiveTool) { return; }
	StopUsingTool();

	if (NewToolIndex == -1) // Go to next item
	{
		ActiveTool = (ActiveTool + 1) % Tools.Num();
	}
	else // Go to selected item
	{
		ActiveTool = NewToolIndex;
	}
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
	else if (Tools[ActiveTool] == Tool_WateringCan)
	{
		WaterPlant(Hit);
	}
	else if (Tools[ActiveTool] == Tool_Axe)
	{
		UseAxe();
	}
}

void UGardeningCharacterHelper::StopUsingTool()
{
	if (WateredPlant)
	{
		WateredPlant->SetProgressBarVisibility(false);
		WateredPlant->StopGrowing();
		WateredPlant = nullptr;
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

void UGardeningCharacterHelper::WaterPlant(const FHitResult Hit)
{
	APlant* HitPlant = Cast<APlant>(Hit.GetActor());
	if (HitPlant)
	{
		HitPlant->SetProgressBarVisibility(true);
		HitPlant->StartGrowing();
		WateredPlant = HitPlant;
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
