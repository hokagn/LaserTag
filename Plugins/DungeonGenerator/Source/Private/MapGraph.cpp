// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGraph.h"

FMapBlock UMapGraph::GetBlock(int x, int h, int z)
{
	int Index = x + h * XWidth + z * XWidth * Height;
	if (RoomBlockArray.Max() <= Index)
	{
		return FMapBlock();
	}
	return RoomBlockArray[Index];
}

void UMapGraph::SetBlock(int x, int h, int z, FMapBlock BlockData)
{
	int Index = x + h * XWidth + z * XWidth * Height;
	if(RoomBlockArray.Max() <= Index)
	{
		return;
	}
	RoomBlockArray[Index] = BlockData;
}

void UMapGraph::AddBlock(FMapBlock BlockData)
{
	RoomBlockArray.Add(BlockData);
}

void UMapGraph::SetSize(int x, int h, int z)
{
	XWidth = x > 0 ? x : 1;
	Height = h > 0 ? h : 1;
	ZWidth = z > 0 ? z : 1;
	RoomBlockArray.Init(FMapBlock(), XWidth * Height * ZWidth);
}
