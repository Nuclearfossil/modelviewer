#pragma once

#include <string>

struct aiScene;

bool OpenSceneFile(const char* sourceFilename);
const aiScene* GetScene();