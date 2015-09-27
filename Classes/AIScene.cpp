#include "AIScene.h"

Scene* AIScene::createScene()
{
	auto scene = Scene::create();
	auto layer = AIScene::create();
	scene->addChild(layer);
	return scene;
}

bool AIScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	isOn = true;
	srand((unsigned)time(nullptr));
	size = Director::getInstance()->getVisibleSize();
	auto bg = Sprite::createWithSpriteFrameName("background1.jpg");
	bg->setPosition(size/2);
	this->addChild(bg,-1);

	addButton();
	initGame();
	this->schedule(schedule_selector(AIScene::runGame),0.1);
	//RunAI();
	return true;
}

void AIScene::addButton()
{
	int boardWidth=GAME_TILE_WIDTH*GAME_COLS+(GAME_COLS+1)*GAME_TILE_DISTANCE;
	int boardHeight=GAME_TILE_WIDTH*GAME_ROWS+(GAME_ROWS+1)*GAME_TILE_DISTANCE;
	auto colorBg = Sprite::create();
	colorBg->setTextureRect(Rect(0,0,boardWidth,boardHeight));
	colorBg->setColor(ccc3(180,180,180));
	colorBg->setPosition(size.width/2,size.height/10*6);
	orgin.x = (size.width-boardWidth)/2;
	orgin.y = size.height/10*6 - boardHeight/2;
	this->addChild(colorBg,0);

	auto bu1 = Sprite::createWithSpriteFrameName("topNor.png");
	auto bu2 = Sprite::createWithSpriteFrameName("topDown.png");
	auto topButton = MenuItemSprite::create(bu1,bu2,nullptr,CC_CALLBACK_1(AIScene::ButtonListener,this));
	topButton->setTag(101);
	topButton->setPosition(size.width/2,size.height/10*3-10);

	auto bd1 = Sprite::createWithSpriteFrameName("bottomNor.png");
	auto bd2 = Sprite::createWithSpriteFrameName("bottomDown.png");
	auto downButton = MenuItemSprite::create(bd1,bd2,nullptr,CC_CALLBACK_1(AIScene::ButtonListener,this));
	downButton->setTag(102);
	downButton->setPosition(size.width/2,size.height/10*2-30);

	auto bl1 = Sprite::createWithSpriteFrameName("leftNor.png");
	auto bl2 = Sprite::createWithSpriteFrameName("leftDown.png");
	auto leftButton = MenuItemSprite::create(bl1,bl2,nullptr,CC_CALLBACK_1(AIScene::ButtonListener,this));
	leftButton->setTag(103);
	leftButton->setPosition(size.width/10*3,size.height/10*2+10);

	auto br1 = Sprite::createWithSpriteFrameName("rightNor.png");
	auto br2 = Sprite::createWithSpriteFrameName("rightDown.png");
	auto rightButton = MenuItemSprite::create(br1,br2,nullptr,CC_CALLBACK_1(AIScene::ButtonListener,this));
	rightButton->setTag(104);
	rightButton->setPosition(size.width/10*7,size.height/10*2+10);

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(AIScene::menuCloseCallback, this));

	closeItem->setPosition(Vec2(size.width - closeItem->getContentSize().width/2 ,
			 closeItem->getContentSize().height/2));

	auto menu = Menu::create(topButton,downButton,leftButton,rightButton,closeItem,nullptr);
	menu->setPosition(Point::ZERO);
	this->addChild(menu,1);
}

void AIScene::ButtonListener( Ref *render )
{
	auto nowButton = (MenuItemSprite*)render;
	int tag = nowButton->getTag();
	if (tag==101)
	{
		if (nowDir == DOWN)
		{
			return;
		}
		else
		{
			nowDir = UP;
		}

	}
	else if (tag==102)
	{
		if (nowDir == UP)
		{
			return;
		}
		else
		{
			nowDir = DOWN;
		}
	}
	else if (tag==103)
	{
		if (nowDir == RIGHT)
		{
			return;
		}
		else
		{
			nowDir = LEFT;
		}
	}
	else if (tag==104)
	{
		if (nowDir==LEFT)
		{
			return;
		}
		else
		{
			nowDir = RIGHT;
		}
	}
}

