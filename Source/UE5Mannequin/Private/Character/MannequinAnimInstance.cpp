// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MannequinAnimInstance.h"

#include "GameFramework/PawnMovementComponent.h"

#include "Character/MannequinCharacter.h"

UMannequinAnimInstance::UMannequinAnimInstance()
	: Super()
{
	const auto Locomotion = FName("Locomotion");

	const auto Stand = FName(TEXT("Stand"));
	const auto Crouch = FName(TEXT("Crouch"));
	const auto JumpStart = FName(TEXT("JumpStart"));
	const auto JumpLoop = FName(TEXT("JumpLoop"));
	const auto JumpEnd = FName(TEXT("JumpEnd"));
	const auto Sprint = FName(TEXT("Sprint"));

#pragma region Condition
	AddNativeTransitionBinding(Locomotion, Stand, Crouch, FCanTakeTransition::CreateUObject(this, &UMannequinAnimInstance::ShouldCrouch));
	AddNativeTransitionBinding(Locomotion, Crouch, Stand, FCanTakeTransition::CreateUObject(this, &UMannequinAnimInstance::ShouldStand));

	AddNativeTransitionBinding(Locomotion, Stand, JumpStart, FCanTakeTransition::CreateUObject(this, &UMannequinAnimInstance::ShouldJumpStart));
	AddNativeTransitionBinding(Locomotion, JumpStart, JumpLoop, FCanTakeTransition::CreateUObject(this, &UMannequinAnimInstance::ShouldJumpLoop));
	AddNativeTransitionBinding(Locomotion, JumpLoop, JumpEnd, FCanTakeTransition::CreateUObject(this, &UMannequinAnimInstance::ShouldJumpEnd));
	AddNativeTransitionBinding(Locomotion, JumpEnd, Stand, FCanTakeTransition::CreateUObject(this, &UMannequinAnimInstance::ShouldStand));

	AddNativeTransitionBinding(Locomotion, Stand, Sprint, FCanTakeTransition::CreateUObject(this, &UMannequinAnimInstance::ShouldSprint));
	AddNativeTransitionBinding(Locomotion, Sprint, Stand, FCanTakeTransition::CreateUObject(this, &UMannequinAnimInstance::ShouldStand));
#pragma endregion

#pragma region Callback
	AddNativeStateEntryBinding(Locomotion, Stand, FOnGraphStateChanged::CreateUObject(this, &UMannequinAnimInstance::StandEntered));
	AddNativeStateEntryBinding(Locomotion, Crouch, FOnGraphStateChanged::CreateUObject(this, &UMannequinAnimInstance::CrouchEntered));
	AddNativeStateEntryBinding(Locomotion, JumpStart, FOnGraphStateChanged::CreateUObject(this, &UMannequinAnimInstance::JumpStartEntered));
	AddNativeStateEntryBinding(Locomotion, JumpLoop, FOnGraphStateChanged::CreateUObject(this, &UMannequinAnimInstance::JumpLoopEntered));
	AddNativeStateEntryBinding(Locomotion, JumpEnd, FOnGraphStateChanged::CreateUObject(this, &UMannequinAnimInstance::JumpEndEntered));
	AddNativeStateEntryBinding(Locomotion, Sprint, FOnGraphStateChanged::CreateUObject(this, &UMannequinAnimInstance::SprintEntered));
#pragma endregion
}

void UMannequinAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const auto Pawn = TryGetPawnOwner();
	if (nullptr != Pawn)
	{
		const auto Velocity = Pawn->GetVelocity();
		Speed = Velocity.Size2D();
		Direction = FRotator::NormalizeAxis((Velocity.Rotation() - Pawn->GetActorRotation()).Yaw);
		
		const auto MoveComp = Pawn->GetMovementComponent();
		if (nullptr != MoveComp)
		{
			IsFalling = MoveComp->IsFalling();
			IsCrouching = MoveComp->IsCrouching();
		}

		const auto Chara = Cast<AMannequinCharacter>(Pawn);
		if (nullptr != Chara)
		{
			IsSprinting = Chara->IsSprinting();
			//IsTargeting = Chara->IsTargeting();

			//Chara->UpdateAimOffset(DeltaSeconds);
			//const auto InterpSpeed = 15.0f;
			//AimOffsetYaw = FMath::FInterpTo(AimOffsetYaw, Chara->GetAimOffsetYaw(), DeltaSeconds, InterpSpeed);
			//AimOffsetPitch = FMath::FInterpTo(AimOffsetPitch, Chara->GetAimOffsetPitch(), DeltaSeconds, InterpSpeed);
		}

	}
}