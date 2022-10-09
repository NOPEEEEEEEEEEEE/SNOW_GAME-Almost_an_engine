#pragma once
#include "Pawn.h"
#include "Projectile.h"

class Turret : public Pawn
{
public:
	Turret();
	Turret(Entity target_ID);
	~Turret();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	void SetShootFrequency(float frequency);

	Projectile* Shoot();
	Projectile* Shoot(Vector3D projectile_direction);
	void Aim();

	bool GetCanShoot();
	bool GetIsInRange();
	//void SetProjectileIndex(float index);
	int GetProjectileIndex();

	Projectile* GetProjectileByIndex(int index);
	int GetMaxProjectiles();

	void SetTargetID(Entity target_ID);

	void deleteProjectileByIndex(int index);
    Actor* GetTower();
	void deleteTower();

	int GetProjInExistance();

	void SetDisabled(bool disabled);
	bool GetDisabled();

private:

	//Turret constants
	Entity m_target_ID{};
	int m_max_sim_projectiles = 30;
	float m_projectile_speed = 70;
	float m_shootin_frequency = 2;
	Actor* m_tower = new Actor;
	float m_range = 400;


	//variables
	bool b_is_in_range = false;
	bool b_can_shoot = true;
	bool b_disabled = false;
	float m_delay_time = 0;
	std::vector<Projectile*> m_Projectiles;
	int m_projectile_shot_index = 0;
	int m_projectiles_in_existance=0;

};

