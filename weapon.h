// Weapon Class

class Weapon
{
private:
	int m_iMagSize;
	int m_iChamberTime;
	int m_iReloadTime;
	int m_iBurstCount;

	int m_iAimTime;
	int m_iDefaultAimTime;

	int m_iMagRemaining;
	bool m_bReloading;
	bool m_bAiming;
	bool m_bFiring;
	bool m_bHasFired;

	int m_iSupression;
	int m_iSupressionArea;

	char *m_pName;

	int m_iReloadSpeedCheck;
	int m_iChamberSpeedCheck;
	int m_iAimSpeedCheck;
	int m_iBurstCountCheck;
public:
	Weapon(char *name, int magsize, int chambertime, int reloadtime, int aimtime, int burstcount, int supression, int supressionarea);
	void Update(int frametime);

	void Reload();
	void Fire();
	void CeaseFire();
	bool hasFired();

	char *getName();

	int getSupression();
	int getSupressionArea();

	void setAimDelay(int aimdelay);
	void ResetAimDelay();
};
