#pragma once
//#include "Actor.h"
//#include "Matrix4x4.h"
//#include "Object.h"

class Socket: public Object
{

public:
	Socket(Entity actor_attached_to_ID,Vector3D rel_pos, Rotator3D rel_rot_to_actor)
	{
		m_actor_attached_to_ID = actor_attached_to_ID;
		m_rel_pos_to_actor = rel_pos;
		m_rel_rot_to_actor = rel_rot_to_actor;
	}

	~Socket()
	{

	}

	virtual void Tick(float DeltaTime) override
	{
		UpdateSocket(DeltaTime);
	}
	virtual void BeginPlay() override
	{
		

	}
	void UpdateSocket(float DeltaTime)
	{
     auto & transform = m_EntityComponentSystemManager->GetComponent<TransformComponent>(m_actor_attached_to_ID);
	 Matrix4x4 world_pos, temp;
	 world_pos.setIdentity();

	 temp.setIdentity();
	 temp.setRotationRadiansX(transform.rotation.GetRadianValue().m_x);
	 world_pos *= temp;

	 temp.setIdentity();
	 temp.setRotationRadiansY(transform.rotation.GetRadianValue().m_y);
	 world_pos *= temp;

	// std::cout << world_cam.getXDirection().m_x << " " << world_cam.getXDirection().m_y << " " << world_cam.getXDirection().m_z << std::endl;
	 if(b_inherit_position)
	 m_position = transform.position + world_pos.getZDirection() * m_rel_pos_to_actor.m_z
		                             + world_pos.getYDirection() * m_rel_pos_to_actor.m_y
		                             + world_pos.getXDirection() * m_rel_pos_to_actor.m_x;

	 if (b_inherit_rotation)
		 m_rotation = transform.rotation;


	}
	Vector3D GetPosition()
	{
		return m_position;
	}
	Rotator3D GetRotation()
	{
		return m_rotation;
	}
	void SetInheritPosition(bool inherit)
	{
		b_inherit_position = inherit;
	}
	void SetInheritRotation(bool inherit)
	{
		b_inherit_rotation = inherit;
	}

private:
	Vector3D m_position;
	Rotator3D m_rotation;

	bool b_inherit_position = true;
	bool b_inherit_rotation = true;

	Vector3D m_rel_pos_to_actor;
	Rotator3D m_rel_rot_to_actor;

	Entity m_actor_attached_to_ID;


};