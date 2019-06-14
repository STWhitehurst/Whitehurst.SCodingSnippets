#include "CPhysicsManager.h"

CPhysicsManager::CPhysicsManager::CPhysicsManager()
{
	componentContainer.clear();
	m_tPlanetoidCollider.center = XMFLOAT3{ 0.0f, -20.0f, 0.0f };
	m_tPlanetoidCollider.radius = 20.0f;
}

CPhysicsManager::CPhysicsManager::~CPhysicsManager()
{

}

void CPhysicsManager::CPhysicsManager::CreatePhysComponent(cPhysicsComponent * physicsComponent)
{
	componentContainer.push_back(physicsComponent);
}

void CPhysicsManager::CPhysicsManager::RemovePhysComponent(cPhysicsComponent * physicsComponent)
{
	for (int i = 0; i < componentContainer.size(); i++)
	{
		if (componentContainer[i] == physicsComponent)
		{
			componentContainer.erase(componentContainer.begin() + i);
		}
	}
}

void CPhysicsManager::CPhysicsManager::Update(float fT)
{

	float radius = m_tPlanetoidCollider.radius;
	if (m_bPlanetoidColliderStatus)
		end::add_sphere(m_tPlanetoidCollider);

	for (int i = 0; i < componentContainer.size(); i++)
	{

		//Calculate current Position
		XMMATRIX *  worldTransform = componentContainer[i]->GetWorldTransform();
		XMFLOAT3 curPos;
		curPos.x = worldTransform->r[3].m128_f32[0];
		curPos.y = worldTransform->r[3].m128_f32[1];
		curPos.z = worldTransform->r[3].m128_f32[2];

		if (componentContainer[i]->GetOnWallStatus() && radius == m_tPlanetoidCollider.radius)
			radius = componentContainer[i]->GetOnWallRad();
		else
			radius = m_tPlanetoidCollider.radius;

		//Distance check with the component and the Planetoid
		float fDistance = end::Distance(curPos, m_tPlanetoidCollider.center);
		//Calculate and Update Gravity
		//If the component's current Position is greater than the Planet's Radius
		if (fDistance > radius)
		{
			//Calculate the new Velocity and Set it
			XMFLOAT4 fVelocity = componentContainer[i]->GetVelocity();
			XMFLOAT3 fCopVeloc = XMFLOAT3{ fVelocity.x, fVelocity.y, fVelocity.z };
			fCopVeloc = end::Normalize(fCopVeloc);

			XMFLOAT4 newVelocity;
			newVelocity.x = fCopVeloc.x * fT;
			newVelocity.y = fCopVeloc.y * fT;
			newVelocity.z = fCopVeloc.z * fT;
			newVelocity.w = 1.0f;

			componentContainer[i]->ApplyForce(newVelocity);

			if (componentContainer[i]->GetThrownStatus() == false)
			{
				//Calculate the new Gravity and Set it
				XMFLOAT3 fGravity = end::GetVector(curPos, m_tPlanetoidCollider.center);
				end::Normalize(fGravity);
				fGravity.x *= -GRAVITYBASE * fT;
				fGravity.y *= -GRAVITYBASE * fT;
				fGravity.z *= -GRAVITYBASE * fT;

				XMFLOAT4 newGravity;
				newGravity.x = fGravity.x;
				newGravity.y = fGravity.y;
				newGravity.z = fGravity.z;
				newGravity.w = 1.0f;

				componentContainer[i]->SetGravity(newGravity);
				componentContainer[i]->ApplyGravity(newGravity);
			}
		}
		if (fDistance < radius)
		{
			XMMATRIX moveOut = XMMatrixTranslation(0.0f, (radius - fDistance), 0.0f) * *componentContainer[i]->GetWorldTransform();
			componentContainer[i]->SetWorldTransform(moveOut);
		}

		//Terrain Following
		worldTransform = componentContainer[i]->GetWorldTransform();
		curPos.x = worldTransform->r[3].m128_f32[0];
		curPos.y = worldTransform->r[3].m128_f32[1];
		curPos.z = worldTransform->r[3].m128_f32[2];

		XMFLOAT3 playerY = end::GetVector(curPos, m_tPlanetoidCollider.center);
		playerY = end::Normalize(playerY);

		//Set Your New Up
		XMFLOAT4 newUp = XMFLOAT4(-playerY.x, -playerY.y, -playerY.z, 1.0f);
		componentContainer[i]->SetUp(newUp);

		XMFLOAT3 playerZ = end::Cross(end::Normalize(end::turnToFloat3(worldTransform->r[0])), playerY);
		if (end::NORMALIZED(playerZ) == false)
		{
			playerZ = end::Normalize(playerZ);
		}

		XMFLOAT3 playerX = end::Cross(playerY, playerZ);
		if (end::NORMALIZED(playerX) == false)
		{
			playerX = end::Normalize(playerX);
		}

		worldTransform->r[0] = end::turnToVector(playerX);
		worldTransform->r[1] = end::turnToVector(end::Inverse(playerY));
		worldTransform->r[2] = end::turnToVector(end::Inverse(playerZ));

		componentContainer[i]->SetWorldTransform(*worldTransform);

		//Apply Velocity to World Transform
		if (fDistance > radius && componentContainer[i]->GetOnWallStatus() == false)
		{
			XMMATRIX transform = *componentContainer[i]->GetWorldTransform();
			XMFLOAT4 velocity = componentContainer[i]->GetVelocity();
			XMFLOAT3 fCopVeloc = XMFLOAT3{ velocity.x, velocity.y, velocity.z };
			fCopVeloc = end::Normalize(fCopVeloc);
			transform.r[3].m128_f32[0] += (fCopVeloc.x * fT * 6);
			transform.r[3].m128_f32[1] += (fCopVeloc.y * fT * 6);
			transform.r[3].m128_f32[2] += (fCopVeloc.z * fT * 6);
			componentContainer[i]->SetWorldTransform(transform);
		}
		radius++;
		XMFLOAT4 zeroFloat = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		componentContainer[i]->SetVelocity(zeroFloat);

		//Call Update Functions
		componentContainer[i]->Update();
	}
}

bool CPhysicsManager::CPhysicsManager::GetCollidersStatus()
{
	return m_bPlanetoidColliderStatus;
}

void CPhysicsManager::CPhysicsManager::SetColliders(bool bStatus)
{
	for (int i = 0; i < componentContainer.size(); i++)
	{
		componentContainer[i]->SetColliderStatus(bStatus);
	}

	m_bPlanetoidColliderStatus = bStatus;
}
