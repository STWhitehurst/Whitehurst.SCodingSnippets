#include "CBall.h"
#include "CPlayer.h"



CBall::CBall()
{
}

CBall::CBall(XMFLOAT4X4* pWorldTransform, enum eBallType eBallType, enum CPlayer::eGameType eCurrentGameType, bool bActiveStatus, CSceneManager* pcSceneManager, CPhysicsManager::CPhysicsManager* pcPhysicsManager, CSound* pcSound, CAssetManager* pcAsset, end::TSphere* tPlanetoid)
{
	//Connect Systems and Containers
	m_pcSceneManager = pcSceneManager;
	m_pcPhysicsManager = pcPhysicsManager;
	m_pcSoundManager = pcSound;
	m_pcAssetManager = pcAsset;
	m_ptPlanetoid = tPlanetoid;

	//Set up Model and Ball Stats
	nmodelId = MODELIDNUM;
	worldMatrix = *pWorldTransform;
	m_eStatus = eIDLE;
	m_nNumBounced = 0;
	m_bIsActive = bActiveStatus;
	m_fThrownTimer = FTHROWNTIME;
	m_pcOwner = nullptr;
	m_eBallType = eBallType;
	m_eCurrentGameType = eCurrentGameType;

	//Set Type Specific Model and Ball Stats
	switch (eBallType)
	{
	case eNORM:
	{
		m_bBlockable = true;
		m_bCatchable = true;
		nTextureId = NORMTEXTUREIDNUM;
		break;
	}
	case eFIRE:
	{
		m_bBlockable = false;
		m_bCatchable = false;
		nTextureId = FIRETEXTUREIDNUM;
		m_fExplosionTimer = FEXPLOSIONTIME;
		m_fScaleAmount = 1.0f;
		break;
	}
	case eWIND:
	{
		m_bBlockable = false;
		m_bCatchable = false;
		nTextureId = WINDTEXTUREIDNUM;
		m_fActivationTimer = FBALLACTIVATIONTIME;
		break;
	}
	case eICE:
	{
		m_bBlockable = false;
		m_bCatchable = false;
		nTextureId = ICETEXTUREIDNUM;
		m_fRollTimer = FROLLTIME;
		m_fPrevRollTime = FROLLTIME;
		break;
	}
	case eELECTRIC:
	{
		m_bBlockable = true;
		m_bCatchable = true;
		nTextureId = ELECTRICTEXTUREIDNUM;
		break;
	}
	}

	//Make a Ball Physics Component 
	m_pcBallComponent = new CPhysicsManager::cBallPhysComponent(XMLoadFloat4x4(pWorldTransform));
}


CBall::~CBall()
{
}

