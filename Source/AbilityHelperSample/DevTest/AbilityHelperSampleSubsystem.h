// AbilityHelperSampleSubsystem.h
// DevTest: 项目级 EditorSubsystem，用于绑定 OnPostProcessGameplayEffect/Ability 委托

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "AbilityHelperSampleSubsystem.generated.h"

class UGameplayEffect;
class UGameplayAbility;
class UPrimaryDataAsset;
struct FTableRowBase;

/**
 * 项目级 EditorSubsystem
 * 在 Initialize 时绑定 UAbilityEditorHelperSubsystem 的 OnPostProcessGameplayEffect/Ability/CustomDataAsset 委托
 * 处理 FGameplayEffectSampleConfig、FGameplayAbilitySampleConfig 和 FSampleCustomDataAssetConfig 派生类型的扩展字段
 */
UCLASS()
class ABILITYHELPERSAMPLE_API UAbilityHelperSampleSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	/** 处理 GE 派生类型的扩展字段 */
	void HandlePostProcessGameplayEffect(const FTableRowBase* Config, UGameplayEffect* GE);

	/** 处理 GA 派生类型的扩展字段 */
	void HandlePostProcessGameplayAbility(const FTableRowBase* Config, UGameplayAbility* GA);

	/** 处理自定义 DataAsset 派生类型的扩展字段 */
	void HandlePostProcessCustomDataAsset(const FTableRowBase* Config, UPrimaryDataAsset* Asset);

	/** GE 委托句柄 */
	FDelegateHandle PostProcessGEDelegateHandle;

	/** GA 委托句柄 */
	FDelegateHandle PostProcessGADelegateHandle;

	/** 自定义 DataAsset 委托句柄 */
	FDelegateHandle PostProcessCustomAssetDelegateHandle;
};
