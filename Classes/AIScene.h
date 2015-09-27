#pragma once
#include "Block.h"
#define GAME_TILE_WIDTH 30
#define GAME_COLS 12
#define GAME_ROWS 12
#define GAME_TILE_DISTANCE 2
using namespace std;
class AIScene:public Layer
{
public:
#define MIN 0
	struct point
	{
		int x;
		int y;
		int sx;
		int index;
		int G;
		int F;
		point *pre;
		point():pre(NULL),G(0),F(0)
		{}
	};
	enum Dir
	{
		UP=0,
		DOWN,
		LEFT,
		RIGHT
	};
	CREATE_FUNC(AIScene);
	virtual bool init();
	void initGame();
	void addFood();
	static Scene* createScene();
	void addButton();
	void ButtonListener(Ref *render);
	void addSnake();
	Point calPos(int x,int y);
	void runGame(float dt);
	bool A_star(int intmap[][GAME_COLS],int type);
	
private:
	Size size;
	int map[GAME_ROWS][GAME_COLS];
	Vec2 orgin;
	int food_pos_x;
	int food_pos_y;
	Vector<Block*> _snake;
	Dir nowDir;
	int distance(point p1,point p2);
	int H_distance(point p1,point p2);
	int F_min(vector<point*> *open);
	void check(point *p,point *vecmap[][GAME_COLS],vector<point*> *open,vector<point*> *close,point *end_pos);
	void isValue(point *p,point *now,point *end_pos,vector<point*> *open,vector<point*> *close);
	bool isFind(vector<point*> *open,point *end_pos);
	void calDir(int next_x,int next_y,int now_x,int now_y);
	void RunAI();
	bool startPathFinding();
	bool moveSankeAndFindPath();
	point checkAroundAndDeal();
	void menuCloseCallback(Ref* pSender);
	bool findOtherToWei(int intmap[][GAME_COLS]);
	vector<point*> route;
	bool isOn;
};
