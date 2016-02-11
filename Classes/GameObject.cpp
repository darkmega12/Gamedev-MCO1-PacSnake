#include "GameObject.h"
#include<stdio.h>
#include<string.h>


USING_NS_CC;

void GameObject::create(std::string filename, Vec2 position, int direction)
{
	objectSprite = Sprite::create(filename);
	objectSprite->setAnchorPoint(Vec2());
	objectSprite->setPosition(position.x, position.y);
	currDirection = direction;

}

Vec2 GameObject::getPosition()
{
	return objectSprite->getPosition();
}

int GameObject::getX()
{
	return xTile = (int)objectSprite->getPosition().x / 45;
}

int GameObject::getY()
{
	return yTile = (int)objectSprite->getPosition().y / 45;
}

int GameObject::getDirection()
{
	return currDirection;
}

void GameObject::setDirection(int direction)
{
	currDirection = direction;
}