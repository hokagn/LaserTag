// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonSpawner.h"
#include <RoomBlock.h>
#define BASEROOMCODE -1

void ADungeonSpawner::BeginPlay()
{
	Super::BeginPlay();
}

void ADungeonSpawner::MakeMap()
{
	RandomGenerator.Initialize(Seed);
	CalcMaxHeightPerFloor();


	ActiveMapGraph = NewObject<UMapGraph>();
	ActiveMapGraph->SetSize(Size.x, Size.h, Size.z);
	int Len = MaxHeightPerFloor.Num();
	int TotalHeight = 0;
	for (int Floor = 0; Floor < Len; Floor++)
	{
		for (int i = 0; i < Size.x; i++)
			for (int j = 0; j < MaxHeightPerFloor[Floor]; j++)
			{
				for (int k = 0; k < Size.z; k++)
				{
					ActiveMapGraph->SetBlock(i, j + TotalHeight, k,
						{
						(MaxHeightPerFloor[Floor]-1 == j),
						(j == 0),
						(k == 0),
						(k == Size.z - 1),
						(i == Size.x - 1),
						(i == 0),
						BASEROOMCODE
						}
					);
				}
			}
		TotalHeight += MaxHeightPerFloor[Floor];
	}

	int RoomCode = 0;
	while(true)
	for (USectionShapeData* Data : SectionDatas)
	{
		if (SetSectionLocation(Data))
		{
			RoomCodeToSectionDataIndex.Add(SectionDatas.IndexOfByKey(Data));
			InitSection(Data, CurrentOffset);
			PlaceSection(Data, RoomCode);
			MakeEntrance(Data, RoomCode);
			RoomCode++;
		}
		else
		{
			goto EndLoop;
		}
	}
	EndLoop:


	for (int j = 0; j < Size.h; j++)
	{
		for (int i = 0; i < Size.x; i++)
		{
			for (int k = 0; k < Size.z; k++)
			{
				SpawnBlock(i, j, k);
			}
		}
	}

	for (USectionShapeData* Data : SectionDatas)
	{
		SpawnSectionObjects(Data);
	}
}

bool ADungeonSpawner::SetSectionLocation(USectionShapeData* Data)
{
	int NewX, NewZ;
	switch (RandomGenerator.RandRange(0, 1))
	{
	case 0:
		NewX = CurrentOffset.x + Data->PosEnd.x + Data->NegEnd.x + RandomGenerator.RandRange(0, 2) + 1;
		if (NewX < Size.x)
		{
			CurrentOffset.x = NewX;
			return true;
		}
	case 1:
		NewZ = CurrentOffset.z + Data->PosEnd.z + Data->NegEnd.z + RandomGenerator.RandRange(0, 2) + 1;
		if (NewZ < Size.z)
		{
			CurrentOffset.z = NewZ;
			return true;
		}
	}
	if(CurrentFloor < MaxHeightPerFloor.Num() - 2)
	{
		CurrentOffset.h += MaxHeightPerFloor[CurrentFloor];
		CurrentFloor++;
		CurrentOffset.x = 1;
		CurrentOffset.z = 1;
		return SetSectionLocation(Data);
	}
	return false;
}

void ADungeonSpawner::SpawnBlock(int i, int j, int k)
{
	FVector Location = FVector(i * 400.0f, k * 400.0f, j * 400.0f);
	FMapBlock CurBlock = ActiveMapGraph->GetBlock(i, j, k);
	UWorld* World = GetWorld();
	FRotator Rotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnParams;
	//Skip Empty Block
	if (
		(CurBlock.IsUpBlocked ||
			CurBlock.IsDownBlocked ||
			CurBlock.IsLeftBlocked ||
			CurBlock.IsRightBlocked ||
			CurBlock.IsFrontBlocked ||
			CurBlock.IsBackwardBlocked)
		)
	{
		ARoomBlock* NewBlockActor;
		if (CurBlock.Code == BASEROOMCODE)
		{
			NewBlockActor = World->SpawnActor<ARoomBlock>(BaseSectionData->BlockClass, Location, Rotation, SpawnParams);
			if(NewBlockActor)
			{
				NewBlockActor->SetValue(CurBlock, BaseSectionData);
			}
		}
		else
		{
			NewBlockActor = World->SpawnActor<ARoomBlock>(SectionDatas[RoomCodeToSectionDataIndex[CurBlock.Code]]->BlockClass, Location, Rotation, SpawnParams);
			if (NewBlockActor)
			{
				NewBlockActor->SetValue(CurBlock, SectionDatas[RoomCodeToSectionDataIndex[CurBlock.Code]]);
			}
			
		}
	}
}

void ADungeonSpawner::CalcMaxHeightPerFloor()
{
	int MaxHeight = 0;
	int SumMaxHeight = 0;
	while (true)
	{
		for (USectionShapeData* Data : SectionDatas)
		{
			int Height = Data->PosEnd.h + FMath::Abs(Data->NegEnd.h) + 1;
			MaxHeight = MaxHeight < Height ? Height : MaxHeight;
		}
		SumMaxHeight += MaxHeight;
		if (SumMaxHeight > Size.h) { break; }
		MaxHeightPerFloor.Add(MaxHeight);
	}
}

