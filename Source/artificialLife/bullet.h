// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "bullet.generated.h"

UCLASS()
class ARTIFICIALLIFE_API Abullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Abullet();

	UPROPERTY(EditAnywhere, category = "Components")
	class UStaticMeshComponent* bulletMesh;

	UPROPERTY(EditAnywhere, category = "Components")
	class UProjectileMovementComponent* bulletMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Projectile movement component.
	//UPROPERTY(VisibleAnywhere, category = "movement")
	//UProjectileMovementComponent* ProjectileMovementComponent;

	void FireInDirection(const FVector& ShootDirection);

	UFUNCTION(BlueprintCallable)
	void onHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit);
};
