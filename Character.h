#pragma once
#include "Pawn.h"
#include <iostream>


struct CameraComponent
{
	Vector3D position{};
	Rotator3D rotation{};
	Vector3D scale{};
	Vector3D offset_pos{};
	Vector3D relative_pos_to_cht{};
	float FOV=0;
	float aspect = 0;
	float near_plane = 0;
	float far_plane = 0;
};

class Character: public Pawn
{
public:
	Character()
	{

	}
	
	~Character()
	{


	}
	/*void SetPosition(Vector3D position)
	{
		m_position = position;
	}
	Vector3D GetPosition()
	{
		return m_position;
	}*/
	virtual void Tick(float DeltaTime) override
	{

		Pawn::Tick(DeltaTime);

	}

	virtual void BeginPlay() override
	{
		Pawn::BeginPlay();
		//std::cout << "Character" << std::endl;

	}

protected:


private:
	//Vector3D m_position;

};