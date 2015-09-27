#pragma once
#include "cocos2d.h"
USING_NS_CC;
class Block:public Sprite
{
public:
	enum TYPE
	{
		NOR,
		TOU,
		WEI
	};
	static Block* create(TYPE type,int x,int y);
	virtual bool init(TYPE type,int x,int y);
	void setSnakeColor(TYPE type);
	int pos_x;
	int pos_y;
private:
	TYPE _type;
};