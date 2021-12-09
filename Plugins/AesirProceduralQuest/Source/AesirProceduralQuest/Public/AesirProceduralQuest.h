// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FAesirProceduralQuestModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
