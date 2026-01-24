// By hzFishy - 2026 - Do whatever you want with it.

#pragma once

#include "Widgets/SCompoundWidget.h"


class POINTGRAPHPATHFINDINGEDITOR_API SPGPGraphEditorPanel : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPGPGraphEditorPanel)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
};