void AIScene::initGame()
{
	for (int i=0;i<GAME_ROWS;i++)
	{
		for (int j=0;j<GAME_COLS;j++)
		{
			map[i][j]=0;
		}
	}
	addSnake();
	addFood();
	nowDir = UP;
}

void AIScene::addFood()
{
	bool flag = false;
	for (int i=0;i<GAME_COLS;i++)
	{
		for (int j=0;j<GAME_ROWS;j++)
		{
			if (map[j][i]==0)
			{
				flag = true;
			}
		}
	}
	if (!flag)
	{
		this->unscheduleAllSelectors();
		return;
	}
	int pos;
	do 
	{
		pos = rand()%144;
	} while (map[pos/12][pos%12]!=0);
	auto food = Sprite::create("star.png");
	food->setScale(0.35);
	food->setTag(1);
	food_pos_y = pos/12;
	food_pos_x = pos%12;
	food->setPosition(calPos(food_pos_x,food_pos_y));
	this->addChild(food,1);
	map[food_pos_y][food_pos_x] = 2;
}

void AIScene::addSnake()
{
	auto sp1 = Block::create(Block::TOU,5,4);
	sp1->setPosition(calPos(5,4));
	this->addChild(sp1,1);
	_snake.pushBack(sp1);
	auto sp2 = Block::create(Block::WEI,5,3);
	sp2->setPosition(calPos(5,3));
	this->addChild(sp2,1);
	_snake.pushBack(sp2);
	map[sp1->pos_y][sp1->pos_x]=1;
	map[sp2->pos_y][sp2->pos_x]=1;
}

Point AIScene::calPos( int x,int y )
{
	return Point(orgin.x+(x+0.5)*GAME_TILE_WIDTH+(x+1)*GAME_TILE_DISTANCE,orgin.y+(y+0.5)*GAME_TILE_WIDTH+(y+1)*GAME_TILE_DISTANCE);
}

