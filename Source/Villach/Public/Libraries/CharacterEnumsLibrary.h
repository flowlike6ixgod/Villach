// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/** Character movement states */
UENUM(BlueprintType)
enum class EVillachCharacterState : uint8
{
	None,
	InAir,
	Grounded
};

UENUM(BlueprintType)
enum class EVillachMovementState : uint8
{
	Idle,
	Run,
	Sprint
};

template <typename EnumName>
static FString GetEnumToString(const EnumName EnumValue)
{
	return StaticEnum<EnumName>()->GetNameStringByValue(static_cast<int64>(EnumValue));
}