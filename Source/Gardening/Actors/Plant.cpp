// Fill out your copyright notice in the Description page of Project Settings.


#include "Plant.h"
#include "Blueprint/UserWidget.h"
#include "Components/TimelineComponent.h"
#include "Components/WidgetComponent.h"
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

	if (!GrowthCurve) { UE_LOG(LogTemp, Error, TEXT("GrowthCurve is not set on Plant.cpp")); }

	FOnTimelineFloat TimelineProgress;
	TimelineProgress.BindUFunction(this, FName("TimelineProgress"));

	GrowthTimeline.AddInterpFloat(GrowthCurve, TimelineProgress);
	GrowthTimeline.SetLooping(false);
	StartScale = Mesh->GetRelativeScale3D();
	GrowthTimelineLength = GrowthTimeline.GetTimelineLength();

	UActorComponent* ProgressBar = this->GetComponentByClass(UWidgetComponent::StaticClass());
	if (!ProgressBar) { UE_LOG(LogTemp, Error, TEXT("ProgressBar is not found")); }

	ProgressBarWidget = Cast<UWidgetComponent>(ProgressBar);
	if (!ProgressBarWidget) { UE_LOG(LogTemp, Error, TEXT("Couldn't cast ProgressBar component")); }

	ProgressBarWidget->SetVisibility(false);
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
	GrowthTimeline.Play();
}

void APlant::StopGrowing()
{
	GrowthTimeline.Stop();
}

void APlant::TimelineProgress(float Value)
{
	const FVector NewMeshScale = FMath::Lerp(StartScale, MaxScale, Value);
	Mesh->SetRelativeScale3D(NewMeshScale);
	GrowthProgress = GrowthTimeline.GetPlaybackPosition();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void APlant::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                            int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GrowthTimeline.GetPlaybackPosition() >= GrowthTimeline.GetTimelineLength() * 0.4)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Rustle, GetActorLocation());
	}
}

void APlant::SetProgressBarVisibility(bool bSetVisible) const
{
	if (ProgressBarWidget)
	{
		ProgressBarWidget->SetVisibility(bSetVisible);
	}
}
