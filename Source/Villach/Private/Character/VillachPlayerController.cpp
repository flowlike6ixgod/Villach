// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/VillachPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "Character/VillachCharacter.h"

void AVillachPlayerController::BindAction(UInputMappingContext* MappingContext)
{
	if (MappingContext)
	{
		const TArray<FEnhancedActionKeyMapping>& Mappings = MappingContext->GetMappings();

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			TSet<const UInputAction*> UniqueActions;
			for (const FEnhancedActionKeyMapping& Keymapping : Mappings)
			{
				UniqueActions.Add(Keymapping.Action);
			}
			for (const UInputAction* UniqueAction : UniqueActions)
			{
				EnhancedInputComponent->BindAction(UniqueAction, ETriggerEvent::Triggered, Cast<UObject>(this), UniqueAction->GetFName());
			}
		}
	}
}

void AVillachPlayerController::SetupEnhancedSubsystem()
{
	if (VillachCharacter)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			FModifyContextOptions ContextOptions;
			ContextOptions.bForceImmediately = 1;
			Subsystem->AddMappingContext(PrimaryInputMapping, 1, ContextOptions);
		}
	}
}

void AVillachPlayerController::MoveForwardAction(const FInputActionValue& Value)
{
	if (VillachCharacter)
	{
		VillachCharacter->MoveForwardAction(Value.GetMagnitude());
	}
}

void AVillachPlayerController::MoveRightAction(const FInputActionValue& Value)
{
	if (VillachCharacter)
	{
		VillachCharacter->MoveRightAction(Value.GetMagnitude());
	}
}

void AVillachPlayerController::SprintAction(const FInputActionValue& bValue)
{
	if (VillachCharacter)
	{
		VillachCharacter->SprintAction(bValue.Get<bool>());
	}
}

void AVillachPlayerController::JumpAction(const FInputActionValue& bValue)
{
	if (VillachCharacter)
	{
		VillachCharacter->JumpAction(bValue.Get<bool>());
	}
}

void AVillachPlayerController::LookUpAction(const FInputActionValue& Value)
{
	if (VillachCharacter)
	{
		VillachCharacter->LookUpAction(Value.GetMagnitude());
	}
}

void AVillachPlayerController::TurnRightAction(const FInputActionValue& Value)
{
	if (VillachCharacter)
	{
		VillachCharacter->TurnRightAction(Value.GetMagnitude());
	}
}


void AVillachPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->ClearActionEventBindings();
		EnhancedInputComponent->ClearActionValueBindings();

		BindAction(PrimaryInputMapping);
	}
}

void AVillachPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	VillachCharacter = Cast<AVillachCharacter>(InPawn);

	SetupEnhancedSubsystem();
}

void AVillachPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	VillachCharacter = Cast<AVillachCharacter>(GetPawn());
	SetupEnhancedSubsystem();
}
