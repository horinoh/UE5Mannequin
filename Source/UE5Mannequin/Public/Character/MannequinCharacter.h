// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MannequinCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

class UInputAction;
struct FInputActionInstance;

UCLASS()
class UE5MANNEQUIN_API AMannequinCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMannequinCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnMove(const FInputActionInstance& Instance);
	virtual void OnLook(const FInputActionInstance& Instance);
	virtual void OnCrouch(const FInputActionInstance& Instance) { Super::Crouch(); }
	virtual void OnUnCrouch(const FInputActionInstance& Instance) { Super::UnCrouch(); }
	virtual void OnSprint(const FInputActionInstance& Instance) { SetSprint(true); }
	virtual void OnUnSprint(const FInputActionInstance& Instance) { SetSprint(false); }

#pragma region Sprint
	bool CanSprint() const;
	bool IsSprinting() const;
	bool SetSprint(bool bNewSprint);
#pragma endregion

	FORCEINLINE float GetSpeedScale() const { return (IsSprinting() ? 1.5f : 1.0f) * (/*IsTargeting() ? 0.75f :*/1.0f); }

#pragma region Network
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSetSprint(bool bNewSprint);
	virtual bool ServerSetSprint_Validate(bool bNewSprint);
	virtual void ServerSetSprint_Implementation(bool bNewSprint);
	UPROPERTY(Transient, Replicated)
	uint8 bWantsToSprint : 1;
#pragma endregion

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_Move;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_Look;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_Jump;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_Crouch;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_Sprint;
};
