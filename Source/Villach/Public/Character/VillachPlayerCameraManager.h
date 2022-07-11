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

protected:

	TObjectPtr<AVillachCharacter> PossessedCharacter;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	USkeletalMeshComponent* CameraMeshComp;

	UPROPERTY(EditAnywhere, Category = Camera)
	float InterpSpeed;

	UPROPERTY(EditAnywhere, Category = Camera)
	FTransform SmoothPivotTarget;

	UPROPERTY(EditAnywhere, Category = Camera)
	FVector PivotLocation;

	UPROPERTY(EditAnywhere, Category = Camera)
	FRotator TargetCameraRotation;

	UPROPERTY(EditAnywhere, Category = Camera)
	FVector TargetCameraLocation;

	UPROPERTY(EditAnywhere, Category = Camera)
	FRotator CameraRotationYaw;

public:

	void OnPosses(AVillachCharacter* NewCharacter);

	bool CustomCamera(const FVector& Location, const FRotator& Rotation, float DeltaTime, float& FOV);

	float GetCameraBehavior(FName CurveName) const;

	FVector CalculateAxisLag(FVector CurrentLocation, FVector TargetLocation, FRotator CameraRotation, FVector LagSpeed, float DeltaTime);

	virtual void UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime) override;
};
