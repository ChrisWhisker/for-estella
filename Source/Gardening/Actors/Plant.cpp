// Fill out your copyright notice in the Description page of Project Settings.


#include "Plant.h"
#include "Components/TimelineComponent.h"

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

	if (GrowthCurve)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
		CurveTimeline.AddInterpFloat(GrowthCurve, TimelineProgress);
		CurveTimeline.SetLooping(false);
		StartScale = Mesh->GetRelativeScale3D();
	}
}

void APlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurveTimeline.TickTimeline(DeltaTime);
}

void APlant::TimelineProgress(float Value)
{
	FVector NewMeshScale = FMath::Lerp(StartScale, MaxScale, Value);
	Mesh->SetRelativeScale3D(NewMeshScale);
}

void APlant::StartGrowing()
{
	CurveTimeline.Play();
}

void APlant::StopGrowing()
{
	CurveTimeline.Stop();
}