void AIScene::runGame( float dt )
{
	if (isOn)
	{
		RunAI();
	}
	switch (nowDir)
	{
	case AIScene::UP:
		{
			if ((_snake.at(0)->pos_y >= (GAME_ROWS-1))||map[_snake.at(0)->pos_y+1][_snake.at(0)->pos_x]==1)
			{
				this->unscheduleAllSelectors();
				return;
			}
			auto block = Block::create(Block::TOU,_snake.at(0)->pos_x,_snake.at(0)->pos_y+1);
			block->setPosition(calPos(block->pos_x,block->pos_y));
			//map[block->pos_y][block->pos_x]=1;
			this->addChild(block,1);
			_snake.at(0)->setSnakeColor(Block::NOR);
			_snake.insert(0,block);
			if (map[block->pos_y][block->pos_x]!=2)
			{
				map[block->pos_y][block->pos_x]=1;
				_snake.at(_snake.size()-1)->removeFromParentAndCleanup(true);
				map[_snake.at(_snake.size()-1)->pos_y][_snake.at(_snake.size()-1)->pos_x]=0;
				_snake.erase(_snake.size()-1);
				_snake.at(_snake.size()-1)->setSnakeColor(Block::WEI);
			}
			else
			{
				this->getChildByTag(1)->removeFromParentAndCleanup(true);
				map[block->pos_y][block->pos_x]=1;
				addFood();
			}
			break;
		}

	case AIScene::DOWN:
		{
			if ((_snake.at(0)->pos_y <= 0)||map[_snake.at(0)->pos_y-1][_snake.at(0)->pos_x]==1)
			{
				this->unscheduleAllSelectors();
				return;
			}
			auto block = Block::create(Block::TOU,_snake.at(0)->pos_x,_snake.at(0)->pos_y-1);
			block->setPosition(calPos(block->pos_x,block->pos_y));
			//map[block->pos_y][block->pos_x]=1;
			this->addChild(block,1);
			_snake.at(0)->setSnakeColor(Block::NOR);
			_snake.insert(0,block);
			if (map[block->pos_y][block->pos_x]!=2)
			{
				map[block->pos_y][block->pos_x]=1;
				_snake.at(_snake.size()-1)->removeFromParentAndCleanup(true);
				map[_snake.at(_snake.size()-1)->pos_y][_snake.at(_snake.size()-1)->pos_x]=0;
				_snake.erase(_snake.size()-1);
				_snake.at(_snake.size()-1)->setSnakeColor(Block::WEI);
			}
			else
			{
				this->getChildByTag(1)->removeFromParentAndCleanup(true);
				map[block->pos_y][block->pos_x]=1;
				addFood();
			}
			break;
		}
	case AIScene::LEFT:
		{
			if ((_snake.at(0)->pos_x <= 0)||map[_snake.at(0)->pos_y][_snake.at(0)->pos_x-1]==1)
			{
				this->unscheduleAllSelectors();
				return;
			}
			auto block = Block::create(Block::TOU,_snake.at(0)->pos_x-1,_snake.at(0)->pos_y);
			block->setPosition(calPos(block->pos_x,block->pos_y));
			//map[block->pos_y][block->pos_x]=1;
			this->addChild(block,1);
			_snake.at(0)->setSnakeColor(Block::NOR);
			_snake.insert(0,block);
			if (map[block->pos_y][block->pos_x]!=2)
			{
				map[block->pos_y][block->pos_x]=1;
				_snake.at(_snake.size()-1)->removeFromParentAndCleanup(true);
				map[_snake.at(_snake.size()-1)->pos_y][_snake.at(_snake.size()-1)->pos_x]=0;
				_snake.erase(_snake.size()-1);
				_snake.at(_snake.size()-1)->setSnakeColor(Block::WEI);
			}
			else
			{
				this->getChildByTag(1)->removeFromParentAndCleanup(true);
				map[block->pos_y][block->pos_x]=1;
				addFood();
			}
			break;
		}
	case AIScene::RIGHT:
		{
			if ((_snake.at(0)->pos_x >= (GAME_COLS-1))||map[_snake.at(0)->pos_y][_snake.at(0)->pos_x+1]==1)
			{
				this->unscheduleAllSelectors();
				return;
			}
			auto block = Block::create(Block::TOU,_snake.at(0)->pos_x+1,_snake.at(0)->pos_y);
			block->setPosition(calPos(block->pos_x,block->pos_y));
			//map[block->pos_y][block->pos_x]=1;
			this->addChild(block,1);
			_snake.at(0)->setSnakeColor(Block::NOR);
			_snake.insert(0,block);
			if (map[block->pos_y][block->pos_x]!=2)
			{
				map[block->pos_y][block->pos_x]=1;
				_snake.at(_snake.size()-1)->removeFromParentAndCleanup(true);
				map[_snake.at(_snake.size()-1)->pos_y][_snake.at(_snake.size()-1)->pos_x]=0;
				_snake.erase(_snake.size()-1);
				_snake.at(_snake.size()-1)->setSnakeColor(Block::WEI);
			}
			else
			{
				this->getChildByTag(1)->removeFromParentAndCleanup(true);
				map[block->pos_y][block->pos_x]=1;
				addFood();
			}
			break;
		}
	default:
		break;
	}
}