void ADungeonSpawner::PlaceSection(USectionShapeData* Section, int RoomCode)
{
	for(FMapVector BlockToPlace : Section->SectionData.SectionBlocks)
	{
		FMapVector RealBlockLoc = BlockToPlace + Section->SectionOffset;
		ActiveMapGraph->SetBlock(RealBlockLoc.x, RealBlockLoc.h, RealBlockLoc.z,
			{ 
				!(Section->GetIsOccupied(BlockToPlace.x, BlockToPlace.h + 1, BlockToPlace.z)),
				!(Section->GetIsOccupied(BlockToPlace.x, BlockToPlace.h - 1, BlockToPlace.z)),
				!(Section->GetIsOccupied(BlockToPlace.x, BlockToPlace.h, BlockToPlace.z - 1)),
				!(Section->GetIsOccupied(BlockToPlace.x, BlockToPlace.h, BlockToPlace.z + 1)),
				!(Section->GetIsOccupied(BlockToPlace.x + 1, BlockToPlace.h, BlockToPlace.z)),
				!(Section->GetIsOccupied(BlockToPlace.x - 1, BlockToPlace.h, BlockToPlace.z)),

				RoomCode 
			});
	}
}

void ADungeonSpawner::InitSection(USectionShapeData* Section, const FMapVector& Offset)
{
	Section->SectionOffset = Offset;
	Section->InitMapData();
}

void ADungeonSpawner::SpawnSectionObjects(USectionShapeData* Section)
{
	FMapVector Center = Section->SectionData.Center + Section->SectionOffset;
	for (FMapSectionObject ObjectData : Section->SectionObjects)
	{
		FVector Location = FVector(
			(ObjectData.Location.x + Center.x) * 100.0f,
			(ObjectData.Location.z + Center.z) * 100.0f,
			(ObjectData.Location.h + Center.h) * 100.0f);
		Location += ObjectData.Offset.GetLocation();

		FRotator Rotation = ObjectData.Offset.GetRotation().Rotator();

		FActorSpawnParameters SpawnParams;

		GetWorld()->SpawnActor<AActor>(ObjectData.ObjectClass, Location, Rotation, SpawnParams);
	}
}

void ADungeonSpawner::MakeEntrance(USectionShapeData* Section, int RoomCode)
{
	U3DBoolArray* DoorArray = NewObject<U3DBoolArray>();
	DoorArray->SetSize(
		Section->PosEnd.x + Section->NegEnd.x,
		Section->PosEnd.h + Section->NegEnd.h,
		Section->PosEnd.z + Section->NegEnd.z);
	FMapVector AdjustedCenter = Section->SectionData.Center + Section->SectionOffset;
	DoorArray->Reset(false);

	for (int i = 0; i < Section->SectionData.EntranceCount; i++)
	{
		int Len = Section->SectionData.SectionBlocks.Num();
		TArray<int> Shuffle;
		do {
			Shuffle.AddUnique(RandomGenerator.RandRange(0, Len - 1));
		} while (Shuffle.Num() < Len);

		for (int j = 0; j < Len; j++)
		{
			FMapVector Rand = Section->SectionData.SectionBlocks[Shuffle[j]];
			if (Section->GetIsOccupied(Rand.x, Rand.h, Rand.z) &&
					(
						//Short-Circuit Evaluation
						MakeDoorIfConnectedToOthers(Section, Rand, DoorArray, AdjustedCenter, { 0,0,-1 }) ||
						MakeDoorIfConnectedToOthers(Section, Rand, DoorArray, AdjustedCenter, { 0,0,1 }) ||
						MakeDoorIfConnectedToOthers(Section, Rand, DoorArray, AdjustedCenter, { 1,0,0 }) ||
						MakeDoorIfConnectedToOthers(Section, Rand, DoorArray, AdjustedCenter, { -1,0,0 })
					)
				)
			{
				break;
			}
			
		}
	}
}

bool ADungeonSpawner::MakeDoorIfConnectedToOthers(USectionShapeData* Section, FMapVector& Rand, U3DBoolArray* DoorArray, FMapVector& AdjustedCenter, FMapVector Offset)
{
	if (!(Section->GetIsOccupied(Rand + Offset)))
	{
		DoorArray->Set(Rand.x + Section->NegEnd.x, Rand.h + Section->NegEnd.h, Rand.z + Section->NegEnd.z, true);
		MakeDoor(Rand, AdjustedCenter);
		return true;
	}
	return false;
}

void ADungeonSpawner::MakeDoor(FMapVector& Rand, FMapVector& AdjustedCenter)
{
	FMapBlock Block1 = ActiveMapGraph->GetBlock(Rand.x + AdjustedCenter.x, Rand.h + AdjustedCenter.h, Rand.z + AdjustedCenter.z);
	FMapBlock Block2 = ActiveMapGraph->GetBlock(Rand.x + AdjustedCenter.x, Rand.h + AdjustedCenter.h, Rand.z + AdjustedCenter.z - 1);
	Block1.IsLeftBlocked = false;
	Block2.IsRightBlocked = false;
	ActiveMapGraph->SetBlock(Rand.x + AdjustedCenter.x, Rand.h + AdjustedCenter.h, Rand.z + AdjustedCenter.z, Block1);
	ActiveMapGraph->SetBlock(Rand.x + AdjustedCenter.x, Rand.h + AdjustedCenter.h, Rand.z + AdjustedCenter.z - 1, Block2);
}
