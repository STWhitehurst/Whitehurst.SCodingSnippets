#include "CPowerUp.h"

CPowerUp::CPowerUp()
{
}

CPowerUp::CPowerUp(XMFLOAT4X4 * pWorldTransform, ePowerUpType ePowerUpType, CSceneManager * pcScene, CPhysicsManager::CPhysicsManager * pcPhysics, CSound * pcSound, CAssetManager * pcAsset)
{
	//Connect Systems
	m_pcScene = pcScene;
	m_pcSound = pcSound;
	m_pcAsset = pcAsset;
	m_pcPhysics = pcPhysics;

	//Set up Model
	nmodelId = MODELIDNUM;
	worldMatrix = *pWorldTransform;
	m_eType = ePowerUpType;
	m_bIsActive = true;
	m_bIsActivated = false;
	m_fActivationTimer = FACTIVATIONTIME;

	switch (ePowerUpType)
	{
	case eJUMPBOOST:
	{
		nTextureId = JUMPBOOSTTEXTUREIDNUM;
		break;
	}
	case eSPEEDBOOST:
	{
		nTextureId = SPEEDBOOSTTEXTUREIDNUM;
		break;
	}
	case eDODGEBOOST:
	{
		nTextureId = DODGEBOOSTTEXTUREIDNUM;
		break;
	}
	case eTHROWBOOST:
	{
		nTextureId = THROWBOOSTTEXTUREIDNUM;
		break;
	}
	case eREFLECTOR:
	{
		nTextureId = REFLECTORTEXTUREIDNUM;
		break;
	}
	}

	//Make a Static Mesh Physics Component
	m_pcComponent = new CPhysicsManager::cStaticMeshPhysComponent(XMLoadFloat4x4(pWorldTransform));
}

CPowerUp::~CPowerUp()
{
}

void CPowerUp::Update(float fT)
{
	if (m_bIsActivated)
	{
		if (m_fActivationTimer > 0.0f)
		{
			m_fActivationTimer -= fT;

			if (m_fActivationTimer < 0.0f)
			{
				m_fActivationTimer = 0.0f;
			}
		}
	}

	XMStoreFloat4x4(&worldMatrix, *m_pcComponent->GetWorldTransform());
}

bool CPowerUp::GetActiveStatus()
{
	return m_bIsActive;
}

void CPowerUp::SetActiveStatus(bool bStatus)
{
	m_bIsActive = bStatus;
}

bool CPowerUp::GetActivationStatus()
{
	return m_bIsActivated;
}

void CPowerUp::SetActivationStatus(bool bStatus)
{
	m_bIsActivated = bStatus;
}

float CPowerUp::GetActivationTime()
{
	return m_fActivationTimer;
}

CPowerUp::ePowerUpType CPowerUp::GetPowerUpType()
{
	return m_eType;
}

CPhysicsManager::cStaticMeshPhysComponent * CPowerUp::GetComponent()
{
	return m_pcComponent;
}
