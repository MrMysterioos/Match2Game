#include "DataManager.h"
#include "json/document.h"
#include "cocos2d.h"

#include <json/writer.h>

USING_NS_CC;
using namespace rapidjson;

DataManager* DataManager::_instance = 0;

DataManager * DataManager::GetInstance()
{
	if (!_instance) {
		_instance = new DataManager();
	}
	return _instance;
}

std::vector<LevelData> DataManager::GetLevelData(std::string path) {
	Document document;

	std::vector<LevelData> levels;

	std::string jsonData = FileUtils::getInstance()->getStringFromFile(path);
	document.Parse(jsonData.c_str());

	auto arr = document.GetArray();
	for (auto& el : arr) {
		levels.push_back(LevelData(el["num"].GetInt(), el["src"].GetString(), el["fin"].GetBool()));
	}

	return levels;
}

void DataManager::SaveLevelData(std::vector<LevelData> levels, std::string path)
{
	Document document;
	std::string jsonData = FileUtils::getInstance()->getStringFromFile(path);
	document.Parse(jsonData.c_str());

	auto arr = document.GetArray();
	for (int i = 0; i < levels.size(); i++) {
		arr[i]["fin"].SetBool(levels[i].isFinish);
	}

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	document.Accept(writer);
	FileUtils::getInstance()->writeStringToFile(buffer.GetString(), "json/levels.json");
}

BuildLevelData DataManager::GetBuildLevelData(std::string path)
{
	Document document;
	std::string jsonData = FileUtils::getInstance()->getStringFromFile(path);
	document.Parse(jsonData.c_str());

	BuildLevelData bld;
	bld.lvlId = document["id"].GetInt();
	bld.goal = document["goal"].GetInt();
	bld.moves = document["moves"].GetInt();
	bld.width = document["width"].GetInt();
	bld.height = document["height"].GetInt();

	auto arr = document["map"].GetArray();
	bld.matrix = std::vector<std::vector<int>>(bld.width, std::vector<int>(bld.height, 0));

	for (int ix = 0; ix < bld.width; ix++) {
		for (int iy = 0; iy < bld.height; iy++) {
			bld.matrix[ix][iy] = arr[ix].GetArray()[iy].GetInt();
		}
	}

	return bld;
}
