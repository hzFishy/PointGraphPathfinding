// By hzFishy - 2026 - Do whatever you want with it.

#pragma once

#include "Widgets/SCompoundWidget.h"
class UPGPGraphNetworkDefinition;
class SWidgetSwitcher;


class POINTGRAPHPATHFINDINGEDITOR_API SPGPGraphEditorPanel : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPGPGraphEditorPanel)
		{
		}
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);
	
protected:
	TSharedPtr<SWidgetSwitcher> GraphDefinitionWidgetSwitcher;
	TSharedPtr<STextBlock> GraphDefinitionNameWidget;
	TWeakObjectPtr<UPGPGraphNetworkDefinition> SelectedGraphNetwork;
	
	void OnObjectChanged(const FAssetData& AssetData);
	FReply OnClearClicked();
	
	void SetGraphDefinition(UPGPGraphNetworkDefinition* NewDef);
	void ClearGraphDefinition();
};
