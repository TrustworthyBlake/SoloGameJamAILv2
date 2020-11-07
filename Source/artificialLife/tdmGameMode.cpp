// Fill out your copyright notice in the Description page of Project Settings.


#include "tdmGameMode.h"
#include "tdmPlayerController.h"
#include "tdmPlayerState.h"
#include "tdmGameState.h"
#include "artificialLifeCharacter.h"

AtdmGameMode::AtdmGameMode()
	: Super() {
}

void AtdmGameMode::PostLogin(APlayerController* newPlayer)
{
	Super::PostLogin(newPlayer);

	playersAlive.Add(Cast<AtdmPlayerController>(newPlayer));
}

void AtdmGameMode::playerKilled(class AartificialLifeCharacter* killed, class AartificialLifeCharacter* killer) {

	if (killed) {
		if (AtdmPlayerController* player = Cast<AtdmPlayerController>(killed->GetController())) {
			playersAlive.RemoveSingle(player);
		}
		if (playersAlive.Num() == 1 && playersAlive.IsValidIndex(0)) {
			winPlayer(Cast<AtdmPlayerState>(playersAlive[0]->PlayerState));
		}
	}
}

void AtdmGameMode::winPlayer(class AtdmPlayerState* winner)
{
	if (AtdmGameState* state = GetGameState<AtdmGameState>()) {
		state->winner = winner;
	}
}
