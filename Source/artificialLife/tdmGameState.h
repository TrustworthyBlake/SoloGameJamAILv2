// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "tdmGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FwinPlayerLocate);

/**
 * 
 */
UCLASS()
class ARTIFICIALLIFE_API AtdmGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = onRep_winPlayer, Transient, BlueprintReadOnly, Category = "Game State")
	class AtdmPlayerState* winner = nullptr;

	UFUNCTION()
	void onRep_winPlayer();

	UPROPERTY(BlueprintAssignable, Category = "Game State")
	FwinPlayerLocate onWinPlayerFound;
};
