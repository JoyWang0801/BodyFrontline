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


UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_HealPack UMETA(DisplayName = "HealPack"),
	EIT_DamageBoost UMETA(DisplayName = "DamageBoost")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Chasing_Player UMETA(DisplayName = "Chasing Player"),
	EES_Attacking_Base UMETA(DisplayName = "Attacking Base"),
	EES_Attacking_RBC UMETA(DisplayName = "Attacking RBC")
};