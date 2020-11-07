// Fill out your copyright notice in the Description page of Project Settings.


#include "tdmGameState.h"
#include "Net/UnrealNetwork.h"

void AtdmGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AtdmGameState, winner);
}

void AtdmGameState::onRep_winPlayer()	{
	onWinPlayerFound.Broadcast();
}
