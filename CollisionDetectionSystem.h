#pragma once
#include "MathUtilities.h"

struct CollisionResult
{
	bool collided = false;
	Object* collider1 = nullptr;
	Object* collider2 = nullptr;
	Vector3D InsideDelta{}; // HOW MUCH DID THE BOUNDING BOXES INTERSECT
	
};
struct RayCastResult
{
	bool collided = false;
	Object* ray_owner = nullptr;
	Object* objectCollided = nullptr;
	Vector3D hit_point{};


};
struct BoundingBox
{
public:
	void SetOwner(Object* BBowner)
	{
		owner = BBowner;
	}
	Object* GetOwner()
	{
		return owner;
	}
	

	Vector3D center{};
	Vector3D offset{};
	float sizeX = 0, sizeY = 0, sizeZ = 0;

	Vector3D push_direction{};

	float Max_x=0, Min_x = 0,
		Max_y = 0, Min_y = 0,
		Max_z = 0, Min_z = 0;
	bool collision_enabled = true;


private:
	Object* owner = nullptr;


};

class CollisionDetectionSystem : public System
{
public:
	CollisionDetectionSystem()
	{

	}
	~CollisionDetectionSystem()
	{

	}
	void setECS(EntityComponentSystemManager* ECS)
	{
		m_EntityComponentSystemManager = ECS;
	}
	

	RayCastResult RayCast(Vector3D begin_point, Vector3D end_point,Object* owner)
	{
		RayCastResult RCRes;
		RCRes.collided = false;
        RCRes.ray_owner = owner;
		Vector3D min = begin_point;
		Vector3D max = end_point;

		if (begin_point.m_x > end_point.m_x)
		{
			min.m_x = end_point.m_x;
			max.m_x = begin_point.m_x;
		}
		if (begin_point.m_y > end_point.m_y)
		{
			min.m_y = end_point.m_y;
			max.m_y = begin_point.m_y;
		}
		if (begin_point.m_z > end_point.m_z)
		{
			min.m_z = end_point.m_z;
			max.m_z = begin_point.m_z;
		}
		
		for (auto const& entity : m_Entities)
		{
          auto& bb = m_EntityComponentSystemManager->GetComponent<BoundingBox>(entity);
		  if(entity != owner->GetEntityID() && (bb.sizeX!= 0 || bb.sizeY != 0 || bb.sizeZ != 0) )
		  {

			RayCastResult tempRes;
			tempRes.collided = false;
			
			tempRes = CheckRaytoBBCollision(begin_point, end_point, &bb);
			
			if(tempRes.collided 
				                && tempRes.hit_point.m_x> min.m_x && tempRes.hit_point.m_x < max.m_x
				                && tempRes.hit_point.m_y> min.m_y && tempRes.hit_point.m_y < max.m_y
				                && tempRes.hit_point.m_z> min.m_z && tempRes.hit_point.m_z < max.m_z)
			{ 
			   if (!RCRes.collided || sqaredDistance(begin_point, RCRes.hit_point) > sqaredDistance(begin_point, tempRes.hit_point))
			   {
				  
				RCRes.hit_point = tempRes.hit_point;
				RCRes.objectCollided = tempRes.objectCollided;

               }
			    RCRes.collided = true;
            }
			
          }
		

		}

        return RCRes;


	}
	CollisionResult SimpleBBCollisionCheck(BoundingBox& box1, BoundingBox& box2)
	{
		CollisionResult CRes;

		CRes.collided = (box1.Min_x <= box2.Max_x && box1.Max_x >= box2.Min_x) &&
			(box1.Min_y <= box2.Max_y && box1.Max_y >= box2.Min_y) &&
			(box1.Min_z <= box2.Max_z && box1.Max_z >= box2.Min_z);

		if (CRes.collided)
		{
			CRes.collider1 = box1.GetOwner();
			CRes.collider2 = box2.GetOwner();
			
		}
		return CRes;
	}

