#include "Block.h"

Block* Block::create( TYPE type ,int x,int y)
{
	auto b = new Block();
	if (b&&b->init(type,x,y))
	{
		b->autorelease();
		return b;
	}
	else
	{
		delete b;
		b=nullptr;
		return nullptr;
	}
}

bool Block::init( TYPE type ,int x,int y)
{
	if (!Sprite::init())
	{
		return false;
	}
	pos_x=x;
	pos_y=y;
	this->setContentSize(Size(30,30));
	switch (type)
	{
	case Block::NOR:
		{
			this->setTextureRect(Rect(0,0,30,30));
			this->setColor(ccc3(0xff,0x82,0x47));
			break;
		}
	case Block::TOU:
		{
			this->setTextureRect(Rect(0,0,30,30));
			this->setColor(Color3B::RED);
			break;
		}
		
	case Block::WEI:
		{
			this->setTextureRect(Rect(0,0,30,30));
			this->setColor(Color3B::BLUE);
			break;
		}
	default:
		break;
	}
	return true;
}

void Block::setSnakeColor( TYPE type )
{
	if (type==Block::NOR)
	{
		this->setColor(ccc3(0xff,0x82,0x47));
	}
	else if (type==Block::TOU)
	{
		this->setColor(Color3B::RED);
	}
	else if (type==Block::WEI)
	{
		this->setColor(Color3B::BLUE);
	}
}


