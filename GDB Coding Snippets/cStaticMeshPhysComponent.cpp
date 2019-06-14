#include "cStaticMeshPhysComponent.h"

CPhysicsManager::cStaticMeshPhysComponent::cStaticMeshPhysComponent(XMMATRIX worldTransform)
{
	m_worldTransform = worldTransform;
	m_tMeshCollider.radius = 0.50f;
	m_tMeshCollider.center.x = worldTransform.r[3].m128_f32[0];
	m_tMeshCollider.center.y = worldTransform.r[3].m128_f32[1];
	m_tMeshCollider.center.z = worldTransform.r[3].m128_f32[2];
	m_eComponentType = eStaticMesh;
}

CPhysicsManager::cStaticMeshPhysComponent::~cStaticMeshPhysComponent()
{

}

void CPhysicsManager::cStaticMeshPhysComponent::SetPosition(XMFLOAT4 fPosition)
{
	m_tMeshCollider.center.x = fPosition.x;
	m_tMeshCollider.center.y = fPosition.y;
	m_tMeshCollider.center.z = fPosition.z;

	m_worldTransform.r[3] = XMVectorSet(fPosition.x, fPosition.y, fPosition.z, 1.0f);
}

void CPhysicsManager::cStaticMeshPhysComponent::Update()
{
	if (!m_bWaypoint)
	{
		m_tMeshCollider.center.x = m_worldTransform.r[3].m128_f32[0] + (m_tMeshCollider.radius * m_up.m128_f32[0]);
		m_tMeshCollider.center.y = m_worldTransform.r[3].m128_f32[1] + (m_tMeshCollider.radius * m_up.m128_f32[1]);
		m_tMeshCollider.center.z = m_worldTransform.r[3].m128_f32[2] + (m_tMeshCollider.radius * m_up.m128_f32[2]);
	}
	else
	{
		m_tMeshCollider.center.x = m_worldTransform.r[3].m128_f32[0];
		m_tMeshCollider.center.y = m_worldTransform.r[3].m128_f32[1];
		m_tMeshCollider.center.z = m_worldTransform.r[3].m128_f32[2];
	}

	if (m_bDrawColliders)
	{
		end::add_sphere(m_tMeshCollider);
	}
}
