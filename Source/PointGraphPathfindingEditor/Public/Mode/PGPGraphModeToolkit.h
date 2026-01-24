// By hzFishy - 2026 - Do whatever you want with it.

#pragma once

#include "Widgets/SCompoundWidget.h"

class SPGPGraphEditorPanel;
/**
 * 
 */
class POINTGRAPHPATHFINDINGEDITOR_API FPGPGraphModeToolkit : public FModeToolkit
{
	TSharedPtr<SPGPGraphEditorPanel> GraphEditorPanel;
	
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode) override;
	
	virtual TSharedPtr<SWidget> GetInlineContent() const override;
};
