#include "SnowLauncher.h"
#include "Windows.h"

SnowLauncher::SnowLauncher()
{
	m_Projectiles.resize(m_max_sim_projectiles);
}

SnowLauncher::~SnowLauncher()
{
}

void SnowLauncher::Tick(float DeltaTime)
{
	

	if ((m_delay_time  - GetTickCount64() / 100)<= 0)b_can_shoot = true;
	else b_can_shoot = false;
 

}

void SnowLauncher::BeginPlay()
{

}

void SnowLauncher::SetShootDelay(float delay)
{
	m_shoot_delay = delay;
}

Projectile* SnowLauncher::Shoot(Vector3D projectile_direction)
{

	m_delay_time = GetTickCount64() / 100 + m_shoot_delay*10;

	if (m_projectile_shot_index == m_max_sim_projectiles)m_projectile_shot_index = 0;
	m_Projectiles[m_projectile_shot_index] = new Projectile(projectile_direction, m_projectile_speed);
    m_projectile_shot_index++;
	m_Projectiles[m_projectile_shot_index - 1]->SetMass(2);
   return m_Projectiles[m_projectile_shot_index-1];

}

void SnowLauncher::Aim()
{

}

bool SnowLauncher::GetCanShoot()
{
	return b_can_shoot;
}

int SnowLauncher::GetProjectileIndex()
{
	return m_projectile_shot_index;
}

Projectile* SnowLauncher::GetProjectileByIndex(int index)
{
	return m_Projectiles[index];
}

int SnowLauncher::GetMaxProjectiles()
{
	return m_max_sim_projectiles;
}

void SnowLauncher::deleteProjectileByIndex(int index)
{
	delete m_Projectiles[index];
	m_Projectiles[index] = nullptr;
}

