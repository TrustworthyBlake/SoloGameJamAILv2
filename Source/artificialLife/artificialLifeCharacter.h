// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "artificialLifeCharacter.generated.h"

UCLASS(config=Game)
class AartificialLifeCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* TwinBlast_Electro;
public:
	AartificialLifeCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;


	// Custom logic
	UPROPERTY(EditAnywhere, Category = "shooting")
	TSubclassOf<class Abullet> BPbullet;

	// Player health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "health")
	float maxPlayerHP;

	UPROPERTY(ReplicatedUsing = onRep_currentPlayerHP, EditAnywhere, BlueprintReadWrite, Category = "health") //-------->
	float currentPlayerHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "health")
	float tempPlayerHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "health")
	float playerHPpercent;

	UFUNCTION(BlueprintPure, Category = "health")
	float getPlayerHP();

	UFUNCTION(BlueprintCallable, Category = "health")
	void updatePlayerHP(float HP);

	//UFUNCTION()
	//void setPlayerDamage();

	UFUNCTION(BlueprintCallable, Category = "health")
	void playerTakeDamage(float damage);

	// Gun muzzle offset from the camera location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	UPROPERTY(ReplicatedUsing = onRep_Kill, BlueprintReadOnly, Category = Gameplay)
	AartificialLifeCharacter* killer;

	UFUNCTION()
	void onRep_kill();

	UFUNCTION(BlueprintImplementableEvent)
	void displayDeathScreen();

	UFUNCTION()
	void onRep_currentPlayerHP();

	//UPROPERTY(ReplicatedUsing = onRep_currentPlayerHP)
	//;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* AnimMontage;



	FORCEINLINE class USkeletalMeshComponent* GetMesh() const { return TwinBlast_Electro; }

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)//, WithValidation)
	void serverOnShoot();

	//bool serverOnShootValidate();

	void shoot();

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	// Custom logic
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

