// Fill out your copyright notice in the Description page of Project Settings.


#include "SectionShapeData.h"

void USectionShapeData::InitMapData()
{
	OccupiedArray = NewObject<U3DBoolArray>(this, U3DBoolArray::StaticClass());
	OccupiedArray->SetSize(PosEnd.x + NegEnd.x, PosEnd.h + NegEnd.h, PosEnd.z + NegEnd.z);
	for (FMapVector BlockPos : SectionData.SectionBlocks)
	{
		OccupiedArray->Set(BlockPos.x + NegEnd.x, BlockPos.h + NegEnd.h, BlockPos.z + NegEnd.z, true);
	}
}
