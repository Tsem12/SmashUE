// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSettings.h"

#include "InputMappingContext.h"

bool FLocalMultiplayerProfileData::ContainsKey(const FKey& Key, ELocalMultiplayerInputMappingType MappingType) const
{
	UInputMappingContext* MappingContext = GetIMCFromType(MappingType);
	if(!MappingContext) return false;
	TArray<FEnhancedActionKeyMapping> Inputs = MappingContext->GetMappings();
	for (FEnhancedActionKeyMapping Input : Inputs)
	{
		if(Input.Key == Key)
		{
			return true;
		}
	}
	return false;
}

UInputMappingContext* FLocalMultiplayerProfileData::GetIMCFromType(ELocalMultiplayerInputMappingType MappingType) const
{
	switch (MappingType)
	{
		case ELocalMultiplayerInputMappingType::InGame:
			return IMCInGame;
		case ELocalMultiplayerInputMappingType::Menu:
			return IMCMenu;
	}

	return nullptr;
}

int ULocalMultiplayerSettings::GetNbKeyboardProfiles() const
{
	return KeyboardProfileDatas.Num();
}

int ULocalMultiplayerSettings::FindKeyboardProfileIndexFromKey(const FKey& Key,ELocalMultiplayerInputMappingType MappingType) const
{
	for (int i = 0; i < GetNbKeyboardProfiles(); ++i)
	{
		if(KeyboardProfileDatas[i].ContainsKey(Key, MappingType))
		{
			return i;
		}
	}
	return -1;
}




