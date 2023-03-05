// Fill out your copyright notice in the Description page of Project Settings.

#include "Coin.h"
#include "Subwaytest.h"
#include "Engine/CollisionProfile.h"
#include "SubwaytestCharacter.h"


// Sets default values
ACoin::ACoin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	myCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	myCollisionSphere->InitSphereRadius(150.0f);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> GoodMesh(TEXT("/Game/StarterContent/Shapes/Shape_Torus.Shape_Torus"));
	
	// Create the mesh component
	CollectableMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GoodMesh"));
	RootComponent = myCollisionSphere;
	CollectableMeshComponent->SetStaticMesh(GoodMesh.Object);
	CollectableMeshComponent->SetWorldScale3D(FVector(.5f, .5f, .5f));

	CollectableMeshComponent->AttachTo(RootComponent);

	

}

// Called when the game starts or when spawned
void ACoin::BeginPlay()
{
	Super::BeginPlay();
	myCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACoin::HandleCollision);
}

// Called every frame
void ACoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACoin::HandleCollision(UPrimitiveComponent* OnComponentBeginOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FString name = OtherActor->GetName();
	if (!name.Contains("MySubwaytestCharacter"))
		return;

	UWorld* const World = GetWorld();
	if (World)
		Destroy();

}

