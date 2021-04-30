// Fill out your copyright notice in the Description page of Project Settings.


#include "SeedPickup.h"
#include "Components/CapsuleComponent.h"
#include "Gardening/Character/GardeningCharacter.h"
#include "Kismet/GameplayStatics.h"

ASeedPickup::ASeedPickup()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	TrunkMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Trunk"));
	TrunkMesh->SetupAttachment(Root);
	
	LeavesMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Leaves"));
	LeavesMesh->SetupAttachment(Root);
	
	Trigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(Root);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASeedPickup::OnOverlapBegin);
}

void ASeedPickup::BeginPlay()
{
	Super::BeginPlay();
}

void ASeedPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	if (!bIsActive) { return; }

	AGardeningCharacter* GardeningCharacter = Cast<AGardeningCharacter>(OtherActor);
	if (!GardeningCharacter) { return; }

	GiveSeeds(GardeningCharacter, SeedsPerPickup);
}

int32 ASeedPickup::GiveSeeds(AGardeningCharacter* Character, int32 SeedsToGive)
{
	SeedsToGive = FMath::Min(SeedsToGive, Character->GetMaxSeeds() - Character->SeedCount);
	Character->SeedCount += SeedsToGive;

	if (SeedsToGive > 0)
	{
		bIsActive = false;
		LeavesMesh->SetHiddenInGame(true);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Rustle, GetActorLocation());
		GetWorldTimerManager().SetTimer(ResetTimer, this, &ASeedPickup::ResetPickup, ResetDelay);
	}
	return SeedsToGive;
}

void ASeedPickup::ResetPickup()
{
	TArray<AActor*> CharactersInTrigger;
	Trigger->GetOverlappingActors(OUT CharactersInTrigger, TSubclassOf<AGardeningCharacter>());
	int32 TotalSeedsGiven = 0;

	// Divide seeds between all characters in trigger
	if (CharactersInTrigger.Num() > 0)
	{
		const int32 SeedsToGive = FMath::Floor(SeedsPerPickup / CharactersInTrigger.Num());

		for (AActor* OverlappingActor : CharactersInTrigger)
		{
			AGardeningCharacter* GardeningCharacter = Cast<AGardeningCharacter>(OverlappingActor);
			if (!GardeningCharacter) { continue; }
			TotalSeedsGiven += GiveSeeds(GardeningCharacter, SeedsToGive);
		}
	}

	if (TotalSeedsGiven == 0)
	{
		bIsActive = true;
		LeavesMesh->SetHiddenInGame(false);
	}
}
