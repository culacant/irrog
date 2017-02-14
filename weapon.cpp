#include "weapon.h"

#include <iostream>

// Weapon Class
Weapon::Weapon(char *name, int magsize, int chambertime, int reloadtime, int aimtime, int burstcount, int supression, int supressionarea)
{
	m_iMagSize = magsize;

	m_iMagRemaining = m_iMagSize;

	m_iChamberTime = chambertime;
	m_iReloadTime = reloadtime;
	m_iAimTime = aimtime;
	m_iBurstCount = burstcount;

	m_iSupression = supression;
	m_iSupressionArea = supressionarea;
	
	m_pName = name;
}

void Weapon::Update(int frametime)
{
	static int f_iReloadSpeedCheck = m_iReloadTime;
	static int f_iChamberSpeedCheck = m_iChamberTime;
	static int f_iAimSpeedCheck = m_iAimTime;

	static int f_iBurstCountCheck = m_iBurstCount;

	if(m_iMagRemaining == 0)
	{
		Reload();
		f_iBurstCountCheck = m_iBurstCount;
	}

	if(m_bReloading)
	{
		f_iReloadSpeedCheck -= frametime;
		if(f_iReloadSpeedCheck < 0)
		{
			f_iReloadSpeedCheck = m_iReloadTime;
			m_iMagRemaining = m_iMagSize;
			m_bReloading = false;
		}
	}
	if(m_bAiming)
	{
		f_iAimSpeedCheck -= frametime;
		if(f_iAimSpeedCheck < 0)
		{
			f_iAimSpeedCheck = m_iAimTime;
			m_bAiming = false;
		}
	}
	if(m_bFiring && !m_bReloading && !m_bAiming)
	{
		f_iChamberSpeedCheck -= frametime;
		if(f_iChamberSpeedCheck < 0)
		{
			f_iChamberSpeedCheck = m_iChamberTime;
			m_iMagRemaining--;
			f_iBurstCountCheck--;
			if(f_iBurstCountCheck < 0)
			{
				f_iBurstCountCheck = m_iBurstCount;
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