	CollisionResult CheckBBtoBBCollision(BoundingBox& box1,BoundingBox& box2)
	{
		CollisionResult CRes;
		CRes.InsideDelta = Vector3D(0, 0, 0);
		CRes.collided = (box1.Min_x <= box2.Max_x && box1.Max_x >= box2.Min_x) &&
			            (box1.Min_y <= box2.Max_y && box1.Max_y >= box2.Min_y) &&
			            (box1.Min_z <= box2.Max_z && box1.Max_z >= box2.Min_z);


		if (CRes.collided)
		{

			CRes.collider1 = box1.GetOwner();
			CRes.collider2 = box2.GetOwner();
			float distance_centers_x = abs(box2.center.m_x - box1.center.m_x);
			float distance_centers_y = abs(box2.center.m_y - box1.center.m_y);
			float distance_centers_z = abs(box2.center.m_z - box1.center.m_z);

			bool pushingX= distance_centers_x > abs((box1.sizeX / 2) - (box2.sizeX / 2));
			bool pushingY = distance_centers_y > abs((box1.sizeY / 2) - (box2.sizeY / 2));
			bool pushingZ = distance_centers_z > abs((box1.sizeZ / 2) - (box2.sizeZ / 2));
			

			Vector3D delta;

			delta.m_x = box2.Min_x - box1.Max_x;
			if (abs(box2.Max_x - box1.Min_x) < abs(box2.Min_x - box1.Max_x))
			delta.m_x = box2.Max_x - box1.Min_x;

			delta.m_y = box2.Min_y - box1.Max_y;
			if (abs(box2.Max_y - box1.Min_y) < abs(box2.Min_y - box1.Max_y))
			delta.m_y = box2.Max_y - box1.Min_y;

			delta.m_z = box2.Min_z - box1.Max_z;
			if (abs(box2.Max_z - box1.Min_z) < abs(box2.Min_z - box1.Max_z))
			delta.m_z = box2.Max_z - box1.Min_z;


			if (abs(delta.m_y) > abs(delta.m_x))pushingY = false;
			else pushingX = false;
			if (abs(delta.m_z) > abs(delta.m_y))pushingZ = false;
			else pushingY = false;
			if (abs(delta.m_z) > abs(delta.m_x))pushingZ = false;
			else pushingX = false;

			box1.push_direction.m_x = pushingX* (box2.center.m_x - box1.center.m_x) / abs(box1.center.m_x - box2.center.m_x);
			box1.push_direction.m_y = pushingY * (box2.center.m_y - box1.center.m_y) / abs(box1.center.m_y - box2.center.m_y);
			box1.push_direction.m_z = pushingZ * (box2.center.m_z - box1.center.m_z) / abs(box1.center.m_z - box2.center.m_z);

			box2.push_direction.m_x = pushingX * (box1.center.m_x - box2.center.m_x) / abs(box1.center.m_x - box2.center.m_x);
			box2.push_direction.m_y = pushingY * (box1.center.m_y - box2.center.m_y) / abs(box1.center.m_y - box2.center.m_y);
			box2.push_direction.m_z = pushingZ * (box1.center.m_z - box2.center.m_z) / abs(box1.center.m_z - box2.center.m_z);

			
			if(pushingX)
			{ 
				CRes.InsideDelta.m_x = delta.m_x;

            }
			
			if (pushingY)
			{
				CRes.InsideDelta.m_y = delta.m_y;

			}

			if (pushingZ)
			{
				CRes.InsideDelta.m_z = delta.m_z;

			}
			
		}
            
   

		return CRes;
		
	}
	

private:
	EntityComponentSystemManager* m_EntityComponentSystemManager = nullptr;

