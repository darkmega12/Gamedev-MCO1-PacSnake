#include "PacSnakeScene.h"
#include "GameObject.h"
#include "SimpleAudioEngine.h"  

#include<stdio.h>
#include<cmath>

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

static int mapArray[20][15];
static std::deque<GameObject> snakeBody;
GameObject head;
bool gameOver = false;
cocos2d::Sprite* food;
static bool hasFood = false;
static int allowMove = 0;

struct TileType{
	enum type{ WALL = 0, FREE = -1, FOOD = 1, BODY = 2 , HEAD = 3};
};

struct Direction{
	enum dir{ UP = 1, LEFT = 2, DOWN = 3, RIGHT = 4 };
};

Vec2 getFoodPosition()
{
	int x, y;
	struct timeval tp;
	gettimeofday(&tp, NULL);
	long int random = tp.tv_sec * 1000 + tp.tv_usec * 1000;
	do {
		x = (int)abs((rand() * random) % 20);
		y = (int)abs((rand() * random) % 15);
	} while (mapArray[x][y] != TileType::FREE);
	mapArray[x][y] = TileType::FOOD;
	return Vec2(x * 45, y * 45);
}


void HelloWorld::objectCollision()
{
	switch (mapArray[head.getX()][head.getY()])
	{
		case TileType::WALL:
		case TileType::BODY:
			gameOver = true;
			break;
		case TileType::FOOD:
			hasFood = true;
			mapArray[head.getX()][head.getY()] = TileType::HEAD;
			food->setPosition(getFoodPosition());
			break;
	}
}

void HelloWorld::moveBody(Vec2 headPos)
{
	int size = (int)snakeBody.size();
	Vec2 tempPos, currPos = headPos;
	for (int i = 0; i < size - 1; i++)
	{
		mapArray[snakeBody[i + 1].getX()][snakeBody[i + 1].getY()] = TileType::FREE;
		tempPos = snakeBody[i + 1].getPosition();
		snakeBody[i + 1].objectSprite->setPosition(currPos);
		currPos = tempPos;
		if (i == 0)
		{
			mapArray[head.getX()][head.getY()] = TileType::HEAD;
		}
		else
		{
			mapArray[snakeBody[i].getX()][snakeBody[i].getY()] = TileType::BODY;
		}
	}
}

void HelloWorld::move()
{
	Vec2 headPos = head.getPosition();
	switch (head.getDirection())
	{
	case Direction::UP:
		head.objectSprite->setPosition(head.getPosition().x, head.getPosition().y + 45);
		break;
	case Direction::DOWN:
		head.objectSprite->setPosition(head.getPosition().x, head.getPosition().y - 45);
		break;
	case Direction::LEFT:
		head.objectSprite->setPosition(head.getPosition().x - 45, head.getPosition().y);
		break;
	case Direction::RIGHT:
		head.objectSprite->setPosition(head.getPosition().x + 45, head.getPosition().y);
		break;
	}

	objectCollision();

	if (hasFood)
	{
		Vec2 pos = head.getPosition();
		GameObject body;
		body.create("pac_whole.png", pos, Direction::DOWN);
		this->addChild(body.objectSprite, 5);
		snakeBody.pop_front();
		snakeBody.push_front(body);
		snakeBody.push_front(head);
		hasFood = false;
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pacman_eatfruit.wav");
	}
	moveBody(headPos);
}

void initPacman()
{
	//auto 
}
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	if (!Layer::init())
	{
		return false;
	}
	
	food = cocos2d::Sprite::create("food.png");

	//first is x coordinate, second is y
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (i == 0 || i == 19 || j == 0 || j == 14)
			{
				mapArray[i][j] = TileType::WALL;
				auto background = Sprite::create("background.png");
				background->setAnchorPoint(Vec2(0, 0));
				background->setPosition(i * 45,j  * 45);
				this->addChild(background, 0);
			}
			else
			{
				mapArray[i][j] = TileType::FREE;
			}
			
		}
	}
	food->setTag(50);
	food->setAnchorPoint(Vec2());
	food->setPosition(getFoodPosition());
	this->addChild(food, 0);

	Vec2 initPos = Vec2(450, 315);
	head.create("pac_right.png", initPos, Direction::RIGHT);
	mapArray[(int)initPos.x / 45][(int)initPos.y / 45] = TileType::HEAD;
	snakeBody.push_front(head);
	this->addChild(head.objectSprite, 10);


	
	this->scheduleUpdate();

	return true;
}

void HelloWorld::update(float)
{
	if (gameOver)
	{
		std::string score = "Game Over! Score ";
		score += std::to_string(snakeBody.size());
		auto label = Label::createWithSystemFont(score, "Arial", 40);
		label->setAnchorPoint(cocos2d::Vec2(0.0, 0.0));
		label->setPosition(300, 300);
		this->addChild(label, 11);
	}

	auto eventListener = EventListenerKeyboard::create();
	eventListener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event){

		switch (keyCode){
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
			if (head.getDirection() != Direction::RIGHT)
			{
				head.setDirection(Direction::LEFT);
				head.objectSprite->setTexture("pac_left.png");
			}
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
			if (head.getDirection() != Direction::LEFT)
			{
				head.setDirection(Direction::RIGHT);
				head.objectSprite->setTexture("pac_right.png");
			}
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
		case EventKeyboard::KeyCode::KEY_W:
			if (head.getDirection() != Direction::DOWN)
			{
				head.setDirection(Direction::UP);
				head.objectSprite->setTexture("pac_up.png");
			}
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		case EventKeyboard::KeyCode::KEY_S:
			if (head.getDirection() != Direction::UP)
			{
				head.setDirection(Direction::DOWN);
				head.objectSprite->setTexture("pac_down.png");
			}
			break;
		}
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, head.objectSprite);
	if (!gameOver)
	{
		move();
	}
}

struct PacBody
{
	

};