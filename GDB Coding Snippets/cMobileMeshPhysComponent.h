/**********************************************************************
*Filename:								cMobileMeshPhysComponent.h
*Date:									02/16/2019
*Mod. Date:								02/16/2019
*Mod. Initials:							STW
*Author:								Sydney Whitehurst
*Purpose:								Objects with this component will
*										generally be stationary, but will
*										have the capability for movement
*										through either directly setting
*										the velocity or by applying 
*										forces to the object.
************************************************************************/
#pragma once
#include "cPhysicsComponent.h"

namespace CPhysicsManager
{
	class cMobileMeshPhysComponent :
		public cPhysicsComponent
	{
	public:
		cMobileMeshPhysComponent(XMMATRIX worldTransform);
		~cMobileMeshPhysComponent();

		void Update();
	};
}

