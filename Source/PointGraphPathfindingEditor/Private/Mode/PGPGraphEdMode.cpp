// By hzFishy - 2026 - Do whatever you want with it.


#include "Mode/PGPGraphEdMode.h"

#include "EditorModeManager.h"
#include "Core/PGPEditorCore.h"
#include "Mode/PGPGraphModeToolkit.h"

UPGPGraphEdMode::UPGPGraphEdMode()
{
	Info = FEditorModeInfo(
		PGP::Editor::GraphEditorModeName,
		INVTEXT("Point Graph Pathfinding"),
		FSlateIcon(),
		true);
}

void UPGPGraphEdMode::Enter()
{
	Super::Enter();
}

void UPGPGraphEdMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FPGPGraphModeToolkit);
}

void UPGPGraphEdMode::Exit()
{
	Super::Exit();
}