bool AIScene::A_star(int intmap[][GAME_COLS],int type)
{
	route.clear();
	vector<point*> *open;
	vector<point*> *close;
	open = new vector<point*>();
	close = new vector<point*>();
	point *vecmap[GAME_ROWS][GAME_COLS];
	point *begin_pos;
	point *end_pos;
	int count = 0;
	for (int i=0;i<GAME_ROWS;i++)
	{
		for (int j=0;j<GAME_COLS;j++)
		{
			point *tp = new point;
			tp->x = j;
			tp->y = i;
			tp->sx = intmap[i][j];
			tp->index = count++;
			vecmap[i][j]=tp;
			if (tp->sx==2)
			{
				end_pos = tp;
			}
			if (tp->sx==3)
			{
				begin_pos = tp;
			}
		}
	}
	open->push_back(begin_pos);
	bool flag = false;
	while(open->size()>0)
	{
		int index = F_min(open);
		point *nowP = open->at(index);
		close->push_back(open->at(index));
		open->erase(open->begin()+index);
		check(nowP,vecmap,open,close,end_pos);
		if (isFind(open,end_pos))
		{
			flag = true;
			break;
		}	
	}
	if (flag)
	{
		point *end_P = end_pos;
		while(end_P->pre!=NULL)
		{
			//log("%d %d",end_P->x,end_P->y);
			route.insert(route.begin(),end_P);
			if (end_P->pre->pre!=NULL)
			{
				end_P = end_P->pre;
			}
			else
			{
				//calDir(end_P->x,end_P->y,_snake.at(0)->pos_x,_snake.at(0)->pos_y);
				break;
			}
		}
	}
	if (route.size()<1)
	{
		return false;
	}
	if (type==1&&route.size()<=1)
	{
		int tpmap[GAME_ROWS][GAME_COLS];
		if (begin_pos->x+1<GAME_COLS&&intmap[begin_pos->y][begin_pos->x+1]!=1&&intmap[begin_pos->y][begin_pos->x+1]!=2)
		{
			memcpy(tpmap,intmap,sizeof(tpmap));
			tpmap[begin_pos->y][begin_pos->x]=1;
			tpmap[begin_pos->y][begin_pos->x+1]=3;
			if (findOtherToWei(tpmap))
			{
				return true;
			}
		}
		if (begin_pos->x-1>=0&&intmap[begin_pos->y][begin_pos->x-1]!=1&&intmap[begin_pos->y][begin_pos->x-1]!=2)
		{
			memcpy(tpmap,intmap,sizeof(tpmap));
			tpmap[begin_pos->y][begin_pos->x]=1;
			tpmap[begin_pos->y][begin_pos->x-1]=3;
			if (findOtherToWei(tpmap))
			{
				return true;
			}
		}
		if (begin_pos->y+1<GAME_ROWS&&intmap[begin_pos->y+1][begin_pos->x]!=1&&intmap[begin_pos->y+1][begin_pos->x]!=2)
		{
			memcpy(tpmap,intmap,sizeof(tpmap));
			tpmap[begin_pos->y][begin_pos->x]=1;
			tpmap[begin_pos->y+1][begin_pos->x]=3;
			if (findOtherToWei(tpmap))
			{
				return true;
			}
		}
		if (begin_pos->y-1>=0&&intmap[begin_pos->y-1][begin_pos->x]!=1&&intmap[begin_pos->y-1][begin_pos->x]!=2)
		{
			memcpy(tpmap,intmap,sizeof(tpmap));
			tpmap[begin_pos->y][begin_pos->x]=1;
			tpmap[begin_pos->y-1][begin_pos->x]=3;
			if (findOtherToWei(tpmap))
			{
				return true;
			}
		}
		return false;
	}
	return flag;
}

int AIScene::F_min(vector<point*> *open)
{

	int index = 0;
	for(int i=0;i<open->size();i++)
	{
		if(open->at(i)->F <= open->at(index)->F)
		{
			index = i;
		}
	}
	return index;
}

int AIScene::distance(point p1,point p2) 
{
	double dis = sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
	return int(dis*10);
}

int AIScene::H_distance(point p1,point p2)
{
	return (abs(p1.x-p2.x)+abs(p1.y-p2.y))*10;
}

void AIScene::check(point *p,point *vecmap[][GAME_COLS],vector<point*> *open,vector<point*> *close,point *end_pos)
{
	if ((p->x-1)>=0)
	{
		isValue(vecmap[p->y][p->x-1],p,end_pos,open,close);
	}
	if ((p->x+1)<GAME_COLS)
	{
		isValue(vecmap[p->y][p->x+1],p,end_pos,open,close);
	}
	if ((p->y-1)>=0)
	{
		isValue(vecmap[p->y-1][p->x],p,end_pos,open,close);
	}
	if ((p->y+1)<GAME_ROWS)
	{
		isValue(vecmap[p->y+1][p->x],p,end_pos,open,close);
	}
}

void AIScene::isValue(point *p,point *now,point *end_pos,vector<point*> *open,vector<point*> *close)
{
	if (p->sx==1)
	{
		return;
	}
	for (int i=0;i<close->size();i++)
	{
		if (close->at(i)->index==p->index)
		{
			return;
		}
	}
	for (int i=0;i<open->size();i++)
	{
		if (open->at(i)->index==p->index)
		{
			int now_G = distance(*p,*now)+now->G;
			if (now_G<p->G)
			{
				p->G = now_G;
				p->pre = now;
			}
			return ;
		}
	}
	p->pre = now;
	p->G = distance(*p,*now)+now->G;
	p->F = p->G + H_distance(*p,*end_pos);
	open->push_back(p);
}

