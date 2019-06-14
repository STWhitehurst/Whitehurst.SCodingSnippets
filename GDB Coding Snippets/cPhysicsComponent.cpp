/**********************************************************************
*Filename:								cPhysicsComponent.cpp
*Date:									02/16/2019
*Mod. Date:								02/16/2019
*Mod. Initials:							STW
*Author:								Sydney Whitehurst
*Purpose:								An abstract base class for all
*										physics components.
************************************************************************/
#include "cPhysicsComponent.h"

CPhysicsManager::cPhysicsComponent::cPhysicsComponent()
{
	m_worldTransform = XMMatrixIdentity();
	m_velocity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_gravity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_up = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_bIsThrown = false;
}

CPhysicsManager::cPhysicsComponent::~cPhysicsComponent()
{

}

XMMATRIX * CPhysicsManager::cPhysicsComponent::GetWorldTransform()
{
	return &m_worldTransform;
}


void CPhysicsManager::cPhysicsComponent::SetWorldTransform(XMMATRIX objectTransform)
{
	m_worldTransform = objectTransform;
}

XMFLOAT3 CPhysicsManager::cPhysicsComponent::GetPosition()
{
	XMFLOAT3 fResult;

	XMStoreFloat3(&fResult, m_worldTransform.r[3]);

	return fResult;
}

void CPhysicsManager::cPhysicsComponent::SetPosition(XMFLOAT4 fPosition)
{
	m_worldTransform.r[3] = XMVectorSet(fPosition.x, fPosition.y, fPosition.z, 1.0f);
}

void CPhysicsManager::cPhysicsComponent::ApplyForce(XMFLOAT4 fForceVector)
{
	XMVECTOR force = XMVectorSet(fForceVector.x, fForceVector.y, fForceVector.z, fForceVector.w);
	m_velocity += force;
}

void CPhysicsManager::cPhysicsComponent::ApplyGravity(XMFLOAT4 fGravityVector)
{
	XMVECTOR gravity = XMVectorSet(fGravityVector.x, fGravityVector.y, fGravityVector.z, fGravityVector.w);
	m_velocity -= gravity;
}

XMFLOAT4 CPhysicsManager::cPhysicsComponent::GetVelocity()
{
	return XMFLOAT4(m_velocity.m128_f32[0], m_velocity.m128_f32[1], m_velocity.m128_f32[2], m_velocity.m128_f32[3]);
}

void CPhysicsManager::cPhysicsComponent::SetVelocity(XMFLOAT4 fVelocity)
{
	m_velocity = XMVectorSet(fVelocity.x, fVelocity.y, fVelocity.z, fVelocity.w);
}

XMFLOAT4 CPhysicsManager::cPhysicsComponent::GetGravity()
{
	return XMFLOAT4(m_gravity.m128_f32[0], m_gravity.m128_f32[1], m_gravity.m128_f32[2], m_gravity.m128_f32[3]);
}

void CPhysicsManager::cPhysicsComponent::SetGravity(XMFLOAT4 fVelocity)
{
	m_gravity = XMVectorSet(fVelocity.x, fVelocity.y, fVelocity.z, fVelocity.w);
}

XMFLOAT4 CPhysicsManager::cPhysicsComponent::GetUp()
{
	return XMFLOAT4(m_up.m128_f32[0], m_up.m128_f32[1], m_up.m128_f32[2], m_up.m128_f32[3]);
}

void CPhysicsManager::cPhysicsComponent::SetUp(XMFLOAT4 fUp)
{
	m_up = XMVectorSet(fUp.x, fUp.y, fUp.z, fUp.w);
}

int CPhysicsManager::cPhysicsComponent::GetComponentType()
{
	return (int)m_eComponentType;
}

bool CPhysicsManager::cPhysicsComponent::GetThrownStatus()
{
	return m_bIsThrown;
}

void CPhysicsManager::cPhysicsComponent::SetThrownStatus(bool bStatus)
{
	m_bIsThrown = bStatus;
}

bool CPhysicsManager::cPhysicsComponent::GetColliderStatus()
{
	return m_bDrawColliders;
}

void CPhysicsManager::cPhysicsComponent::SetColliderStatus(bool bStatus)
{
	m_bDrawColliders = bStatus;
}

bool CPhysicsManager::cPhysicsComponent::GetOnWallStatus()
{
	return m_bOnWall;
}

void CPhysicsManager::cPhysicsComponent::SetOnWallStatus(bool bStatus)
{
	m_bOnWall = bStatus;
}

float CPhysicsManager::cPhysicsComponent::GetOnWallRad()
{
	return m_fOnWallRadius;
}

void CPhysicsManager::cPhysicsComponent::SetOnWallRad(float fRadius)
{
	m_fOnWallRadius = fRadius;
}
