// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MannequinPlayerController.generated.h"

class UInputMappingContext;

/**
 * 
 */
UCLASS()
class UE5MANNEQUIN_API AMannequinPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	AMannequinPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* IMC_Normal;
};
