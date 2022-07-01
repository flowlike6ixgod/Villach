// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <EnhancedInputSubsystemInterface.h>

#include "CoreMinimal.h"
#include "VillachCharacter.h"
#include "GameFramework/PlayerController.h"
#include "VillachPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VILLACH_API AVillachPlayerController : public APlayerController
{
	GENERATED_BODY()
	AVillachPlayerController();
public:

	virtual void BindAction(UInputMappingContext* MappingContext);

	void SetupEnhancedSubsystem();

public:

	void MoveForwardAction(const FInputActionValue& Value);

	void MoveRightAction(const FInputActionValue& Value);

	void SprintAction(const FInputActionValue& bValue);

	void JumpAction(const FInputActionValue& bValue);

	void LookUpAtRate(const FInputActionValue& Value);

	void TurnAtRate(const FInputActionValue& Value);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	TObjectPtr<UInputMappingContext> PrimaryInputMapping = nullptr;

	TObjectPtr<AVillachCharacter> VillachCharacter;

protected:
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;
};
