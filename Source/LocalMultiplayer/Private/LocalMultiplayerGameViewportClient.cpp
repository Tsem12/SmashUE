// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerGameViewportClient.h"

#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"

void ULocalMultiplayerGameViewportClient::PostInitProperties()
{
	Super::PostInitProperties();
	MaxSplitscreenPlayers = 8;
	LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
}

bool ULocalMultiplayerGameViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	if (!LocalMultiplayerSubsystem)
	{
		LocalMultiplayerSubsystem = GetGameInstance()->GetSubsystem<ULocalMultiplayerSubsystem>();
	}

	if (EventArgs.IsGamepad())
	{
		int PlayerID = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromGamepadDeviceID(
			EventArgs.InputDevice.GetId());;
		if (PlayerID == -1)
		{
			PlayerID = LocalMultiplayerSubsystem->AssignNewPlayerToGamepadDeviceID(EventArgs.InputDevice.GetId());
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Connect Gamepad"));
		}

		APlayerController* Controller = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerID);
		FInputKeyParams Param(EventArgs.Key, EventArgs.Event, EventArgs.AmountDepressed, EventArgs.IsGamepad(),
		                      EventArgs.InputDevice);
		return Controller->InputKey(Param);
	}
	else if (!EventArgs.IsGamepad())
	{
		int ID = LocalMultiplayerSettings->FindKeyboardProfileIndexFromKey(
			EventArgs.Key, LocalMultiplayerSubsystem->GetCurrentMappingType());
		int PlayerID = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(ID);
		if (ID != -1)
		{
			if (PlayerID == -1)
			{
				PlayerID = LocalMultiplayerSubsystem->AssignNewPlayerToKeyboardProfile(ID);
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Connect Keyboard"));
			}
			APlayerController* Controller = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerID);
			FInputKeyParams Param(EventArgs.Key, EventArgs.Event, EventArgs.AmountDepressed, EventArgs.IsGamepad(),
			                      EventArgs.InputDevice);
			return Controller->InputKey(Param);
		}
	}
	return Super::InputKey(EventArgs);
}

bool ULocalMultiplayerGameViewportClient::InputAxis(FViewport* InViewport, FInputDeviceId InputDevice, FKey Key,
                                                    float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	if (!LocalMultiplayerSubsystem)
	{
		LocalMultiplayerSubsystem = GetGameInstance()->GetSubsystem<ULocalMultiplayerSubsystem>();
	}

	if (bGamepad)
	{
		UInputSettings* InputSettings = UInputSettings::GetInputSettings();
		int PlayerID = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromGamepadDeviceID(InputDevice.GetId());
		if (PlayerID == -1)
		{
			if (InputSettings->AxisConfig[InputDevice.GetId()].AxisProperties.DeadZone < Delta)
			{
				PlayerID = LocalMultiplayerSubsystem->AssignNewPlayerToGamepadDeviceID(InputDevice.GetId());
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Connect Gamepad"));
			}
			else
			{
				return false;
			}
		}

		APlayerController* Controller = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerID);
		FInputKeyParams Param(Key, Delta, DeltaTime, NumSamples, bGamepad);
		return Controller->InputKey(Param);
	}
	else if (!bGamepad)
	{
		int ID = LocalMultiplayerSettings->FindKeyboardProfileIndexFromKey(
			Key,
			LocalMultiplayerSubsystem->GetCurrentMappingType()
		);
		int PlayerID = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(ID);
		if (ID != -1)
		{
			if (PlayerID == -1)
			{
				PlayerID = LocalMultiplayerSubsystem->AssignNewPlayerToKeyboardProfile(ID);
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Connect Keyboard"));
			}

			APlayerController* Controller = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerID);
			FInputKeyParams Param(Key, Delta, DeltaTime, NumSamples, bGamepad);
			return Controller->InputKey(Param);
		}
	}

	return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
}
