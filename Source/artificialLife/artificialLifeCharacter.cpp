// Copyright Epic Games, Inc. All Rights Reserved.

#include "artificialLifeCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "bullet.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "tdmGameMode.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Animation/AnimInstance.h"

//////////////////////////////////////////////////////////////////////////
// AartificialLifeCharacter

AartificialLifeCharacter::AartificialLifeCharacter()
{
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

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AartificialLifeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AartificialLifeCharacter::shoot);

	PlayerInputComponent->BindAxis("MoveForward", this, &AartificialLifeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AartificialLifeCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AartificialLifeCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AartificialLifeCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AartificialLifeCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AartificialLifeCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AartificialLifeCharacter::OnResetVR);
}

void AartificialLifeCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AartificialLifeCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AartificialLifeCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AartificialLifeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AartificialLifeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AartificialLifeCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AartificialLifeCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
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

void AartificialLifeCharacter::BeginPlay() {
	// Call the base class  
	Super::BeginPlay();

	maxPlayerHP = 100.0f;
	currentPlayerHP = maxPlayerHP;
	playerHPpercent = 1.0f;
}

float AartificialLifeCharacter::getPlayerHP() {
	return playerHPpercent;
}

void AartificialLifeCharacter::updatePlayerHP(float HP) {
	currentPlayerHP += HP;
	currentPlayerHP = FMath::Clamp(currentPlayerHP, 0.0f, maxPlayerHP);
	tempPlayerHP = playerHPpercent;
	playerHPpercent = currentPlayerHP / maxPlayerHP;
	UE_LOG(LogTemp, Warning, TEXT("hp should update"));
	/*if (currentPlayerHP <= 0.0f) {
		onRep_kill();
	}*/
}

void AartificialLifeCharacter::playerTakeDamage(float damage) {
	updatePlayerHP(-damage);
}

void AartificialLifeCharacter::onRep_currentPlayerHP() {
	updatePlayerHP(0);
}

void AartificialLifeCharacter::serverOnShoot_Implementation() {
	shoot();
}


void AartificialLifeCharacter::shoot() {
	//UE_LOG(LogTemp, Warning, TEXT("shoot pressed"));

	if (!HasAuthority()) {
		if (AnimMontage != NULL) {
			UAnimInstance* AnimInstance = TwinBlast_Electro->GetAnimInstance();//AnimMontage'/Game/ParagonTwinblast/Characters/Heroes/TwinBlast/Animations/DoubleShot_Fire_Lft_Montage.DoubleShot_Fire_Lft_Montage'
			//animInstance->Montage_Play(AttackMontage->Montage, 1.0f, EMontagePlayReturnType::Duration, 0.0f, true);
			if (AnimInstance != nullptr)
			{
				AnimInstance->Montage_Play(AnimMontage, 1.f);
			}
		}
		serverOnShoot();
	}
	else {
		// Get the camera transform.
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// Set MuzzleOffset to spawn projectiles slightly in front of the camera.
		MuzzleOffset.Set(100.0f, 60.0f, -40.0f);

		// Transform MuzzleOffset from camera space to world space.
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		// Skew the aim to be slightly upwards.
		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 2.0f;

		UWorld* World = GetWorld();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		Abullet* bullet = World->SpawnActor<Abullet>(BPbullet, MuzzleLocation, MuzzleRotation, SpawnParams);

		if (bullet) {
			FVector LaunchDirection = MuzzleRotation.Vector();
			bullet->FireInDirection(LaunchDirection);
		}
	}
}

/*bool AartificialLifeCharacter::serverOnShootValidate() {
	return true;
}*/

void AartificialLifeCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AartificialLifeCharacter, killer);
	DOREPLIFETIME(AartificialLifeCharacter, currentPlayerHP);
}

void AartificialLifeCharacter::onRep_kill() {
	if (IsLocallyControlled()) {
		displayDeathScreen();
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Block);
	SetLifeSpan(05.0f);
	//Destroy();
}