// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/VillachPlayerCameraManager.h"

#include <Kismet/KismetMathLibrary.h>

AVillachPlayerCameraManager::AVillachPlayerCameraManager()
{
}

void AVillachPlayerCameraManager::OnPosses(AVillachCharacter* NewCharacter)
{
	if (NewCharacter)
	{
		// Set new controlled character
		PossessedCharacter = NewCharacter;
	}

	// Get character position for camera init
	const FVector& AttachLocation = PossessedCharacter->GetActorTransform().GetLocation();
	SetActorLocation(AttachLocation);
}


