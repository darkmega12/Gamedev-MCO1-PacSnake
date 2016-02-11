#pragma once

#include<string.h>
#include<stdio.h>
#include "cocos2d.h"

class GameObject
{
public:
	void create(std::string filename, cocos2d::Vec2 position, int direction);
	cocos2d::Vec2 getPosition();
	int getDirection();
	int getX();
	int getY();
	void setDirection(int direction);
	cocos2d::Sprite getObject();
	cocos2d::Sprite* objectSprite;
private:
	int currDirection;
	int xTile;
	int yTile;
};
