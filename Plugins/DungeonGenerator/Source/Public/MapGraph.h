// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapGraph.generated.h"

USTRUCT(BlueprintType)
struct FMapBlock
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsUpBlocked;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDownBlocked;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsLeftBlocked;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsRightBlocked;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsFrontBlocked;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsBackwardBlocked;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Code;
};

USTRUCT(BlueprintType)
struct FMapVector
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int x;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int h;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int z;

	bool operator==(const FMapVector& b) const
	{
		return x == b.x && h == b.h && z == b.z;
	}
	FMapVector operator+(const FMapVector& b) const
	{
		return {x+b.x, h+b.h, z+b.z};
	}
};

USTRUCT(BlueprintType)
struct FMapSection
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMapVector> SectionBlocks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int EntranceCount = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMapVector Center;
};
/**
 * 
 */
UCLASS(Blueprintable)
class DUNGEONGENERATOR_API UMapGraph : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	FMapBlock GetBlock(int x, int h, int z);
	UFUNCTION(BlueprintCallable)
	void SetBlock(int x, int h, int z, FMapBlock BlockData);
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
