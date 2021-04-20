// Fill out your copyright notice in the Description page of Project Settings.


#include "SeedPickup.h"
#include "Components/CapsuleComponent.h"
#include "Gardening/Character/GardeningCharacterHelper.h"
#include "Gardening/Character/GardeningCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ASeedPickup::ASeedPickup()
{
	PrimaryActorTick.bCanEverTick = true;

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
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	if (!bIsActive) { return; }

	AGardeningCharacter* GardeningCharacter = Cast<AGardeningCharacter>(OtherActor);
	if (!GardeningCharacter) { return; }

	const int32 SeedsToAdd = FMath::Min(3, Helper->GetMaxSeeds() - Helper->GetSeedCount());
	Helper->SetSeedCount(Helper->GetSeedCount() + SeedsToAdd);

	if (SeedsToAdd > 0)
	{
		bIsActive = false;
		LeavesMesh->SetHiddenInGame(true);
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Rustle, GetActorLocation());
		GetWorldTimerManager().SetTimer(ResetTimer, this, &ASeedPickup::ResetPickup, ResetDelay);
	}
}

void ASeedPickup::ResetPickup()
{
	bIsActive = true;
	LeavesMesh->SetHiddenInGame(false);
}
