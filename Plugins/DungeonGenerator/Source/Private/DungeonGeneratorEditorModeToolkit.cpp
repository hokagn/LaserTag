// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeonGeneratorEditorModeToolkit.h"
#include "DungeonGeneratorEditorMode.h"
#include "Engine/Selection.h"

#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "DungeonGeneratorEditorModeToolkit"

FDungeonGeneratorEditorModeToolkit::FDungeonGeneratorEditorModeToolkit()
{
}

void FDungeonGeneratorEditorModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
}

void FDungeonGeneratorEditorModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(NAME_Default);
}


FName FDungeonGeneratorEditorModeToolkit::GetToolkitFName() const
{
	return FName("DungeonGeneratorEditorMode");
}

FText FDungeonGeneratorEditorModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("DisplayName", "DungeonGeneratorEditorMode Toolkit");
}

#undef LOCTEXT_NAMESPACE
