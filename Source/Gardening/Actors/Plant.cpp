// Copyright 2021 Chris Worcester All Rights Reserved.


#include "Plant.h"
#include "Blueprint/UserWidget.h"
#include "Components/TimelineComponent.h"
#include "Components/WidgetComponent.h"
#include "Gardening/Game/GardeningGameState.h"
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

	if (GetWorld() == nullptr) { UE_LOG(LogTemp, Error, TEXT("GetWorld() returned null so game state is not set.")); }
	else
	{
		GameState = GetWorld()->GetGameState<AGardeningGameState>();
	}

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
	Health = MaxHealth;
}

void APlant::SecondarySetup(const int32 TeamNum)
{
	TeamNumber = TeamNum;
	AddHeight(FMath::RandRange(.25f, .5f));
	GameState->AddPlantToGarden(TeamNum, this);
}

void APlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ProgressBarWidget && ProgressBarWidget->IsVisible())
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
	GameState->AddGardenHeightFeet(TeamNumber, FeetToAdd);
	HeightInFeet += FeetToAdd;
	return true;
}

float APlant::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                         AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("Health left: %f"), Health);

	if (Health <= 0)
	{
		CutDown();
	}

	return DamageToApply;
}

float APlant::CutDown()
{
	if (GetWorld() != nullptr)
	{
		GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &APlant::DestroySelf, 0.1f, false);
		GameState->AddGardenHeightFeet(TeamNumber, -HeightInFeet);
		GameState->RemovePlantFromGarden(TeamNumber, this);
		return HeightInFeet;
	}
	return 0;
}

void APlant::DestroySelf()
{
	Destroy();
}
