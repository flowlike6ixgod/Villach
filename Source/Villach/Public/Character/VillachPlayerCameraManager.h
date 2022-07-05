// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VillachCharacter.h"
#include "Camera/PlayerCameraManager.h"
#include "VillachPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class VILLACH_API AVillachPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	AVillachPlayerCameraManager();

	void OnPosses(AVillachCharacter* NewCharacter);

protected:

	TObjectPtr<AVillachCharacter> PossessedCharacter;

};