void CBall::Update(float fT, vector<CBall*> m_vSpawnedBalls, vector<CBarrier*> theBarriers, vector<CPlayer*> pcLobby)
{
	if (m_bIsActive)
	{
		XMMATRIX *  worldTransform = m_pcBallComponent->GetWorldTransform();
		XMFLOAT3 curPos;
		curPos.x = worldTransform->r[3].m128_f32[0];
		curPos.y = worldTransform->r[3].m128_f32[1];
		curPos.z = worldTransform->r[3].m128_f32[2];

		if (end::Distance(curPos, m_pcPhysicsManager->m_tPlanetoidCollider.center) > 50.0f)
		{
			m_bIsActive = false;
		}

		for (int i = 0; i < theBarriers.size(); i++)
		{
			//Check against walls
			int m_nClosetPlane = end::classify_sphere_to_wall(*theBarriers[i]->GetWallCollider(), m_pcBallComponent->m_tBallCollider);
			if (m_nClosetPlane == -1)
			{
				m_pcBallComponent->m_tBallCollider.collided = false;
				theBarriers[i]->GetWallCollider()->collided = false;
			}
			else
			{
				m_pcBallComponent->m_tBallCollider.collided = true;
				theBarriers[i]->GetWallCollider()->collided = true;

				if (m_eStatus != eHELD)
				{
					if (m_eBallType != eELECTRIC)
					{
						m_pcBallComponent->SetThrownStatus(false);

						XMVECTOR lerp = end::bounceoffwalls(*theBarriers[i]->GetWallCollider(), m_pcBallComponent->m_tBallCollider, m_nClosetPlane);
						XMVECTOR negateForce = XMVectorSet(-m_fLevelOfForce.m128_f32[0], -m_fLevelOfForce.m128_f32[1], -m_fLevelOfForce.m128_f32[2], 1.0f);
						negateForce.m128_f32[0] += lerp.m128_f32[0];
						negateForce.m128_f32[1] += lerp.m128_f32[1];
						negateForce.m128_f32[2] += lerp.m128_f32[2];

						m_fLevelOfForce = negateForce;
						m_nNumBounced++;

						switch (m_eBallType)
						{
						case eICE:
						{
							m_pcSoundManager->PlaySoundFile(m_pcAssetManager->getSound(16));
							break;
						}
						case eNORM:
						{
							m_pcSoundManager->PlaySoundFile(m_pcAssetManager->getSound(6));
							break;
						}
						case eFIRE:
						{
							m_eStatus = CBall::eEXPLODING;
							m_pcSoundManager->PlaySoundFile(m_pcAssetManager->getSound(20));
							break;
						}
						}
					}
					else
					{
						if (m_nNumBounced < 1 && m_pcTarget == nullptr)
						{
							for (int j = 0; j < (int)pcLobby.size(); j++)
							{
								m_pcBallComponent->SetThrownStatus(false);

								switch (m_eCurrentGameType)
								{
								case CPlayer::eSOLO:
								{
									int nWallSide = end::classify_pill_to_plane(pcLobby[j]->GetCharacterComponent()->m_tCharacterCollider, theBarriers[i]->GetWallCollider()->sides[m_nClosetPlane]);
									if (pcLobby[j] != m_pcOwner && nWallSide == 1)
									{
										float fDistance = end::Distance(m_pcBallComponent->GetPosition(), pcLobby[j]->GetCharacterComponent()->GetPosition());

										if (fDistance <= m_fClosestEnemyDistance)
										{
											m_fClosestEnemyDistance = fDistance;
											m_pcTarget = pcLobby[j];
										}
									}
									break;
								}
								case CPlayer::eTEAM:
								{
									int nWallSide = end::classify_pill_to_plane(pcLobby[j]->GetCharacterComponent()->m_tCharacterCollider, theBarriers[i]->GetWallCollider()->sides[m_nClosetPlane]);
									if (pcLobby[j] != m_pcOwner && pcLobby[j]->GetPlayerSide() != m_pcOwner->GetPlayerSide() && nWallSide == 1)
									{
										float fDistance = end::Distance(m_pcBallComponent->GetPosition(), pcLobby[j]->GetCharacterComponent()->GetPosition());

										if (fDistance <= m_fClosestEnemyDistance)
										{
											m_fClosestEnemyDistance = fDistance;
											m_pcTarget = pcLobby[j];
										}
									}
									break;
								}
								}

							}

							if (m_pcTarget != nullptr)
							{
								m_pcBallComponent->SetWorldTransform(end::Look_At(m_pcBallComponent->GetWorldTransform()->r[3], m_pcTarget->GetCharacterComponent()->GetPosition()));
								m_fClosestEnemyDistance = 50.0f;
							}
							else
							{
								XMVECTOR lerp = end::bounceoffwalls(*theBarriers[i]->GetWallCollider(), m_pcBallComponent->m_tBallCollider, m_nClosetPlane);
								XMVECTOR negateForce = XMVectorSet(-m_fLevelOfForce.m128_f32[0], -m_fLevelOfForce.m128_f32[1], -m_fLevelOfForce.m128_f32[2], 1.0f);
								negateForce.m128_f32[0] += lerp.m128_f32[0];
								negateForce.m128_f32[1] += lerp.m128_f32[1];
								negateForce.m128_f32[2] += lerp.m128_f32[2];

								m_fLevelOfForce = negateForce;
							}
						}
						else
						{
							m_pcBallComponent->SetThrownStatus(false);

							XMVECTOR lerp = end::bounceoffwalls(*theBarriers[i]->GetWallCollider(), m_pcBallComponent->m_tBallCollider, m_nClosetPlane);
							XMVECTOR negateForce = XMVectorSet(-m_fLevelOfForce.m128_f32[0], -m_fLevelOfForce.m128_f32[1], -m_fLevelOfForce.m128_f32[2], 1.0f);
							negateForce.m128_f32[0] += lerp.m128_f32[0];
							negateForce.m128_f32[1] += lerp.m128_f32[1];
							negateForce.m128_f32[2] += lerp.m128_f32[2];

							m_fLevelOfForce = negateForce;
						}

						m_nNumBounced++;
						m_pcSoundManager->PlaySoundFile(m_pcAssetManager->getSound(22));
					}
				}
			}
		}

		switch (m_eStatus)
		{
		case eIDLE:
		{
			XMMATRIX ballMatrix = *m_pcBallComponent->GetWorldTransform();
			ballMatrix = XMMatrixMultiply(XMMatrixTranslation((fT*m_fLevelOfForce.m128_f32[0]), (fT*m_fLevelOfForce.m128_f32[1]), (fT*m_fLevelOfForce.m128_f32[2])), ballMatrix);

			m_pcBallComponent->SetWorldTransform(ballMatrix);

			break;
		}

		//Move to hand position
		case eHELD:
		{
			if (m_pcOwner != nullptr)
			{
				XMMATRIX playerTransform = *m_pcOwner->GetCharacterComponent()->GetWorldTransform();
				XMMATRIX newWorldZTransform = XMMatrixMultiply(XMMatrixTranslation(0.0f, 2.0f, 0.0f), playerTransform);
				XMMATRIX newWorldXTransform = XMMatrixMultiply(XMMatrixTranslation(-3.0f, 0.0f, 0.0f), newWorldZTransform);
				m_pcBallComponent->SetWorldTransform(newWorldXTransform);

				switch (m_eBallType)
				{
				case CBall::eFIRE:
				{
					if (m_eBallType == CBall::eFIRE && m_fEffectCooldown <= 0.0f)
					{
						m_cEffects.sortedPOS = m_pcBallComponent->GetPosition();

						m_cEffects.ParticleInsertSorted(m_cEffects.sorted, 3, m_cEffects.sortedPOS, 1.0f, 52);
						m_fEffectCooldown = 0.2f;
					}
					else
					{
						m_fEffectCooldown -= fT;
					}
				}
				break;
				case CBall::eICE:
				{
					if (m_eBallType == CBall::eICE && m_fEffectCooldown <= 0.0f)
					{
						m_cEffects.sortedPOS = m_pcBallComponent->GetPosition();

						m_cEffects.ParticleInsertSorted(m_cEffects.sorted, 3, m_cEffects.sortedPOS, 1.0f, 54);
						m_fEffectCooldown = 0.2f;
					}
					else
					{
						m_fEffectCooldown -= fT;
					}
				}
				break;
				case CBall::eELECTRIC:
				{
					if (m_eBallType == CBall::eELECTRIC && m_fEffectCooldown <= 0.0f)
					{
						m_cEffects.sortedPOS = m_pcBallComponent->GetPosition();

						m_cEffects.ParticleInsertSorted(m_cEffects.sorted, 3, m_cEffects.sortedPOS, 1.0f, 55);
						m_fEffectCooldown = 0.2f;
					}
					else
					{
						m_fEffectCooldown -= fT;
					}
				}
				break;
				default:
				{
				}
				break;
				}
			}
			else
			{
				m_eStatus = eIDLE;
			}
			break;
		}
		//If the player is blocking, so are you
		//Move to in front of the player!
		case eBLOCKING:
		{
			//Set New Position
			if (m_pcOwner != nullptr)
			{
				XMMATRIX playerTransform = *m_pcOwner->GetCharacterComponent()->GetWorldTransform();
				XMMATRIX newWorldYTransform = XMMatrixMultiply(XMMatrixTranslation(0.0f, 2.0f, 0.0f), playerTransform);
				XMMATRIX newWorldZTransform = XMMatrixMultiply(XMMatrixTranslation(0.0f, 0.0f, 1.0f), newWorldYTransform);
				m_pcBallComponent->SetWorldTransform(newWorldZTransform);
			}
			else
			{
				m_eStatus = eIDLE;
			}
			break;
		}
		//If the ball has been thrown, travel!
		case eTHROWN:
		{
			switch (m_eBallType)
			{
			case CBall::eFIRE:
			{
				if (m_eBallType == CBall::eFIRE && m_fEffectCooldown <= 0.0f)
				{
					m_cEffects.sortedPOS = m_pcBallComponent->GetPosition();

					m_cEffects.ParticleInsertSorted(m_cEffects.sorted, 3, m_cEffects.sortedPOS, 1.0f, 52);
					m_fEffectCooldown = 0.2f;
				}
				else
				{
					m_fEffectCooldown -= fT;
				}
			}
			break;
			case CBall::eICE:
			{
				if (m_eBallType == CBall::eICE && m_fEffectCooldown <= 0.0f)
				{
					m_cEffects.sortedPOS = m_pcBallComponent->GetPosition();

					m_cEffects.ParticleInsertSorted(m_cEffects.sorted, 3, m_cEffects.sortedPOS, 1.0f, 54);
					m_fEffectCooldown = 0.2f;
				}
				else
				{
					m_fEffectCooldown -= fT;
				}
			}
			break;
			case CBall::eELECTRIC:
			{
				if (m_eBallType == CBall::eELECTRIC && m_fEffectCooldown <= 0.0f)
				{
					m_cEffects.sortedPOS = m_pcBallComponent->GetPosition();

					m_cEffects.ParticleInsertSorted(m_cEffects.sorted, 3, m_cEffects.sortedPOS, 1.0f, 55);
					m_fEffectCooldown = 0.2f;
				}
				else
				{
					m_fEffectCooldown -= fT;
				}
			}
			break;
			default:
			{
			}
			break;
			}

			//Distance check to know when to Bounce
			if (end::Distance(curPos, m_ptPlanetoid->center) <= m_ptPlanetoid->radius)
			{
				if (m_eBallType != eICE && m_eBallType != eELECTRIC)
				{
					if (m_nNumBounced <= MAXBOUNCES)
					{
						m_fLevelOfForce.m128_f32[2] /= 3.0f;
						if (m_nNumBounced == 0)
						{
							m_fLevelOfForce.m128_f32[1] = 2.0f;
							m_pcSoundManager->PlaySoundFile(m_pcAssetManager->getSound(8));
						}
						else
						{
							m_fLevelOfForce.m128_f32[1] = 1.5f;
							m_pcSoundManager->PlaySoundFile(m_pcAssetManager->getSound(9));
						}
						m_nNumBounced++;
					}
					else
					{
						m_pcOwner = nullptr;
						m_nNumBounced = 0;
						m_eStatus = eIDLE;
						m_fLevelOfForce = XMVectorZero();
						m_pcSoundManager->PlaySoundFile(m_pcAssetManager->getSound(1));
					}
				}
				if (m_eBallType == eICE)
				{
					CSceneManager::TSceneObj* ptIceTrap = new CSceneManager::TSceneObj();
					ptIceTrap->nmodelId = MODELIDNUM;
					ptIceTrap->nTextureId = ICETEXTUREIDNUM;

					XMMATRIX scaled = XMMatrixScaling(4.0f, 0.5f, 4.0f) * *worldTransform;
					XMStoreFloat4x4(&ptIceTrap->worldMatrix, scaled);
					m_pcBallComponent->m_ptIceTrapModels.push_back(ptIceTrap);
					m_pcSceneManager->addObjectToScene(ptIceTrap, false);

					end::TSphere tIceTrap;
					tIceTrap.center = XMFLOAT3(scaled.r[3].m128_f32[0], scaled.r[3].m128_f32[1], scaled.r[3].m128_f32[2]);
					tIceTrap.radius = 3.0f;
					m_pcBallComponent->m_tIceTraps.push_back(tIceTrap);
					m_pcBallComponent->m_fIceTrapsTimes.push_back(6.0f);

					m_pcSceneManager->removeObjectFromScene(this);
					m_pcPhysicsManager->RemovePhysComponent(m_pcBallComponent);
					m_eStatus = eTRAP;
					m_pcSoundManager->PlaySoundFile(m_pcAssetManager->getSound(15));
				}
				if (m_eBallType == eELECTRIC)
				{
					if (m_nNumBounced <= ELECTRICMAXBOUNCES)
					{
						if (m_fLevelOfForce.m128_f32[2] > 0)
							m_fLevelOfForce.m128_f32[2] += 5.0f;
						else
							m_fLevelOfForce.m128_f32[2] -= 5.0f;

						if (m_nNumBounced == 0)
							m_fLevelOfForce.m128_f32[1] = 5.0f;
						else
							m_fLevelOfForce.m128_f32[1] = 2.5f;
						m_nNumBounced++;

						if (m_nNumBounced < ELECTRICMAXBOUNCES)
						{
							m_pcSoundManager->PlaySoundFile(m_pcAssetManager->getSound(22));
						}
					}
					else
					{
						m_pcOwner = nullptr;
						m_nNumBounced = 0;
						m_eStatus = eIDLE;
						m_fLevelOfForce = XMVectorZero();
					}
				}
			}

			XMMATRIX ballMatrix = *m_pcBallComponent->GetWorldTransform();
			ballMatrix = XMMatrixMultiply(XMMatrixTranslation((fT*m_fLevelOfForce.m128_f32[0]), (fT*m_fLevelOfForce.m128_f32[1]), (fT*m_fLevelOfForce.m128_f32[2])), ballMatrix);

			m_pcBallComponent->SetWorldTransform(ballMatrix);

			//Update Thrown Timer
			if (m_fThrownTimer > 0)
			{
				m_fThrownTimer -= fT;

				if (m_fThrownTimer < 0)
					m_fThrownTimer = 0;
			}

			//Turn gravity back on for ball to come out of orbit
			if (m_fThrownTimer <= 0.0f)
			{
				m_pcBallComponent->SetThrownStatus(false);
			}

			if (m_nNumBounced > 0)
				m_fLevelOfForce.m128_f32[1] -= GRAVITYBASE * fT;
			break;
		}
		case eEXPLODING:
		{
			if (m_pcBallComponent->m_tBallCollider.radius != 4.0f)
			{
				m_pcBallComponent->m_tBallCollider.radius = end::Lerp(m_pcBallComponent->m_tBallCollider.radius, 4.0f, fT*4.0f);
			}
			XMMATRIX worldTransform = *m_pcBallComponent->GetWorldTransform();
			if (m_fScaleAmount != 8.0f)
			{
				m_fScaleAmount = end::Lerp(m_fScaleAmount, 8.0f, fT*4.0f);
				worldTransform = XMMatrixMultiply(XMMatrixScaling(m_fScaleAmount, m_fScaleAmount, m_fScaleAmount), worldTransform);
			}

			m_pcBallComponent->SetWorldTransform(worldTransform);

			//Update Thrown Timer
			if (m_fExplosionTimer > 0)
			{
				m_fExplosionTimer -= fT * 8.0f;

				if (m_fExplosionTimer < 0)
					m_fExplosionTimer = 0;
			}

			if (m_fExplosionTimer <= 0.0f)
			{
				m_pcSoundManager->PlaySoundFile(m_pcAssetManager->getSound(1));
				m_bIsActive = false;
			}
			break;
		}
		case eACTIVATED:
		{
			//nmodelId = WINDBALLMODELIDNUM;
			//Scale Ball
			XMMATRIX playerTransform = *m_pcOwner->GetCharacterComponent()->GetWorldTransform();

			m_pcBallComponent->m_tBallCollider.radius = 6.0f;
			XMMATRIX newWorldYTransform = XMMatrixMultiply(XMMatrixTranslation(0.0f, 0.0f, 0.0f), playerTransform);
			XMMATRIX newWorldScaleTransform = XMMatrixMultiply(XMMatrixScaling(12.0f, 12.0f, 12.0f), newWorldYTransform);

			//Set New Position
			m_pcBallComponent->m_tBallCollider.center = XMFLOAT3(newWorldScaleTransform.r[3].m128_f32[0], newWorldScaleTransform.r[3].m128_f32[1], newWorldScaleTransform.r[3].m128_f32[2]);
			m_pcBallComponent->SetWorldTransform(newWorldScaleTransform);
			XMStoreFloat4x4(&worldMatrix, newWorldScaleTransform);


			//Update Activation Timer
			if (m_fActivationTimer > 0)
			{
				m_fActivationTimer -= fT;

				if (m_fActivationTimer < 0)
					m_fActivationTimer = 0;
			}

			//Turn inactive when Timer is done.
			if (m_fActivationTimer <= 0.0f || m_pcOwner->GetLegStatus() == CPlayer::eLEGRESPAWNING)
			{
				m_bIsActive = false;
			}
			break;
		}
		case eROLLING:
		{
			if (m_fRollTimer > 12.0f)
			{
				XMMATRIX ballMatrix = *m_pcBallComponent->GetWorldTransform();
				ballMatrix = XMMatrixMultiply(XMMatrixTranslation((fT*m_fLevelOfForce.m128_f32[0]), (fT*m_fLevelOfForce.m128_f32[1]), (fT*m_fLevelOfForce.m128_f32[2])), ballMatrix);

				m_pcBallComponent->SetWorldTransform(ballMatrix);

				if (m_fPrevRollTime - m_fRollTimer >= 0.5f)
				{
					CSceneManager::TSceneObj* ptIceTrap = new CSceneManager::TSceneObj();
					ptIceTrap->nmodelId = MODELIDNUM;
					ptIceTrap->nTextureId = ICETEXTUREIDNUM;

					XMMATRIX scaled = XMMatrixMultiply(XMMatrixScaling(4.0f, 0.5f, 4.0f), ballMatrix);
					XMStoreFloat4x4(&ptIceTrap->worldMatrix, scaled);
					m_pcBallComponent->m_ptIceTrapModels.push_back(ptIceTrap);
					m_pcSceneManager->addObjectToScene(ptIceTrap, false);

					end::TSphere tIceTrap;
					tIceTrap.center = XMFLOAT3(scaled.r[3].m128_f32[0], scaled.r[3].m128_f32[1], scaled.r[3].m128_f32[2]);
					tIceTrap.radius = 3.0f;
					m_pcBallComponent->m_tIceTraps.push_back(tIceTrap);
					m_pcBallComponent->m_fIceTrapsTimes.push_back(6.0f);
					m_fPrevRollTime = m_fRollTimer;
					m_pcSoundManager->PlaySoundFile(m_pcAssetManager->getSound(15));
				}
			}
			else
			{
				XMMATRIX ballMatrix = *m_pcBallComponent->GetWorldTransform();

				CSceneManager::TSceneObj* ptIceTrap = new CSceneManager::TSceneObj();
				ptIceTrap->nmodelId = MODELIDNUM;
				ptIceTrap->nTextureId = ICETEXTUREIDNUM;

				XMMATRIX scaled = XMMatrixMultiply(XMMatrixScaling(4.0f, 0.5f, 4.0f), ballMatrix);
				XMStoreFloat4x4(&ptIceTrap->worldMatrix, scaled);
				m_pcBallComponent->m_ptIceTrapModels.push_back(ptIceTrap);
				m_pcSceneManager->addObjectToScene(ptIceTrap, false);

				end::TSphere tIceTrap;
				tIceTrap.center = XMFLOAT3(scaled.r[3].m128_f32[0], scaled.r[3].m128_f32[1], scaled.r[3].m128_f32[2]);
				tIceTrap.radius = 3.0f;
				m_pcBallComponent->m_tIceTraps.push_back(tIceTrap);
				m_pcBallComponent->m_fIceTrapsTimes.push_back(6.0f);

				m_pcSceneManager->removeObjectFromScene(this);
				m_pcPhysicsManager->RemovePhysComponent(m_pcBallComponent);
				m_eStatus = eTRAP;
				m_pcSoundManager->PlaySoundFile(m_pcAssetManager->getSound(1));
			}

			//Update Roll Timer
			if (m_fRollTimer > 0)
			{
				m_fRollTimer -= fT;

				if (m_fRollTimer < 0)
					m_fRollTimer = 0;
			}
			break;
		}
		case eTRAP:
		{
			if (m_pcBallComponent->m_fIceTrapsTimes.size() == 0)
			{
				m_bIsActive = false;
			}

			//Update Roll Timer
			if (m_fRollTimer > 0)
			{
				m_fRollTimer -= fT;

				if (m_fRollTimer < 0)
					m_fRollTimer = 0;
			}
			break;
		}
		}
		switch (m_eBallType)
		{
		case eNORM:
		{
			if (m_nNumBounced > 0)
				m_pcOwner = nullptr;
			break;
		}
		case eFIRE:
		{
			if (m_nNumBounced > 0)
			{
				m_eStatus = eEXPLODING;
			}
			break;
		}
		case eICE:
		{
			//If your traps have melted away, remove them
			for (int j = 0; j < m_pcBallComponent->m_fIceTrapsTimes.size(); j++)
			{
				m_pcBallComponent->m_fIceTrapsTimes[j] -= fT;

				if (m_pcBallComponent->m_fIceTrapsTimes[j] <= 0.0f)
					m_pcBallComponent->m_fIceTrapsTimes[j] = 0.0f;

				if (m_pcBallComponent->m_fIceTrapsTimes[j] == 0.0f)
				{
					m_pcSceneManager->removeObjectFromScene(m_pcBallComponent->m_ptIceTrapModels[j]);
					m_pcBallComponent->m_ptIceTrapModels.erase(m_pcBallComponent->m_ptIceTrapModels.begin() + j);
					m_pcBallComponent->m_tIceTraps.erase(m_pcBallComponent->m_tIceTraps.begin() + j);
					m_pcBallComponent->m_fIceTrapsTimes.erase(m_pcBallComponent->m_fIceTrapsTimes.begin() + j);
					j--;
				}

			}

			if (m_nNumBounced > 0)
			{
				m_bIsActive = false;
			}
			break;
		}
		case eELECTRIC:
		{
			if (m_nNumBounced > 2)
			{
				m_pcTarget = nullptr;
				m_pcOwner = nullptr;
			}
			if (m_nNumBounced > ELECTRICMAXBOUNCES)
			{
				m_pcSoundManager->PlaySoundFile(m_pcAssetManager->getSound(1));
				m_bIsActive = false;
			}
		}
		}

		for (int i = 0; i < m_vSpawnedBalls.size(); i++)
		{
			if (this != m_vSpawnedBalls[i])
			{
				if (end::ClassifySphereToSphere(m_pcBallComponent->m_tBallCollider, m_vSpawnedBalls[i]->GetBallComponent()->m_tBallCollider) == 1)
				{
					m_pcBallComponent->m_tBallCollider.collided = true;
					m_vSpawnedBalls[i]->GetBallComponent()->m_tBallCollider.collided = true;

					XMVECTOR bounceVector = end::SphereToSphereLerp(m_pcBallComponent->m_tBallCollider, m_vSpawnedBalls[i]->GetBallComponent()->m_tBallCollider);
					XMVECTOR negateForce = XMVectorSet(m_vSpawnedBalls[i]->GetLevelOfForceVector().m128_f32[0], m_vSpawnedBalls[i]->GetLevelOfForceVector().m128_f32[1], m_vSpawnedBalls[i]->GetLevelOfForceVector().m128_f32[2], 1.0f);
					negateForce.m128_f32[0] += bounceVector.m128_f32[0];
					negateForce.m128_f32[1] += bounceVector.m128_f32[1];
					negateForce.m128_f32[2] += bounceVector.m128_f32[2];

					m_vSpawnedBalls[i]->SetLevelOfForceVector(negateForce);
					m_vSpawnedBalls[i]->GetBallComponent()->SetThrownStatus(false);
					//m_pcSoundManager->PlaySoundFile(m_pcAssetManager->getSound(23));

					//Disown owners if not activated
					if (m_vSpawnedBalls[i]->GetBallStatus() != eACTIVATED && m_vSpawnedBalls[i]->GetBallStatus() != eHELD)
					{
						m_vSpawnedBalls[i]->SetOwner(nullptr);
					}
					if (m_eStatus != eACTIVATED && m_eStatus != eHELD)
					{
						m_pcOwner = nullptr;
					}

					if (m_eStatus == eACTIVATED && m_vSpawnedBalls[i]->GetBallComponent()->m_ptIceTrapModels.size() == 0 && m_vSpawnedBalls[i]->GetOwner() != m_pcOwner)
					{
						XMVECTOR movingBallForceVector = m_vSpawnedBalls[i]->GetLevelOfForceVector();
						if (movingBallForceVector.m128_f32[2] == 0.0f)
						{
							XMFLOAT3 activatedPostion = m_pcBallComponent->GetPosition();
							XMFLOAT3 movingPosition = m_vSpawnedBalls[i]->GetBallComponent()->GetPosition();

							movingBallForceVector.m128_f32[0] = activatedPostion.x + movingPosition.x;
							//movingBallForceVector.m128_f32[1] = activatedPostion.y + movingPosition.y;
							movingBallForceVector.m128_f32[2] = activatedPostion.z + movingPosition.z;
						}
						else if (movingBallForceVector.m128_f32[2] > 0)
						{
							movingBallForceVector.m128_f32[2] = -movingBallForceVector.m128_f32[2];
							movingBallForceVector.m128_f32[2] += 15.0f;
						}
						else
						{
							movingBallForceVector.m128_f32[2] = -movingBallForceVector.m128_f32[2];
							movingBallForceVector.m128_f32[2] -= 15.0f;
						}

						m_vSpawnedBalls[i]->SetLevelOfForceVector(movingBallForceVector);
						m_pcOwner->IncreaseBlockCount();

						if (m_vSpawnedBalls[i]->GetBallType() == CBall::eFIRE)
						{
							m_vSpawnedBalls[i]->SetBallType(eNORM);
						}
					}
				}
				else
				{
					if (m_vSpawnedBalls[i]->GetBallStatus() == eIDLE)
					{
						m_vSpawnedBalls[i]->SetLevelOfForceVector(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));
					}
				}
			}
		}

		XMStoreFloat4x4(&worldMatrix, *m_pcBallComponent->GetWorldTransform());
	}
	m_cEffects.Update(m_cEffects.sorted, fT);
}

