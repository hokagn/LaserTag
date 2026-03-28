// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonSpawner.h"
#include <RoomBlock.h>

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

	int RoomCode = BASEROOMCODE + 1;
	while(true)
	for (USectionShapeData* Data : SectionDatas)
	{
		if (SetSectionLocation(Data))
		{
			RoomCodeToSectionDataIndex.Add(RoomCode, SectionDatas.IndexOfByKey(Data));
			InitSection(Data, CurrentOffset);
			PlaceSection(Data, RoomCode);
			MakeEntrance(Data, RoomCode);
			RoomCode++;
			CurrentOffset.x += Data->PosEnd.x;
		}
		else
		{
			goto BREAK;
		}	
	}

	BREAK:

	int Len = MaxHeightPerFloor.Num();
	int TotalHeight = 0;
	for (int Floor = 0; Floor < Len; Floor++)
	{
		for (int i = 0; i < Size.x; i++)
			for (int j = 0; j < MaxHeightPerFloor[Floor]; j++)
				for (int k = 0; k < Size.z; k++)
				{
					FMapBlock CurBlock = ActiveMapGraph->GetBlock(i, j + TotalHeight, k);
					if (CurBlock.Code == BASEROOMCODE)
					{
						ActiveMapGraph->SetBlock(i, j + TotalHeight, k,
							FMapBlock(
								(MaxHeightPerFloor[Floor] - 1 == j) || ActiveMapGraph->GetBlock(i, j + TotalHeight + 1, k).IsDownBlocked,
								(j == 0) || ActiveMapGraph->GetBlock(i, j + TotalHeight - 1, k).IsUpBlocked,
								(k == 0) || ActiveMapGraph->GetBlock(i, j + TotalHeight, k - 1).IsRightBlocked,
								(k == Size.z - 1) || ActiveMapGraph->GetBlock(i, j + TotalHeight, k + 1).IsLeftBlocked,
								(i == Size.x - 1) || ActiveMapGraph->GetBlock(i + 1, j + TotalHeight, k).IsBackwardBlocked,
								(i == 0) || ActiveMapGraph->GetBlock(i - 1, j + TotalHeight, k).IsFrontBlocked,
								BASEROOMCODE
							)
						);
					}
				}
		TotalHeight += MaxHeightPerFloor[Floor];
	}

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
	NewX = CurrentOffset.x + abs(Data->NegEnd.x) + RandomGenerator.RandRange(0, 2) + 1;
	if (NewX + Data->PosEnd.x < Size.x-1)
	{
		CurrentOffset.x = NewX;
		CurrentOffset.z = FilledMap.z + Data->NegEnd.z + RandomGenerator.RandRange(-1, 1);
		return true;
	}
	else
	{
		NewZ = FilledMap.z + abs(Data->NegEnd.z) + RandomGenerator.RandRange(0, 2) + 3;
		if (NewZ + Data->PosEnd.z < Size.z-1)
		{
			CurrentOffset.x = 1;
			CurrentOffset.z = FMath::Max(FilledMap.z, NewZ);
			FilledMap.z = NewZ + Data->PosEnd.z;
			return true;
		}
	}
	
	if(CurrentFloor < MaxHeightPerFloor.Num() - 1)
	{
		CurrentOffset.h += MaxHeightPerFloor[CurrentFloor];
		CurrentFloor++;
		CurrentOffset.x = 1;
		CurrentOffset.z = 1;
		FilledMap.x = 1;
		FilledMap.z = 1;
		return SetSectionLocation(Data);
	}
	return false;
}

