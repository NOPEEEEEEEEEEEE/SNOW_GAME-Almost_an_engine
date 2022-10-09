#pragma once
#include "Actor.h"
#include "Vector3D.h"


class Projectile:public Actor
{
public:
	Projectile(Vector3D impulse_direction, float speed);
	~Projectile();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	float GetLifeTime();
	bool GetProjectileDead();

   void SetDamageAmount(float damage);
	float GetDamageAmount();


private:
	Vector3D m_impulse_direction;
	float m_speed;
    float m_life_time = 6;
	float m_damage = 20;


	bool b_projectile_dead = false;
	
	float m_init_time;

};

