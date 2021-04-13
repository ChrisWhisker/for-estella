// Fill out your copyright notice in the Description page of Project Settings.


#include "GardeningCharacterHelper.h"
#include "DrawDebugHelpers.h"
#include "Gardening/Actors/Plant.h"

UGardeningCharacterHelper::UGardeningCharacterHelper()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Tools.Add(TEXT("Seeds"));
	// Tools.Add(TEXT("Watering Can"));
	// Tools.Add(TEXT("Axe"));
}


void UGardeningCharacterHelper::BeginPlay()
{
	Super::BeginPlay();
}

void UGardeningCharacterHelper::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGardeningCharacterHelper::UseTool()
{
	// UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	// UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = Trace(Hit, ShotDirection);
	// UE_LOG(LogTemp, Warning, TEXT("%s"), bSuccess ? TEXT("true") : TEXT("false"));

	if (bSuccess)
	{
		// UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
		// UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
		AActor* HitActor = Hit.GetActor();
		if (!HitActor) { return; }
		UE_LOG(LogTemp, Warning, TEXT("%s hit"), *HitActor->GetName());

		if (Tools[ActiveTool] == Tool_Seeds)
		{
			const float PlantYaw = FMath::RandRange(0.f, 360.f);
			// UE_LOG(LogTemp, Warning, TEXT("%f"), PlantYaw);
			const FRotator PlantingDirection = FRotator(0.f, PlantYaw, 0.f);
			GetWorld()->SpawnActor<APlant>(PlantBpClass, Hit.Location, PlantingDirection);
		}
		else if (Tools[ActiveTool] == Tool_WateringCan)
		{
			APlant* HitPlant = Cast<APlant>(HitActor);
			if (HitPlant)
			{
				HitPlant->Grow();
			}
		}
	}
}

bool UGardeningCharacterHelper::Trace(FHitResult& Hit, FVector& ShotDirection)
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) { return false; }
	AController* OwnerController = OwnerPawn->GetController();
	if (!OwnerController) { return false; }

	FVector StartLocation;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(OUT StartLocation, OUT Rotation);
	ShotDirection = -Rotation.Vector();
	const FVector EndLocation = StartLocation + Rotation.Vector() * MaxRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	// DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Magenta, false, 2.f, 0, 10.f);
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

	// UE_LOG(LogTemp, Warning, TEXT("%s is active"), *Tools[ActiveTool]);
}
