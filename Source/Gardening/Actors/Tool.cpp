// Copyright 2021 Chris Worcester All Rights Reserved.


#include "Tool.h"

ATool::ATool()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void ATool::BeginPlay()
{
	Super::BeginPlay();
}
