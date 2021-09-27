// Copyright 2021 Chris Worcester All Rights Reserved.


#include "Axe.h"
#include "Components/BoxComponent.h"
#include "Gardening/Character/GardeningCharacter.h"
#include "Plant.h"

AAxe::AAxe()
{
	PrimaryActorTick.bCanEverTick = false;

	// Put the mesh in the character's hand
	const FVector MeshTranslation = FVector(50.057, -28.216, 3.24);
	const FRotator MeshRotation = FRotator(-0.312, 159.39, 255.057);
	const FVector MeshScale = FVector(2.5, 2.5, 2.5);
	Mesh->SetRelativeTransform(FTransform(MeshRotation, MeshTranslation, MeshScale), false);
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Put trigger on axe head and set up collision
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Impact Trigger"));
	Trigger->SetupAttachment(Mesh);
	Trigger->SetRelativeLocation(FVector(1.667, 0, -1.334));
	Trigger->SetBoxExtent(FVector(8, 3, 8.6));
	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
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
	if (!Character->bIsAttacking || !bAxeHeadActive) { return; }

	APlant* HitPlant = Cast<APlant>(OtherActor);
	if (HitPlant)
	{
		const FPointDamageEvent DamageEvent(DamagePerStrike, FHitResult(), FVector(0, 0, 0), nullptr);
		HitPlant->TakeDamage(DamagePerStrike, DamageEvent, Character->GetController(), this);
	}
}
