// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MannequinCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "EnhancedInputComponent.h"
#include "InputAction.h"

#pragma region Netowrk
#include "Net/UnrealNetwork.h"
#pragma endregion

#include "Character/MannequinCharaMovementComponent.h"

// Sets default values
AMannequinCharacter::AMannequinCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMannequinCharaMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

#pragma region Capsule
	const auto CapsuleComp = GetCapsuleComponent();
	if (nullptr != CapsuleComp)
	{
#pragma region Camera
		SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
		if (nullptr != SpringArmComp)
		{
			SpringArmComp->SetupAttachment(reinterpret_cast<USceneComponent*>(CapsuleComp));
			SpringArmComp->bUsePawnControlRotation = true;
			SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 100.0f, 50.0f), FRotator(-20.0f, 0.0f, 0.0f));

			CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
			if (nullptr != CameraComp)
			{
				CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
				CameraComp->bUsePawnControlRotation = false;
			}
		}
#pragma endregion
	}
#pragma endregion

#pragma region SkelMesh
	const auto SkelMeshComp = GetMesh();
	if (nullptr != SkelMeshComp)
	{
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
		if (SkelMesh.Succeeded())
		{
			SkelMeshComp->SetSkeletalMesh(SkelMesh.Object);
		}
		SkelMeshComp->SetRelativeLocationAndRotation(FVector::UpVector * -90.0f, FRotator(0.0f, -90.0f, 0.0f));

#pragma region Animation
		//!< リファレンスをコピーで得られる文字列から '' 内だけ抜き出し、さらに . 以降を削除したものを指定する
		//!<	"AAA'BBB.CCC'" -> "BBB"
		static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/Animation/ABP_Mannequin"));
		if (AnimBPClass.Succeeded())
		{
			SkelMeshComp->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			SkelMeshComp->SetAnimInstanceClass(AnimBPClass.Class);
		}
#pragma endregion
	}
#pragma endregion

#pragma region Movement
	const auto MoveComp = GetCharacterMovement();
	if (nullptr != MoveComp)
	{		
		MoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;
		MoveComp->SetCrouchedHalfHeight(0.65f);
	}
#pragma endregion

#pragma region Input
	static ConstructorHelpers::FObjectFinder<UInputAction> Move(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Move.IA_Move'"));
	if (Move.Succeeded())
	{
		IA_Move = Move.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> Look(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Look.IA_Look'"));
	if (Look.Succeeded())
	{
		IA_Look = Look.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> Jump(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Jump.IA_Jump'"));
	if (Jump.Succeeded())
	{
		IA_Jump = Jump.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> Crouch(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Crouch.IA_Crouch'"));
	if (Crouch.Succeeded())
	{
		IA_Crouch = Crouch.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> Sprint(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Sprint.IA_Sprint'"));
	if (Sprint.Succeeded())
	{
		IA_Sprint = Sprint.Object;
	}
#pragma endregion
}

// Called when the game starts or when spawned
void AMannequinCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMannequinCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMannequinCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (auto Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (nullptr != IA_Move)
		{
			Input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMannequinCharacter::OnMove);
		}
		if (nullptr != IA_Look)
		{
			Input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMannequinCharacter::OnLook);
		}
		if (nullptr != IA_Jump)
		{
			Input->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &Super::Jump);
		}
		if (nullptr != IA_Crouch)
		{
			Input->BindAction(IA_Crouch, ETriggerEvent::Started, this, &AMannequinCharacter::OnCrouch);
			Input->BindAction(IA_Crouch, ETriggerEvent::Completed, this, &AMannequinCharacter::OnUnCrouch);
		}
		if (nullptr != IA_Sprint)
		{
			Input->BindAction(IA_Sprint, ETriggerEvent::Started, this, &AMannequinCharacter::OnSprint);
			Input->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &AMannequinCharacter::OnUnSprint);
		}
	}
}

void AMannequinCharacter::OnMove(const FInputActionInstance& Instance)
{
	auto Value = Instance.GetValue().Get<FVector2D>();

	AddMovementInput(GetActorRightVector(), Value.X);
	AddMovementInput(GetActorForwardVector(), Value.Y);
}
void AMannequinCharacter::OnLook(const FInputActionInstance& Instance)
{
	auto Value = Instance.GetValue().Get<FVector2D>();

	AddControllerYawInput(Value.X);
	AddControllerPitchInput(Value.Y);
}

#pragma region Sprint
bool AMannequinCharacter::CanSprint() const
{
	const auto PC = Cast<APlayerController>(GetController());
	if (nullptr != PC && !PC->bCinematicMode)
	{
		//!< #TODO スプリントしない条件なら false を返す : 死亡中、装備中、リロード中、発砲中、...
		return IsValid(this);
	}
	return true;
}
bool AMannequinCharacter::IsSprinting() const
{
	if (bWantsToSprint)
	{
		//!< 前方向に一定以上の速度があるか?
		const auto Velocity = GetVelocity();
		return !Velocity.IsZero() && (Velocity.GetSafeNormal2D() | GetActorForwardVector()) > FMath::Cos(FMath::DegreesToRadians(30.f));
	}
	return false;
}
bool AMannequinCharacter::SetSprint(bool bNewSprint)
{
	if (CanSprint())
	{
		bWantsToSprint = bNewSprint;

		if (!HasAuthority())
		{
			ServerSetSprint(bNewSprint);
		}
		return true;
	}
	return false;
}
#pragma endregion

#pragma region Netowrk
bool AMannequinCharacter::ServerSetSprint_Validate(bool bNewSprint)
{
	return true;
}
void AMannequinCharacter::ServerSetSprint_Implementation(bool bNewSprint)
{
	SetSprint(bNewSprint);
}
void AMannequinCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AMannequinCharacter, bWantsToSprint, COND_SkipOwner);
	//DOREPLIFETIME_CONDITION(AMannequinCharacter, bIsTargeting, COND_SkipOwner);
	//DOREPLIFETIME_CONDITION(AMannequinCharacter, AimOffsetYaw, COND_SkipOwner);
	//DOREPLIFETIME_CONDITION(AMannequinCharacter, AimOffsetPitch, COND_SkipOwner);
}
#pragma endregion
