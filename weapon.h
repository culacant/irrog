// Weapon Class

class Weapon
{
private:
	int m_iMagSize;
	int m_iChamberTime;
	int m_iReloadTime;
	int m_iAimTime;
	int m_iBurstCount;

	int m_iMagRemaining;
	bool m_bReloading;
	bool m_bAiming;
	bool m_bFiring;
	bool m_bHasFired;
public:
	Weapon(int magsize, int chambertime, int reloadtime, int aimtime, int burstcount);
	void Update(int frametime);

	void Reload();
	void Fire();
	void CeaseFire();
	bool hasFired();
};
