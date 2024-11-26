// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSubsystem.h"

#include "EnhancedInputSubsystems.h"
#include "LocalMultiplayerSettings.h"
#include "Kismet/GameplayStatics.h"

void ULocalMultiplayerSubsystem::CreateAndInitPlayer(ELocalMultiplayerInputMappingType MappingType)
{
	LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();

	for (int i = 0; i < LocalMultiplayerSettings->GetNbKeyboardProfiles(); ++i)
	{
		APlayerController* PlayerController = UGameplayStatics::CreatePlayer(GetWorld(), i);
	}

	for (int i = 0; i < LocalMultiplayerSettings->NbMaxGamepads; ++i)
	{
		APlayerController* PlayerController = UGameplayStatics::CreatePlayer(GetWorld(), i + LocalMultiplayerSettings->GetNbKeyboardProfiles());
	}
	CurrentMappingType = MappingType;
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex)
{
	if(PlayerIndexFromKeyBoardProfileIndex.Contains(KeyboardProfileIndex))
	{
		return PlayerIndexFromKeyBoardProfileIndex[KeyboardProfileIndex];
	}
	else
	{
		return -1;
	}
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex)
{
	if(GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex) != -1)
	{
		PlayerIndexFromKeyBoardProfileIndex[KeyboardProfileIndex] = LastAssignedPlayerIndex;
	}
	else
	{
		PlayerIndexFromKeyBoardProfileIndex.Add(KeyboardProfileIndex, LastAssignedPlayerIndex);
	}
	AssignKeyboardMapping(LastAssignedPlayerIndex, KeyboardProfileIndex, CurrentMappingType);
	LastAssignedPlayerIndex++;
	return PlayerIndexFromKeyBoardProfileIndex[KeyboardProfileIndex];
}

void ULocalMultiplayerSubsystem::AssignKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex, ELocalMultiplayerInputMappingType MappingType) const
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerIndex);
	
	if(!PlayerController) return;

	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
	if(!Player) return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	FModifyContextOptions ModifyContextOptions;
	ModifyContextOptions.bForceImmediately = true;
	InputSystem->AddMappingContext(LocalMultiplayerSettings->KeyboardProfileDatas[KeyboardProfileIndex].GetIMCFromType(MappingType), 0, ModifyContextOptions);
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID)
{
	if(PlayerIndexFromGamepadProfileIndex.Contains(DeviceID))
	{
		return PlayerIndexFromGamepadProfileIndex[DeviceID];
	}
	else
	{
		return -1;
	}
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToGamepadDeviceID(int DeviceID)
{
	if(GetAssignedPlayerIndexFromGamepadDeviceID(DeviceID) != -1)
	{
		PlayerIndexFromGamepadProfileIndex[DeviceID] = LastAssignedPlayerIndex;
	}
	else
	{
		PlayerIndexFromGamepadProfileIndex.Add(DeviceID, LastAssignedPlayerIndex);
	}
	LastAssignedPlayerIndex++;
	AssignGamepadInputMapping(PlayerIndexFromGamepadProfileIndex[DeviceID], CurrentMappingType);
	return PlayerIndexFromGamepadProfileIndex[DeviceID];
}

void ULocalMultiplayerSubsystem::AssignGamepadInputMapping(int PlayerIndex, ELocalMultiplayerInputMappingType MappingType) const
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerIndex);
	
	if(!PlayerController) return;

	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
	if(!Player) return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	FModifyContextOptions ModifyContextOptions;
	ModifyContextOptions.bForceImmediately = true;
	InputSystem->AddMappingContext(LocalMultiplayerSettings->GamepadProfileData.GetIMCFromType(MappingType), 0, ModifyContextOptions);
}

ELocalMultiplayerInputMappingType ULocalMultiplayerSubsystem::GetCurrentMappingType()
{
	return CurrentMappingType;
}
