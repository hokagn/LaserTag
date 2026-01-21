// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeonGeneratorEditorMode.h"
#include "DungeonGeneratorEditorModeToolkit.h"
#include "Tools/EdModeInteractiveToolsContext.h"
#include "InteractiveToolManager.h"
#include "DungeonGeneratorEditorModeCommands.h"
#include "Modules/ModuleManager.h"


//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
// AddYourTool Step 1 - include the header file for your Tools here
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
#include "Tools/DungeonGeneratorSimpleTool.h"
#include "Tools/DungeonGeneratorInteractiveTool.h"

// step 2: register a ToolBuilder in FDungeonGeneratorEditorMode::Enter() below


#define LOCTEXT_NAMESPACE "DungeonGeneratorEditorMode"

const FEditorModeID UDungeonGeneratorEditorMode::EM_DungeonGeneratorEditorModeId = TEXT("EM_DungeonGeneratorEditorMode");

FString UDungeonGeneratorEditorMode::SimpleToolName = TEXT("DungeonGenerator_ActorInfoTool");
FString UDungeonGeneratorEditorMode::InteractiveToolName = TEXT("DungeonGenerator_MeasureDistanceTool");


UDungeonGeneratorEditorMode::UDungeonGeneratorEditorMode()
{
	FModuleManager::Get().LoadModule("EditorStyle");

	// appearance and icon in the editing mode ribbon can be customized here
	Info = FEditorModeInfo(UDungeonGeneratorEditorMode::EM_DungeonGeneratorEditorModeId,
		LOCTEXT("ModeName", "DungeonGenerator"),
		FSlateIcon(),
		true);
}


UDungeonGeneratorEditorMode::~UDungeonGeneratorEditorMode()
{
}


void UDungeonGeneratorEditorMode::ActorSelectionChangeNotify()
{
}

void UDungeonGeneratorEditorMode::Enter()
{
	UEdMode::Enter();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// AddYourTool Step 2 - register the ToolBuilders for your Tools here.
	// The string name you pass to the ToolManager is used to select/activate your ToolBuilder later.
	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////// 
	const FDungeonGeneratorEditorModeCommands& SampleToolCommands = FDungeonGeneratorEditorModeCommands::Get();

	RegisterTool(SampleToolCommands.SimpleTool, SimpleToolName, NewObject<UDungeonGeneratorSimpleToolBuilder>(this));
	RegisterTool(SampleToolCommands.InteractiveTool, InteractiveToolName, NewObject<UDungeonGeneratorInteractiveToolBuilder>(this));

	// active tool type is not relevant here, we just set to default
	GetToolManager()->SelectActiveToolType(EToolSide::Left, SimpleToolName);
}

void UDungeonGeneratorEditorMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FDungeonGeneratorEditorModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> UDungeonGeneratorEditorMode::GetModeCommands() const
{
	return FDungeonGeneratorEditorModeCommands::Get().GetCommands();
}

#undef LOCTEXT_NAMESPACE
