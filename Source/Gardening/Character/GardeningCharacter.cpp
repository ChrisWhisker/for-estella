// Fill out your copyright notice in the Description page of Project Settings.

#include "GardeningCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Gardening/Actors/Axe.h"
#include "Gardening/Actors/Bucket.h"
#include "Gardening/Actors/Plant.h"
#include "Gardening/Actors/Sack.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

//////////////////////////////////////////////////////////////////////////
// AGardeningCharacter

AGardeningCharacter::AGardeningCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	WateringTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Watering Trigger"));
	WateringTrigger->SetupAttachment(GetMesh());

	WateringTrigger->OnComponentBeginOverlap.AddDynamic(this, &AGardeningCharacter::OnTriggerOverlapBegin);
	WateringTrigger->OnComponentEndOverlap.AddDynamic(this, &AGardeningCharacter::OnTriggerOverlapEnd);

	WaterSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Water Spawn Point"));
	WaterSpawnPoint->SetupAttachment(GetMesh());

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

	// Spawn tool actors in the character's hand and hide inactive ones
	if (!AxeClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Axe class is missing on the Gardening Character"));
		return;
	}
	Axe = GetWorld()->SpawnActor<AAxe>(AxeClass);
	Axe->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("ToolSocket"));
	Axe->SetOwner(this);
	Axe->SetActorHiddenInGame(true);

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
	ActiveToolIndex = (ActiveToolIndex + 1) % Tools.Num();

	if (Tools[ActiveToolIndex] == Tool_Seeds)
	{
		Sack->SetActorHiddenInGame(false);
		Bucket->SetActorHiddenInGame(true);
		Axe->SetActorHiddenInGame(true);
	}
	else if (Tools[ActiveToolIndex] == Tool_Water)
	{
		Sack->SetActorHiddenInGame(true);
		Bucket->SetActorHiddenInGame(false);
		Axe->SetActorHiddenInGame(true);
	}
	else if (Tools[ActiveToolIndex] == Tool_Axe)
	{
		Sack->SetActorHiddenInGame(true);
		Bucket->SetActorHiddenInGame(true);
		Axe->SetActorHiddenInGame(false);
	}
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
		UE_LOG(LogTemp, Warning, TEXT("Watering"));
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
	bIsPourWaterHeld = false;
	StopWatering();
}

void AGardeningCharacter::PlantSeed(const FHitResult Hit)
{
	if (!Hit.GetActor()->GetClass()->GetName().Contains(TEXT("Landscape")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *Hit.GetActor()->GetName());
		return;
	}

	const float PlantYaw = FMath::RandRange(0.f, 360.f);
	const FRotator PlantingDirection = FRotator(0.f, PlantYaw, 0.f);
	AActor* Spawned = GetWorld()->SpawnActor<APlant>(PlantClass, Hit.Location, PlantingDirection);
	APlant* SpawnedPlant = Cast<APlant>(Spawned);

	if (SpawnedPlant)
	{
		SpawnedPlant->Character = this;
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

	WaterSoundComponent = UGameplayStatics::SpawnSoundAttached(WaterSound, WateringTrigger);
	WaterSoundComponent->Stop();
	WaterSoundComponent->FadeIn(WaterSoundFadeInSeconds, 1.f);
	WateringParticleComponent = UGameplayStatics::SpawnEmitterAttached(WaterParticle, WaterSpawnPoint);
}

void AGardeningCharacter::StopWatering()
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

void AGardeningCharacter::UseAxe() const
{
	UE_LOG(LogTemp, Warning, TEXT("Axe used."));
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

	return GetWorld()->LineTraceSingleByChannel(OUT Hit, StartLocation, EndLocation,
	                                            ECollisionChannel::ECC_GameTraceChannel1, Params);
}

void AGardeningCharacter::SetGardenHeightFeet(const float HeightInFeet)
{
	GardenHeightFeet = HeightInFeet;
}
