#include "CGameLogic.h"

void CGameLogic::SpawnABall(CBall::eBallType eBallType)
{
	XMFLOAT4X4 ballMatrix;
	XMMATRIX ballTransform = XMMatrixIdentity();
	ballTransform = XMMatrixMultiply(XMMatrixTranslation(2.0f, 2.0f, 0.0f), ballTransform);
	XMStoreFloat4x4(&ballMatrix, ballTransform);

	CBall* cBall = new CBall(&ballMatrix, eBallType, m_eCurrentGameType, true, &m_cScene, &m_cPhysicsManager, m_pcSound, &m_cAsset, &m_cPhysicsManager.m_tPlanetoidCollider);

	m_cPhysicsManager.CreatePhysComponent(cBall->GetBallComponent());

	//Add it to the spawner and Scene
	m_cScene.addObjectToScene(cBall, false);

	m_pcSpawnedBalls.push_back(cBall);

	switch (eBallType)
	{
	case CBall::eNORM:
	{
		m_pcInput->tLogicSwitch.m_bNormBall1 = false;
		break;
	}
	case CBall::eFIRE:
	{
		m_pcInput->tLogicSwitch.m_bFireBall2 = false;
		break;
	}
	case CBall::eWIND:
	{
		m_pcInput->tLogicSwitch.m_bWindBall3 = false;
		break;
	}
	case CBall::eICE:
	{
		m_pcInput->tLogicSwitch.m_bIceBall4 = false;
		break;
	}
	}
}

void CGameLogic::MakeAPowerUp(CPowerUp * cPowerUp, XMFLOAT4X4 * ballMatrix, CPowerUp::ePowerUpType ePowerUpType, TPowerUpSpawner * tPowerUpSpawner, int nSpawnerIndex)
{
	//Make a Ball and Add its Ball Physics Component to the Physics Manager Component Container
	cPowerUp = new CPowerUp(ballMatrix, ePowerUpType, &m_cScene, &m_cPhysicsManager, m_pcSound, &m_cAsset);

	m_cPhysicsManager.CreatePhysComponent(cPowerUp->GetComponent());

	//Add it to the spawner and Scene
	m_pcSpawnedPowerUps.push_back(cPowerUp);

	m_cScene.addObjectToScene(cPowerUp, false);

	tPowerUpSpawner->m_pcLastSpawnedPowerUps[nSpawnerIndex] = cPowerUp;
}

