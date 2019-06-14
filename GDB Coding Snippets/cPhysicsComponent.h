/**********************************************************************
*Filename:								cPhysicsComponent.h
*Date:									02/16/2019
*Mod. Date:								02/16/2019
*Mod. Initials:							STW
*Author:								Sydney Whitehurst
*Purpose:								An abstract base class for all
*										physics components.
************************************************************************/

#pragma once
#include <d3d11_2.h>
#include <DirectXMath.h>
#include <vector>
#include "ColliderTypes.h"
using namespace DirectX;

namespace CPhysicsManager
{
	enum ComponentType {eBall, eCharacter, eMobileMesh, eStaticMesh};
	class cPhysicsComponent
	{
	protected:
		XMMATRIX			m_worldTransform;
		XMVECTOR			m_velocity;
		XMVECTOR			m_gravity;
		XMVECTOR			m_up;
		ComponentType		m_eComponentType;
		bool				m_bIsThrown = false;
		bool				m_bActivated = false;
		bool				m_bDrawColliders = false;
		bool				m_bOnWall = false;
		float				m_fOnWallRadius;

	public:
		cPhysicsComponent();
		~cPhysicsComponent();


		/**********************************************************************
		*GetWorldTransform():					Returns the component's 
		*										WorldTransform. This represents
		*										the world transform matrix of
		*										component's parent.
		*
		*Ins:									N/A
		*
		*Outs:									N/A
		*
		*Returns:								XMMatrix*
		*
		*Mod. Date:								02/16/2019
		*Mod. Initials:							STW
		************************************************************************/
		XMMATRIX* GetWorldTransform();

		/**********************************************************************
		*SetWorldTransform():					This function will directly set
		*										the world transform of the 
		*										component.
		*
		*Ins:									XMMATRIX
		*
		*Outs:									N/A
		*
		*Returns:								void
		*
		*Mod. Date:								02/19/2019
		*Mod. Initials:							STW
		************************************************************************/
		virtual void SetWorldTransform(XMMATRIX objectTransform);

		/**********************************************************************
		*GetPosition():							Returns the component's position
		*										directly. Useful if only the 
		*										position is needed without any
		*										other data.
		*
		*Ins:									N/A
		*
		*Outs:									N/A
		*
		*Returns:								XMFLOAT3*
		*
		*Mod. Date:								02/16/2019
		*Mod. Initials:							STW
		************************************************************************/
		XMFLOAT3 GetPosition();

		/**********************************************************************
		*SetPosition():							This function will allow for the
		*										position of the object to be set
		*										directly, for use with teleportation
		*										effects or debugging.
		*
		*Ins:									XMFLOAT4
		*
		*Outs:									N/A
		*
		*Returns:								void
		*
		*Mod. Date:								02/16/2019
		*Mod. Initials:							STW
		************************************************************************/
		virtual void SetPosition(XMFLOAT4 fPosition);

		/**********************************************************************
		*ApplyForce():							This function will apply the 
		*										passed in force to the 
		*										velocity of the component.
		*
		*Ins:									XMFLOAT4
		*
		*Outs:									N/A
		*
		*Returns:								void
		*
		*Mod. Date:								02/16/2019
		*Mod. Initials:							STW
		************************************************************************/
		void ApplyForce(XMFLOAT4 fForceVector);

		/**********************************************************************
		*ApplyGravity():						This function will apply the
		*										passed in gravity to the
		*										velocity of the component.
		*
		*Ins:									XMFLOAT4
		*
		*Outs:									N/A
		*
		*Returns:								void
		*
		*Mod. Date:								02/28/2019
		*Mod. Initials:							STW
		************************************************************************/
		void ApplyGravity(XMFLOAT4 fGravityVector);

		/**********************************************************************
		*GetVelocity():							Returns the velocity in a XMFLOAT4.
		*
		*Ins:									N/A
		*
		*Outs:									N/A
		*
		*Returns:								XMFLOAT4
		*
		*Mod. Date:								02/16/2019
		*Mod. Initials:							STW
		************************************************************************/
		XMFLOAT4 GetVelocity();

		/**********************************************************************
		*SetVelocity():							This function will directly set
		*										the velocity of the object,
		*										allowing for instant changes in
		*										direction and speed.
		*
		*Ins:									XMFLOAT4
		*
		*Outs:									N/A
		*
		*Returns:								void
		*
		*Mod. Date:								02/16/2019
		*Mod. Initials:							STW
		************************************************************************/
		void SetVelocity(XMFLOAT4 fVelocity);