	RayCastResult CheckRaytoBBCollision(Vector3D begin_point, Vector3D end_point,BoundingBox* box )
	{

		RayCastResult RayRes;
		Vector3D intersection_point{};

		float line_Max_x;
		if (begin_point.m_x > end_point.m_x)line_Max_x = begin_point.m_x;
		else line_Max_x = end_point.m_x;

		float line_Min_x;
		if (begin_point.m_x < end_point.m_x)line_Min_x = begin_point.m_x;
		else line_Min_x = end_point.m_x;

		float line_Max_y;
		if (begin_point.m_y > end_point.m_y)line_Max_y = begin_point.m_y;
		else line_Max_y = end_point.m_y;

		float line_Min_y;
		if (begin_point.m_y < end_point.m_y)line_Min_y = begin_point.m_y;
		else line_Min_y = end_point.m_y;

		float line_Max_z;
		if (begin_point.m_z > end_point.m_z)line_Max_z = begin_point.m_z;
		else line_Max_z = end_point.m_z;

		float line_Min_z;
		if (begin_point.m_z < end_point.m_z)line_Min_z = begin_point.m_z;
		else line_Min_z = end_point.m_z;

		BoundingBox boxplm;
		boxplm.Max_x = line_Max_x;
		boxplm.Min_x = line_Min_x;
		boxplm.Max_y = line_Max_y;
		boxplm.Min_y = line_Min_y;
		boxplm.Max_z = line_Max_z;
		boxplm.Min_z = line_Min_z;

		
		//CHECK MAX X AXIS PLANE
		intersection_point.m_x = box->Max_x;


		intersection_point.m_y = (intersection_point.m_x - begin_point.m_x) * (end_point.m_y - begin_point.m_y) /
			(end_point.m_x - begin_point.m_x)+ begin_point.m_y;

		intersection_point.m_z = (intersection_point.m_x - begin_point.m_x) * (end_point.m_z - begin_point.m_z) /
			(end_point.m_x - begin_point.m_x)+ begin_point.m_z;

		if (intersection_point.m_z > box->Min_z && intersection_point.m_z < box->Max_z && intersection_point.m_y > box->Min_y && intersection_point.m_y < box->Max_y)
		{   
			RayRes.hit_point = intersection_point;
			RayRes.collided = true;
			RayRes.objectCollided = box->GetOwner();

		}
		//CHECK MIN X AXIS PLANE
		intersection_point.m_x = box->Min_x;

        
		intersection_point.m_y = (intersection_point.m_x - begin_point.m_x) * (end_point.m_y - begin_point.m_y) /
			(end_point.m_x - begin_point.m_x) + begin_point.m_y;
		intersection_point.m_z = (intersection_point.m_x - begin_point.m_x) * (end_point.m_z - begin_point.m_z) /
			(end_point.m_x - begin_point.m_x) + begin_point.m_z;

		if (intersection_point.m_z > box->Min_z && intersection_point.m_z < box->Max_z && intersection_point.m_y > box->Min_y && intersection_point.m_y < box->Max_y)
		{
			
			
			if(!RayRes.collided ||(sqaredDistance(begin_point, RayRes.hit_point)> sqaredDistance(begin_point, intersection_point)))
			{ 
			RayRes.hit_point = intersection_point;
            RayRes.collided = true;
            }
			RayRes.objectCollided = box->GetOwner();

		}
	
		//CHECK MAX Y AXIS PLANE
		intersection_point.m_y = box->Max_y;


        intersection_point.m_x = (intersection_point.m_y - begin_point.m_y) * (end_point.m_x - begin_point.m_x) /
			(end_point.m_y - begin_point.m_y) + begin_point.m_x;
		intersection_point.m_z = (intersection_point.m_y - begin_point.m_y) * (end_point.m_z - begin_point.m_z) /
			(end_point.m_y - begin_point.m_y) + begin_point.m_z;

		

		if (intersection_point.m_z > box->Min_z && intersection_point.m_z < box->Max_z && intersection_point.m_x > box->Min_x && intersection_point.m_x < box->Max_x)
		{
			
			if (!RayRes.collided || (sqaredDistance(begin_point, RayRes.hit_point) > sqaredDistance(begin_point, intersection_point)))
			{ 
			RayRes.hit_point = intersection_point;
            RayRes.collided = true;
            }
			RayRes.objectCollided = box->GetOwner();

		}
		//CHECK MIN Y AXIS PLANE
		intersection_point.m_y = box->Min_y;


		intersection_point.m_x = (intersection_point.m_y - begin_point.m_y) * (end_point.m_x - begin_point.m_x) /
			(end_point.m_y - begin_point.m_y) + begin_point.m_x;
		intersection_point.m_z = (intersection_point.m_y - begin_point.m_y) * (end_point.m_z - begin_point.m_z) /
			(end_point.m_y - begin_point.m_y) + begin_point.m_z;


		if (intersection_point.m_z > box->Min_z && intersection_point.m_z < box->Max_z && intersection_point.m_x > box->Min_x && intersection_point.m_x < box->Max_x)
		{

			if (!RayRes.collided || (sqaredDistance(begin_point, RayRes.hit_point) > sqaredDistance(begin_point, intersection_point)))
			{ 
			RayRes.hit_point = intersection_point;
			RayRes.collided = true;
            }
			RayRes.objectCollided = box->GetOwner();

		}

		//CHECK MAX Z AXIS PLANE
		intersection_point.m_z = box->Max_z;

		intersection_point.m_x = (intersection_point.m_z - begin_point.m_z) * (end_point.m_x - begin_point.m_x) /
			(end_point.m_z - begin_point.m_z) + begin_point.m_x;
		intersection_point.m_y = (intersection_point.m_z - begin_point.m_z) * (end_point.m_y - begin_point.m_y) /
			(end_point.m_z - begin_point.m_z) + begin_point.m_y;

		if (intersection_point.m_x > box->Min_x && intersection_point.m_x < box->Max_x && intersection_point.m_y > box->Min_y && intersection_point.m_y < box->Max_y)
		{

			if (!RayRes.collided || (sqaredDistance(begin_point, RayRes.hit_point) > sqaredDistance(begin_point, intersection_point)))
			RayRes.hit_point = intersection_point;
			RayRes.collided = true;
			RayRes.objectCollided = box->GetOwner();

		}
		//CHECK MIN Z AXIS PLANE
		intersection_point.m_z = box->Min_z;

		intersection_point.m_x = (intersection_point.m_z - begin_point.m_z) * (end_point.m_x - begin_point.m_x) /
			(end_point.m_z - begin_point.m_z) + begin_point.m_x;
		intersection_point.m_y = (intersection_point.m_z - begin_point.m_z) * (end_point.m_y - begin_point.m_y) /
			(end_point.m_z - begin_point.m_z) + begin_point.m_y;

		if (intersection_point.m_x > box->Min_x && intersection_point.m_x < box->Max_x && intersection_point.m_y > box->Min_y && intersection_point.m_y < box->Max_y)
		{

			if (!RayRes.collided || (sqaredDistance(begin_point, RayRes.hit_point) > sqaredDistance(begin_point, intersection_point)))
			RayRes.hit_point = intersection_point;
			RayRes.collided = true;
			RayRes.objectCollided = box->GetOwner();

		}

		return RayRes;
	}
	



};