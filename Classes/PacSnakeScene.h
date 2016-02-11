#ifndef __PACSNAKE_SCENE_H__
#define __PACSNAKE_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	void update(float) override;
	void objectCollision();
	void move();
	void moveBody(cocos2d::Vec2 headPos);
};

#endif // __HELLOWORLD_SCENE_H__