bool AIScene::isFind(vector<point*> *open,point *end_pos)
{
	bool flag = false;
	point *end_P;
	for(int i=0;i<open->size();i++)
	{
		if(open->at(i)->index==end_pos->index)
		{
			flag = true;
			end_P = open->at(i);
		}
	}
	//if(flag)
	//{
	//	while(end_P->pre!=NULL)
	//	{
	//		//cout<<end_P->x<<' '<<end_P->y<<endl;
	//		log("%d %d",end_P->x,end_P->y);
	//		end_P = end_P->pre;
	//	}
	//}
	return flag;
}

void AIScene::calDir( int next_x,int next_y,int now_x,int now_y )
{
	Dir dir;
	if (next_x==now_x)
	{
		if (next_y>now_y)
		{
			dir=UP;
		}
		else
		{
			dir=DOWN;
		}
	}
	else
	{
		if (next_x>now_x)
		{
			dir=RIGHT;
		}
		else
		{
			dir=LEFT;
		}
	}
	switch (dir)
	{
	case AIScene::UP:
		if (nowDir!=DOWN)
		{
			nowDir=UP;
		}
		break;
	case AIScene::DOWN:
		if (nowDir!=UP)
		{
			nowDir=DOWN;
		}
		break;
	case AIScene::LEFT:
		if (nowDir!=RIGHT)
		{
			nowDir=LEFT;
		}
		break;
	case AIScene::RIGHT:
		if (nowDir!=LEFT)
		{
			nowDir=RIGHT;
		}
		break;
	default:
		break;
	}
}

void AIScene::RunAI()
{
	Point first_pos;
	auto canFindPath= false;                  //可以找到吃苹果的路线
	auto canFindTail = false;   //可以找到自己尾巴的路线
	if (_snake.size()<(GAME_ROWS*GAME_COLS))
	{
		
		canFindPath = startPathFinding();
	}
	if (canFindPath)
	{
		//log("successSHIWU");
		//log("%d %d",route.at(0)->x,route.at(0)->y);
		first_pos.x = route.at(0)->x;
		first_pos.y = route.at(0)->y;
		if (_snake.size()==(GAME_COLS*GAME_ROWS-1))
		{
			calDir(first_pos.x,first_pos.y,_snake.at(0)->pos_x,_snake.at(0)->pos_y);
			return ;
		}
		canFindTail=moveSankeAndFindPath();
		if (canFindTail)
		{
			//log("successWEI");
			calDir(first_pos.x,first_pos.y,_snake.at(0)->pos_x,_snake.at(0)->pos_y);
		}
	}
	if (canFindTail==false||canFindPath==false)
	{
		point tp = checkAroundAndDeal();
		if (tp.x>=0)
		{
			//log("canfindnext %d %d",tp.x,tp.y);
			calDir(tp.x,tp.y,_snake.at(0)->pos_x,_snake.at(0)->pos_y);
		}
	}
}

bool AIScene::startPathFinding()
{
	int intmap[GAME_ROWS][GAME_COLS];
	memcpy(intmap,map,sizeof(map));
	intmap[_snake.at(0)->pos_y][_snake.at(0)->pos_x]=3;
	return A_star(intmap,0);
}

bool AIScene::moveSankeAndFindPath()
{
	vector<point*> nextSnake;
	for (int i=route.size()-1;i>=0;i--)
	{
		point *tp = new point();
		tp->x=route.at(i)->x;
		tp->y=route.at(i)->y;
		nextSnake.push_back(tp);
	}
	for (int i=0;i<_snake.size();i++)
	{
		point *tp = new point();
		tp->x=_snake.at(i)->pos_x;
		tp->y=_snake.at(i)->pos_y;
		nextSnake.push_back(tp);
	}
	for (int i=0;i<route.size()-1;i++)
	{
		nextSnake.pop_back();
	}
	int intmap[GAME_ROWS][GAME_COLS];
	memset(intmap,0,sizeof(intmap));
	for (int i=0;i<nextSnake.size();i++)
	{
		//log("%d %d",nextSnake.at(i)->x,nextSnake.at(i)->y);
		if (i==0)
		{
			intmap[nextSnake.at(i)->y][nextSnake.at(i)->x]=3;
		}
		else if (i==nextSnake.size()-1)
		{
			intmap[nextSnake.at(i)->y][nextSnake.at(i)->x]=2;
		}
		else
		{
			intmap[nextSnake.at(i)->y][nextSnake.at(i)->x]=1;
		}
	}
	return A_star(intmap,1);
}

