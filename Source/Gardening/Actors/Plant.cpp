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

void APlant::Grow() // TODO Make this scale on held, not pressed
{
	if (Mesh->GetRelativeScale3D().Z < MaxHeight)
	{
		float NewZScale = FMath::Lerp(this->GetActorScale().Z, MaxHeight, .5f * GrowSpeed);
		UE_LOG(LogTemp, Warning, TEXT("NewZScale (lerped) is %f"), NewZScale);
		FVector NewMeshScale = FVector(Mesh->GetRelativeScale3D().X, Mesh->GetRelativeScale3D().Y, NewZScale);
		UE_LOG(LogTemp, Warning, TEXT("NewMeshScale (FVector) is {%f, %f, %f}"), NewMeshScale.X, NewMeshScale.Y,
		       NewMeshScale.Z);
		Mesh->SetRelativeScale3D(NewMeshScale);
	}
}
