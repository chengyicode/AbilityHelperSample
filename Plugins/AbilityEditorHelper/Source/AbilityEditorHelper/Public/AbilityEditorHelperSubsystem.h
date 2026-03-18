// AbilityEditorHelperSubsystem.h

#pragma once

#include "CoreMinimal.h"
#include "AbilityEditorHelperSettings.h"
#include "Engine/DataTable.h"
#include "EditorSubsystem.h"
#include "AbilityEditorHelperSubsystem.generated.h"

class UGameplayEffect;
class UGameplayAbility;
class UPrimaryDataAsset;

/**
 * 后处理委托：在 CreateOrImportGameplayEffect 完成基础配置后广播
 * 项目 Source 可注册此委托来处理派生类的扩展字段
 * @param Config - 配置结构体指针（可转换为派生类型）
 * @param GE - 已创建/更新的 GameplayEffect 对象
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPostProcessGameplayEffect, const FTableRowBase* /*Config*/, UGameplayEffect* /*GE*/);

/**
 * 后处理委托：在 CreateOrImportGameplayAbility 完成基础配置后广播
 * 项目 Source 可注册此委托来处理派生类的扩展字段
 * @param Config - 配置结构体指针（可转换为派生类型）
 * @param GA - 已创建/更新的 GameplayAbility 对象
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPostProcessGameplayAbility, const FTableRowBase* /*Config*/, UGameplayAbility* /*GA*/);

/**
 * 后处理委托：在 CreateOrImportCustomDataAsset 创建/更新自定义资产后广播
 * 项目 Source 可注册此委托来将配置数据应用到自定义资产
 * @param Config - 配置结构体指针（可转换为派生类型，如 FCustomDataAssetConfig 的子类）
 * @param Asset - 已创建/更新的 UPrimaryDataAsset 对象
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPostProcessCustomDataAsset, const FTableRowBase* /*Config*/, UPrimaryDataAsset* /*Asset*/);

/**
 * 在编辑器环境中启动时加载并缓存 AbilityEditorHelperSettings 中的 DataTable
 */
UCLASS()
class ABILITYEDITORHELPER_API UAbilityEditorHelperSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override
	{
		CachedGameplayEffectDataTable = nullptr;
		CachedGameplayAbilityDataTable = nullptr;
		CachedCustomDataAssetDataTable = nullptr;
		Super::Deinitialize();
	}

	/** 获取缓存的 GE DataTable */
	UFUNCTION(BlueprintCallable, Category="AbilityEditorHelper|Subsystem")
	UDataTable* GetCachedGameplayEffectDataTable() const { return CachedGameplayEffectDataTable; }

	/** 获取缓存的 GA DataTable */
	UFUNCTION(BlueprintCallable, Category="AbilityEditorHelper|Subsystem")
	UDataTable* GetCachedGameplayAbilityDataTable() const { return CachedGameplayAbilityDataTable; }

	/** 获取缓存的自定义 DataAsset DataTable */
	UFUNCTION(BlueprintCallable, Category="AbilityEditorHelper|Subsystem")
	UDataTable* GetCachedCustomDataAssetDataTable() const { return CachedCustomDataAssetDataTable; }

	/**
	 * 后处理委托：项目 Source 可注册此委托来处理派生类的扩展字段
	 * 使用示例（在项目模块的 StartupModule 中）：
	 *
	 * FCoreDelegates::OnPostEngineInit.AddLambda([]() {
	 *     if (GEditor) {
	 *         auto* Subsystem = GEditor->GetEditorSubsystem<UAbilityEditorHelperSubsystem>();
	 *         if (Subsystem) {
	 *             Subsystem->OnPostProcessGameplayEffect.AddLambda([](const FTableRowBase* Config, UGameplayEffect* GE) {
	 *                 // 处理扩展字段...
	 *             });
	 *         }
	 *     }
	 * });
	 */
	FOnPostProcessGameplayEffect OnPostProcessGameplayEffect;

	/** GameplayAbility 后处理委托 */
	FOnPostProcessGameplayAbility OnPostProcessGameplayAbility;

	/**
	 * 自定义 DataAsset 后处理委托：项目 Source 可注册此委托来将配置数据应用到自定义资产
	 * 使用示例（在项目的 EditorSubsystem 的 Initialize 中）：
	 *
	 * HelperSubsystem->OnPostProcessCustomDataAsset.AddUObject(this, &UMySubsystem::HandlePostProcessCustomDataAsset);
	 *
	 * void UMySubsystem::HandlePostProcessCustomDataAsset(const FTableRowBase* Config, UPrimaryDataAsset* Asset)
	 * {
	 *     const FMyAssetConfig* MyConfig = static_cast<const FMyAssetConfig*>(Config);
	 *     UMyDataAsset* MyAsset = Cast<UMyDataAsset>(Asset);
	 *     if (MyConfig && MyAsset)
	 *     {
	 *         MyAsset->MyField = MyConfig->MyField;
	 *     }
	 * }
	 */
	FOnPostProcessCustomDataAsset OnPostProcessCustomDataAsset;

	/** 广播 GE 后处理委托（供 AbilityEditorHelperLibrary 内部调用） */
	void BroadcastPostProcessGameplayEffect(const FTableRowBase* Config, UGameplayEffect* GE);

	/** 广播 GA 后处理委托（供 AbilityEditorHelperLibrary 内部调用） */
	void BroadcastPostProcessGameplayAbility(const FTableRowBase* Config, UGameplayAbility* GA);

	/** 广播自定义 DataAsset 后处理委托（供 AbilityEditorHelperLibrary 内部调用） */
	void BroadcastPostProcessCustomDataAsset(const FTableRowBase* Config, UPrimaryDataAsset* Asset);

private:
	/** 缓存的 GE 配置 DataTable（编辑器运行期内存缓存） */
	UPROPERTY(Transient)
	TObjectPtr<UDataTable> CachedGameplayEffectDataTable = nullptr;

	/** 缓存的 GA 配置 DataTable（编辑器运行期内存缓存） */
	UPROPERTY(Transient)
	TObjectPtr<UDataTable> CachedGameplayAbilityDataTable = nullptr;

	/** 缓存的自定义 DataAsset 配置 DataTable（编辑器运行期内存缓存） */
	UPROPERTY(Transient)
	TObjectPtr<UDataTable> CachedCustomDataAssetDataTable = nullptr;
};


