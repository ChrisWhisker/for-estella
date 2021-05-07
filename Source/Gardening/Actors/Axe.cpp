// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe.h"
#include "Components/BoxComponent.h"
#include "Gardening/Character/GardeningCharacter.h"
#include "Plant.h"

AAxe::AAxe()
{
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(Mesh);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AAxe::OnOverlapBegin);
	Trigger->SetActive(false); // Inactive until player switches tools
}

void AAxe::SecondarySetup(AGardeningCharacter* Char)
{
	Character = Char;
	if (!Character)
	{
		UE_LOG(LogTemp, Error, TEXT("Character not found on Plant"));
		return;
	}
}

void AAxe::BeginPlay()
{
	Super::BeginPlay();
}

void AAxe::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                          int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Character)
	{
		UE_LOG(LogTemp, Error, TEXT("Character not found on Axe"));
		return;
	}

	if (!Character->bIsAttacking)
	{
		return;
	}

	// TODO Height is not being reflected on HUD correctly
	APlant* HitPlant = Cast<APlant>(OtherActor);
	if (HitPlant)
	{
		const float KilledPlantHeight = HitPlant->CutDown();
		UE_LOG(LogTemp, Warning, TEXT("KilledPlantHeight is %f"), KilledPlantHeight);
		Character->AddGardenHeightFeet(-KilledPlantHeight);
	}
}
