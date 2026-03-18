// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilityEditorTypes.h"
#include "GameplayEffect.h"
#include "GameplayEffectComponent.h"
#include "AbilityEditorHelperLibrary.generated.h"

class UBlueprint;
class UAbilityEditorHelperSettings;

/**
 * 从 GameplayEffect 中移除指定类型的 GEComponent（模板函数）
 * @param GE        目标 GameplayEffect
 * @return          是否成功移除（找到并移除返回 true）
 */
template<typename T>
bool RemoveGEComponent(UGameplayEffect* GE)
{
	if (!GE)
	{
		return false;
	}

	// 通过反射获取 GEComponents 数组
	FArrayProperty* GEComponentsProp = CastField<FArrayProperty>(
		UGameplayEffect::StaticClass()->FindPropertyByName(TEXT("GEComponents"))
	);

	if (!GEComponentsProp)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AbilityEditorHelper] 无法通过反射找到 GEComponents 属性"));
		return false;
	}

	TArray<TObjectPtr<UGameplayEffectComponent>>* ComponentsPtr =
		GEComponentsProp->ContainerPtrToValuePtr<TArray<TObjectPtr<UGameplayEffectComponent>>>(GE);

	if (!ComponentsPtr)
	{
		return false;
	}

	// 查找并移除指定类型的组件
	for (int32 i = ComponentsPtr->Num() - 1; i >= 0; --i)
	{
		if ((*ComponentsPtr)[i] && (*ComponentsPtr)[i]->IsA<T>())
		{
			ComponentsPtr->RemoveAt(i);
			return true;
		}
	}

	return false;
}

/**
 * 
 */
