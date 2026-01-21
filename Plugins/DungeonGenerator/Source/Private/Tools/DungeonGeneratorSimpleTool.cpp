// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeonGeneratorSimpleTool.h"
#include "InteractiveToolManager.h"
#include "ToolBuilderUtil.h"
#include "CollisionQueryParams.h"
#include "Engine/World.h"
#include "Engine/HitResult.h"
#include "Misc/MessageDialog.h"

// localization namespace
#define LOCTEXT_NAMESPACE "DungeonGeneratorSimpleTool"

/*
 * ToolBuilder implementation
 */

UInteractiveTool* UDungeonGeneratorSimpleToolBuilder::BuildTool(const FToolBuilderState& SceneState) const
{
	UDungeonGeneratorSimpleTool* NewTool = NewObject<UDungeonGeneratorSimpleTool>(SceneState.ToolManager);
	NewTool->SetWorld(SceneState.World);
	return NewTool;
}



/*
 * ToolProperties implementation
 */

UDungeonGeneratorSimpleToolProperties::UDungeonGeneratorSimpleToolProperties()
{
	ShowExtendedInfo = true;
}


/*
 * Tool implementation
 */

UDungeonGeneratorSimpleTool::UDungeonGeneratorSimpleTool()
{
}


void UDungeonGeneratorSimpleTool::SetWorld(UWorld* World)
{
	this->TargetWorld = World;
}


void UDungeonGeneratorSimpleTool::Setup()
{
	USingleClickTool::Setup();

	Properties = NewObject<UDungeonGeneratorSimpleToolProperties>(this);
	AddToolPropertySource(Properties);
}


void UDungeonGeneratorSimpleTool::OnClicked(const FInputDeviceRay& ClickPos)
{
	// we will create actor at this position
	FVector NewActorPos = FVector::ZeroVector;

	// cast ray into world to find hit position
	FVector RayStart = ClickPos.WorldRay.Origin;
	FVector RayEnd = ClickPos.WorldRay.PointAt(99999999.f);
	FCollisionObjectQueryParams QueryParams(FCollisionObjectQueryParams::AllObjects);
	FHitResult Result;
	if (TargetWorld->LineTraceSingleByObjectType(Result, RayStart, RayEnd, QueryParams))
	{
		if (AActor* ClickedActor = Result.GetActor())
		{
			FText ActorInfoMsg;

			if (Properties->ShowExtendedInfo)
			{
				ActorInfoMsg = FText::Format(LOCTEXT("ExtendedActorInfo", "Name: {0}\nClass: {1}"), 
					FText::FromString(ClickedActor->GetName()), 
					FText::FromString(ClickedActor->GetClass()->GetName())
				);
			}
			else
			{
				ActorInfoMsg = FText::Format(LOCTEXT("BasicActorInfo", "Name: {0}"), FText::FromString(Result.GetActor()->GetName()));
			}

			FText Title = LOCTEXT("ActorInfoDialogTitle", "Actor Info");
			// JAH TODO: consider if we can highlight the actor prior to opening the dialog box or make it non-modal
			FMessageDialog::Open(EAppMsgType::Ok, ActorInfoMsg, Title);
		}
	}
}


#undef LOCTEXT_NAMESPACE