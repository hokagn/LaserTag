// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeonGeneratorModule.h"
#include "DungeonGeneratorEditorModeCommands.h"

#define LOCTEXT_NAMESPACE "DungeonGeneratorModule"

void FDungeonGeneratorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FDungeonGeneratorEditorModeCommands::Register();
}

void FDungeonGeneratorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FDungeonGeneratorEditorModeCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDungeonGeneratorModule, DungeonGenerator)