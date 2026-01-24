// By hzFishy - 2026 - Do whatever you want with it.


#include "Mode/PGPGraphEditorPanel.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SPGPGraphEditorPanel::Construct(const FArguments& InArgs)
{
	
	ChildSlot
	[
		SNew(STextBlock).Text(INVTEXT("TestText"))
	];
	
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
