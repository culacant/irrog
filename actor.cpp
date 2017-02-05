#include "actor.h"

#include <stdlib.h>
#include <cstddef>
#include <cmath>

#include <iostream>

// Impact Class

Impact::Impact(int posx, int posy)
{
	m_iPosX = posx;
	m_iPosY = posy;
	m_iTimeout = 1000;
}

void Impact::Update(int frametime)
{
	m_iTimeout -= frametime;
}

int Impact::getPosX()
{
	return m_iPosX;
}
int Impact::getPosY()
{
	return m_iPosY;
}

void Impact::setPosX(int posx)
{
	m_iPosX = posx;
}
void Impact::setPosY(int posy)
{
	m_iPosY = posy;
}

int Impact::TimeoutHit()
{
	if(m_iTimeout < 0)
		return 1;
	else
		return 0;
}

// Actor Class

Actor::Actor(int posx = 1, int posy = 1, char graphic = '@', int team = 1)
{
	m_cGraphic = graphic;

	m_iTeam = team;

	m_iPosX = posx;
	m_iPosY = posy;

	m_iTargetPosX = -1;
	m_iTargetPosY = -1;

	m_sState = IDLE;
	m_sFacing = SOUTH;

	m_lPath = NULL;
	m_lBulletImpacts = new std::list<Impact>();

//	m_wWeapon = new Weapon(5,1000,5000);
	m_wWeapon = new Weapon(3000,10,5000,1000,100);
	m_iMoveSpeed = 1000/2;

	m_bVisible = true;
}

Actor::~Actor()
{
	delete m_lBulletImpacts;
	delete m_wWeapon;
	delete m_lPath;
}

void Actor::setGraphic(char graphic)
{
	m_cGraphic = graphic;
}
char Actor::Graphic()
{
	return m_cGraphic;
}

int Actor::getTeam()
{
	return m_iTeam;
}

void Actor::setPos(int x, int y)
{
	m_iPosX = x;
	m_iPosY = y;
}
int Actor::getPosX()
{
	return m_iPosX;
}
int Actor::getPosY()
{
	return m_iPosY;
}

FACING Actor::getFacing()
{
	return m_sFacing;
}

void Actor::setFacing(FACING facing)
{
	m_sFacing = facing;
}
void Actor::setFacing(int x, int y)
{
/*
	int dx = m_iPosX - x;
	int dy = m_iPosY - y;
	float angle = atan2(dx,dy)
	if(angle >= 0.785 && angle < 2.356)
//	-45 -> 45
		m_sFacing = EAST;
	if(angle >= 2.356 && angle < 3.927)
// 135 -> 225
		m_sFacing = WEST;
	if(angle > -0.785 && angle < 0.785)
// 255 -> 345
		m_sFacing = SOUTH;
	if(angle > 0.785 && angle < 2.356)
// 45 -> 135
		m_sFacing = NORTH;
		*/
}


bool Actor::isFacing(int x, int y)
{
	int dx = m_iPosX - x;
	int dy = m_iPosY - y;
	if( dx > 0 && m_sFacing == EAST)
		return true;
	else if( dx < 0 && m_sFacing == WEST)
		return true;
	else if( dy > 0 && m_sFacing == SOUTH)
		return true;
	else if( dy < 0 && m_sFacing == NORTH)
		return true;
	else
		return false;
}
void Actor::setTargetPos(int x, int y)
{
	m_sState = FIRING;
	m_iTargetPosX = x;
	m_iTargetPosY = y;
}
int Actor::getTargetPosX()
{
	return m_iTargetPosX;
}
int Actor::getTargetPosY()
{
	return m_iTargetPosY;
}
void Actor::clearTargetPos()
{
	m_sState = IDLE;
	m_iTargetPosX = -1;
	m_iTargetPosY = -1;
}

std::list<Impact>* Actor::getBulletImpacts()
{
	return m_lBulletImpacts;
}

void Actor::setPath(std::list<Point> *path)
{
	clearTargetPos();
	m_sState = MOVING;
	if(m_lPath)
		delete m_lPath;
	m_lPath = path;
}

std::list<Point> *Actor::getPath()
{
	return m_lPath;
}

void Actor::Kill()
{
	m_sState = DEAD;
	m_cGraphic = 'x';
}
int Actor::isDead()
{
		if(m_sState == DEAD)
			return 1;
		else
			return 0;
}

void Actor::Update(int frametime)
{
	static int f_iMoveSpeedCheck = m_iMoveSpeed;
	m_wWeapon->Update(frametime);
	if(m_sState == FIRING)
	{
		m_wWeapon->Fire();
		m_lBulletImpacts->clear();
		if(m_wWeapon->hasFired())
		{
// get beaten zone by rotating point
			int x = m_iTargetPosX + (rand()%9 - 4);
			int y = m_iTargetPosY + (rand()%3 - 1);
			int ox = m_iTargetPosX;
			int oy = m_iTargetPosY;
			int dx = m_iTargetPosX - getPosX();
			int dy = m_iTargetPosY - getPosY();
			float db = atan2(dx,dy) - 1.57;
			int rx = std::cos(db) * (x-ox) - std::sin(db) * (y-oy) + ox;
			int ry = std::sin(db) * (x-ox) + std::cos(db) * (y-oy) + oy;
			Impact f_iImpact = Impact(rx,ry);
			m_lBulletImpacts->push_front(f_iImpact);
		}
	}
	else
		m_wWeapon->CeaseFire();

	if(m_lPath && m_sState == MOVING)
	{
		f_iMoveSpeedCheck -= frametime;
		if(f_iMoveSpeedCheck < 0)
		{
			f_iMoveSpeedCheck = m_iMoveSpeed;
			if(!m_lPath->empty())
			{
				setPos((*m_lPath->begin()).X,(*m_lPath->begin()).Y);
				m_lPath->pop_front();
			}
		}
	}
}

bool Actor::Visible()
{
	return m_bVisible;
}
bool Actor::setVisible(bool draw)
{
		m_bVisible = draw;
}
