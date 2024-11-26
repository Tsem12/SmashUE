// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LocalMultiplayerSubsystem.generated.h"

class ULocalMultiplayerSettings;
enum class ELocalMultiplayerInputMappingType;
/**
 * 
 */
UCLASS()
class LOCALMULTIPLAYER_API ULocalMultiplayerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CreateAndInitPlayer(ELocalMultiplayerInputMappingType MappingType);

	int GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex);

	int AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex);

	void AssignKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex, ELocalMultiplayerInputMappingType MappingType) const;

	int GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID);
	
	int AssignNewPlayerToGamepadDeviceID(int DeviceID);

	void AssignGamepadInputMapping(int PlayerIndex, ELocalMultiplayerInputMappingType MappingType) const;

	ELocalMultiplayerInputMappingType GetCurrentMappingType();

protected:
	UPROPERTY()
	ELocalMultiplayerInputMappingType CurrentMappingType;
	
	UPROPERTY()
	uint8 LastAssignedPlayerIndex = 0;

	UPROPERTY()
	TMap<int, int> PlayerIndexFromKeyBoardProfileIndex;

	UPROPERTY()
	TMap<int, int> PlayerIndexFromGamepadProfileIndex;

	UPROPERTY()
	TObjectPtr<const ULocalMultiplayerSettings> LocalMultiplayerSettings;
};
