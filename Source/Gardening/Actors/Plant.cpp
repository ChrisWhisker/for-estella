// Fill out your copyright notice in the Description page of Project Settings.


#include "Plant.h"
#include "Blueprint/UserWidget.h"
#include "Components/TimelineComponent.h"
#include "Components/WidgetComponent.h"
#include "Gardening/Character/GardeningCharacterHelper.h"
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

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PlantingSound, GetActorLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PlantingParticle, GetActorLocation());

	const float MaxScaleZ = FMath::RandRange(MaxScale.Z - (MaxScale.Z * MaxHeightVariance),
	                                         MaxScale.Z + (MaxScale.Z * MaxHeightVariance));
	MaxScale.Z = MaxScaleZ;

	if (!GrowthCurve)
	{
		UE_LOG(LogTemp, Error, TEXT("GrowthCurve is not set on Plant.cpp"));
		return;
	}

	FOnTimelineFloat TimelineProgress;
	TimelineProgress.BindUFunction(this, FName("TimelineProgress"));

	GrowthTimeline.AddInterpFloat(GrowthCurve, TimelineProgress);
	GrowthTimeline.SetLooping(false);
	StartScale = Mesh->GetRelativeScale3D();
	GrowthTimelineLength = GrowthTimeline.GetTimelineLength();

	UActorComponent* ProgressBar = this->GetComponentByClass(UWidgetComponent::StaticClass());
	if (!ProgressBar)
	{
		UE_LOG(LogTemp, Error, TEXT("ProgressBar is not found"));
		return;
	}

	ProgressBarWidget = Cast<UWidgetComponent>(ProgressBar);
	if (!ProgressBarWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't cast ProgressBar component"));
		return;
	}

	ProgressBarWidget->SetVisibility(false);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APlant::InitialSetupDelayed, 0.05, false);
}

void APlant::InitialSetupDelayed() const
{
	CharacterHelper->AddFeetToGardenHeight(FMath::RandRange(.25f, .5f));
}

void APlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ProgressBarWidget->IsVisible())
	{
		GrowthTimeline.TickTimeline(DeltaTime);
	}
}

void APlant::StartGrowing()
{
	ProgressBarWidget->SetVisibility(true);
	if (GrowthProgress / GrowthTimelineLength < 1.f)
	{
		GrowthTimeline.Play();
	}
}

void APlant::StopGrowing()
{
	ProgressBarWidget->SetVisibility(false);
	GrowthTimeline.Stop();
}

void APlant::TimelineProgress(float Value)
{
	const float CurrentHeight = Mesh->GetComponentScale().Z;
	const FVector NewMeshScale = FMath::Lerp(StartScale, MaxScale, Value);
	Mesh->SetRelativeScale3D(NewMeshScale);
	GrowthProgress = GrowthTimeline.GetPlaybackPosition();

	const float GrowthPercent = GrowthProgress / GrowthTimelineLength;
	const float FeetToAdd = (NewMeshScale.Z - CurrentHeight) * ScaleToFeetMultiplier;
	CharacterHelper->AddFeetToGardenHeight(FeetToAdd);

	if (GrowthPercent > 0.9f && !bGrowingSoundPlayed)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), RustleSound, GetActorLocation());
		bGrowingSoundPlayed = true;
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void APlant::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                            int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GrowthTimeline.GetPlaybackPosition() >= GrowthTimeline.GetTimelineLength() * 0.4f)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), RustleSound, GetActorLocation());
	}
}
