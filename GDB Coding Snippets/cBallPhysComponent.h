/**********************************************************************
*Filename:								cBallPhysComponent.h
*Date:									02/16/2019
*Mod. Date:								02/16/2019
*Mod. Initials:							STW
*Author:								Sydney Whitehurst
*Purpose:								This component will be used for
*										the various types of balls.
*										Objects with this component can be
*										moved directly changing their position,
*										through instantaneous velocity changes
*										or by applying forces to the object
************************************************************************/
#pragma once
#include "cPhysicsComponent.h"
#include "CSceneManager.h"

namespace CPhysicsManager
{
	class cBallPhysComponent :
		public cPhysicsComponent
	{
	public:
		end::TSphere m_tBallCollider;
		std::vector<end::TSphere> m_tIceTraps;
		std::vector<float> m_fIceTrapsTimes;
		std::vector< CSceneManager::TSceneObj*> m_ptIceTrapModels;

		cBallPhysComponent(XMMATRIX worldTransform);
		~cBallPhysComponent();

		void SetPosition(XMFLOAT4 fPosition);

		void Update();
	};
}

