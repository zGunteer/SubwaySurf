// Fill out your copyright notice in the Description page of Project Settings.

#include "Floor.h"
#include "Subwaytest.h"
#include "Engine/CollisionProfile.h"

// Sets default values
AFloor::AFloor()
{
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RockMesh(TEXT("StaticMesh'/Game/CubeMesh.CubeMesh'"));

	// Create the mesh component
	FloorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ground"));
	RootComponent = FloorMeshComponent;
	FloorMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	FloorMeshComponent->SetStaticMesh(RockMesh.Object);

	FloorMeshComponent->SetWorldScale3D(FVector(2.0F, 2.0F, 0.2F));

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

