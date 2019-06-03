#pragma once

#include "cocos2d.h"
#include "Items.h"

USING_NS_CC;

class ItemSpriteFactory {
public:
	static Sprite* CreateItemSprite(Item type) {
		std::string textureName = "";
		if (type == Item::Wall) {
			textureName = "wall_0001.png";
		}
		else if (type == Item::Cocos) {
			textureName = "characters_0006.png";
		}
		else if (type == Item::Orange) {
			textureName = "characters_0003.png";
		}
		else if (type == Item::Bread) {
			textureName = "characters_0004.png";
		}
		else if (type == Item::Milk) {
			textureName = "characters_0001.png";
		}
		else if (type == Item::Tomato) {
			textureName = "characters_0002.png";
		}
		else if (type == Item::Broccoli) {
			textureName = "characters_0005.png";
		}
		else if (type == Item::Cristal) {
			textureName = "characters_0007.png";
		}

		if (textureName != "") {
			return Sprite::create("textures/characters/" + textureName);
		}
		return nullptr;
	}
};