void CGameLogic::SpawnBalls(float fT)
{
	//When it's time to spawn and there are balls to spawn
	for (int i = 0; i < m_tBallSpawners.m_pcMobileMeshComponents.size(); i++)
	{
		if (m_tBallSpawners.m_fSpawnTimers[i] <= 0.0f && m_tBallSpawners.m_cContainer.size() > 0)
		{
			if (m_tBallSpawners.m_pcLastSpawnedBalls[i] == nullptr)
			{
				CBall * theBall = m_tBallSpawners.m_cContainer[0];
				XMFLOAT4X4 ball4x4;
				XMMATRIX spawnerMatrix = *m_tBallSpawners.m_pcMobileMeshComponents[i]->GetWorldTransform();
				spawnerMatrix.r[0] = XMVector3Normalize(spawnerMatrix.r[0]);
				spawnerMatrix.r[1] = XMVector3Normalize(spawnerMatrix.r[1]);
				spawnerMatrix.r[2] = XMVector3Normalize(spawnerMatrix.r[2]);
				spawnerMatrix.r[0].m128_f32[0] = 1.0f;
				spawnerMatrix.r[1].m128_f32[1] = 1.0f;
				spawnerMatrix.r[2].m128_f32[2] = 1.0f;
				XMStoreFloat4x4(&ball4x4, spawnerMatrix);
				m_tBallSpawners.RemoveFromSpawner(0);

				int number = rand() % 5;
				if (number == 0)
					MakeABall(theBall, &ball4x4, CBall::eNORM, &m_tBallSpawners, i);
				else if (number == 1)
					MakeABall(theBall, &ball4x4, CBall::eFIRE, &m_tBallSpawners, i);
				else if (number == 2)
					MakeABall(theBall, &ball4x4, CBall::eICE, &m_tBallSpawners, i);
				else if (number == 3)
					MakeABall(theBall, &ball4x4, CBall::eWIND, &m_tBallSpawners, i);
				else if (number == 4)
					MakeABall(theBall, &ball4x4, CBall::eELECTRIC, &m_tBallSpawners, i);
				m_tBallSpawners.m_fSpawnTimers[i] = 5.5f;
			}
			else
			{
				XMVECTOR lsbPosition = m_tBallSpawners.m_pcLastSpawnedBalls[i]->GetBallComponent()->GetWorldTransform()->r[3];
				XMVECTOR spawnerPosition = m_tBallSpawners.m_pcMobileMeshComponents[i]->GetWorldTransform()->r[3];

				if (lsbPosition.m128_f32[0] != spawnerPosition.m128_f32[0]
					&& lsbPosition.m128_f32[1] != spawnerPosition.m128_f32[1]
					&& lsbPosition.m128_f32[2] != spawnerPosition.m128_f32[2])
				{
					CBall * theBall = m_tBallSpawners.m_cContainer[0];
					XMFLOAT4X4 ball4x4;
					XMMATRIX spawnerMatrix = *m_tBallSpawners.m_pcMobileMeshComponents[i]->GetWorldTransform();
					spawnerMatrix.r[0] = XMVector3Normalize(spawnerMatrix.r[0]);
					spawnerMatrix.r[1] = XMVector3Normalize(spawnerMatrix.r[1]);
					spawnerMatrix.r[2] = XMVector3Normalize(spawnerMatrix.r[2]);
					spawnerMatrix.r[0].m128_f32[0] = 1.0f;
					spawnerMatrix.r[1].m128_f32[1] = 1.0f;
					spawnerMatrix.r[2].m128_f32[2] = 1.0f;
					XMStoreFloat4x4(&ball4x4, spawnerMatrix);
					m_tBallSpawners.RemoveFromSpawner(0);

					int number = rand() % 5;
					switch (number)
					{
					case 0:
					{
						MakeABall(theBall, &ball4x4, CBall::eNORM, &m_tBallSpawners, i);
						break;
					}
					case 1:
					{
						MakeABall(theBall, &ball4x4, CBall::eFIRE, &m_tBallSpawners, i);
						break;
					}
					case 2:
					{
						MakeABall(theBall, &ball4x4, CBall::eICE, &m_tBallSpawners, i);
						break;
					}
					case 3:
					{
						MakeABall(theBall, &ball4x4, CBall::eWIND, &m_tBallSpawners, i);
						break;
					}
					case 4:
					{
						MakeABall(theBall, &ball4x4, CBall::eELECTRIC, &m_tBallSpawners, i);
						break;
					}
					}

					m_tBallSpawners.m_fSpawnTimers[i] = FBALLSPAWNERTIME;
				}
			}
		}
	}
}

