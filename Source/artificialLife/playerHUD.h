// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h" 
#include "playerHUD.generated.h"

/**
 * 
 */
UCLASS()
class ARTIFICIALLIFE_API AplayerHUD : public AHUD
{
	GENERATED_BODY()


public:
	AplayerHUD();

	// Primary draw call for the HUD.
	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

protected:
	// This will be drawn at the center of the screen.
	UPROPERTY(EditDefaultsOnly)
		UTexture2D* CrosshairTexture;
	
private:
	
	UPROPERTY(EditAnywhere, Category = "health")
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "health")
	class UUserWidget* playerHPWidget;
};