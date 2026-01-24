// By hzFishy - 2026 - Do whatever you want with it.

#pragma once

#include "Tools/UEdMode.h"
#include "PGPGraphEdMode.generated.h"


UCLASS()
class POINTGRAPHPATHFINDINGEDITOR_API UPGPGraphEdMode : public UEdMode
{
	GENERATED_BODY()
	
	UPGPGraphEdMode();
	
	virtual void Enter() override;
	
	virtual void CreateToolkit() override;
	
	virtual void Exit() override;
};