AIScene::point AIScene::checkAroundAndDeal()
{
	vector<point*> around;
	point result_P;
	point food_P;
	food_P.x = food_pos_x;
	food_P.y = food_pos_y;
	int intmap[GAME_ROWS][GAME_COLS];
	if (_snake.at(0)->pos_x+1<GAME_COLS&&map[_snake.at(0)->pos_y][_snake.at(0)->pos_x+1]!=1)
	{
		//log("you");
		memcpy(intmap,map,sizeof(map));
		if (intmap[_snake.at(0)->pos_y][_snake.at(0)->pos_x+1]!=2)
		{
			intmap[_snake.at(0)->pos_y][_snake.at(0)->pos_x+1]=3;
			intmap[food_pos_y][food_pos_x]=0;
			intmap[_snake.at(_snake.size()-1)->pos_y][_snake.at(_snake.size()-1)->pos_x]=0;
			intmap[_snake.at(_snake.size()-2)->pos_y][_snake.at(_snake.size()-2)->pos_x]=2;
		}
		else
		{
			intmap[_snake.at(0)->pos_y][_snake.at(0)->pos_x+1]=3;
			intmap[_snake.at(_snake.size()-1)->pos_y][_snake.at(_snake.size()-1)->pos_x]=2;
		}
		if (A_star(intmap,1))
		{
			point *tp=new point();
			tp->x=_snake.at(0)->pos_x+1;
			tp->y=_snake.at(0)->pos_y;
			tp->F = distance(*tp,food_P);
			around.push_back(tp);
			//log("you x y F %d %d %d",tp->x,tp->y,tp->F);
		}
		
	}
	if (_snake.at(0)->pos_x-1>=0&&map[_snake.at(0)->pos_y][_snake.at(0)->pos_x-1]!=1)
	{
		//log("zou");
		memcpy(intmap,map,sizeof(map));
		if (intmap[_snake.at(0)->pos_y][_snake.at(0)->pos_x-1]!=2)
		{
			intmap[_snake.at(0)->pos_y][_snake.at(0)->pos_x-1]=3;
			intmap[food_pos_y][food_pos_x]=0;
			intmap[_snake.at(_snake.size()-1)->pos_y][_snake.at(_snake.size()-1)->pos_x]=0;
			intmap[_snake.at(_snake.size()-2)->pos_y][_snake.at(_snake.size()-2)->pos_x]=2;
		}
		else
		{
			intmap[_snake.at(0)->pos_y][_snake.at(0)->pos_x-1]=3;
			intmap[_snake.at(_snake.size()-1)->pos_y][_snake.at(_snake.size()-1)->pos_x]=2;
		}
		if (A_star(intmap,1))
		{
			point *tp=new point();
			tp->x=_snake.at(0)->pos_x-1;
			tp->y=_snake.at(0)->pos_y;
			tp->F = distance(*tp,food_P);
			around.push_back(tp);
			//log("zou x y F %d %d %d",tp->x,tp->y,tp->F);
		}
	}
	if (_snake.at(0)->pos_y-1>=0&&map[_snake.at(0)->pos_y-1][_snake.at(0)->pos_x]!=1)
	{
		//log("xia");
		memcpy(intmap,map,sizeof(map));
		if (intmap[_snake.at(0)->pos_y-1][_snake.at(0)->pos_x]!=2)
		{
			intmap[_snake.at(0)->pos_y-1][_snake.at(0)->pos_x]=3;
			intmap[food_pos_y][food_pos_x]=0;
			intmap[_snake.at(_snake.size()-1)->pos_y][_snake.at(_snake.size()-1)->pos_x]=0;
			intmap[_snake.at(_snake.size()-2)->pos_y][_snake.at(_snake.size()-2)->pos_x]=2;
		}
		else
		{
			intmap[_snake.at(0)->pos_y-1][_snake.at(0)->pos_x]=3;
			intmap[_snake.at(_snake.size()-1)->pos_y][_snake.at(_snake.size()-1)->pos_x]=2;
		}
		if (A_star(intmap,1))
		{
			point *tp=new point();
			tp->x=_snake.at(0)->pos_x;
			tp->y=_snake.at(0)->pos_y-1;
			tp->F = distance(*tp,food_P);
			around.push_back(tp);
			//log("xia x y F %d %d %d",tp->x,tp->y,tp->F);
		}
	}
	if (_snake.at(0)->pos_y+1<GAME_ROWS&&map[_snake.at(0)->pos_y+1][_snake.at(0)->pos_x]!=1)
	{
		//log("shang");
		memcpy(intmap,map,sizeof(map));
		if (intmap[_snake.at(0)->pos_y+1][_snake.at(0)->pos_x]!=2)
		{
			intmap[_snake.at(0)->pos_y+1][_snake.at(0)->pos_x]=3;
			intmap[food_pos_y][food_pos_x]=0;
			intmap[_snake.at(_snake.size()-1)->pos_y][_snake.at(_snake.size()-1)->pos_x]=0;
			intmap[_snake.at(_snake.size()-2)->pos_y][_snake.at(_snake.size()-2)->pos_x]=2;
		}
		else
		{
			intmap[_snake.at(0)->pos_y+1][_snake.at(0)->pos_x]=3;
			intmap[_snake.at(_snake.size()-1)->pos_y][_snake.at(_snake.size()-1)->pos_x]=2;
		}
		if (A_star(intmap,1))
		{
			point *tp=new point();
			tp->x=_snake.at(0)->pos_x;
			tp->y=_snake.at(0)->pos_y+1;
			tp->F = distance(*tp,food_P);
			around.push_back(tp);
			//log("shang x y F %d %d %d",tp->x,tp->y,tp->F);
		}
	}
	if (around.size()<=0)
	{
		result_P.x=-1;
		result_P.y=-1;
		return result_P;
	}
	int MAX_F=0;
	int index=0;
	for (int i=0;i<around.size();i++)
	{
		if (around.at(i)->F>MAX_F)
		{
			index = i;
			MAX_F=around.at(i)->F;
		}
	}
	//log("MAXF %d",MAX_F);
	result_P.x=around.at(index)->x;
	result_P.y=around.at(index)->y;
	return result_P;
}


