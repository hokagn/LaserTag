// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BlockRule.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class DUNGEONGENERATOR_API UBlockRule : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool RuleCheck(UMapGraph *Graph, FMapVector Center);
};