UCLASS()
class ABILITYEDITORHELPER_API UAbilityEditorHelperLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, Category="AbilityEditorHelper")
	static const UAbilityEditorHelperSettings* GetAbilityEditorHelperSettings();

	/**
	 * 打开 UAbilityEditorHelperSettings 中配置的 EditorUtilityWidget。
	 * 若未配置或加载失败则输出警告并返回 false。
	 */
	UFUNCTION(BlueprintCallable, Category="AbilityEditorHelper|EditorWidget")
	static bool OpenEditorUtilityWidget();

	/**
	 * 在指定路径和父类的基础上创建一个Blueprint资产并返回。
	 * 支持路径格式：
	 *   - /Game/Folder/AssetName
	 *   - /Game/Folder/AssetName.AssetName
	 * 非编辑器环境下返回nullptr并置bOutSuccess=false。
	 */
	UFUNCTION(BlueprintCallable, Category="AbilityEditorHelper|Blueprint", meta=(DisplayName="Create Blueprint Asset", Keywords="Create Blueprint Asset"))
	static UBlueprint* CreateBlueprintAsset(const FString& BlueprintPath, TSubclassOf<UObject> ParentClass, bool& bOutSuccess);

	/**
	 * 获取或创建指定路径的Blueprint资产：
	 *  - 若资产已存在则直接加载并返回；
	 *  - 若不存在则创建并返回。
	 * 非编辑器环境仅尝试加载，无法创建时返回nullptr且bOutSuccess=false。
	 */
	UFUNCTION(BlueprintCallable, Category="AbilityEditorHelper|Blueprint", meta=(DisplayName="Get Or Create Blueprint Asset", Keywords="Get Or Create Blueprint Asset"))
	static UBlueprint* GetOrCreateBlueprintAsset(const FString& BlueprintPath, TSubclassOf<UObject> ParentClass, bool& bOutSuccess);

	/**
	 * 根据 FGameplayEffectConfig 在指定路径创建或更新 GameplayEffect 资产，并写入配置数据。
	 * - 若资产已存在则覆盖关键字段；
	 * - 若资产不存在则在编辑器下创建新资产并导入配置。
	 * 非编辑器环境下仅尝试加载但不会创建，失败则返回nullptr且bOutSuccess=false。
	 */
	UFUNCTION(BlueprintCallable, Category="AbilityEditorHelper|GameplayEffect", meta=(DisplayName="Create Or Import GameplayEffect From Config", Keywords="GameplayEffect Create Import From Config"))
	static UGameplayEffect* CreateOrImportGameplayEffect(const FString& GameplayEffectPath, const FGameplayEffectConfig& Config, bool& bOutSuccess);

	/**
	 * 基于 UAbilityEditorHelperSettings 中的 DataTable 与 GameplayEffectPath，批量创建/更新 GameplayEffect。
	 * 不返回创建的对象或数量，处理结果通过日志输出。
	 * @param bClearGameplayEffectFolderFirst  在导入前是否先清理 GameplayEffectPath 路径下（含子目录）中不在 DataTable 的 GE 资产
	 */
	UFUNCTION(BlueprintCallable, Category="AbilityEditorHelper|GameplayEffect", meta=(DisplayName="Create Or Update GameplayEffects From Settings", Keywords="GameplayEffect Import Update From Settings", CPP_Default_bClearGameplayEffectFolderFirst="false"))
	static void CreateOrUpdateGameplayEffectsFromSettings(bool bClearGameplayEffectFolderFirst = false);

	/** 
	 * 将任意 UStruct 的描述导出为 Schema(JSON)。输入 StructPath 完整路径（如 /Script/Module.StructName）。
	 */
	UFUNCTION(BlueprintCallable, Category="AbilityEditorHelper|Schema")
	static bool WriteStructSchemaToJson(UScriptStruct* StructType, const FString& OutJsonFilePath, FString& OutError);

	/**
	 * 便捷函数：生成 Schema 到插件 Content/Python/Schema 目录，文件名 <StructName>.schema.json
	 */
	UFUNCTION(BlueprintCallable, Category="AbilityEditorHelper|Schema")
	static bool GenerateStructSchemaToPythonFolder(UScriptStruct* StructType, FString& OutError);

	/**
	 * 批量生成 Schema：根据 UAbilityEditorHelperSettings 中配置的结构体列表，批量导出所有 Schema 到 Python/Schema 目录
	 * @param bClearSchemaFolderFirst  生成前是否先清空 Schema 文件夹
	 * @param OutSuccessCount  成功导出的结构体数量
	 * @param OutFailureCount  导出失败的结构体数量
	 * @param OutErrors        所有失败的错误信息（每行一个错误）
	 * @return                 是否全部成功
	 */
	UFUNCTION(BlueprintCallable, Category="AbilityEditorHelper|Schema")
	static bool GenerateAllSchemasFromSettings(bool bClearSchemaFolderFirst, int32& OutSuccessCount, int32& OutFailureCount, FString& OutErrors);

	/**
	 * 从结构体路径字符串加载 UScriptStruct
	 * @param StructPath  结构体路径（格式：/Script/ModuleName.StructName）
	 * @return            加载的结构体指针，失败返回 nullptr
	 */
	UFUNCTION(BlueprintCallable, Category="AbilityEditorHelper|Schema")
	static UScriptStruct* LoadStructFromPath(const FString& StructPath);

	/**
	 * 从指定 JSON 文件导入数据到目标 DataTable。
	 * @param TargetDataTable      目标数据表
	 * @param JsonFileName         JSON 文件名（将与 UAbilityEditorHelperSettings::JsonPath 拼接成完整路径）
	 * @param bClearBeforeImport   导入前是否清空现有行
	 * @param OutImportedRowCount  成功导入的行数（由引擎返回）
	 * @param OutError             失败时的错误信息
	 * @return                     是否导入成功（依据引擎返回值>=0判定）
	 */
	UFUNCTION(BlueprintCallable, Category="AbilityEditorHelper|DataTable", meta=(DisplayName="Import DataTable From JSON File", Keywords="DataTable Import JSON"))
	static bool ImportDataTableFromJsonFile(UDataTable* TargetDataTable, const FString& JsonFileName, bool bClearBeforeImport, int32& OutImportedRowCount, FString& OutError);

	/**
	 * 将简化的属性字符串解析为 FGameplayAttribute
	 * @param AttributeString  简化格式字符串（如 "TestAttributeSet.TestPropertyOne"）
	 * @param OutAttribute     输出的 FGameplayAttribute
	 * @return                 是否解析成功
	 */
	UFUNCTION(BlueprintCallable, Category="AbilityEditorHelper|Attribute", meta=(DisplayName="Parse Attribute String"))
	static bool ParseAttributeString(const FString& AttributeString, FGameplayAttribute& OutAttribute);

	/**
	 * 从 JSON 文件导入数据并更新 GameplayEffects（增量更新）
	 * 该函数会：
	 * 1. 读取 JSON 文件
	 * 2. 与现有 DataTable 数据比较，找出新增或变化的行
	 * 3. 只对变化的行更新 DataTable
	 * 4. 只对变化的行创建/更新 GameplayEffect 资产
	 *
	 * @param JsonFileName             JSON 文件名（相对于 Settings::JsonPath）
	 * @param bClearGameplayEffectFolderFirst  是否先清理不在 DataTable 中的 GE 资产
	 * @param OutUpdatedRowNames       被更新的行名列表
	 * @param OutError                 错误信息
	 * @return                         是否成功
	 */
	UFUNCTION(BlueprintCallable, Category="AbilityEditorHelper|GameplayEffect", meta=(DisplayName="Import And Update GameplayEffects From JSON"))
	static bool ImportAndUpdateGameplayEffectsFromJson(const FString& JsonFileName, bool bClearGameplayEffectFolderFirst, TArray<FName>& OutUpdatedRowNames);

	// ===========================================
	// GameplayAbility 相关函数
	// ===========================================

	/**
	 * 根据 FGameplayAbilityConfig 在指定路径创建或更新 GameplayAbility 蓝图资产，并写入配置数据。
	 * - 若资产已存在则覆盖关键字段；
	 * - 若资产不存在则在编辑器下创建新资产并导入配置。
	 * 非编辑器环境下仅尝试加载但不会创建，失败则返回nullptr且bOutSuccess=false。
	 */
	UFUNCTION(BlueprintCallable, Category="AbilityEditorHelper|GameplayAbility", meta=(DisplayName="Create Or Import GameplayAbility From Config"))
	static UGameplayAbility* CreateOrImportGameplayAbility(const FString& GameplayAbilityPath, const FGameplayAbilityConfig& Config, bool& bOutSuccess);

	/**
	 * 基于 UAbilityEditorHelperSettings 中的 DataTable 与 GameplayAbilityPath，批量创建/更新 GameplayAbility。
	 * 不返回创建的对象或数量，处理结果通过日志输出。
	 * @param bClearGameplayAbilityFolderFirst  在导入前是否先清理 GameplayAbilityPath 路径下不在 DataTable 的 GA 资产
	 */
	UFUNCTION(BlueprintCallable, Category="AbilityEditorHelper|GameplayAbility", meta=(DisplayName="Create Or Update GameplayAbilities From Settings"))
	static void CreateOrUpdateGameplayAbilitiesFromSettings(bool bClearGameplayAbilityFolderFirst = false);

	/**
	 * 从 JSON 文件导入数据并更新 GameplayAbilities（增量更新）
	 * @param JsonFileName                      JSON 文件名（相对于 Settings::JsonPath）
	 * @param bClearGameplayAbilityFolderFirst  是否先清理不在 DataTable 中的 GA 资产
	 * @param OutUpdatedRowNames                被更新的行名列表
	 * @return                                  是否成功
	 */
	UFUNCTION(BlueprintCallable, Category="AbilityEditorHelper|GameplayAbility", meta=(DisplayName="Import And Update GameplayAbilities From JSON"))
	static bool ImportAndUpdateGameplayAbilitiesFromJson(const FString& JsonFileName, bool bClearGameplayAbilityFolderFirst, TArray<FName>& OutUpdatedRowNames);