void ADungeonSpawner::SpawnBlock(int i, int j, int k)
{
	FVector Location = FVector(i * 200.0f, k * 200.0f, j * 400.0f);
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
				(Section->GetIsOccupied(BlockToPlace.x, BlockToPlace.h + 1, BlockToPlace.z)),
				(Section->GetIsOccupied(BlockToPlace.x, BlockToPlace.h - 1, BlockToPlace.z)),
				(Section->GetIsOccupied(BlockToPlace.x, BlockToPlace.h, BlockToPlace.z - 1)),
				(Section->GetIsOccupied(BlockToPlace.x, BlockToPlace.h, BlockToPlace.z + 1)),
				(Section->GetIsOccupied(BlockToPlace.x + 1, BlockToPlace.h, BlockToPlace.z)),
				(Section->GetIsOccupied(BlockToPlace.x - 1, BlockToPlace.h, BlockToPlace.z)),

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
		Section->PosEnd.x + Section->NegEnd.x + 3,
		Section->PosEnd.h + Section->NegEnd.h + 3,
		Section->PosEnd.z + Section->NegEnd.z + 3);
	FMapVector AdjustedCenter = Section->SectionData.Center + Section->SectionOffset;
	DoorArray->Reset(false);

	for (int i = 0; i < Section->SectionData.EntranceCount; i++)
	{
		int Len = Section->SectionData.SectionBlocks.Num();
		TArray<int> Shuffle;
		do {
			Shuffle.AddUnique(RandomGenerator.RandRange(0, Len - 1));
		} while (Shuffle.Num() < Len);
		const int FuncLen = 4;
		TArray<int> FuncShuffle;
		do {
			FuncShuffle.AddUnique(RandomGenerator.RandRange(0, FuncLen - 1));
		} while (FuncShuffle.Num() < FuncLen);
		for (int j = 0; j < Len; j++)
		{
			FMapVector Rand = Section->SectionData.SectionBlocks[Shuffle[j]];
			if (DoorArray->Get(Rand.x, Rand.h, Rand.z) || Rand.h != 0)continue;
				for (int k = 0; k < FuncLen; k++)
				{
					switch (FuncShuffle[k])
					{
					case 0:
						if (MakeDoorIfConnectedToOthers(Section, Rand, DoorArray, AdjustedCenter, { 0,0,-1 }))goto BREAK;
						break;
					case 1:
						if(MakeDoorIfConnectedToOthers(Section, Rand, DoorArray, AdjustedCenter, { 0,0,1 }))goto BREAK;
						break;
					case 2:
						if(MakeDoorIfConnectedToOthers(Section, Rand, DoorArray, AdjustedCenter, { 1,0,0 }))goto BREAK;
						break;
					case 3:
						if(MakeDoorIfConnectedToOthers(Section, Rand, DoorArray, AdjustedCenter, { -1,0,0 }))goto BREAK;
						break;
					}
					
				}
		}
	BREAK:
		1; // expression for goto
	}
}

bool ADungeonSpawner::MakeDoorIfConnectedToOthers(USectionShapeData* Section, FMapVector& Rand, U3DBoolArray* DoorArray, FMapVector& AdjustedCenter, FMapVector Offset)
{
	FMapVector Loc = AdjustedCenter + Offset + Rand;
	if (Loc.x < 0 || Loc.h < 0 || Loc.z < 0 ||
		Loc.x > Size.z || Loc.z > Size.z || Loc.z > Size.z)
	{
		return false;
	}
	if (ActiveMapGraph->GetBlock(Loc).Code != ActiveMapGraph->GetBlock(AdjustedCenter + Rand).Code)
	{
		DoorArray->Set(Rand.x + AdjustedCenter.x, Rand.h + AdjustedCenter.h, Rand.z + AdjustedCenter.z, true);
		MakeDoor(Rand, AdjustedCenter, Offset);
		return true;
	}
	return false;
}

void ADungeonSpawner::MakeDoor(FMapVector& Rand, FMapVector& AdjustedCenter, FMapVector& Offset)
{
	FMapBlock Block1 = ActiveMapGraph->GetBlock(Rand + AdjustedCenter);
	FMapBlock Block2 = ActiveMapGraph->GetBlock(Rand + AdjustedCenter + Offset);
	if (Offset.x == 1)
	{
		Block1.IsFrontBlocked = false;
		Block2.IsBackwardBlocked = false;
	}
	else if (Offset.x == -1)
	{
		Block1.IsBackwardBlocked = false;
		Block2.IsFrontBlocked = false;
	}
	if (Offset.z == 1)
	{
		Block1.IsRightBlocked = false;
		Block2.IsLeftBlocked = false;
	}
	else if (Offset.z == -1)
	{
		Block1.IsLeftBlocked = false;
		Block2.IsRightBlocked = false;
	}

	ActiveMapGraph->SetBlock(Rand.x + AdjustedCenter.x, Rand.h + AdjustedCenter.h, Rand.z + AdjustedCenter.z, Block1);
	ActiveMapGraph->SetBlock(Rand + AdjustedCenter + Offset, Block2);
}
