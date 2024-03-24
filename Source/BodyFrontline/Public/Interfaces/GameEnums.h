#pragma once

UENUM(BlueprintType)
enum class EGameDifficulty : uint8
{
	EGD_Easy UMETA(DisplayName = "Easy"),
	EGD_Medium UMETA(DisplayName = "Medium"),
	EGD_Hard UMETA(DisplayName = "Hard")
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Alive UMETA(DisplayName = "Alive"),
	ECS_Dead UMETA(DisplayName = "Dead")
};