#include "Projectile.h"
#include "Windows.h"

Projectile::Projectile(Vector3D impulse_direction , float speed): m_impulse_direction(impulse_direction), m_speed(speed)
{
	//Register the time of the creation
	m_init_time = GetTickCount64()/1000;

	b_projectile_dead = false;
}

Projectile::~Projectile()
{

}

void Projectile::Tick(float DeltaTime)
{
	auto& transform = m_EntityComponentSystemManager->GetComponent<TransformComponent>(m_ID);
	Vector3D temp;
	//calculate the impulse direction(that is set as a parameter in the constructor)
	temp = transform.position+ m_impulse_direction * DeltaTime * m_speed;
	
	transform.position = temp;
	//if the life time runs out, set projectile to be dead
	if (GetTickCount64() / 1000 == m_init_time + m_life_time)
	{
			b_projectile_dead = true;
	}
		
}


void Projectile::BeginPlay()
{

}

float Projectile::GetLifeTime()
{
	return m_life_time;
}

bool Projectile::GetProjectileDead()
{
	return b_projectile_dead;
}

void Projectile::SetDamageAmount(float damage)
{
	m_damage = damage;
}

float Projectile::GetDamageAmount()
{
	return m_damage;
}