private:
	/** 获取 GA 设置和 DataTable */
	static bool GetGASettingsAndDataTable(const UAbilityEditorHelperSettings*& OutSettings, UDataTable*& OutDataTable);

	/** 获取 GA 基础路径 */
	static FString GetGameplayAbilityBasePath(const UAbilityEditorHelperSettings* Settings);

	/** 清理 GA 文件夹中不在 DataTable 中的资产 */
	static void CleanupGameplayAbilityFolder(const FString& BasePath, const UDataTable* DataTable);

public:
	// ===========================================
	// 自定义资产（CustomAsset）相关函数
	// ===========================================

	/**
	 * 根据 FCustomAssetConfig（或其派生结构体）在指定路径创建或更新 UCustomDataAsset 资产。
	 * - 若资产已存在则调用 ApplyConfig() 更新字段；
	 * - 若资产不存在则在编辑器下创建新的 UCustomDataAsset（或 Settings 中配置的派生类）。
	 * 仅在编辑器环境下可创建新资产，非编辑器环境下仅尝试加载。
	 *
	 * @param AssetPath    资产路径（如 /Game/Data/Assets/DA_MyAsset）
	 * @param Config       来自 DataTable 的配置行
	 * @param bOutSuccess  是否成功
	 * @return             创建/更新的资产指针，失败返回 nullptr
	 */
	UFUNCTION(BlueprintCallable, Category="AbilityEditorHelper|CustomAsset", meta=(DisplayName="Create Or Import Custom Asset From Config"))
	static UCustomDataAsset* CreateOrImportCustomAsset(const FString& AssetPath, const FCustomAssetConfig& Config, bool& bOutSuccess);

	/**
	 * 基于 UAbilityEditorHelperSettings 中的 CustomAssetDataTable 与 CustomAssetPath，
	 * 批量创建/更新自定义资产。
	 * 处理结果通过日志输出（创建数/更新数/跳过数/失败数）。
	 *
	 * @param bClearCustomAssetFolderFirst  是否先清理路径下不在 DataTable 中的自定义资产
	 */
	UFUNCTION(BlueprintCallable, Category="AbilityEditorHelper|CustomAsset", meta=(DisplayName="Create Or Update Custom Assets From Settings", CPP_Default_bClearCustomAssetFolderFirst="false"))
	static void CreateOrUpdateCustomAssetsFromSettings(bool bClearCustomAssetFolderFirst = false);

	/**
	 * 从 JSON 文件增量导入数据并更新自定义资产（只处理变化的行）。
	 * 流程：
	 *   1. 读取并解析 JSON 文件
	 *   2. 与 DataTable 现有行做 JSON 差分，找出新增或变化的行
	 *   3. 仅更新变化行的 DataTable 数据
	 *   4. 仅对变化行创建/更新资产
	 *
	 * @param JsonFileName                   JSON 文件名（相对于 Settings::JsonPath）
	 * @param bClearCustomAssetFolderFirst   是否先清理不在 DataTable 中的自定义资产
	 * @param OutUpdatedRowNames             被更新的行名列表
	 * @return                               是否全部成功（有任何失败返回 false）
	 */
	UFUNCTION(BlueprintCallable, Category="AbilityEditorHelper|CustomAsset", meta=(DisplayName="Import And Update Custom Assets From JSON"))
	static bool ImportAndUpdateCustomAssetsFromJson(const FString& JsonFileName, bool bClearCustomAssetFolderFirst, TArray<FName>& OutUpdatedRowNames);

private:
	/** 获取自定义资产设置和 DataTable */
	static bool GetCustomAssetSettingsAndDataTable(const UAbilityEditorHelperSettings*& OutSettings, UDataTable*& OutDataTable);

	/** 获取自定义资产基础存放路径 */
	static FString GetCustomAssetBasePath(const UAbilityEditorHelperSettings* Settings);

	/** 获取自定义资产名称前缀（带尾部 _ 保证，如 DA_） */
	static FString GetCustomAssetPrefix(const UAbilityEditorHelperSettings* Settings);

	/** 清理自定义资产文件夹中不在 DataTable 中的资产 */
	static void CleanupCustomAssetFolder(const FString& BasePath, const FString& AssetPrefix, const UDataTable* DataTable);
};
