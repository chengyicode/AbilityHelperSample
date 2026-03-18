// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityEditorTypes.h"

DEFINE_LOG_CATEGORY(LogAbilityEditor);

void UCustomDataAsset::ApplyConfig_Implementation(const FCustomAssetConfig& Config)
{
	Description = Config.Description;
}
