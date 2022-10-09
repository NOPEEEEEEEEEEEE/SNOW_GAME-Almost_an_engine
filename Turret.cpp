#include "Turret.h"

Turret::Turret() 
{
	m_Projectiles.resize(m_max_sim_projectiles);
}
Turret::Turret(Entity target_ID): m_target_ID(target_ID)
{
	m_Projectiles.resize(m_max_sim_projectiles);
}

Turret::~Turret()
{
}

void Turret::Tick(float DeltaTime)
{

	if ((m_delay_time - GetTickCount64() / 100) <= 0)b_can_shoot = true;
	else b_can_shoot = false;

	auto& target_transform = m_EntityComponentSystemManager->GetComponent<TransformComponent>(m_target_ID);
	auto& transform = m_EntityComponentSystemManager->GetComponent<TransformComponent>(m_ID);

	if (sqaredDistance(target_transform.position, transform.position) < m_range* m_range)
	{
		Vector3D turret_oritentation_vector;
		turret_oritentation_vector = target_transform.position - transform.position + Vector3D(0,10,0);

		turret_oritentation_vector = Vector3D::normalize(turret_oritentation_vector);


		float rotation_Y = float(atan2(turret_oritentation_vector.m_x, turret_oritentation_vector.m_z) * 180 / 3.14);

		float rotation_X = float(-atan2(turret_oritentation_vector.m_y, sqrt(pow(turret_oritentation_vector.m_z, 2) + pow(turret_oritentation_vector.m_x, 2))) * 180 / 3.14);

		transform.rotation = Rotator3D(rotation_X, rotation_Y, 0);

		b_is_in_range = true;
		
	}
	else
	{
		b_is_in_range = false;
	}

}

void Turret::BeginPlay()
{
}
void Turret::SetShootFrequency(float frequency)
{
	m_shootin_frequency = frequency;
}

Projectile* Turret::Shoot()
{
	auto& transform = m_EntityComponentSystemManager->GetComponent<TransformComponent>(m_ID);

	m_delay_time = GetTickCount64() / 100 + m_shootin_frequency * 10;

	if (m_projectile_shot_index == m_max_sim_projectiles)m_projectile_shot_index = 0;

	Vector3D m_shoot_direction;
	m_shoot_direction.m_x = sin(transform.rotation.GetRadianValue().m_y);
	m_shoot_direction.m_z = cos(transform.rotation.GetRadianValue().m_y);
	m_shoot_direction.m_y = -sin(transform.rotation.GetRadianValue().m_x);
	m_Projectiles[m_projectile_shot_index] = new Projectile(m_shoot_direction, m_projectile_speed);

	m_projectile_shot_index++;
	m_projectiles_in_existance++;


	return m_Projectiles[m_projectile_shot_index - 1];

}

Projectile* Turret::Shoot(Vector3D projectile_direction)
{
	if (m_projectile_shot_index == m_max_sim_projectiles)m_projectile_shot_index = 0;
	m_Projectiles[m_projectile_shot_index] = new Projectile(projectile_direction, m_projectile_speed);
	m_projectile_shot_index++;
	m_projectiles_in_existance++;
	return m_Projectiles[m_projectile_shot_index - 1];

}
Actor* Turret::GetTower()
{
	return m_tower;
}

void Turret::deleteTower()
{
	delete m_tower;
	m_tower = nullptr;
}

int Turret::GetProjInExistance()
{
	return m_projectiles_in_existance;
}

void Turret::SetDisabled(bool disabled)
{
	b_disabled = disabled;
}

bool Turret::GetDisabled()
{
	return b_disabled;
}

void Turret::Aim()
{

}

bool Turret::GetCanShoot()
{
	return b_can_shoot;
}

bool Turret::GetIsInRange()
{
	return b_is_in_range;
}

int Turret::GetProjectileIndex()
{
	return m_projectile_shot_index;
}

Projectile* Turret::GetProjectileByIndex(int index)
{
	return m_Projectiles[index];
}

int Turret::GetMaxProjectiles()
{
	return m_max_sim_projectiles;
}
void Turret::SetTargetID(Entity target_ID)
{
	m_target_ID = target_ID;
}
void Turret::deleteProjectileByIndex(int index)
{
	//std::cout << "deleted"<< index<<std::endl;
    m_projectiles_in_existance--;
	delete m_Projectiles[index];
	m_Projectiles[index] = nullptr;
	
}