void CGameLogic::SpawnPlayer(float fT)
{
	for (int i = 0; i < m_tPlayerSpawner.m_cContainer.size(); i++)
	{
		if (m_tPlayerSpawner.m_cContainer[i]->GetRespawnTimer() <= 0.0f)
		{
			int randIndex = rand() % NNUMOFPLAYERSPAWNERS;

			if (m_eCurrentGameType == CPlayer::eTEAM && m_tPlayerSpawner.m_cContainer[i]->GetPlayerType() == CPlayer::eUSER)
			{
				randIndex = rand() % 2;
			}

			if (m_eCurrentGameType == CPlayer::eTEAM && m_tPlayerSpawner.m_cContainer[i]->GetPlayerType() == CPlayer::eAI)
			{
				randIndex = (rand() % 2) + 2;
			}

			if (m_tPlayerSpawner.m_pcLastSpawnedPlayer[randIndex] != nullptr)
			{
				XMFLOAT3 lsbPosition = m_tPlayerSpawner.m_pcLastSpawnedPlayer[randIndex]->GetCharacterComponent()->GetPosition();
				XMFLOAT3 spawnerPosition = m_tPlayerSpawner.m_pcMobileMeshComponents[randIndex]->GetPosition();

				if (lsbPosition.x != spawnerPosition.x && lsbPosition.y != spawnerPosition.y && lsbPosition.z != spawnerPosition.z)
				{
					m_tPlayerSpawner.m_cContainer[i]->GetCharacterComponent()->SetPosition(XMFLOAT4(spawnerPosition.x, spawnerPosition.y, spawnerPosition.z, 1.0f));
				}
				else
				{
					m_tPlayerSpawner.m_cContainer[i]->GetCharacterComponent()->SetPosition(XMFLOAT4(spawnerPosition.x + 5.0f, spawnerPosition.y, spawnerPosition.z - 5.0f, 1.0f));
				}
			}
			else
			{
				XMFLOAT3 respawnPosition = m_tPlayerSpawner.m_pcMobileMeshComponents[randIndex]->GetPosition();
				m_tPlayerSpawner.m_cContainer[i]->GetCharacterComponent()->SetPosition(XMFLOAT4(respawnPosition.x, respawnPosition.y, respawnPosition.z, 1.0f));
			}

			m_tPlayerSpawner.m_cContainer[i]->GetCharacterComponent()->GetWorldTransform()->r[0] = XMVector3Normalize(m_tPlayerSpawner.m_cContainer[i]->GetCharacterComponent()->GetWorldTransform()->r[0]);
			m_tPlayerSpawner.m_cContainer[i]->GetCharacterComponent()->GetWorldTransform()->r[1] = XMVector3Normalize(m_tPlayerSpawner.m_cContainer[i]->GetCharacterComponent()->GetWorldTransform()->r[1]);
			m_tPlayerSpawner.m_cContainer[i]->GetCharacterComponent()->GetWorldTransform()->r[2] = XMVector3Normalize(m_tPlayerSpawner.m_cContainer[i]->GetCharacterComponent()->GetWorldTransform()->r[2]);
			
			//Update LastSpawned
			m_tPlayerSpawner.m_pcLastSpawnedPlayer[randIndex] = m_tPlayerSpawner.m_cContainer[i];

			//Set Respawn location and add back to the scene and physicsmanager
			XMStoreFloat4x4(&m_tPlayerSpawner.m_cContainer[i]->worldMatrix, *m_tPlayerSpawner.m_cContainer[i]->GetCharacterComponent()->GetWorldTransform());
			m_cScene.addObjectToScene(m_tPlayerSpawner.m_cContainer[i], false);
			m_cPhysicsManager.CreatePhysComponent(m_tPlayerSpawner.m_cContainer[i]->GetCharacterComponent());

			//Set Status to 
			m_tPlayerSpawner.m_cContainer[i]->SetInvincibilityStatus(true);
			m_tPlayerSpawner.m_cContainer[i]->SetHandStatus(CPlayer::ePlayerHandStatus::eEMPTY);
			m_tPlayerSpawner.m_cContainer[i]->SetLegStatus(CPlayer::ePlayerLegStatus::eIDLE);

			//Reset Respawn Timer
			m_tPlayerSpawner.m_cContainer[i]->ResetRespawnTimer();
			//Remove from Respawn Container
			m_tPlayerSpawner.RemoveFromSpawner(i);
			i--;
		}
	}
}

