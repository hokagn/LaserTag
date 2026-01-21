// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapGraph.h"
#include "RoomBlock.generated.h"

class USectionShapeData;

UCLASS()
class DUNGEONGENERATOR_API ARoomBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Init();

	UFUNCTION(BlueprintCallable)
	void SetValue(FMapBlock NewBlock, USectionShapeData* Data);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMapBlock BlockData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USectionShapeData* ShapeData;
};