void AIScene::menuCloseCallback(Ref* pSender)
{
	if (isOn)
	{
		isOn = false;
	}
	else
	{
		isOn = true;
	}

}

bool AIScene::findOtherToWei(int intmap[][GAME_COLS])
{
	vector<point*> *open;
	vector<point*> *close;
	open = new vector<point*>();
	close = new vector<point*>();
	point *vecmap[GAME_ROWS][GAME_COLS];
	point *begin_pos;
	point *end_pos;
	int count = 0;
	for (int i=0;i<GAME_ROWS;i++)
	{
		for (int j=0;j<GAME_COLS;j++)
		{
			point *tp = new point;
			tp->x = j;
			tp->y = i;
			tp->sx = intmap[i][j];
			tp->index = count++;
			vecmap[i][j]=tp;
			if (tp->sx==2)
			{
				end_pos = tp;
			}
			if (tp->sx==3)
			{
				begin_pos = tp;
			}
		}
	}
	open->push_back(begin_pos);
	bool flag = false;
	while(open->size()>0)
	{
		int index = F_min(open);
		point *nowP = open->at(index);
		close->push_back(open->at(index));
		open->erase(open->begin()+index);
		check(nowP,vecmap,open,close,end_pos);
		if (isFind(open,end_pos))
		{
			flag = true;
			break;
		}	
	}

	//if (flag)
	//{
	//	point *end_P = end_pos;
	//	while(end_P->pre!=NULL)
	//	{
	//		//log("%d %d",end_P->x,end_P->y);
	//		route.insert(route.begin(),end_P);
	//		if (end_P->pre->pre!=NULL)
	//		{
	//			end_P = end_P->pre;
	//		}
	//		else
	//		{
	//			//calDir(end_P->x,end_P->y,_snake.at(0)->pos_x,_snake.at(0)->pos_y);
	//			break;
	//		}
	//	}
	//}
	return flag;
}