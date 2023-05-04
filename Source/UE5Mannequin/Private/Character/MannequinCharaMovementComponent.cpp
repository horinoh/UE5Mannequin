// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MannequinCharaMovementComponent.h"

#include "Character/MannequinCharacter.h"

float UMannequinCharaMovementComponent::GetMaxSpeed() const
{
	const auto Owner = Cast<AMannequinCharacter>(GetCharacterOwner());
	if (nullptr != Owner)
	{
		return Super::GetMaxSpeed() * Owner->GetSpeedScale();
	}
	return Super::GetMaxSpeed();
}
float UMannequinCharaMovementComponent::GetMaxAcceleration() const
{
	const auto Owner = Cast<AMannequinCharacter>(GetCharacterOwner());
	if (nullptr != Owner)
	{
		return Super::GetMaxAcceleration() * Owner->GetSpeedScale();
	}
	return Super::GetMaxAcceleration();
}