// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomBlock.h"

// Sets default values
void ARoomBlock::Init_Implementation()
{
}

void ARoomBlock::SetValue(FMapBlock NewBlock, USectionShapeData* Data)
{
	BlockData = NewBlock;
	ShapeData = Data;
}
