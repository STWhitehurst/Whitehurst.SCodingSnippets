/**********************************************************************
*Filename:								CPhysicsManager.h
*Date:									02/16/2019
*Mod. Date:								02/16/2019
*Mod. Initials:							STW
*Author:								Sydney Whitehurst
*Purpose:								Manages how objects interact within the world.
*										This class will manage all of the math and logic
*										functionalities that maintain and alter the position
*										and velocity of objects. This class will also do collision
*										checking and reactions appropriately. This is the base to
*										apply our gravity physics for the feature of the game.
*										Player and ball terrain clamping are controlled here as
*										well.This supports AABBs and BVH, with capsule collision.
*
************************************************************************/
#pragma once
#include "cPhysicsComponent.h"
#include "cBallPhysComponent.h"
#include "cCharacterPhysComponent.h"
#include "cMobileMeshPhysComponent.h"
#include "cStaticMeshPhysComponent.h"
#include "ColliderTypes.h"
#include <vector>
#include <array>
#define GRAVITYBASE 7.0f
using namespace std;

//#if _DEBUG
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)
//#endif

namespace CPhysicsManager
{
	class CPhysicsManager
	{
	protected:
		vector<cPhysicsComponent*> componentContainer;
		
	public:
		CPhysicsManager();
		~CPhysicsManager();

		bool m_bPlanetoidColliderStatus = false;
		end::TSphere m_tPlanetoidCollider;
		end::TPlane testplane;
		end::TWall testwall;

		/**********************************************************************
		*CreatePhysComponent():					This function will create and
		*										add a new physics component to
		*										the container of registered objects
		*										in the physics manager.
		*
		*Ins:									cPhysicsComponent*
		*
		*Outs:									N/A
		*
		*Returns:								void
		*
		*Mod. Date:								02/16/2019
		*Mod. Initials:							STW
		************************************************************************/
		void CreatePhysComponent(cPhysicsComponent* physicsComponent);

		/**********************************************************************
		*CreatePhysComponent():					This function will remove
		*										the physics component from
		*										the container in the physics manager.
		*
		*Ins:									cPhysicsComponent*
		*
		*Outs:									N/A
		*
		*Returns:								void
		*
		*Mod. Date:								02/16/2019
		*Mod. Initials:							STW
		************************************************************************/
		void RemovePhysComponent(cPhysicsComponent* physicsComponent);

		/**********************************************************************
		*Update():								This function will perform
		*										positional and collision updates
		*										for all registered objects
		*
		*Ins:									float fT
		*
		*Outs:									N/A
		*
		*Returns:								void
		*
		*Mod. Date:								02/16/2019
		*Mod. Initials:							STW
		************************************************************************/
		void Update(float fT);

		/**********************************************************************
		*GetColliders():						Returns the colliders Status
		*
		*Ins:									N/A
		*
		*Outs:									N/A
		*
		*Returns:								bool
		*
		*Mod. Date:								03/17/2019
		*Mod. Initials:							STW
		************************************************************************/
		bool GetCollidersStatus();

		/**********************************************************************
		*SetColliders():						Turns the colliders on or off
		*
		*Ins:									bool
		*
		*Outs:									N/A
		*
		*Returns:								void
		*
		*Mod. Date:								03/17/2019
		*Mod. Initials:							STW
		************************************************************************/
		void SetColliders(bool bStatus);
	};
}

