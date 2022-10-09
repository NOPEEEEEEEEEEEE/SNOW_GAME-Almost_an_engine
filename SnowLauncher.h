#pragma once
#include "Actor.h"
#include "Projectile.h"
class SnowLauncher: public Actor
{
public:
	SnowLauncher();
	~SnowLauncher();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	void SetShootDelay(float delay);

	Projectile* Shoot(Vector3D projectile_direction);
	void Aim();

	bool GetCanShoot();
	//void SetProjectileIndex(float index);
	int GetProjectileIndex();

	Projectile* GetProjectileByIndex(int index);
	int GetMaxProjectiles();

	void deleteProjectileByIndex(int index);

private:

	//Weapon constants
	float m_shoot_delay = 0.5 ;
	int m_max_sim_projectiles = 40;
	float m_projectile_speed = 70;


	//variables
    bool b_can_shoot = true;
	float m_delay_time = 0;
	std::vector<Projectile*> m_Projectiles;
	int m_projectile_shot_index = 0;


};

