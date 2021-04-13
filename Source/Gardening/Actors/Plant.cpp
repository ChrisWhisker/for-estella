// Fill out your copyright notice in the Description page of Project Settings.


#include "Plant.h"

APlant::APlant()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void APlant::BeginPlay()
{
	Super::BeginPlay();
}

void APlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlant::Grow()
// TODO Make this scale on held, not pressed. Look through shooter code for how timers do something similar
{
	if (Mesh->GetRelativeScale3D().Z < MaxHeight)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current component scale is {%f, %f, %f}"), Mesh->GetRelativeScale3D().X,
		       Mesh->GetRelativeScale3D().Y, Mesh->GetRelativeScale3D().Z);
		// float NewZScale = FMath::Lerp(Mesh->GetRelativeScale3D().Z, MaxHeight,
		//                               GrowSpeed * GetWorld()->GetDeltaSeconds());
		// UE_LOG(LogTemp, Warning, TEXT("NewZScale (lerped) is %f"), NewZScale);
		// FVector NewMeshScale = FVector(Mesh->GetRelativeScale3D().X, Mesh->GetRelativeScale3D().Y, NewZScale);
		FVector NewMeshScale = FVector(1.f, 1.f, 3.f);
		UE_LOG(LogTemp, Warning, TEXT("NewMeshScale (FVector) is {%f, %f, %f}"), NewMeshScale.X, NewMeshScale.Y,
		       NewMeshScale.Z);

		Mesh->SetRelativeScale3D(NewMeshScale);
	}
}
