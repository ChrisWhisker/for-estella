// Fill out your copyright notice in the Description page of Project Settings.


#include "GardeningCharacterHelper.h"
#include "DrawDebugHelpers.h"
#include "Gardening/Actors/Plant.h"

// Sets default values for this component's properties
UGardeningCharacterHelper::UGardeningCharacterHelper()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Tools.Add(TEXT("Seeds"));
	Tools.Add(TEXT("Watering Can"));
	Tools.Add(TEXT("Axe"));
}


// Called when the game starts
void UGardeningCharacterHelper::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UGardeningCharacterHelper::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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

		const float PlantYaw = FMath::RandRange(0.f, 360.f);
		UE_LOG(LogTemp, Warning, TEXT("%f"), PlantYaw);
		const FRotator PlantingDirection = FRotator(0.f, PlantYaw, 0.f);

		if (ActiveTool == 0) // TODO Make this an enum
		{
			GetWorld()->SpawnActor<APlant>(PlantBpClass, Hit.Location, PlantingDirection);
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

	// Rotation.Vector() gives you the vector pointing in the direction of the provided rotation
	const FVector EndLocation = StartLocation + Rotation.Vector() * MaxRange;

	FCollisionQueryParams Params;
	// Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	// DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Magenta, false, 2.f, 0, 10.f);

	return GetWorld()->LineTraceSingleByChannel(OUT Hit, StartLocation, EndLocation,
	                                            ECollisionChannel::ECC_GameTraceChannel1,
	                                            Params);
}

void UGardeningCharacterHelper::SwitchTool(int32 ToolNum)
{
	if (ToolNum == ActiveTool) { return; }

	if (ToolNum == -1) // Go to next item
	{
		if (ActiveTool < Tools.Num() - 1)
		{
			ActiveTool += 1;
		}
		else
		{
			ActiveTool = 0;
		}
	}
	else // Go to selected item
	{
		ActiveTool = ToolNum;
	}

	UE_LOG(LogTemp, Warning, TEXT("%s is active"), *Tools[ActiveTool]);
}