		/**********************************************************************
		*GetGravity():							Returns the gravity in a XMFLOAT4.
		*
		*Ins:									N/A
		*
		*Outs:									N/A
		*
		*Returns:								XMFLOAT4
		*
		*Mod. Date:								02/28/2019
		*Mod. Initials:							STW
		************************************************************************/
		XMFLOAT4 GetGravity();

		/**********************************************************************
		*SetGravity():							This function will directly set
		*										the gravity of the object,
		*										allowing for instant changes in
		*										direction and speed.
		*
		*Ins:									XMFLOAT4
		*
		*Outs:									N/A
		*
		*Returns:								void
		*
		*Mod. Date:								02/28/2019
		*Mod. Initials:							STW
		************************************************************************/
		void SetGravity(XMFLOAT4 fVelocity);

		/**********************************************************************
		*Update():								Virtual Update Function for
		*										derived classes.
		*
		*Ins:									N/A
		*
		*Outs:									N/A
		*
		*Returns:								void
		*
		*Mod. Date:								02/28/2019
		*Mod. Initials:							STW
		************************************************************************/
		virtual void Update() = 0;

		/**********************************************************************
		*GetUp():								Returns the up in a XMFLOAT4.
		*
		*Ins:									N/A
		*
		*Outs:									N/A
		*
		*Returns:								XMFLOAT4
		*
		*Mod. Date:								02/28/2019
		*Mod. Initials:							STW
		************************************************************************/
		XMFLOAT4 GetUp();

		/**********************************************************************
		*SetUp():								This function will directly set
		*										the up vector of the object,
		*										allowing for instant changes in
		*										direction and speed.
		*
		*Ins:									XMFLOAT4
		*
		*Outs:									N/A
		*
		*Returns:								void
		*
		*Mod. Date:								02/28/2019
		*Mod. Initials:							STW
		************************************************************************/
		void SetUp(XMFLOAT4 fUp);

		/**********************************************************************
		*GetComponentType():					Returns the Component Type.
		*
		*Ins:									N/A
		*
		*Outs:									N/A
		*
		*Returns:								int
		*
		*Mod. Date:								03/06/2019
		*Mod. Initials:							STW
		************************************************************************/
		int GetComponentType();

		/**********************************************************************
		*GetThrownStatus():						Returns the Thrown Status.
		*
		*Ins:									N/A
		*
		*Outs:									N/A
		*
		*Returns:								bool
		*
		*Mod. Date:								03/06/2019
		*Mod. Initials:							STW
		************************************************************************/
		bool GetThrownStatus();

		/**********************************************************************
		*SetThrownStatus():						Sets the Thrown Status.
		*
		*Ins:									bool
		*
		*Outs:									N/A
		*
		*Returns:								void
		*
		*Mod. Date:								03/06/2019
		*Mod. Initials:							STW
		************************************************************************/
		void SetThrownStatus(bool bStatus);

		/**********************************************************************
		*GetColliderStatus():					Returns the Collider Status.
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
		bool GetColliderStatus();

		/**********************************************************************
		*SetColliderStatus():					Sets the Collider Status.
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
		void SetColliderStatus(bool bStatus);

		/**********************************************************************
		*GetOnWallStatus():					Returns the OnWall Status.
		*
		*Ins:									N/A
		*
		*Outs:									N/A
		*
		*Returns:								bool
		*
		*Mod. Date:								03/17/2019
		*Mod. Initials:							NCH
		************************************************************************/
		bool GetOnWallStatus();

		/**********************************************************************
		*SetOnWallStatus():					Sets the OnWall Status.
		*
		*Ins:									bool
		*
		*Outs:									N/A
		*
		*Returns:								void
		*
		*Mod. Date:								03/17/2019
		*Mod. Initials:							NCH
		************************************************************************/
		void SetOnWallStatus(bool bStatus);

		/**********************************************************************
		*GetOnWallRadStatus():					Returns the OnWallRad Status.
		*
		*Ins:									N/A
		*
		*Outs:									N/A
		*
		*Returns:								float
		*
		*Mod. Date:								03/17/2019
		*Mod. Initials:							NCH
		************************************************************************/
		float GetOnWallRad();

		/**********************************************************************
		*SetOnWallRadStatus():					Sets the OnWallRad Status.
		*
		*Ins:									float
		*
		*Outs:									N/A
		*
		*Returns:								void
		*
		*Mod. Date:								03/17/2019
		*Mod. Initials:							NCH
		************************************************************************/
		void SetOnWallRad(float fRadius);
	};
}


