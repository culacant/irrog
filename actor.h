#ifndef ACTOR_H
#define ACTOR_H

#include "weapon.h"

#include <list>

enum UNITSTATE
{
	DEAD,
	IDLE,
	MOVING,
	FIRING,
};

enum FACING
{
	NORTH,
	EAST,
	SOUTH,
	WEST,
};

// Coordinate Struct
struct Point
{
	int X;
	int Y;
};

// Impact Class

class Impact
{
private:
	int m_iPosX;
	int m_iPosY;
	int m_iSupression;
	int m_iSupressionArea;
public:
	Impact(int posx, int posy, int supression, int supressionArea);

	int getPosX();
	int getPosY();
	void setPosX(int posx);
	void setPosY(int posy);

	int getSupression();
	int getSupressionArea();
};

// Actor Class

class Actor
{
private:
	char m_cGraphic;

	int m_iTeam;

	int m_iPosX;
	int m_iPosY;

	int m_iTargetPosX;
	int m_iTargetPosY;

	UNITSTATE m_sState;

	FACING m_sFacing;

	std::list<Impact> *m_lBulletImpacts;
	std::list<Point> *m_lPath;

	int m_iMoveSpeed;
	int m_iDefaultMoveSpeed;

	Weapon *m_wWeapon;

	bool m_bVisible;

	int m_iSupression;
	bool m_bInCover;

	int m_iMoveSpeedCheck;
public:
	Actor(int posx, int posy, char graphic, int team);
	~Actor();

	void setGraphic(char graphic);
	char Graphic();

	int getTeam();

	void setPos(int x, int y);
	int getPosX();
	int getPosY();

	FACING getFacing();
	Point getFacingDir();

	void setFacing(FACING facing);
	void setFacing(int x, int y);
	bool isFacing(int x, int y);

	void setTargetPos(int x, int y);
	int getTargetPosX();
	int getTargetPosY();
	void clearTargetPos();
	std::list<Impact>* getBulletImpacts();

	void Kill();
	int isDead();

	void setPath(std::list<Point> *path);
	std::list<Point> *getPath();

	void Update(int frametime);

	bool Visible();
	bool setVisible(bool draw);

	void Supress(int supression);
	int getSupression();
	Weapon* getWeapon();

	UNITSTATE getState();

	bool inCover();
	void setCover(bool cover);
};

#endif
