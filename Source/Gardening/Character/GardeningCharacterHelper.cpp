// Fill out your copyright notice in the Description page of Project Settings.


#include "GardeningCharacterHelper.h"
#include "DrawDebugHelpers.h"
#include "Gardening/Actors/Plant.h"
#include "Kismet/GameplayStatics.h"

UGardeningCharacterHelper::UGardeningCharacterHelper()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGardeningCharacterHelper::BeginPlay()
{
	Super::BeginPlay();
}

void UGardeningCharacterHelper::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult Hit;
	bool bSuccess = Trace(Hit);
	if (!bSuccess) { return; }
	if (!Cast<APlant>(Hit.GetActor()))
	{
		StopUsingTool();
	}
}

void UGardeningCharacterHelper::UseTool()
{
	FHitResult Hit;
	bool bSuccess = Trace(Hit);
	// UE_LOG(LogTemp, Warning, TEXT("%s"), bSuccess ? TEXT("true") : TEXT("false"));

	if (bSuccess)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor) { return; }
		UE_LOG(LogTemp, Warning, TEXT("%s hit"), *HitActor->GetName());

		if (Tools[ActiveTool] == Tool_Seeds)
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
}

void UGardeningCharacterHelper::StopUsingTool()
{
	if (WateredPlant)
	{
		WateredPlant->StopGrowing();
	}
}

APlant* UGardeningCharacterHelper::PlantSeed(FHitResult Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit class is %s"), *Hit.GetActor()->GetClass()->GetName());

	if (!Hit.GetActor()->GetClass()->GetName().Contains(TEXT("Landscape")))
	{
		return nullptr;
	}

	const float PlantYaw = FMath::RandRange(0.f, 360.f);
	const FRotator PlantingDirection = FRotator(0.f, PlantYaw, 0.f);
	AActor* Spawned = GetWorld()->SpawnActor<APlant>(PlantBpClass, Hit.Location, PlantingDirection);
	APlant* SpawnedPlant = Cast<APlant>(Spawned);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PlantingSound, SpawnedPlant->GetActorLocation());

	if (SpawnedPlant) { return SpawnedPlant; }
	return nullptr;
}

void UGardeningCharacterHelper::WaterPlant(FHitResult Hit)
{
	APlant* HitPlant = Cast<APlant>(Hit.GetActor());
	if (HitPlant)
	{
		HitPlant->StartGrowing();
		WateredPlant = HitPlant;
	}
}

void UGardeningCharacterHelper::UseAxe()
{
	return;
}

bool UGardeningCharacterHelper::Trace(FHitResult& Hit)
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

void UGardeningCharacterHelper::SwitchTool(const int32 NewToolIndex)
{
	if (NewToolIndex == ActiveTool) { return; }

	if (NewToolIndex == -1) // Go to next item
	{
		ActiveTool = (ActiveTool + 1) % Tools.Num();
	}
	else // Go to selected item
	{
		ActiveTool = NewToolIndex;
	}
}
