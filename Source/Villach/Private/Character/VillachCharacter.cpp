// Copyright Epic Games, Inc. All Rights Reserved.

#include "Villach/Public/Character/VillachCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AVillachCharacter

AVillachCharacter::AVillachCharacter()
{
	bAllowTickBeforeBeginPlay = true;
	PrimaryActorTick.bCanEverTick = true;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateSensitivity = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
	// Sprinting 
	SprintMultiplier = 1.5f;
	bIsSprinting = false;
	StaminaPerSprint = 2.f;

	bIsJumping = false;
	StaminaPerJump = 15.f;

	// Energy
	MaxStamina = 100.f;
	CurrentStamina = MaxStamina;
	StaminaRegenPerSecond = 3.f;
	
}

void AVillachCharacter::SetMovementState(const EVillachMovementState NewState)
{
	if (MovementState != NewState)
	{
		PrevMovementState = MovementState;
		MovementState = NewState;
		OnMovementStateChanged();
	}
}

void AVillachCharacter::SetCharacterState(EVillachCharacterState NewState)
{
	if (CharacterState != NewState)
	{
		PrevCharacterState = CharacterState;
		CharacterState = NewState;
	}
}

void AVillachCharacter::UpdateCharacterMovement()
{
	if (CharacterState == EVillachCharacterState::Grounded)
	{
		OnMovementStateChanged();
	}
	if (MovementState == EVillachMovementState::Run)
	{
		bUseControllerRotationYaw = true;
	}
	else
	{
		bUseControllerRotationYaw = false;
	}
}

void AVillachCharacter::RestoreStamina(float DeltaSeconds)
{
	// Always regenerate energy when current < max energy value
	if (CurrentStamina <= MaxStamina)
	{
		CurrentStamina = FMath::Min(MaxStamina, CurrentStamina + StaminaRegenPerSecond * DeltaSeconds);
	}
}

void AVillachCharacter::ReduceStamina(float StaminaAmount, float DeltaSeconds)
{
	// Reduce energy if character in sprinting state
	if (CurrentStamina >= StaminaPerSprint)
	{
		CurrentStamina = FMath::Min(MaxStamina, CurrentStamina - StaminaAmount * DeltaSeconds);
	}
}
//////////////////////////////////////////////////////////////////////////
// Input

void AVillachCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bIsSprinting)
	{
		// Restores stamina if character is not sprinting
		RestoreStamina(DeltaSeconds);
	}

	UpdateCharacterMovement();

	if (CurrentStamina >= StaminaPerSprint && bIsSprinting && MovementState == EVillachMovementState::Sprint)
	{
		ReduceStamina(StaminaPerSprint, DeltaSeconds);
	}
	else
	{
		//SprintAction(false);
	}
}

void AVillachCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (GetCharacterMovement()->MovementMode == MOVE_Walking)
	{
		SetCharacterState(EVillachCharacterState::Grounded);
	}
	else if (GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		SetCharacterState(EVillachCharacterState::InAir);
	}
}

void AVillachCharacter::OnMovementStateChanged()
{
	if (GetGroundSpeed() <= 0.f)
	{
		SetMovementState(EVillachMovementState::Idle);
	}
	// if character speed > than default run speed, state changed to Sprint
	else if (GetGroundSpeed() > 500.f)
	{
		SetMovementState(EVillachMovementState::Sprint);
	}
	else
	{
		SetMovementState(EVillachMovementState::Run);
	}
}

void AVillachCharacter::SprintAction_Implementation(bool bValue)
{
	if (bValue)
	{
		if (!bIsSprinting && GetGroundSpeed() > 0.f)
		{
			bIsSprinting = true;
			GetCharacterMovement()->MaxWalkSpeed *= SprintMultiplier;
		}
	}
	else
	{
		if (bIsSprinting)
		{
			bIsSprinting = false;
			GetCharacterMovement()->MaxWalkSpeed /= SprintMultiplier;
		}
	}

}

float AVillachCharacter::GetGroundSpeed() const
{
	return UKismetMathLibrary::VSizeXY(GetCharacterMovement()->Velocity);
}

void AVillachCharacter::MoveRightAction_Implementation(float Value)
{
	if (CharacterState == EVillachCharacterState::Grounded)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AVillachCharacter::TurnRightAction_Implementation(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateSensitivity);
}

void AVillachCharacter::JumpAction_Implementation(bool bValue)
{
	if (bValue)
	{
		if (CurrentStamina >= StaminaPerJump && CharacterState != EVillachCharacterState::InAir)
		{
			Jump();
			bIsJumping = true;
			ReduceStamina(StaminaPerJump, 1.f);
		}
	}
	else
	{
		bIsJumping = false;
		StopJumping();
	}
}

void AVillachCharacter::LookUpAction_Implementation(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateSensitivity);
}

void AVillachCharacter::MoveForwardAction_Implementation(float Value)
{
	if (CharacterState == EVillachCharacterState::Grounded)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}
