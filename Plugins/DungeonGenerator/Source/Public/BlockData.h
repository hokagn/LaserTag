#pragma once

#include "CoreMinimal.h"
#include "BlockData.generated.h"
#define BASEROOMCODE 0


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
	int Code = 0;
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
		return { x + b.x, h + b.h, z + b.z };
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

UCLASS()
class DUNGEONGENERATOR_API UBlockData : public UObject
{
	GENERATED_BODY()
};