void CGameLogic::SpawnPowerUp(float fT)
{
	//When it's time to spawn and there are balls to spawn
	for (int i = 0; i < m_tPowerUpSpawners.m_pcMobileMeshComponents.size(); i++)
	{
		if (m_tPowerUpSpawners.m_fSpawnTimers[i] <= 0.0f && m_tPowerUpSpawners.m_cContainer.size() > 0)
		{
			if (m_tPowerUpSpawners.m_pcLastSpawnedPowerUps[i] == nullptr)
			{
				CPowerUp * thePowerUp = m_tPowerUpSpawners.m_cContainer[0];
				XMFLOAT4X4 powerUp4x4;
				XMMATRIX spawnerMatrix = *m_tPowerUpSpawners.m_pcMobileMeshComponents[i]->GetWorldTransform();
				spawnerMatrix.r[0] = XMVector3Normalize(spawnerMatrix.r[0]);
				spawnerMatrix.r[1] = XMVector3Normalize(spawnerMatrix.r[1]);
				spawnerMatrix.r[2] = XMVector3Normalize(spawnerMatrix.r[2]);
				spawnerMatrix.r[0].m128_f32[0] = 1.0f;
				spawnerMatrix.r[1].m128_f32[1] = 1.0f;
				spawnerMatrix.r[2].m128_f32[2] = 1.0f;
				XMStoreFloat4x4(&powerUp4x4, spawnerMatrix);
				m_tPowerUpSpawners.RemoveFromSpawner(0);

				int number = rand() % 4;
				switch (number)
				{
				case 0:
				{
					MakeAPowerUp(thePowerUp, &powerUp4x4, CPowerUp::eJUMPBOOST, &m_tPowerUpSpawners, i);
					break;
				}
				case 1:
				{
					MakeAPowerUp(thePowerUp, &powerUp4x4, CPowerUp::eDODGEBOOST, &m_tPowerUpSpawners, i);
					break;
				}
				case 2:
				{
					MakeAPowerUp(thePowerUp, &powerUp4x4, CPowerUp::eSPEEDBOOST, &m_tPowerUpSpawners, i);
					break;
				}
				case 3:
				{
					MakeAPowerUp(thePowerUp, &powerUp4x4, CPowerUp::eTHROWBOOST, &m_tPowerUpSpawners, i);
					break;
				}
				case 4:
				{
					MakeAPowerUp(thePowerUp, &powerUp4x4, CPowerUp::eREFLECTOR, &m_tPowerUpSpawners, i);
					break;
				}
				}
				m_tPowerUpSpawners.m_fSpawnTimers[i] = FPOWERUPSPAWNERTIME;
				cout << "A Power Up Spawned \n";
			}
			else
			{
				XMVECTOR lsbPosition = m_tPowerUpSpawners.m_pcLastSpawnedPowerUps[i]->GetComponent()->GetWorldTransform()->r[3];
				XMVECTOR spawnerPosition = m_tPowerUpSpawners.m_pcMobileMeshComponents[i]->GetWorldTransform()->r[3];

				if (lsbPosition.m128_f32[0] != spawnerPosition.m128_f32[0]
					&& lsbPosition.m128_f32[1] != spawnerPosition.m128_f32[1]
					&& lsbPosition.m128_f32[2] != spawnerPosition.m128_f32[2])
				{
					CPowerUp * thePowerUp = m_tPowerUpSpawners.m_cContainer[0];
					XMFLOAT4X4 powerUp4x4;
					XMMATRIX spawnerMatrix = *m_tPowerUpSpawners.m_pcMobileMeshComponents[i]->GetWorldTransform();
					spawnerMatrix.r[0] = XMVector3Normalize(spawnerMatrix.r[0]);
					spawnerMatrix.r[1] = XMVector3Normalize(spawnerMatrix.r[1]);
					spawnerMatrix.r[2] = XMVector3Normalize(spawnerMatrix.r[2]);
					spawnerMatrix.r[0].m128_f32[0] = 1.0f;
					spawnerMatrix.r[1].m128_f32[1] = 1.0f;
					spawnerMatrix.r[2].m128_f32[2] = 1.0f;
					XMStoreFloat4x4(&powerUp4x4, spawnerMatrix);
					m_tPowerUpSpawners.RemoveFromSpawner(0);

					int number = rand() % 4;
					switch (number)
					{
					case 0:
					{
						MakeAPowerUp(thePowerUp, &powerUp4x4, CPowerUp::eJUMPBOOST, &m_tPowerUpSpawners, i);
						break;
					}
					case 1:
					{
						MakeAPowerUp(thePowerUp, &powerUp4x4, CPowerUp::eDODGEBOOST, &m_tPowerUpSpawners, i);
						break;
					}
					case 2:
					{
						MakeAPowerUp(thePowerUp, &powerUp4x4, CPowerUp::eSPEEDBOOST, &m_tPowerUpSpawners, i);
						break;
					}
					case 3:
					{
						MakeAPowerUp(thePowerUp, &powerUp4x4, CPowerUp::eTHROWBOOST, &m_tPowerUpSpawners, i);
						break;
					}
					case 4:
					{
						MakeAPowerUp(thePowerUp, &powerUp4x4, CPowerUp::eREFLECTOR, &m_tPowerUpSpawners, i);
						break;
					}
					}

					m_tPowerUpSpawners.m_fSpawnTimers[i] = FPOWERUPSPAWNERTIME;
					cout << "A Power Up Spawned \n";
				}
			}
		}
	}
}

void CGameLogic::RespawnPlayer(CPlayer * cPlayer)
{
	//If the player has a ball in their hand, drop it
	if (cPlayer->GetHeldBall() != nullptr)
	{
		cPlayer->GetHeldBall()->SetOwner(nullptr);
		cPlayer->GetHeldBall()->SetBallStatus(CBall::eIDLE);
		cPlayer->SetHeldBall(nullptr);
	}

	//Remove player from the scene and PhysicsManager
	m_cScene.removeObjectFromScene(cPlayer);
	m_cPhysicsManager.RemovePhysComponent(cPlayer->GetCharacterComponent());

	//Set Respawn Status
	cPlayer->SetLegStatus(CPlayer::ePlayerLegStatus::eLEGRESPAWNING);
	cPlayer->SetHandStatus(CPlayer::ePlayerHandStatus::eHANDRESPAWNING);

	//Add the player to the respawn container
	m_tPlayerSpawner.AddToSpawner(cPlayer);
}

