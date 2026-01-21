// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include <MapGraph.h>
#include "FixedSize3DArray.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONGENERATOR_API U3DBoolArray : public UObject
{
	GENERATED_BODY()
public:
	bool Get(int i, int j, int k);
	bool Set(int i, int j, int k, bool Value);
	bool Set(FMapVector ijk, bool Value);
	void SetSize(int x, int h, int z)
	{
		Size.x = x > 0 ? x : 1;
		Size.h = h > 0 ? h : 1;
		Size.z = z > 0 ? z : 1 ;
	}
	void Reset(bool Value)
	{
		Data.Init(Value, Size.x * Size.h * Size.z);
	}
protected:
	TArray<bool> Data;
	FMapVector Size = { 1,1,1 };
};
