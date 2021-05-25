// Fill out your copyright notice in the Description page of Project Settings.


#include "Plant.h"
#include "Blueprint/UserWidget.h"
#include "Components/TimelineComponent.h"
#include "Components/WidgetComponent.h"
#include "Gardening/Character/GardeningCharacter.h"
#include "Kismet/GameplayStatics.h"

APlant::APlant()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &APlant::OnOverlapBegin);
	Mesh->SetRelativeScale3D(FVector(0.1, 0.1, 0.1));
}

void APlant::BeginPlay()
{
	Super::BeginPlay();

	if (!RustleSound)
	{
		UE_LOG(LogTemp, Error, TEXT("RustleSound is not set on Plant.cpp"));
		return;
	}

	if (!PlantingSound)
	{
		UE_LOG(LogTemp, Error, TEXT("PlantingSound is not set on Plant.cpp"));
		return;
	}

	if (!PlantingParticle)
	{
		UE_LOG(LogTemp, Error, TEXT("PlantingParticle is not set on Plant.cpp"));
		return;
	}

	if (!GrowthCurve)
	{
		UE_LOG(LogTemp, Error, TEXT("GrowthCurve is not set on Plant.cpp"));
		return;
	}

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
	InitialScale = Mesh->GetRelativeScale3D();
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
}

void APlant::SecondarySetup(AGardeningCharacter* Char)
{
	Character = Char;
	if (!Character)
	{
		UE_LOG(LogTemp, Error, TEXT("Character not found on Plant"));
		return;
	}
	HeightChanged.BindUObject(Character, &AGardeningCharacter::AddGardenHeightFeet);

	AddHeight(FMath::RandRange(.25f, .5f));
}

void APlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ProgressBarWidget && ProgressBarWidget->IsVisible())
	{
		GrowthTimeline.TickTimeline(DeltaTime);
	}

	UE_LOG(LogTemp, Display, TEXT("HeightInFeet is %f"), HeightInFeet);
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
	const FVector NewMeshScale = FMath::Lerp(InitialScale, MaxScale, Value);
	Mesh->SetRelativeScale3D(NewMeshScale);
	GrowthProgress = GrowthTimeline.GetPlaybackPosition();

	const float GrowthPercent = GrowthProgress / GrowthTimelineLength;
	const float FeetToAdd = (NewMeshScale.Z - CurrentHeight) * ScaleToFeetMultiplier;
	const bool bHeightAdded = AddHeight(FeetToAdd);

	if (bHeightAdded && GrowthPercent > 0.9f && !bGrowingSoundPlayed)
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

bool APlant::AddHeight(const float FeetToAdd)
{
	const bool bIsBound = HeightChanged.ExecuteIfBound(FeetToAdd);
	if (!bIsBound)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeightChanged is not bound to Character."));
		return false;
	}

	HeightInFeet += FeetToAdd;
	return true;
}

// TODO Cause damage first
float APlant::CutDown()
{
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &APlant::DestroySelf, 0.1f, false);
	return HeightInFeet;
}

void APlant::DestroySelf()
{
	Destroy();
}