void CGameLogic::TBallSpawner::Update(float fT)
{
	for (int i = 0; i < (int)m_pcMobileMeshComponents.size(); i++)
	{
		if (m_pcLastSpawnedBalls[i] != nullptr && m_pcLastSpawnedBalls[i] != NULL)
		{
			XMFLOAT3 lsbPosition = m_pcLastSpawnedBalls[i]->GetBallComponent()->GetPosition();
			XMFLOAT3 spawnerPosition = m_pcMobileMeshComponents[i]->GetPosition();

			if (lsbPosition.x != spawnerPosition.x && lsbPosition.y != spawnerPosition.y && lsbPosition.z != spawnerPosition.z)
			{
				if (m_fSpawnTimers[i] > 0.0f)
				{
					m_fSpawnTimers[i] -= fT;

					if (m_fSpawnTimers[i] < 0.0f)
						m_fSpawnTimers[i] = 0.0f;
				}
			}
		}
		else
		{
			if (m_fSpawnTimers[i] > 0.0f)
			{
				m_fSpawnTimers[i] -= fT;

				if (m_fSpawnTimers[i] < 0.0f)
					m_fSpawnTimers[i] = 0.0f;
			}
		}
	}
}

void CGameLogic::TPlayerSpawner::Update(float fT)
{
	for (int i = 0; i < (int)m_pcMobileMeshComponents.size(); i++)
	{
		if (m_fSpawnTimers[i] > 0.0f)
		{
			m_fSpawnTimers[i] -= fT;

			if (m_fSpawnTimers[i] < 0.0f)
				m_fSpawnTimers[i] = 0.0f;
		}
	}
}

void CGameLogic::TPowerUpSpawner::Update(float fT)
{
	for (int i = 0; i < (int)m_pcMobileMeshComponents.size(); i++)
	{
		if (m_fSpawnTimers[i] > 0.0f)
		{
			m_fSpawnTimers[i] -= fT;

			if (m_fSpawnTimers[i] < 0.0f)
				m_fSpawnTimers[i] = 0.0f;
		}
	}
}

void CGameLogic::MakeABall(CBall* cBall, XMFLOAT4X4* ballMatrix, CBall::eBallType eBallType, TBallSpawner* tBallSpawner, int nSpawnerIndex)
{
	//Make a Ball and Add its Ball Physics Component to the Physics Manager Component Container
	cBall = new CBall(ballMatrix, eBallType, m_eCurrentGameType, true, &m_cScene, &m_cPhysicsManager, m_pcSound, &m_cAsset, &m_cPhysicsManager.m_tPlanetoidCollider);

	m_cPhysicsManager.CreatePhysComponent(cBall->GetBallComponent());

	//Add it to the spawner and Scene
	m_pcSpawnedBalls.push_back(cBall);

	m_cScene.addObjectToScene(cBall, false);

	tBallSpawner->m_pcLastSpawnedBalls[nSpawnerIndex] = cBall;
}

void CGameLogic::SpawnAPowerUp(CPowerUp::ePowerUpType ePowerUpType)
{
	XMFLOAT4X4 powerUpMatrix;
	XMMATRIX powerUpTransform = XMMatrixIdentity();
	powerUpTransform = XMMatrixMultiply(XMMatrixTranslation(2.0f, 2.0f, 0.0f), powerUpTransform);
	XMStoreFloat4x4(&powerUpMatrix, powerUpTransform);

	CPowerUp* cPowerUp = new CPowerUp(&powerUpMatrix, ePowerUpType, &m_cScene, &m_cPhysicsManager, m_pcSound, &m_cAsset);

	m_cPhysicsManager.CreatePhysComponent(cPowerUp->GetComponent());

	//Add it to the spawner and Scene
	m_cScene.addObjectToScene(cPowerUp, false);

	m_pcSpawnedPowerUps.push_back(cPowerUp);

	switch (ePowerUpType)
	{
	case CPowerUp::eJUMPBOOST:
	{
		m_pcInput->tLogicSwitch.m_bPUJumpBoost = false;
		break;
	}
	case CPowerUp::eREFLECTOR:
	{
		m_pcInput->tLogicSwitch.m_bPUReflector = false;
		break;
	}
	case CPowerUp::eSPEEDBOOST:
	{
		m_pcInput->tLogicSwitch.m_bPUSpeedBoost = false;
		break;
	}
	case CPowerUp::eTHROWBOOST:
	{
		m_pcInput->tLogicSwitch.m_bPUThrowBoost = false;
		break;
	}
	case CPowerUp::eDODGEBOOST:
	{
		m_pcInput->tLogicSwitch.m_bPUDodgeBoost = false;
		break;
	}
	}
}

