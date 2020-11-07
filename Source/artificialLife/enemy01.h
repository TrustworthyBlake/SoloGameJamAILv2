// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "enemy01.generated.h"

UCLASS()
class ARTIFICIALLIFE_API Aenemy01 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	Aenemy01();

	// Custom logic
	UPROPERTY(EditAnywhere, Category = "shooting")
	TSubclassOf<class Abullet> BPbullet;

	// Player health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "health")
		float maxEnemyHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "health")
		float currentEnemyHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "health")
		float tempEnemyHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "health")
		float enemyHPpercent;

	UFUNCTION(BlueprintPure, Category = "health")
		float getEnemyHP();

	UFUNCTION(BlueprintCallable, Category = "health")
		void updateEnemyHP(float HP);

	UFUNCTION(BlueprintCallable, Category = "health")
	void enemyTakeDamage(float damage);

	UFUNCTION(BlueprintCallable, Category = "shooting")
	void shoot();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere, Category = "health")
		TSubclassOf<class UUserWidget> HUDWidgetClass2;

	UPROPERTY(EditAnywhere, Category = "health")
		class UUserWidget* enemyHPWidget;

};
