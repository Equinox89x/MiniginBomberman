#pragma once
#include "Scene.h"
#include "MathLib.h"

void MakeMenu();
void CreateGlobalGameObject(dae::Scene* scene, MathLib::GameMode gm, int levelIndex);
void MakeHighScoreScreen();