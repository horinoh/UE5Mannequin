// Fill out your copyright notice in the Description page of Project Settings.


#include "MannequinGameMode.h"

#include "Player/MannequinPlayerController.h"
#include "Character/MannequinCharacter.h"

AMannequinGameMode::AMannequinGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerControllerClass = AMannequinPlayerController::StaticClass();
	DefaultPawnClass = AMannequinCharacter::StaticClass();
}