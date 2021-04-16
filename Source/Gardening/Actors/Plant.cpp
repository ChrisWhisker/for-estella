// Fill out your copyright notice in the Description page of Project Settings.


#include "Plant.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"

APlant::APlant()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &APlant::OnOverlapBegin);
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

void APlant::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                            int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CurveTimeline.GetPlaybackPosition() >= CurveTimeline.GetTimelineLength() * 0.4)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Rustle, GetActorLocation());
	}
}
