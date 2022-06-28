// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Libraries/CharacterEnumsLibrary.h"
#include "VillachCharacter.generated.h"

UCLASS(config=Game)
class AVillachCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AVillachCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void StartJump();

	/** Handler for when a touch input stops. */
	void StopJump();
	
	/** Called for increase character movement speed */
	void StartSprinting();

	/** Returned to default movement speed */
	void StopSprinting();
	
protected:
	///  CHARACTER MOVEMENT
	//
	/** Gets current ground speed */
	UFUNCTION(BlueprintCallable)
	float GetGroundSpeed() const;
	
	/** Multiplier value */
	UPROPERTY(EditAnywhere, Category = Movement)
	float SprintMultiplier;

	/** is character sprinting? */
	UPROPERTY(BlueprintReadOnly)
	bool bIsSprinting;

	/** How much energy is consumed per sprint tick */
	UPROPERTY(EditAnywhere, Category = Movement)
	float StaminaPerSprint;

	/** is character jumping? */
	UPROPERTY(BlueprintReadOnly)
	bool bIsJumping;

	/** How much energy is consumed per jump */
	UPROPERTY(EditAnywhere, Category = Movement)
	float StaminaPerJump;

	///  CHARACTER STATS
	//
	/** Max energy value */
	UPROPERTY(EditAnywhere, Category = "Character | Stats")
	float MaxStamina;

	/** Current energy value */
	UPROPERTY(EditAnywhere, Category = "Character | Stats")
	float CurrentStamina;

	UPROPERTY(EditAnywhere, Category = "Character | Stats")
	float StaminaRegenPerSecond;

protected:
	/// ENUMS
	//
	/** Character current movement state */
	UPROPERTY(BlueprintReadOnly, Category = "Character | State Values")
	EVillachMovementState MovementState = EVillachMovementState::Idle;

	UPROPERTY(BlueprintReadOnly, Category = "Character | State Values")
	EVillachMovementState PrevMovementState;

	UPROPERTY(BlueprintReadOnly, Category = "Character | State Values")
	EVillachCharacterState CharacterState = EVillachCharacterState::Grounded;

	UPROPERTY(BlueprintReadOnly, Category = "Character | State Values")
	EVillachCharacterState PrevCharacterState = EVillachCharacterState::None;;

	/** Sets movement state */
	void SetMovementState(EVillachMovementState NewState);

	/** Sets character state */
	void SetCharacterState(EVillachCharacterState NewState);
	
	/** Update character movement every tick */
	void UpdateCharacterMovement();

public:
	/**
	 * Restore current Stamina value
	 * @param DeltaSeconds regeneration time (sets energy regeneration rate)
	 */
	UFUNCTION(BlueprintCallable)
	void RestoreStamina(float DeltaSeconds);

	/**
	 * Reduce current energy value
	 * @param StaminaAmount the amount of stamina that is taken away with a certain action
	 * @param DeltaSeconds sets smooth stamina drain
	 */
	UFUNCTION(BlueprintCallable)
	void ReduceStamina(float StaminaAmount, float DeltaSeconds);

	/** Get current energy */
	UFUNCTION(BlueprintCallable)
	float GetCurrentStamina() const { return CurrentStamina; };
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void Tick(float DeltaSeconds) override;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	virtual void OnMovementStateChanged();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

