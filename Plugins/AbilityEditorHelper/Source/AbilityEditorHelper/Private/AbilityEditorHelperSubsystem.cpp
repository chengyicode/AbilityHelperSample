// AbilityEditorHelperSubsystem.cpp

#include "AbilityEditorHelperSubsystem.h"
#include "AbilityEditorTypes.h"
#include "GameplayEffect.h"
#include "Abilities/GameplayAbility.h"

void UAbilityEditorHelperSubsystem::BroadcastPostProcessGameplayEffect(const FTableRowBase* Config, UGameplayEffect* GE)
{
	if (OnPostProcessGameplayEffect.IsBound())
	{
		OnPostProcessGameplayEffect.Broadcast(Config, GE);
	}
}

void UAbilityEditorHelperSubsystem::BroadcastPostProcessGameplayAbility(const FTableRowBase* Config, UGameplayAbility* GA)
{
	if (OnPostProcessGameplayAbility.IsBound())
	{
		OnPostProcessGameplayAbility.Broadcast(Config, GA);
	}
}

void UAbilityEditorHelperSubsystem::BroadcastPostProcessCustomAsset(const FTableRowBase* Config, UCustomDataAsset* Asset)
{
	if (OnPostProcessCustomAsset.IsBound())
	{
		OnPostProcessCustomAsset.Broadcast(Config, Asset);
	}
}

void UAbilityEditorHelperSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UAbilityEditorHelperSettings* Settings = GetDefault<UAbilityEditorHelperSettings>();
	if (!Settings)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AbilityEditorHelper] Settings 未找到，无法缓存 GameplayEffectDataTable。"));
		return;
	}

	// 优先取已加载实例，否则同步加载
	CachedGameplayEffectDataTable = Settings->GameplayEffectDataTable.IsValid()
		? Settings->GameplayEffectDataTable.Get()
		: Settings->GameplayEffectDataTable.LoadSynchronous();

	if (CachedGameplayEffectDataTable)
	{
		UE_LOG(LogTemp, Log, TEXT("[AbilityEditorHelper] 已缓存 GameplayEffectDataTable：%s"),
			*CachedGameplayEffectDataTable->GetPathName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[AbilityEditorHelper] 未能加载 GameplayEffectDataTable，请检查设置。"));
	}

	// 优先取已加载实例，否则同步加载
	CachedGameplayAbilityDataTable = Settings->GameplayAbilityDataTable.IsValid()
		? Settings->GameplayAbilityDataTable.Get()
		: Settings->GameplayAbilityDataTable.LoadSynchronous();

	if (CachedGameplayAbilityDataTable)
	{
		UE_LOG(LogTemp, Log, TEXT("[AbilityEditorHelper] 已缓存 GameplayAbilityDataTable：%s"),
			*CachedGameplayAbilityDataTable->GetPathName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[AbilityEditorHelper] 未能加载 GameplayAbilityDataTable，请检查设置。"));
	}

	// 加载自定义资产 DataTable
	CachedCustomAssetDataTable = Settings->CustomAssetDataTable.IsValid()
		? Settings->CustomAssetDataTable.Get()
		: Settings->CustomAssetDataTable.LoadSynchronous();

	if (CachedCustomAssetDataTable)
	{
		UE_LOG(LogTemp, Log, TEXT("[AbilityEditorHelper] 已缓存 CustomAssetDataTable：%s"),
			*CachedCustomAssetDataTable->GetPathName());
	}
	// 自定义资产 DataTable 可以不配置，不打 Warning
}
