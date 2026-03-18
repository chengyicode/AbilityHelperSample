// SampleCustomDataAsset.h
// DevTest: 自定义 DataAsset 示例
// 演示如何创建一个可通过 DataTable + JSON 驱动创建/更新的自定义 UPrimaryDataAsset

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SampleCustomDataAsset.generated.h"

/**
 * 自定义 DataAsset 示例
 * 项目可按此模式创建自己的 DataAsset 类，通过 OnPostProcessCustomDataAsset 委托由配置数据驱动
 */
UCLASS(BlueprintType)
class ABILITYHELPERSAMPLE_API USampleCustomDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** 资产描述 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SampleCustomAsset")
	FString Description;

	/** 示例整数属性，由 FSampleCustomDataAssetConfig::SampleIntValue 写入 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SampleCustomAsset")
	int32 SampleIntValue = 0;

	/** 示例浮点属性，由 FSampleCustomDataAssetConfig::SampleFloatValue 写入 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SampleCustomAsset")
	float SampleFloatValue = 0.0f;

	/** 示例字符串属性，由 FSampleCustomDataAssetConfig::SampleStringValue 写入 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SampleCustomAsset")
	FString SampleStringValue;

	/** 示例布尔属性，由 FSampleCustomDataAssetConfig::bSampleBoolValue 写入 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SampleCustomAsset")
	bool bSampleBoolValue = false;
};
