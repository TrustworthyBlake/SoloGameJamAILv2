// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "tdmGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARTIFICIALLIFE_API AtdmGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AtdmGameMode();
	virtual void PostLogin(APlayerController* newPlayer) override;

	void playerKilled(class AartificialLifeCharacter* killed, class AartificialLifeCharacter* killer);
	void winPlayer(class AtdmPlayerState* winner);

	UPROPERTY(Transient)
		TArray<class AtdmPlayerController*> playersAlive;


};
