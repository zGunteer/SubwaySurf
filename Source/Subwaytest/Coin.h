// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Coin.generated.h"

UCLASS()
class SUBWAYTEST_API ACoin : public AActor
{
	GENERATED_BODY()
	
	/** Sphere collision component */
    	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Enemy, meta = (AllowPrivateAccess = "true"))
    	UStaticMeshComponent* CollectableMeshComponent;
	
	UPROPERTY(EditAnywhere)
	USphereComponent *myCollisionSphere;

public:	
	// Sets default values for this actor's properties
	ACoin();

	UFUNCTION()
	void HandleCollision(UPrimitiveComponent* OnComponentBeginOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	


};
