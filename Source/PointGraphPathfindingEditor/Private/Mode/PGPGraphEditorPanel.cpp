// By hzFishy - 2026 - Do whatever you want with it.


#include "Mode/PGPGraphEditorPanel.h"
#include "PropertyCustomizationHelpers.h"
#include "SlateOptMacros.h"
#include "Core/PGPGraphNetworkDefinition.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

void SPGPGraphEditorPanel::Construct(const FArguments& InArgs)
{
	FAssetData DefaultAsset;
	
	auto AssetPickerWidget = SNew(SObjectPropertyEntryBox)
		.AllowClear(true)
		.AllowCreate(true)
		.AllowedClass(UPGPGraphNetworkDefinition::StaticClass())
		.OnObjectChanged(this, &SPGPGraphEditorPanel::OnObjectChanged);
	
	ChildSlot
	[
		SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBorder)
					.BorderBackgroundColor(FColor::White)
					.Padding(FMargin(5))
					.Content()
					[
						SNew(SVerticalBox)
							+SVerticalBox::Slot()
							.HAlign(HAlign_Center)
							.AutoHeight()
							[
								SNew(STextBlock)
									.Text(INVTEXT("Graph Network Definition"))
							]
							+SVerticalBox::Slot()
								.AutoHeight()
								[
									SNew(SSpacer)
										.Size(FVector2D(1, 10))
								]
							+SVerticalBox::Slot()
								.HAlign(HAlign_Center)
								.AutoHeight()
								[
									SAssignNew(GraphDefinitionWidgetSwitcher, SWidgetSwitcher)
										+SWidgetSwitcher::Slot()
										[
											AssetPickerWidget
										]
										+SWidgetSwitcher::Slot()
										.HAlign(HAlign_Fill)
										[
											SNew(SBorder)
												.BorderBackgroundColor(FColor::White)
												.Padding(FMargin(5))
												.HAlign(HAlign_Fill)
												.Content()
												[
													SNew(SHorizontalBox)
														+SHorizontalBox::Slot()
														.AutoWidth()
															[
																SNew(SBorder)
																	.BorderBackgroundColor(FColor::White)
																	.Padding(FMargin(5))
																	.HAlign(HAlign_Fill)
																	.Content()
																	[
																		SAssignNew(GraphDefinitionNameWidget, STextBlock)
																			.Text(INVTEXT("Not set"))
																	]
															]
														+SHorizontalBox::Slot()
															.AutoWidth()
															[
																SNew(SSpacer)
																	.Size(FVector2D(10, 1))
															]
														+SHorizontalBox::Slot()
															[
																SNew(SButton)
																	.Text(INVTEXT("Clear"))
																	.ForegroundColor(FColor::Red)
																	.OnClicked(this, &SPGPGraphEditorPanel::OnClearClicked)
															]
												]
										]
							]
					]
			]
	];
}

void SPGPGraphEditorPanel::OnObjectChanged(const FAssetData& AssetData)
{
	if (AssetData.IsValid())
	{
		SetGraphDefinition(Cast<UPGPGraphNetworkDefinition>(AssetData.GetAsset()));
	}
	else
	{
		ClearGraphDefinition();
	}
}

FReply SPGPGraphEditorPanel::OnClearClicked()
{
	ClearGraphDefinition();
	return FReply::Handled();
}

void SPGPGraphEditorPanel::SetGraphDefinition(UPGPGraphNetworkDefinition* NewDef)
{
	SelectedGraphNetwork = NewDef;
	GraphDefinitionNameWidget->SetText(FText::FromString(FString::Printf(TEXT("%s"), *SelectedGraphNetwork->GetName())));
	GraphDefinitionWidgetSwitcher->SetActiveWidgetIndex(1);
}

void SPGPGraphEditorPanel::ClearGraphDefinition()
{
	SelectedGraphNetwork = nullptr;
	GraphDefinitionWidgetSwitcher->SetActiveWidgetIndex(0);
}
