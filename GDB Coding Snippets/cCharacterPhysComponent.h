/**********************************************************************
*Filename:								cCharacterPhysComponent.h
*Date:									02/16/2019
*Mod. Date:								02/16/2019
*Mod. Initials:							STW
*Author:								Sydney Whitehurst
*Purpose:								This component will be used for
*										the player and enemy objects.
*										Objects with this component can be
*										moved directly changing their position,
*										through instantaneous velocity changes
*										or by applying forces to the object
************************************************************************/

#pragma once
#include "cPhysicsComponent.h"
namespace CPhysicsManager
{
	class cCharacterPhysComponent :
		public cPhysicsComponent
	{

	public:
		end::TPill m_tCharacterCollider;
		end::TPill m_tBCCollider;
		end::TWall m_playFrusto;
		end::TPlane m_pTarget;

		bool m_bBC = false;

		cCharacterPhysComponent(XMMATRIX worldTransform);
		~cCharacterPhysComponent();

		void Update();

		void SetPosition(XMFLOAT4 fPosition);

		void SetPlayersFrustum();
	};
}

