#pragma once

#include "cocos2d.h"
#include "LevelData.h"

USING_NS_CC;

class LevelPreview : public Node {
public:

	bool init(LevelData data);

	void _SetLevelData(LevelData data);

	static LevelPreview* create(LevelData data)
	{
		LevelPreview *pRet = new(std::nothrow) LevelPreview();
		if (pRet && pRet->init(data))
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = nullptr;
			return nullptr;
		}
	}

private:
	MenuItem * _button;
	Sprite * _mainSprite;
	Label * _label;
	Sprite * _checkSprite;
};