// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/VillachPlayerCameraManager.h"
#include <Kismet/KismetMathLibrary.h>

AVillachPlayerCameraManager::AVillachPlayerCameraManager()
{
	CameraMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	CameraMeshComp->SetupAttachment(RootComponent);

	InterpSpeed = 15.f;
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
	SmoothPivotTarget.SetLocation(AttachLocation);
}

bool AVillachPlayerCameraManager::CustomCamera(const FVector& Location, const FRotator& Rotation, float DeltaTime, float& FOV)
{
	if (PossessedCharacter)
	{
		// Get camera transformation
		const FTransform& PivotTarget = PossessedCharacter->GetActorTransform();

		// Calculate Target Camera Rotation
		TargetCameraRotation = FMath::RInterpTo(GetCameraRotation(), GetOwningPlayerController()->GetControlRotation(),
		                                        DeltaTime, GetCameraBehavior(FName(TEXT("LagSpeed"))));

		// Calculate the Smoothed Pivot Target
		const FVector LagSpeed = UKismetMathLibrary::MakeVector(GetCameraBehavior(FName(TEXT("LagSpeed_X"))),
		                                                        GetCameraBehavior(FName(TEXT("LagSpeed_Y"))),
		                                                        GetCameraBehavior(FName(TEXT("LagSpeed_X"))));

		const FVector& AxisLagResult = CalculateAxisLag(SmoothPivotTarget.GetLocation(), PivotTarget.GetLocation(),
		                                                TargetCameraRotation, LagSpeed, DeltaTime);

		SmoothPivotTarget = UKismetMathLibrary::MakeTransform(AxisLagResult, PivotTarget.Rotator());
		SmoothPivotTarget.SetLocation(AxisLagResult);
		SmoothPivotTarget.SetRotation(PivotTarget.GetRotation());

		// Calculate Pivot Location. Get the Smoothed Pivot Target and apply local offsets for further camera control.
		const FVector PivotForwardVector = UKismetMathLibrary::GetForwardVector(SmoothPivotTarget.Rotator());
		const FVector PivotRightVector = UKismetMathLibrary::GetRightVector(SmoothPivotTarget.Rotator());
		const FVector PivotUpVector = UKismetMathLibrary::GetUpVector(SmoothPivotTarget.Rotator());

		PivotLocation = ((PivotForwardVector * GetCameraBehavior(FName(TEXT("PivotOffset_X")))) + (PivotRightVector *
			GetCameraBehavior(FName(TEXT("PivotOffset_Y")))) + (PivotUpVector *
			GetCameraBehavior(FName(TEXT("PivotOffset_Z")))) + SmoothPivotTarget.GetLocation());

		// Calculate Target Camera Location.

		const FVector TargetForwardVector = UKismetMathLibrary::GetForwardVector(TargetCameraRotation);
		const FVector TargetRightVector = UKismetMathLibrary::GetRightVector(TargetCameraRotation);
		const FVector TargetUpVector = UKismetMathLibrary::GetUpVector(TargetCameraRotation);

		TargetCameraLocation = ((TargetForwardVector * GetCameraBehavior(FName(TEXT("PivotOffset_X")))) + (TargetRightVector *
			GetCameraBehavior(FName(TEXT("PivotOffset_Y")))) + (TargetUpVector *
			GetCameraBehavior(FName(TEXT("PivotOffset_Z")))) + PivotLocation);

		const FVector TraceStart = PossessedCharacter->GetActorLocation();
		const FVector TraceEnd = TargetCameraLocation;
		float TraceRadius = 15.f;

		FCollisionShape CameraShape = FCollisionShape::MakeSphere(TraceRadius);
		CameraShape.SetSphere(TraceRadius);

		FCollisionQueryParams CameraQueryParams;
		CameraQueryParams.AddIgnoredActor(this);
		CameraQueryParams.AddIgnoredActor(PossessedCharacter);

		FHitResult CameraHit;
		GetWorld()->SweepSingleByChannel(CameraHit, TraceStart, TraceEnd, FQuat::Identity, ECollisionChannel::ECC_Visibility, CameraShape, CameraQueryParams);

		if (CameraHit.bBlockingHit)
		{
			TargetCameraLocation = (CameraHit.Location - TraceEnd) + TargetCameraLocation;
		}

		return true;
	}

	return false;
}

float AVillachPlayerCameraManager::GetCameraBehavior(FName CurveName) const
{
	UAnimInstance* CameraInst = CameraMeshComp->GetAnimInstance();
	if (CameraInst)
	{
		return CameraInst->GetCurveValue(CurveName);
	}

	return 0.0f;
}

FVector AVillachPlayerCameraManager::CalculateAxisLag(FVector CurrentLocation, FVector TargetLocation,
                                                      FRotator CameraRotation, FVector LagSpeed,
                                                      float DeltaTime)
{
	CameraRotation.Roll = 0.0f;
	CameraRotation.Pitch = 0.0f;
	CameraRotationYaw = CameraRotation;

	const FVector UnrotCameraLoc = CameraRotationYaw.UnrotateVector(CurrentLocation);
	const FVector UnrotTargetLoc = CameraRotationYaw.UnrotateVector(TargetLocation);

	const FVector InterpResult = UKismetMathLibrary::MakeVector(
		UKismetMathLibrary::FInterpTo(UnrotCameraLoc.X, UnrotTargetLoc.X, DeltaTime, LagSpeed.X),
		UKismetMathLibrary::FInterpTo(UnrotCameraLoc.Y, UnrotTargetLoc.Y, DeltaTime, LagSpeed.Y),
		UKismetMathLibrary::FInterpTo(UnrotCameraLoc.Z, UnrotTargetLoc.Z, DeltaTime, LagSpeed.Z));


	return CameraRotationYaw.RotateVector(InterpResult);
}

void AVillachPlayerCameraManager::UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTargetInternal(OutVT, DeltaTime);

	if (OutVT.Target)
	{
		FVector OutLocation;
		FRotator OutRotation;
		float OutFOV;

		if (OutVT.Target->IsA<AVillachCharacter>())
		{
			if (CustomCamera(OutLocation, OutRotation, DeltaTime, OutFOV))
			{
				OutVT.POV.Location = OutLocation;
				OutVT.POV.Rotation = OutRotation;
				OutVT.POV.FOV = OutFOV;
			}
			else
			{
				OutVT.Target->CalcCamera(DeltaTime, OutVT.POV);
			}
		}
		else
		{
			OutVT.Target->CalcCamera(DeltaTime, OutVT.POV);
		}
	}
}
