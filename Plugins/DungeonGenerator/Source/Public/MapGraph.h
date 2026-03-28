// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlockData.h"
#include <FixedSize3DArray.h>

#include "MapGraph.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class DUNGEONGENERATOR_API UMapGraph : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	U3DBoolArray* IsRoomExist;

	FMapBlock GetBlock(FMapVector xhz);
	UFUNCTION(BlueprintCallable)
	FMapBlock GetBlock(int x, int h, int z);
	UFUNCTION(BlueprintCallable)
	void SetBlock(int x, int h, int z, FMapBlock BlockData);
	void SetBlock(FMapVector xhz, FMapBlock BlockData);
	void AddBlock(FMapBlock BlockData);
	UFUNCTION(BlueprintCallable)
	void SetSize(int x, int h, int z);
protected:
	UPROPERTY(BlueprintReadWrite)
	TArray<FMapBlock> RoomBlockArray;

	UPROPERTY(BlueprintReadWrite)
	TArray<FMapSection> RoomSectionsArray;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int XWidth = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Height = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ZWidth = 1;

};
