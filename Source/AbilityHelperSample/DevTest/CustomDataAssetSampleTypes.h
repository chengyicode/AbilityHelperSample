// CustomDataAssetSampleTypes.h
// DevTest: FCustomDataAssetConfig 派生结构体示例
// 演示如何通过继承 FCustomDataAssetConfig 扩展字段，并由 OnPostProcessCustomDataAsset 委托将数据写入 USampleCustomDataAsset

#pragma once

#include "CoreMinimal.h"
#include "AbilityEditorTypes.h"
#include "CustomDataAssetSampleTypes.generated.h"

/**
 * FCustomDataAssetConfig 的派生结构体示例
 * 继承自 FCustomDataAssetConfig（基础 Description + ParentClass）并新增业务字段
 * 这些字段将通过 OnPostProcessCustomDataAsset 委托应用到 USampleCustomDataAsset 资产
 *
 * 使用 ExcelSheet 元数据将扩展字段放入单独的子表，不影响基类的主表结构
 */
USTRUCT(BlueprintType)
struct ABILITYHELPERSAMPLE_API FSampleCustomDataAssetConfig : public FCustomDataAssetConfig
{
	GENERATED_BODY()

	/** 示例整数属性，将写入 USampleCustomDataAsset::SampleIntValue */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SampleExtension",
		meta = (ExcelHint = "示例整数值", ExcelSheet = "SampleExtension"))
	int32 SampleIntValue = 0;

	/** 示例浮点属性，将写入 USampleCustomDataAsset::SampleFloatValue */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SampleExtension",
		meta = (ExcelHint = "示例浮点值", ExcelSheet = "SampleExtension"))
	float SampleFloatValue = 0.0f;

	/** 示例字符串属性，将写入 USampleCustomDataAsset::SampleStringValue */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SampleExtension",
		meta = (ExcelHint = "示例字符串值", ExcelSheet = "SampleExtension"))
	FString SampleStringValue;

	/** 示例布尔属性，将写入 USampleCustomDataAsset::bSampleBoolValue */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SampleExtension",
		meta = (ExcelHint = "示例布尔值", ExcelSheet = "SampleExtension"))
	bool bSampleBoolValue = false;
};
