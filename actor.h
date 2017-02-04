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
	int m_iTimeout;
public:
	Impact(int posx, int posy);

	void Update(int frametime);

	int getPosX();
	int getPosY();
	void setPosX(int posx);
	void setPosY(int posy);
	int TimeoutHit();
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

	std::list<Impact> *m_lBulletImpacts;
	std::list<Point> *m_lPath;

	int m_iMoveSpeed;

	Weapon *m_wWeapon;

	bool m_bVisible;

public:
	Actor(int posx, int posy, char graphic, int team);

	void setGraphic(char graphic);
	char Graphic();

	int getTeam();

	void setPos(int x, int y);
	int getPosX();
	int getPosY();

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
};

#endif
