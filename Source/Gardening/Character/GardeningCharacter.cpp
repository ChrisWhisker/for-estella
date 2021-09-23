// Fill out your copyright notice in the Description page of Project Settings.

#include "GardeningCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Gardening/Actors/Axe.h"
#include "Gardening/Actors/Bucket.h"
#include "Gardening/Actors/Plant.h"
#include "Gardening/Actors/Sack.h"

//////////////////////////////////////////////////////////////////////////
// AGardeningCharacter

AGardeningCharacter::AGardeningCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	WateringTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Watering Trigger"));
	WateringTrigger->SetupAttachment(GetMesh());

	WateringTrigger->OnComponentBeginOverlap.AddDynamic(this, &AGardeningCharacter::OnTriggerOverlapBegin);
	WateringTrigger->OnComponentEndOverlap.AddDynamic(this, &AGardeningCharacter::OnTriggerOverlapEnd);

	SeedCount = MaxSeeds;
}

void AGardeningCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!PlantClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Plant Blueprint Class is not set on the character."));
		return;
	}

	// Spawn tool actors in the character's hand and hide inactive ones
	if (!AxeClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Axe class is missing on the Gardening Character"));
		return;
	}
	Axe = GetWorld()->SpawnActor<AAxe>(AxeClass);
	Axe->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("ToolSocket"));
	Axe->SetActorHiddenInGame(true);
	Axe->SecondarySetup(this);

	if (!BucketClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Bucket class is missing on the Gardening Character"));
		return;
	}
	Bucket = GetWorld()->SpawnActor<ABucket>(BucketClass);
	Bucket->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("ToolSocket"));
	Bucket->SetOwner(this);
	Bucket->SetActorHiddenInGame(true);

	if (!SackClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Sack class is missing on the Gardening Character"));
		return;
	}
	Sack = GetWorld()->SpawnActor<ASack>(SackClass);
	Sack->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("ToolSocket"));
	Sack->SetOwner(this);
}

void AGardeningCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AGardeningCharacter::FirePressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AGardeningCharacter::FireReleased);
	PlayerInputComponent->BindAction("SwitchTool", IE_Released, this, &AGardeningCharacter::SwitchTool);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AGardeningCharacter::OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                const FHitResult& SweepResult)
{
	if (!bIsPourWaterHeld) { return; }
	APlant* Plant = Cast<APlant>(OtherActor);
	if (!Plant) { return; }

	StartWateringPlant(Plant);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AGardeningCharacter::OnTriggerOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlant* Plant = Cast<APlant>(OtherActor);
	if (!Plant) { return; }

	StopWateringPlant(Plant);
}

void AGardeningCharacter::SwitchTool()
{
	const int32 NewActiveToolIndex = (ActiveToolIndex + 1) % Tools.Num();
	SetActiveTool(Tools[NewActiveToolIndex]);
}

void AGardeningCharacter::SetActiveTool(const FString NewActiveTool)
{
	// If the tool is already active
	if (Tools[ActiveToolIndex] == NewActiveTool)
	{
		return;
	}

	const bool bSeedsActive = NewActiveTool == Tool_Seeds;
	const bool bWaterActive = NewActiveTool == Tool_Water;
	const bool bAxeActive = NewActiveTool == Tool_Axe;

	Sack->SetActorHiddenInGame(!bSeedsActive);
	Bucket->SetActorHiddenInGame(!bWaterActive);
	Axe->SetActorHiddenInGame(!bAxeActive);
	Axe->Trigger->SetActive(bAxeActive);

	ActiveToolIndex = Tools.Find(NewActiveTool);
}

void AGardeningCharacter::FirePressed()
{
	if (Tools[ActiveToolIndex] == Tool_Seeds)
	{
		if (SeedCount <= 0) { return; }

		FHitResult Hit;
		const bool bTraceSucceeded = Trace(Hit);
		if (!bTraceSucceeded) { return; }
		AActor* HitActor = Hit.GetActor();
		if (!HitActor) { return; }
		PlantSeed(Hit);
	}
	else if (Tools[ActiveToolIndex] == Tool_Water)
	{
		bIsPourWaterHeld = true;
		StartWatering();
	}
	else if (Tools[ActiveToolIndex] == Tool_Axe)
	{
		UseAxe();
	}
}

void AGardeningCharacter::FireReleased()
{
	bIsAttacking = false;
	bIsPourWaterHeld = false;
	StopWatering();
}

void AGardeningCharacter::PlantSeed(const FHitResult Hit)
{
	if (!Hit.GetActor()->GetClass()->GetName().Contains(TEXT("Landscape")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't plant a seed on [%s: %s]"), *Hit.GetActor()->GetName(),
		       *Hit.GetComponent()->GetName());
		return;
	}

	const float PlantYaw = FMath::RandRange(0.f, 360.f);
	const FRotator PlantingDirection = FRotator(0.f, PlantYaw, 0.f);
	AActor* Spawned = GetWorld()->SpawnActor<APlant>(PlantClass, Hit.Location, PlantingDirection);
	APlant* SpawnedPlant = Cast<APlant>(Spawned);

	if (SpawnedPlant)
	{
		SeedCount--;
	}
}

void AGardeningCharacter::StartWatering()
{
	TArray<AActor*> PlantsInTrigger;
	WateringTrigger->GetOverlappingActors(PlantsInTrigger, TSubclassOf<APlant>());

	for (AActor* PlantActor : PlantsInTrigger)
	{
		APlant* Plant = Cast<APlant>(PlantActor);
		if (!Plant) { continue; }

		StartWateringPlant(Plant);
	}

	Bucket->StartPouring();
}

void AGardeningCharacter::StopWatering()
{
	for (APlant* WateredPlant : WateredPlants)
	{
		WateredPlant->StopGrowing();
	}
	WateredPlants.Empty();

	Bucket->StopPouring();
}

void AGardeningCharacter::StartWateringPlant(APlant* PlantToWater)
{
	PlantToWater->StartGrowing();

	if (!WateredPlants.Contains(PlantToWater))
	{
		WateredPlants.Add(PlantToWater);
	}
}

void AGardeningCharacter::StopWateringPlant(APlant* WateredPlant)
{
	WateredPlant->StopGrowing();
	WateredPlants.Remove(WateredPlant);
}

void AGardeningCharacter::UseAxe()
{
	bIsAttacking = true;
}

bool AGardeningCharacter::Trace(FHitResult& Hit) const
{
	AController* OwnerController = GetController();
	if (!OwnerController) { return false; }

	FVector StartLocation;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(OUT StartLocation, OUT Rotation);

	const FVector EndLocation = StartLocation + Rotation.Vector() * MaxTraceRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(Axe);

	return GetWorld()->LineTraceSingleByChannel(OUT Hit, StartLocation, EndLocation,
	                                            ECollisionChannel::ECC_GameTraceChannel1, Params);
}
