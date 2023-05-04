// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MannequinCharaMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class UE5MANNEQUIN_API UMannequinCharaMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxAcceleration() const override;
};
