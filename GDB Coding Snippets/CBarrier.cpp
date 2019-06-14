#include "CBarrier.h"



CBarrier::CBarrier()
{
}

CBarrier::CBarrier(float fHeight, float fWidth, float fLength, XMVECTOR wallPosition, end::TSphere tPlanetoidCollider, int nTextureID)
{
	nmodelId = MODELID;
	nTextureId = nTextureID;

	m_WorldTransform = end::placewall(wallPosition.m128_f32[0], wallPosition.m128_f32[1], wallPosition.m128_f32[2]);
	m_WorldTransform = end::wallorient(m_WorldTransform, tPlanetoidCollider, true);
	m_tWallCollider = end::make_wall(m_WorldTransform, fHeight, fWidth, fLength);

	m_tWallCollider.collided = false;
	m_tWallCollider.m_fHeight = fHeight;
	m_tWallCollider.m_fLength = fLength;
	m_tWallCollider.m_fWidth = fWidth;

	m_bDrawColliders = false;

	m_WorldTransform = XMMatrixScaling(fLength * 2, fHeight * 2, fWidth * 2) * m_WorldTransform;
	
	XMStoreFloat4x4(&worldMatrix, m_WorldTransform);
}


CBarrier::~CBarrier()
{
}

XMMATRIX * CBarrier::GetWorldTransform()
{
	return &m_WorldTransform;
}

void CBarrier::SetWorldTransform(XMMATRIX worldTransform)
{
	m_WorldTransform = worldTransform;
}

end::TWall * CBarrier::GetWallCollider()
{
	return &m_tWallCollider;
}

void CBarrier::SetWallCollider(end::TWall tWallCollider)
{
	m_tWallCollider = tWallCollider;
}

bool CBarrier::GetColliderDraw()
{
	return m_bDrawColliders;
}

void CBarrier::SetColliderDraw(bool bColliderDraw)
{
	m_bDrawColliders = bColliderDraw;
}

void CBarrier::Update()
{
	if (m_bDrawColliders)
		end::add_wall(m_tWallCollider);
}
