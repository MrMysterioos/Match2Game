#pragma once

#include "cocos2d.h"
#include "LevelData.h"

USING_NS_CC;

class LevelPreview : public Node {
public:

	bool init(LevelData data);

	void SetCallback(const ccMenuCallback &callback);

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

	void _SetLevelData(LevelData data);

	MenuItemImage * _button;
	Label * _label;
	Sprite * _checkSprite;
};