void CGameLogic::MakeAPlayer(XMFLOAT4X4 * playerMatrix, CPlayer::ePlayerType ePlayerType, CPlayer::eTeamSide eTeam, int nPlayerNumber)
{
	CPlayer * cPlayer = nullptr;
	//Make a Player and Add its Character Component to the Physics Manager Component Container
	if (ePlayerType == CPlayer::ePlayerType::eUSER && m_eCurrentGameType == CPlayer::eTEAM)
		cPlayer = new CPlayer(playerMatrix, ePlayerType, m_eCurrentGameType, eTeam, nPlayerNumber, m_pcRender->GetUIManager(), &m_cPhysicsManager, &m_cScene, m_pcInput, m_pcSound, &m_cAsset, &m_cAnimMan);

	if (ePlayerType == CPlayer::ePlayerType::eUSER && m_eCurrentGameType == CPlayer::eSOLO)
		cPlayer = new CPlayer(playerMatrix, ePlayerType, m_eCurrentGameType, nPlayerNumber, m_pcRender->GetUIManager(), &m_cPhysicsManager, &m_cScene, m_pcInput, m_pcSound, &m_cAsset, &m_cAnimMan);

	if (ePlayerType == CPlayer::ePlayerType::eAI && m_eCurrentGameType == CPlayer::eTEAM)
		cPlayer = new CAIManager(playerMatrix, ePlayerType, eTeam, m_eCurrentGameType, nPlayerNumber, m_pcRender->GetUIManager(), &m_cPhysicsManager, &m_cScene, m_pcInput, m_pcSound, &m_cAsset, m_pcBarriers[0]->GetWallCollider(), m_cSG);

	if (ePlayerType == CPlayer::ePlayerType::eAI && m_eCurrentGameType == CPlayer::eSOLO)
		cPlayer = new CAIManager(playerMatrix, ePlayerType, m_eCurrentGameType, nPlayerNumber, m_pcRender->GetUIManager(), &m_cPhysicsManager, &m_cScene, m_pcInput, m_pcSound, &m_cAsset, m_pcBarriers[0]->GetWallCollider(), m_cSG);
	cPlayer->SetAnimated(&m_cAnimMan);
	//cPlayer->PlayAnimation(0);
	m_cPhysicsManager.CreatePhysComponent(cPlayer->GetCharacterComponent());

	//Add it to the Lobby and Scene
	m_pcLobby.push_back(cPlayer);
	m_tTemplobby.push_back(cPlayer);

	m_cScene.addObjectToScene(cPlayer, false);
}

void CGameLogic::MakeABarrier(CBarrier * cBarrier, float fHeight, float fWidth, float fLength, XMVECTOR wallPosition, int nTextureID)
{
	//Make a barrier and Add its Static Component to the Physics Manager Component Container
	cBarrier = new CBarrier(fHeight, fWidth, fLength, wallPosition, m_cPhysicsManager.m_tPlanetoidCollider, nTextureID);
	
	//Add it to the vector of barriers and Scene
	m_pcBarriers.push_back(cBarrier);

	m_cScene.addObjectToScene(cBarrier, false);
}

void CGameLogic::ResetBallSpawners()
{
	//Disconnect Owners with their Ball and set their hands to empty
	for (int i = 0; i < m_pcLobby.size(); i++)
	{
		m_pcLobby[i]->SetHeldBall(nullptr);
		m_pcLobby[i]->SetHandStatus(CPlayer::eEMPTY);
	}

	//Remove all Spawned Balls from the Scene and Physics Manager
	for (int i = (int)m_pcSpawnedBalls.size() - 1; i >= 0; --i)
	{
		m_cPhysicsManager.RemovePhysComponent(m_pcSpawnedBalls[i]->GetBallComponent());
		m_cScene.removeObjectFromScene(m_pcSpawnedBalls[i]);
		delete m_pcSpawnedBalls[i];
	}
	//Clear all Spawned Balls from vector
	m_pcSpawnedBalls.clear();

	//Delete all waiting to be Spawned Balls
	for (int i = (int)m_tBallSpawners.m_cContainer.size() - 1; i >= 0; --i)
	{
		delete m_tBallSpawners.m_cContainer[i];
	}
	//Clear Ball Spawner Container
	m_tBallSpawners.m_cContainer.clear();

	//Reset Respawn Time and Last Spawned Ball
	for (int i = 0; i < NNUMOFBALLSPAWNERS; i++)
	{
		m_tBallSpawners.m_fSpawnTimers[i] = FBALLSPAWNERTIME;
		m_tBallSpawners.m_pcLastSpawnedBalls[i] = nullptr;
	}

	//Refill Ball Spawner Container
	for (int i = 0; i < m_nNumOfSpawnedBalls; i++)
	{
		CBall* cBall = new CBall();
		m_tBallSpawners.AddToSpawner(cBall);
	}

	//Reset Last Spawned balls
	for (int i = (int)m_tBallSpawners.m_pcMobileMeshComponents.size() - 1; i >= 0; --i)
	{
		m_tBallSpawners.m_pcLastSpawnedBalls[i] = nullptr;
	}
}

