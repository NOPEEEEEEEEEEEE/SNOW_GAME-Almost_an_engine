#pragma once
#include "Object.h"
#include "CollisionDetectionSystem.h"
#include <iostream>
#include "Point.h"
#include "Rotator3D.h"
#include "Prerequisites.h"


class Socket;

struct TransformComponent
{
	Vector3D position{};
	Rotator3D rotation{};
	Vector3D scale{};
};
struct MeshComponent
{
	MeshPtr mesh{};
	MaterialPtr material{};

	bool visible = true;
	float texture_alpha = 1;
};
struct PhysicsComponent
{
	Vector3D gravitational_acceleration = Vector3D(0,-9.8f,0);
	Vector3D velocity = Vector3D(0, 0, 0);

};
struct HealthComponent
{
public:
	float max_health = 100;
	float health = max_health;

	void DealDamange(float damage)
	{
		health = health - damage;

		if (health < 0)health = 0;

	}

};
class CollisionDetectionSystem;

class Actor: public Object
{
public:
	Actor()
	{

	}

	~Actor()
	{

	}
	
	virtual void Tick(float DeltaTime) override
	{
		Object::Tick(DeltaTime);
	}
	virtual void BeginPlay() override
	{
		Object::BeginPlay();
	
	}
	RayCastResult RayCast(Vector3D begin_point, Vector3D end_point)
	{
		RayCastResult RCRes;
		if(m_collision_system)
		RCRes = m_collision_system->RayCast(begin_point, end_point, this);
		//if (RCRes.collided)std::cout << "hit";
		//else std::cout << "miss";
		return RCRes;
	}
	void SetBoundingBoxSize(Vector3D size)
	{
		BBsize = size;
	}
	Vector3D GetBoundingBoxSize()
	{
		return BBsize;
	}
	Vector3D GetPosition()
	{
		auto& pos = m_EntityComponentSystemManager->GetComponent<TransformComponent>(m_ID);
		return pos.position;
	}
	void SetCollisionSystem(std::shared_ptr<CollisionDetectionSystem> system)
	{
		m_collision_system = system;
	}
	void AttachTo(Socket* socket_to_attach_to)
	{
		m_socket_to_attach_to = socket_to_attach_to;
	}
	Socket* GetSocketAttachment()
	{
		return m_socket_to_attach_to;
	}
	void SetMeshVisibility(bool visibility)
	{
		auto& mesh = m_EntityComponentSystemManager->GetComponent<MeshComponent>(m_ID);
		mesh.visible = visibility;
	}
	void SetCollision(bool colision)
	{
		auto& bb = m_EntityComponentSystemManager->GetComponent<BoundingBox>(m_ID);
		bb.collision_enabled = colision;

	}
	void SetGravity(bool gravity)
	{
		m_gravity = gravity;
	}
	bool GetGravity()
	{
		return m_gravity;
	}
	void SetMass(float mass)
	{
		m_mass = mass;
	}
	float GetMass()
	{
		return m_mass;
	}
	bool GetIsGrounded()
	{
		return m_is_grounded;
	}
	void SetIsGrounded(bool is_grounded)
	{
		m_is_grounded = is_grounded;
	}
	bool GetHasHealth()
	{
		return m_has_health;
	}
	void SetHasHealth(bool has_health)
	{
		m_has_health = has_health;
	}
	void SetMaxHealth(float max_health)
	{
		auto& health = m_EntityComponentSystemManager->GetComponent<HealthComponent>(m_ID);
		health.max_health = max_health;
	}


protected:

	bool m_gravity = true;
	float m_mass = 1;
	bool m_is_grounded = false;
	bool m_has_health = false;
private:
	Vector3D BBsize;
	Socket* m_socket_to_attach_to = nullptr;
	std::shared_ptr<CollisionDetectionSystem> m_collision_system;
	


friend class CollisionDetectionSystem;
};