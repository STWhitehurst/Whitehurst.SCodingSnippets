/**********************************************************************
*Filename:								cMobileMeshPhysComponent.cpp
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
#include "cMobileMeshPhysComponent.h"

CPhysicsManager::cMobileMeshPhysComponent::cMobileMeshPhysComponent(XMMATRIX worldTransform)
{
	m_worldTransform = worldTransform;
	m_eComponentType = eMobileMesh;
	m_velocity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

CPhysicsManager::cMobileMeshPhysComponent::~cMobileMeshPhysComponent()
{
}

void CPhysicsManager::cMobileMeshPhysComponent::Update()
{
}