void CGameLogic::ResetPlayerSpawners(float fT)
{
	//If any players still need to respawn
	SpawnPlayer(fT);

	//Clear Last Spawned Players
	for (int i = (int)m_tPlayerSpawner.m_pcMobileMeshComponents.size() - 1; i >= 0; --i)
	{
		m_tPlayerSpawner.m_pcLastSpawnedPlayer[i] = nullptr;
	}

	//Reset the Respawn Timers of the Players
	for (int i = 0; i < (int)m_pcLobby.size(); i++)
	{
		m_pcLobby[i]->ResetRespawnTimer();
	}
}

void CGameLogic::ClearBallSpawners()
{
	//Disconnect Owners with their Ball and set their hands to empty
	for (int i = 0; i < m_pcLobby.size(); i++)
	{
		m_pcLobby[i]->SetHeldBall(nullptr);
		m_pcLobby[i]->SetHandStatus(CPlayer::eEMPTY);
	}

	//Remove all Spawned Balls from the Scene and Physics Manager
	for (int i = (int)m_pcSpawnedBalls.size() - 1; i >= 0; --i)
	{
		m_cPhysicsManager.RemovePhysComponent(m_pcSpawnedBalls[i]->GetBallComponent());
		m_cScene.removeObjectFromScene(m_pcSpawnedBalls[i]);
		delete m_pcSpawnedBalls[i];
	}
	//Clear all Spawned Balls from vector
	m_pcSpawnedBalls.clear();

	//Delete all waiting to be Spawned Balls
	for (int i = (int)m_tBallSpawners.m_cContainer.size() - 1; i >= 0; --i)
	{
		delete m_tBallSpawners.m_cContainer[i];
	}
	//Clear Ball Spawner Container
	m_tBallSpawners.m_cContainer.clear();

	//Clear Last Spawned balls
	for (int i = (int)m_tBallSpawners.m_pcMobileMeshComponents.size() - 1; i >= 0; --i)
	{
		m_tBallSpawners.m_pcLastSpawnedBalls[i] = nullptr;
	}

	//Remove all Ball Spawners from the Physics Manager
	for (int i = (int)m_tBallSpawners.m_pcMobileMeshComponents.size() - 1; i >= 0 ; --i)
	{
		m_cPhysicsManager.RemovePhysComponent(m_tBallSpawners.m_pcMobileMeshComponents[i]);
		delete m_tBallSpawners.m_pcMobileMeshComponents[i];
	}
	m_tBallSpawners.m_pcMobileMeshComponents.clear();

	//Remove all Spawned Power Ups from the Scene and Physics Manager
	for (int i = (int)m_pcSpawnedPowerUps.size() - 1; i >= 0; --i)
	{
		m_cPhysicsManager.RemovePhysComponent(m_pcSpawnedPowerUps[i]->GetComponent());
		m_cScene.removeObjectFromScene(m_pcSpawnedPowerUps[i]);
		delete m_pcSpawnedPowerUps[i];
	}
	//Clear all Spawned Power Ups from vector
	m_pcSpawnedPowerUps.clear();

	//Delete all waiting to be Spawned Power Ups
	for (int i = (int)m_tPowerUpSpawners.m_cContainer.size() - 1; i >= 0; --i)
	{
		delete m_tPowerUpSpawners.m_cContainer[i];
	}
	//Clear Power Up Spawner Container
	m_tPowerUpSpawners.m_cContainer.clear();

	//Clear Last Spawned Power Ups
	for (int i = (int)m_tPowerUpSpawners.m_pcMobileMeshComponents.size() - 1; i >= 0; --i)
	{
		m_tPowerUpSpawners.m_pcLastSpawnedPowerUps[i] = nullptr;
	}

	//Remove all Power Up Spawners from the Physics Manager
	for (int i = (int)m_tPowerUpSpawners.m_pcMobileMeshComponents.size() - 1; i >= 0; --i)
	{
		m_cPhysicsManager.RemovePhysComponent(m_tPowerUpSpawners.m_pcMobileMeshComponents[i]);
		delete m_tPowerUpSpawners.m_pcMobileMeshComponents[i];
	}
	m_tPowerUpSpawners.m_pcMobileMeshComponents.clear();
}

