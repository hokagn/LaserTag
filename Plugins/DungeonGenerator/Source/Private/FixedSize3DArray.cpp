// Fill out your copyright notice in the Description page of Project Settings.


#include "FixedSize3DArray.h"

bool U3DBoolArray::Get(int i, int j, int k)
{
    int Index = i + Size.x * j + Size.x * Size.h * k;
    if (Data.Max() <= Index)
    {
        Data.Init(false, Index + 1);
    }
    return Data[Index];
}

bool U3DBoolArray::Set(int i, int j, int k, bool Value)
{
    int Index = i + Size.x * j + Size.x * Size.h * k;
    if (Data.Max() <= Index)
    {
        Data.Init(false, Index + 1);
    }
    Data[Index] = Value;
    return true;
}

bool U3DBoolArray::Set(FMapVector ijk, bool Value)
{
	return Set(ijk.x, ijk.h, ijk.z, Value);
}
