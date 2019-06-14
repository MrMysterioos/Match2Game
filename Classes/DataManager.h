#pragma once
#include "LevelData.h"
#include "BuildLevelData.h"
#include "json/document.h"

using namespace rapidjson;

class DataManager {
public:
	static DataManager* GetInstance();

	std::vector<LevelData> GetLevelData(std::string path);
	void SaveLevelData(std::vector<LevelData> levels, std::string path);

	BuildLevelData GetBuildLevelData(std::string path);

private:

	static DataManager* _instance;
};