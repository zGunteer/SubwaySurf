// Copyright Epic Games, Inc. All Rights Reserved.

#include "SubwaytestCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Coin.h"
#include "Floor.h"

//////////////////////////////////////////////////////////////////////////
// ASubwaytestCharacter

ASubwaytestCharacter::ASubwaytestCharacter()
{
	//new
	floorDirection = FVector(1,0,0);
	floorPosition = FVector(2000,0,-200);
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	
	//new
	GetCharacterMovement()->MaxWalkSpeed = 800.0F;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 54.0f));
	SpringArm->SetWorldRotation(FRotator(-20.0f, 0.0f, 0.0f));
	SpringArm->AttachTo(RootComponent);
	SpringArm->TargetArmLength = 525.0f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->bEnableCameraRotationLag = false;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = true;

	// Create the chase camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ChaseCamera"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);
	Camera->SetRelativeRotation(FRotator(10.0f, 0.0f, 0.0f));
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;  //end new

	//new 2
	direction = 0;//direction player is facing (values 0 to 3)
	canTurnRight = true;//used to prevent the player turning more than 90 degrees on a single key press
	//end new 2

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASubwaytestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//InputComponent->BindAxis("MoveForward", this, &ASubwaytestCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ASubwaytestCharacter::ChangeDirection);//new 3

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASubwaytestCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASubwaytestCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASubwaytestCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASubwaytestCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ASubwaytestCharacter::OnResetVR);
}


void ASubwaytestCharacter::OnResetVR()
{
	// If Subwaytest is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in Subwaytest.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ASubwaytestCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ASubwaytestCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ASubwaytestCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASubwaytestCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASubwaytestCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASubwaytestCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

//new 3
void ASubwaytestCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (direction == 0)
		MoveForward(1);
	else if (direction == 2)
		MoveForward(-1);
	else if (direction == 1)
		MoveRight(1);
	else if (direction == 3)
		MoveRight(-1);

	//new 6
	AFloor *myFloor;

	//new 7
	ACoin *MyCoin;

	UWorld* const World = GetWorld();
	if (World && count++ > 15) {
		count = 0;
		FVector SpawnLocation = floorPosition;// GetActorLocation() + FVector(0, newCount * 100, -5);
		floorPosition += floorDirection * 400;
		
		//updateFloorDirection(floorDirection);
		const FRotator FloorRotation = FRotator(0, 0, 0);

		myFloor = World->SpawnActor<AFloor>(SpawnLocation + FVector(0,420,0), FloorRotation);
		myFloor = World->SpawnActor<AFloor>(SpawnLocation, FloorRotation);
		myFloor = World->SpawnActor<AFloor>(SpawnLocation + FVector(0,-420,0), FloorRotation);


		SpawnRandomCoins(Coinlocation);
		MyCoin = World->SpawnActor<ACoin>(CoinBluePrint,SpawnLocation + Coinlocation,FloorRotation);

		
	}

	
}

//new 2
void ASubwaytestCharacter::ChangeDirection(float Value)
{

	if(Value != 0 && canTurnRight)
	{
		canTurnRight=false;

		if(Value==1)
		{
			SetActorLocation(GetActorLocation() + FVector(0,420,0));  //right 
		}else if(Value == -1)
			SetActorLocation(GetActorLocation() + FVector(0,-420,0));  //left
	}else if (Value == 0)
		{
			canTurnRight = true;
		}
	return;	
	
	if (Value != 0 && canTurnRight) {
		canTurnRight = false;
		direction += Value;
		if (direction > 3)
			direction = 0;
		else if (direction < 0)
			direction = 3;
	}
	else if (Value == 0)
		canTurnRight = true;
}//end new 2

void ASubwaytestCharacter::SpawnRandomCoins(FVector Location)
{
	float randomCoin = 0.0f;
	int randNum = rand() % 3;

	if (randNum == 0)
		Coinlocation = FVector(0,0,150);
	if (randNum == 1)
		Coinlocation = FVector(0,420,150);
	if (randNum == 2)
		Coinlocation = FVector(0,-420,150);
		
}

/*void ASubwaytestCharacter::updateFloorDirection(FVector& Direction)
{

	float zChange = 0.0F;//used to randomly change the height of the floor piece

	int randNum1 = rand() % 15;
	int randNum2 = rand() % 10;
	if (randNum2 > 7)
		zChange = 0.1;
	else if (randNum2 > 4)
		zChange = -0.1;

	switch (randNum1)
	{

	case 4:
		if (floorDirection.X != -1)
			floorDirection = FVector(1, 0, zChange);
		break;
	case 12:
		if (floorDirection.Y != 1)
			floorDirection = FVector(0, -1, zChange);
		break;
	case 8:
		//newCount = 0;
		if (floorDirection.X != 1)
			floorDirection = FVector(-1, 0, zChange);
		break;
	case 0:
		if (floorDirection.Y != -1)
			floorDirection = FVector(0, 1, zChange);
		break;
	}

}*/
