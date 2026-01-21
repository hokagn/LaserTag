// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <SectionShapeData.h>
#include "Math/RandomStream.h"
#include "DungeonSpawner.generated.h"

UCLASS()
class DUNGEONGENERATOR_API ADungeonSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USectionShapeData*> SectionDatas;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USectionShapeData* BaseSectionData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMapVector Size = { 20, 5, 20 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMapVector Start = { 0, 0, 0 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Seed = 42;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void MakeMap();
	bool SetSectionLocation(USectionShapeData* Data);
	void SpawnBlock(int i, int k, int j);
	void CalcMaxHeightPerFloor();
	void PlaceSection(USectionShapeData* Section, int RoomCode);
	void InitSection(USectionShapeData* Section, const FMapVector& Offset);
	void MakeEntrance(USectionShapeData* Section, int RoomCode);
	bool MakeDoorIfConnectedToOthers(USectionShapeData* Section, FMapVector& Rand, U3DBoolArray* DoorArray, FMapVector& AdjustedCenter, FMapVector Offset);
	void MakeDoor(FMapVector& Rand, FMapVector& AdjustedCenter);
	void SpawnSectionObjects(USectionShapeData* Section);
	TArray<int>MaxHeightPerFloor;
	TArray<long int>RoomCodeToSectionDataIndex;
	FRandomStream RandomGenerator;
	UMapGraph* ActiveMapGraph;
	FMapVector CurrentOffset = {1,0,1};
	int CurrentFloor = 0;
};
