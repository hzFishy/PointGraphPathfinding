// By hzFishy - 2026 - Do whatever you want with it.

#include "PointGraphPathfindingEditor.h"
#include "Mode/PGPModeCommands.h"

#define LOCTEXT_NAMESPACE "FPointGraphPathfindingEditorModule"

void FPointGraphPathfindingEditorModule::StartupModule()
{
	FPGPGraphEditorModeCommands::Register();
}

void FPointGraphPathfindingEditorModule::ShutdownModule()
{
	FPGPGraphEditorModeCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FPointGraphPathfindingEditorModule, PointGraphPathfindingEditor)