#pragma once
#include "EntityComponentSystemManager.h"
#include "Vector3D.h"
#include "Point.h"
#include "HUD.h"
#include <iostream>

struct TransformComponent2D
{
	Point position;
	Point scale;


};

extern EntityComponentSystemManager m_EntityComponentSystemManager;

class HUDWidget
{
public:
	HUDWidget()
	{
		
	}
	~HUDWidget()
	{

	}

	virtual void Tick(float DeltaTime)
	{
		//std::cout << DeltaTime;

	}
	virtual void BeginPlay()
	{
		
	}
	
	void SetEntityID(Entity IDToSet)
	{
		m_ID = IDToSet;
	}

	Entity GetEntityID()
	{
		return m_ID;
	}
	void SetName(const char Name[32])
	{
		memcpy(m_name, Name, sizeof(const char));

	}

	char GetName()
	{
		return m_ID;
	}
	void SetVisibility(bool visible)
	{
		m_visible = visible;
	}
	void SetOffset(WidgetPositionOffset offset)
	{
		m_offset = offset;
	}
	void SetPosition(Point widget_position)
	{
		auto& transform = m_EntityComponentSystemManager->GetComponent<TransformComponent2D>(m_ID);
		transform.position = widget_position;
	}
	void SetScale(Point widget_scale)
	{
		auto& transform = m_EntityComponentSystemManager->GetComponent<TransformComponent2D>(m_ID);
		transform.scale = widget_scale;
	}
	void setECS(EntityComponentSystemManager* ECS)
	{
		m_EntityComponentSystemManager = ECS;
	}

protected:
	Entity m_ID{};
	char m_name[32]{};
	WidgetPositionOffset m_offset = WidgetPositionOffset::Center;
	bool m_visible= true;
	EntityComponentSystemManager* m_EntityComponentSystemManager= nullptr;

};