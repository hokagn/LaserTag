// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <MapGraph.h>
#include <RoomBlock.h>
#include <BlockRule.h>
#include <FixedSize3DArray.h>
#include "Engine/DataAsset.h"
#include "SectionShapeData.generated.h"

USTRUCT(BlueprintType)
struct FMapSectionObject
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Offset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMapVector Location;

	TSubclassOf<AActor> ObjectClass;
};

USTRUCT(BlueprintType)
struct FBlockObject
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Offset;
	TArray<UBlockRule*> Rules;
	TSubclassOf<AActor> ObjectClass;
};

/**
 * 
 */
UCLASS()
class DUNGEONGENERATOR_API USectionShapeData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	void InitMapData();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMapSection SectionData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMapSectionObject> SectionObjects;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBlockObject> BlockObjects;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ARoomBlock> BlockClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMapVector PosEnd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMapVector NegEnd;
	UPROPERTY(BlueprintReadWrite)
	FMapVector SectionOffset;
	//Returns true if the position is occupied or out of bounds
	bool GetIsOccupied(FMapVector ijk)
	{
		return OccupiedArray->Get(ijk.x + NegEnd.x, ijk.h + NegEnd.h, ijk.z + NegEnd.z);
	}
	//Returns true if the position is occupied or out of bounds
	bool GetIsOccupied(int x, int h, int z)
	{
		x += NegEnd.x;
		h += NegEnd.h;
		z += NegEnd.z;
		FMapVector Size = PosEnd + NegEnd;
		if (x > Size.x || h > Size.h || z > Size.z || x < 0 || h < 0 || z < 0)
		{
			return true;
		}
		return OccupiedArray->Get(x, h + NegEnd.h, z + NegEnd.z);
	}

protected:
	U3DBoolArray* OccupiedArray;
};
