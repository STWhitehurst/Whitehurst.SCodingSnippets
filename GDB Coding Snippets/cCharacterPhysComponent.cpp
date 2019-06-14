/**********************************************************************
*Filename:								cCharacterPhysComponent.cpp
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

#include "cCharacterPhysComponent.h"

CPhysicsManager::cCharacterPhysComponent::cCharacterPhysComponent(XMMATRIX worldTransform)
{
	m_worldTransform = worldTransform;
	m_tCharacterCollider.radius = 2.0f;
	m_eComponentType = eCharacter;
	m_tCharacterCollider.btm = XMFLOAT3( m_worldTransform.r[3].m128_f32[0] - (6.0f * m_up.m128_f32[0]), m_worldTransform.r[3].m128_f32[1] - (6.0f * m_up.m128_f32[1]), m_worldTransform.r[3].m128_f32[2] - (6.0f * m_up.m128_f32[2]));
	m_tCharacterCollider.top = XMFLOAT3( m_worldTransform.r[3].m128_f32[0] + (6.0f * m_up.m128_f32[0]), m_worldTransform.r[3].m128_f32[1] + (6.0f * m_up.m128_f32[1]), m_worldTransform.r[3].m128_f32[2] + (6.0f * m_up.m128_f32[2]));
}

CPhysicsManager::cCharacterPhysComponent::~cCharacterPhysComponent()
{

}

void CPhysicsManager::cCharacterPhysComponent::Update()
{
	m_tCharacterCollider.btm = XMFLOAT3(m_worldTransform.r[3].m128_f32[0], m_worldTransform.r[3].m128_f32[1], m_worldTransform.r[3].m128_f32[2]);
	m_tCharacterCollider.top = XMFLOAT3( m_worldTransform.r[3].m128_f32[0] + (6.0f * m_up.m128_f32[0]), m_worldTransform.r[3].m128_f32[1] + (6.0f * m_up.m128_f32[1]), m_worldTransform.r[3].m128_f32[2] + (6.0f * m_up.m128_f32[2]));
	
	if (m_bBC)
	{
		m_tBCCollider.radius = 1.25f;
		XMMATRIX BCMatrix = m_worldTransform;
		BCMatrix = XMMatrixMultiply(XMMatrixTranslation(0.0f, 0.0f, 4.0f), m_worldTransform);
		m_tBCCollider.btm = XMFLOAT3(BCMatrix.r[3].m128_f32[0], BCMatrix.r[3].m128_f32[1], BCMatrix.r[3].m128_f32[2]);
		m_tBCCollider.top = XMFLOAT3(BCMatrix.r[3].m128_f32[0] + (6.0f * m_up.m128_f32[0]), BCMatrix.r[3].m128_f32[1] + (6.0f * m_up.m128_f32[1]), BCMatrix.r[3].m128_f32[2] + (6.0f * m_up.m128_f32[2]));
	}
	else
	{
		m_tBCCollider.radius = 0.0f;
	}

	if (m_bDrawColliders)
	{
		end::add_pill(m_tCharacterCollider);
		end::add_wall(m_playFrusto);
		end::add_plane(m_pTarget);
		if (m_bBC)
			end::add_pill(m_tBCCollider);
	}
	
}

void CPhysicsManager::cCharacterPhysComponent::SetPosition(XMFLOAT4 fPosition)
{
	m_tCharacterCollider.btm.x = fPosition.x;
	m_tCharacterCollider.btm.y = fPosition.y;
	m_tCharacterCollider.btm.z = fPosition.z;

	m_worldTransform.r[3] = XMVectorSet(fPosition.x, fPosition.y, fPosition.z, fPosition.w);
}

void CPhysicsManager::cCharacterPhysComponent::SetPlayersFrustum()
{
	m_playFrusto = end::make_frusto(m_worldTransform, 20.0f, 0.3f, 10.0f, (16.0f / 9.0f));


	XMMATRIX mWB = m_worldTransform;
	mWB = XMMatrixMultiply(XMMatrixTranslation(0.0f, 0.0f, 4.0f), mWB);
	XMMATRIX mWC = m_worldTransform;
	mWC = XMMatrixMultiply(XMMatrixTranslation(0.0f, 4.0f, 4.0f), mWC);
	XMMATRIX mWD = m_worldTransform;
	mWD = XMMatrixMultiply(XMMatrixTranslation(0.0f, 4.0f, 0.0f), mWD);
	
	m_pTarget = end::calculate_plane(end::turnToFloat3(m_worldTransform.r[3]), end::turnToFloat3(mWB.r[3]),
		end::turnToFloat3(mWC.r[3]), end::turnToFloat3(mWD.r[3]));
}

