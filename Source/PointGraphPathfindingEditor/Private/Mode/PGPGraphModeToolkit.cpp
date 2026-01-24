// By hzFishy - 2026 - Do whatever you want with it.


#include "Mode/PGPGraphModeToolkit.h"
#include "SlateOptMacros.h"
#include "Mode/PGPGraphEditorPanel.h"


void FPGPGraphModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
	
	ToolkitWidget = SNew(SPGPGraphEditorPanel);
}

TSharedPtr<SWidget> FPGPGraphModeToolkit::GetInlineContent() const
{
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.VAlign(VAlign_Fill)
		[
			ToolkitWidget.ToSharedRef()
		];
}
