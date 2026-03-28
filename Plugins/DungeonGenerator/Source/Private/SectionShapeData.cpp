// Fill out your copyright notice in the Description page of Project Settings.


#include "SectionShapeData.h"

void USectionShapeData::InitMapData()
{
	OccupiedArray = NewObject<U3DBoolArray>(this, U3DBoolArray::StaticClass());
	OccupiedArray->SetSize(PosEnd.x + NegEnd.x + 1, PosEnd.h + NegEnd.h + 1, PosEnd.z + NegEnd.z + 1);
	for (FMapVector BlockPos : SectionData.SectionBlocks)
	{
		OccupiedArray->Set(BlockPos.x + NegEnd.x, BlockPos.h + NegEnd.h, BlockPos.z + NegEnd.z, true);
	}
}
