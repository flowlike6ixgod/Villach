// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/VillachPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "VillachGameMode.h"

AVillachPlayerController::AVillachPlayerController()
{
}

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
			Subsystem->AddMappingContext(PrimaryInputMapping, 1);
			UE_LOG(LogTemp, Warning, TEXT("SetupEnhancedSubsystem() True"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SetupEnhancedSubsystem() false"));
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

void AVillachPlayerController::LookUpAtRate(const FInputActionValue& Value)
{
	if (VillachCharacter)
	{
		VillachCharacter->LookUpAtRate(Value.GetMagnitude());
	}
}

void AVillachPlayerController::TurnAtRate(const FInputActionValue& Value)
{
	if (VillachCharacter)
	{
		VillachCharacter->TurnAtRate(Value.GetMagnitude());
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
		UE_LOG(LogTemp, Warning, TEXT("SetupInputComponent() True"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetupInputComponent() false"));
	}

}

void AVillachPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	VillachCharacter = Cast<AVillachCharacter>(InPawn);

	SetupEnhancedSubsystem();

	UE_LOG(LogTemp, Warning, TEXT("OnPossess()"));
}
