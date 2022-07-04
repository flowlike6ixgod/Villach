// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <EnhancedInputSubsystemInterface.h>
#include "InputActionValue.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VillachPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VILLACH_API AVillachPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	void SetupEnhancedSubsystem();

public:

	UFUNCTION()
	void MoveForwardAction(const FInputActionValue& Value);

	UFUNCTION()
	void MoveRightAction(const FInputActionValue& Value);

	UFUNCTION()
	void SprintAction(const FInputActionValue& bValue);

	UFUNCTION()
	void JumpAction(const FInputActionValue& bValue);

	UFUNCTION()
	void LookUpAction(const FInputActionValue& Value);

	UFUNCTION()
	void TurnRightAction(const FInputActionValue& Value);

protected:
	/* Input mapping */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	TObjectPtr<UInputMappingContext> PrimaryInputMapping = nullptr;

	TObjectPtr<class AVillachCharacter> VillachCharacter;

protected:
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_Pawn() override;

	virtual void BindAction(UInputMappingContext* MappingContext);

};