void CGameLogic::ClearPlayerSpawners()
{
	//If any Players are still waiting to respawn from the Scene and Physics Manager
	for (int i = (int)m_tPlayerSpawner.m_cContainer.size() - 1; i >= 0; --i)
	{
		m_cPhysicsManager.RemovePhysComponent(m_tPlayerSpawner.m_cContainer[i]->GetCharacterComponent());
		m_cScene.removeObjectFromScene(m_tPlayerSpawner.m_cContainer[i]);
		delete m_tPlayerSpawner.m_cContainer[i];
	}
	//Clear all Players from vector
	m_tPlayerSpawner.m_cContainer.clear();

	//Remove all Players from the Scene and Physics Manager
	for (int i = (int)m_pcLobby.size() - 1; i >= 0; --i)
	{
		m_cPhysicsManager.RemovePhysComponent(m_pcLobby[i]->GetCharacterComponent());
		m_cScene.removeObjectFromScene(m_pcLobby[i]);
		delete m_pcLobby[i];
	}

	//Clear all Players from vector
	m_pcLobby.clear();
	m_tTemplobby.clear();

	//Clear Last Spawned Players
	for (int i = (int)m_tPlayerSpawner.m_pcMobileMeshComponents.size() - 1; i >= 0; --i)
	{
		m_tPlayerSpawner.m_pcLastSpawnedPlayer[i] = nullptr;
	}

	for (int i = 0; i < 4; i++)
	{
		m_pcPlayerObject[i] = nullptr;
	}

	//Remove all Player Spawners from the Physics Manager
	for (int i = (int)m_tPlayerSpawner.m_pcMobileMeshComponents.size() - 1; i >= 0; --i)
	{
		m_cPhysicsManager.RemovePhysComponent(m_tPlayerSpawner.m_pcMobileMeshComponents[i]);
		delete m_tPlayerSpawner.m_pcMobileMeshComponents[i];
	}
	m_tPlayerSpawner.m_pcMobileMeshComponents.clear();
}

void CGameLogic::ClearBarriers()
{
	//Remove Barriers from the Scene
	for (int i = (int)m_pcLobby.size() - 1; i >= 0; --i)
	{
		m_cScene.removeObjectFromScene(m_pcBarriers[i]);
		delete m_pcBarriers[i];
	}

	//Clear all Barriers from vector
	m_pcBarriers.clear();
}

void CGameLogic::SetWorld(eWorldSelect world)
{
	m_eCurrentWorldSelection = world;
}

void CGameLogic::ChooseDowo()
{
	if (m_eCurrentWorldSelection == eKILN)
	{
		int randWorld = (rand() % 3) + 1;
		SetWorld((eWorldSelect)randWorld);
	}
	else
	{
		SetWorld(m_eCurrentWorldSelection);
	}

	m_pcSound->TurnMusicOff(m_cAsset.getMusic(2));
	ClearPlayerSpawners();
	ClearBarriers();
	ClearWaypoints();
}

void CGameLogic::ChooseKiln()
{
	SetWorld(eKILN);
	m_pcSound->TurnMusicOff(m_cAsset.getMusic(2));
	ClearPlayerSpawners();
	ClearBarriers();
	ClearWaypoints();
}

void CGameLogic::ToTheMenu()
{
	for (int i = 0; i < m_pcLobby.size(); i++)
	{
		m_pcLobby[i]->ResetScoreCount();
	}
	m_eCurrentState = eGameState::eMENU;
	m_eCurrentWorldSelection = eKILN;
	m_pcRender->SetMatchTime();
	m_fPreMatchTimer = 10.0f;
	m_pcRender->bTimerStarted = false;

	m_cScene.newScene();
	m_pcRender->GetUIManager()->SendGameInfo(0, 0); //DISABLE UI
	m_cMenuSys.LoadMenuScreens(0);
	m_cMenuSys.SwitchMenuScreen(0);

	m_pcSound->TurnMusicOff(m_cAsset.getMusic(m_nCurrentMusicTrack));
	ClearBallSpawners();
	ClearPlayerSpawners();
	ClearBarriers();

	m_pcCutsceneCam->bIsCutsceneDone = false;
	m_pcCutsceneCam->ResetTimer();
	MenuSetup();
}


