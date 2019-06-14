/**********************************************************************
*Filename:								cBallPhysComponent.cpp
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

#include "cBallPhysComponent.h"

CPhysicsManager::cBallPhysComponent::cBallPhysComponent(XMMATRIX worldTransform)
{
	m_worldTransform = worldTransform;
	m_tBallCollider.radius = 0.50f;
	m_tBallCollider.center.x = worldTransform.r[3].m128_f32[0];
	m_tBallCollider.center.y = worldTransform.r[3].m128_f32[1] + 1.0f;
	m_tBallCollider.center.z = worldTransform.r[3].m128_f32[2];
	m_eComponentType = eBall;
}

CPhysicsManager::cBallPhysComponent::~cBallPhysComponent()
{
}

void CPhysicsManager::cBallPhysComponent::SetPosition(XMFLOAT4 fPosition)
{
	m_tBallCollider.center.x = fPosition.x;
	m_tBallCollider.center.y = fPosition.y;
	m_tBallCollider.center.z = fPosition.z;

	m_worldTransform.r[3] = XMVectorSet(fPosition.x, fPosition.y, fPosition.z, fPosition.w);
}

void CPhysicsManager::cBallPhysComponent::Update()
{
	m_tBallCollider.center.x = m_worldTransform.r[3].m128_f32[0] + (m_tBallCollider.radius * m_up.m128_f32[0]);
	m_tBallCollider.center.y = m_worldTransform.r[3].m128_f32[1] + (m_tBallCollider.radius * m_up.m128_f32[1]);
	m_tBallCollider.center.z = m_worldTransform.r[3].m128_f32[2] + (m_tBallCollider.radius * m_up.m128_f32[2]);

	if (m_bDrawColliders)
	{
		end::add_sphere(m_tBallCollider);

		for (int i = 0; i < m_tIceTraps.size(); i++)
		{
			end::add_sphere(m_tIceTraps[i]);
		}
	}
}

