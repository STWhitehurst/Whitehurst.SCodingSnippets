/**********************************************************************
*Filename:								cStaticMeshPhysComponent.h
*Date:									02/16/2019
*Mod. Date:								02/16/2019
*Mod. Initials:							STW
*Author:								Sydney Whitehurst
*Purpose:								Objects with this component will
*										be stationary by design, and as
*										such will be set to ignore any
*										forces that could apply to them.
*										This component primary exists to
*										hold the bounding volume for the
*										object.
************************************************************************/

#pragma once
#include "cPhysicsComponent.h"

namespace CPhysicsManager
{
	class cStaticMeshPhysComponent :
		public cPhysicsComponent
	{
	public:
		end::TSphere m_tMeshCollider;

		bool m_bWaypoint = false;

		cStaticMeshPhysComponent(XMMATRIX worldTransform);
		
		~cStaticMeshPhysComponent();

		void SetPosition(XMFLOAT4 fPosition);

		void Update();
	};
}

