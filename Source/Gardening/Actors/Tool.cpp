// Fill out your copyright notice in the Description page of Project Settings.


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