bool CBall::GetBlockableStat()
{
	return m_bBlockable;
}

bool CBall::GetCatchableStat()
{
	return m_bCatchable;
}

bool CBall::GetActiveStatus()
{
	return m_bIsActive;
}

void CBall::SetActiveStatus(bool bActiveStatus)
{
	m_bIsActive = false;
}

float CBall::GetThrownTime()
{
	return m_fThrownTimer;
}

void CBall::ResetThrownTime()
{
	m_fThrownTimer = FTHROWNTIME;
}

float CBall::GetExplosionTime()
{
	return m_fExplosionTimer;
}

float CBall::GetRollTime()
{
	return m_fRollTimer;
}

float CBall::GetPrevRollTime()
{
	return m_fPrevRollTime;
}

void CBall::SetPrevRollTime(float fNewTime)
{
	m_fPrevRollTime = fNewTime;
}

int CBall::GetNumBounced()
{
	return (int)m_nNumBounced;
}

void CBall::IncreaseNumBounced()
{
	m_nNumBounced++;
}

void CBall::ResetNumBounced()
{
	m_nNumBounced = 0;
}

CBall::eBallType CBall::GetBallType()
{
	return m_eBallType;
}

void CBall::SetBallType(CBall::eBallType eType)
{
	switch (eType)
	{
	case eNORM:
	{
		m_bBlockable = true;
		m_bCatchable = true;
		nTextureId = NORMTEXTUREIDNUM;
		break;
	}
	case eFIRE:
	{
		m_bBlockable = false;
		m_bCatchable = false;
		nTextureId = FIRETEXTUREIDNUM;
		m_fExplosionTimer = FEXPLOSIONTIME;
		break;
	}
	case eWIND:
	{
		m_bBlockable = false;
		m_bCatchable = false;
		nTextureId = WINDTEXTUREIDNUM;
		m_fActivationTimer = FBALLACTIVATIONTIME;
		break;
	}
	case eICE:
	{
		m_bBlockable = false;
		m_bCatchable = false;
		nTextureId = ICETEXTUREIDNUM;
		m_fRollTimer = FROLLTIME;
		m_fPrevRollTime = FROLLTIME;
		break;
	}
	case eELECTRIC:
	{
		m_bBlockable = true;
		m_bCatchable = true;
		nTextureId = ELECTRICTEXTUREIDNUM;
		break;
	}
	}
	m_eBallType = eType;
}

CBall::eBallStatus CBall::GetBallStatus()
{
	return m_eStatus;
}

void CBall::SetBallStatus(CBall::eBallStatus eStatus)
{
	m_eStatus = eStatus;
}

CPlayer * CBall::GetOwner()
{
	return m_pcOwner;
}

void CBall::SetOwner(CPlayer * pcOwner)
{
	m_pcOwner = pcOwner;
}

CPlayer * CBall::GetTarget()
{
	return m_pcTarget;
}

CPhysicsManager::cBallPhysComponent* CBall::GetBallComponent()
{
	return m_pcBallComponent;
}

XMVECTOR CBall::GetLevelOfForceVector()
{
	return m_fLevelOfForce;
}

void CBall::SetLevelOfForceVector(XMVECTOR fLevelOfForce)
{
	m_fLevelOfForce = fLevelOfForce;
}
