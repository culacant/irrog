#include "weapon.h"

#include <iostream>

// Weapon Class
Weapon::Weapon(char *name, int magsize, int chambertime, int reloadtime, int aimtime, int burstcount, int supression, int supressionarea)
{
	m_iMagSize = magsize;

	m_iMagRemaining = m_iMagSize;

	m_iChamberTime = chambertime;
	m_iReloadTime = reloadtime;
	m_iBurstCount = burstcount;

	m_iDefaultAimTime = aimtime;
	m_iAimTime = m_iDefaultAimTime;

	m_iSupression = supression;
	m_iSupressionArea = supressionarea;
	
	m_pName = name;

	int m_iReloadSpeedCheck = m_iReloadTime;
	int m_iChamberSpeedCheck = m_iChamberTime;
	int m_iAimSpeedCheck = m_iAimTime;
	int m_iBurstCountCheck = m_iBurstCount;
}

void Weapon::Update(int frametime)
{
	if(m_iMagRemaining == 0)
	{
		Reload();
		m_iBurstCountCheck = m_iBurstCount;
	}

	if(m_bReloading)
	{
		m_iReloadSpeedCheck -= frametime;
		if(m_iReloadSpeedCheck < 0)
		{
			m_iReloadSpeedCheck = m_iReloadTime;
			m_iMagRemaining = m_iMagSize;
			m_bReloading = false;
		}
	}
	if(m_bAiming)
	{
		m_iAimSpeedCheck -= frametime;
		if(m_iAimSpeedCheck < 0)
		{
			m_iAimSpeedCheck = m_iAimTime;
			m_bAiming = false;
		}
	}
	if(m_bFiring && !m_bReloading && !m_bAiming)
	{
		m_iChamberSpeedCheck -= frametime;
		if(m_iChamberSpeedCheck < 0)
		{
			m_iChamberSpeedCheck = m_iChamberTime;
			m_iMagRemaining--;
			m_iBurstCountCheck--;
			if(m_iBurstCountCheck < 0)
			{
				m_iBurstCountCheck = m_iBurstCount;
				m_bAiming = true;
			}
			m_bHasFired = true;
		}
	}
}

void Weapon::Reload()
{
	m_bReloading = true;
}
void Weapon::Fire()
{
	m_bFiring = true;
}
void Weapon::CeaseFire()
{
	m_bFiring = false;
}
bool Weapon::hasFired()
{
	if(m_bHasFired)
	{
		m_bHasFired = false;
		return true;
	}
	else
		return false;
}

int Weapon::getSupression()
{
	return m_iSupression;
}
int Weapon::getSupressionArea()
{
	return m_iSupressionArea;
}

char* Weapon::getName()
{
	return m_pName;
}

void Weapon::setAimDelay(int aimdelay)
{
	m_iAimTime = aimdelay + m_iDefaultAimTime;
}
void Weapon::ResetAimDelay()
{
	m_iAimTime = m_iDefaultAimTime;
}
