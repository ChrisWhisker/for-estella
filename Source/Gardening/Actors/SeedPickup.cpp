// Fill out your copyright notice in the Description page of Project Settings.


#include "SeedPickup.h"
#include "Components/CapsuleComponent.h"
#include "Gardening/Character/GardeningCharacterHelper.h"
#include "Gardening/Pawns/GardeningCharacter.h"
#include "Kismet/GameplayStatics.h"

ASeedPickup::ASeedPickup()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Trigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(Root);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASeedPickup::OnOverlapBegin);
}

void ASeedPickup::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* GenericCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AGardeningCharacter* GardeningCharacter = Cast<AGardeningCharacter>(GenericCharacter);
	if (!GardeningCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't get the GardeningCharacter in SeedPickup"));
	}
	Helper = GardeningCharacter->Helper;
}

void ASeedPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASeedPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp,
                                 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const int32 SeedsToAdd = FMath::Min(3, Helper->GetMaxSeeds() - Helper->GetSeedCount());
	Helper->SetSeedCount(Helper->GetSeedCount() + SeedsToAdd);
}
