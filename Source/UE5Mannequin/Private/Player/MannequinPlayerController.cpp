// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MannequinPlayerController.h"

#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"

AMannequinPlayerController::AMannequinPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Normal.IMC_Normal'"));
	if (IMC.Succeeded())
	{
		IMC_Normal = IMC.Object;
	}
}

void AMannequinPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (nullptr != IMC_Normal)
	{
		if (const auto LP = GetLocalPlayer())
		{
			if (auto InputSystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				InputSystem->AddMappingContext(IMC_Normal, 0);
			}
		}
	}
}