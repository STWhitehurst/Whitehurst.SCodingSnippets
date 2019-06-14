#include "CPlayer.h"
#include "CBall.h"




CPlayer::CPlayer()
{

}

CPlayer::CPlayer(XMFLOAT4X4 * pWorldTransform, enum ePlayerType ePlayerType, enum eGameType eCurrentGameType, enum eTeamSide eCurrentTeamSide, int nPlayerNumber, CUIManager * cUISystem, CPhysicsManager::CPhysicsManager* pcPhysics, CSceneManager* pcScene, CInputManager* pcInput, CSound* pcSound, CAssetManager* pcAsset, CAnimationManager* pcAnim)
{
	//Connect Systems, Classes, and containers
	m_cUISystem = cUISystem;
	m_pcInput = pcInput;
	m_pcPhysics = pcPhysics;
	m_pcScene = pcScene;
	m_pcSound = pcSound;
	m_pcAsset = pcAsset;
	m_pcAnim = pcAnim;

	//Set up Model and Player Stats
	nmodelId = CHARACTERMODELIDNUM;
	nTextureId = CHARACTERTEXTUREIDNUM;
	m_pcAnim->m_cRenderer = m_pcAsset;
	SetAnimated(m_pcAnim);
	PlayAnimation(CHARACTERIDLEIDNUM);
	m_nPlayerNum = nPlayerNumber - 1;
	worldMatrix = *pWorldTransform;
	m_fJumpForce = FJUMPFORCE;
	m_fJumpTimer = FJUMPTIME;
	m_fDodgeSpeed = FDODGESPEED;
	m_fDodgeTime = FDODGETIME;
	m_fDodgeForce = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_fBlockTimer = FBLOCKTIME;
	m_fCatchTimer = FCATCHTIME;
	m_fRespawnTimer = FRESPAWNTIME;
	m_fInvincibilityTimer = FINVCINCIBLETIME;
	m_fCoolDownTimer = 0.0f;
	m_fDodgeCoolDownTimer = 0.0f;
	m_fStunTimer = FSTUNTIME;
	m_fFrozenTimer = FFROZENTIMER;
	m_fStartDist = 0.0f;
	m_fCurDist = 0.0f;
	m_fDistFromPlanetoid = 0.0f;
	m_fAnimationTimer = 0.0f;
	m_eHandStatus = eEMPTY;
	m_eLegStatus = eIDLE;
	m_ePlayerType = ePlayerType;
	m_eCurrentGameType = eCurrentGameType;
	m_eTeamSide = eCurrentTeamSide;
	m_nPlayerNumber = (unsigned int)nPlayerNumber;
	m_nAirDodges = MAXAIRDODGE;
	m_nBlocks = 0;
	m_nDeaths = 0;
	m_nKills = 0;
	m_nLives = 0;
	m_nScore = 0;
	m_handPosition = XMVectorSet(pWorldTransform->_41 + 2.0f, pWorldTransform->_42 + 2.0f, pWorldTransform->_43, 1.0f);
	m_pcHeldBall = nullptr;
	m_pcActivatedPowerUp = nullptr;

	//Make a Character Physics Component
	m_pcCharacterComponent = new CPhysicsManager::cCharacterPhysComponent(XMLoadFloat4x4(pWorldTransform));
}

CPlayer::CPlayer(XMFLOAT4X4 * pWorldTransform, ePlayerType ePlayerType, eGameType eCurrentGameType, int nPlayerNumber, CUIManager * cUISystem, CPhysicsManager::CPhysicsManager * pcPhysics, CSceneManager * pcScene, CInputManager * pcInput, CSound * pcSound, CAssetManager * pcAsset, CAnimationManager* pcAnim)
{
	//Connect Systems, Classes, and containers
	m_cUISystem = cUISystem;
	m_pcInput = pcInput;
	m_pcPhysics = pcPhysics;
	m_pcScene = pcScene;
	m_pcSound = pcSound;
	m_pcAsset = pcAsset;
	m_pcAnim = pcAnim;

	//Set up Model and Player Stats
	nmodelId = CHARACTERMODELIDNUM;
	nTextureId = CHARACTERTEXTUREIDNUM;
	m_pcAnim->m_cRenderer = m_pcAsset;
	SetAnimated(m_pcAnim);
	PlayAnimation(CHARACTERIDLEIDNUM);
	m_nPlayerNum = nPlayerNumber - 1;
	worldMatrix = *pWorldTransform;
	m_fJumpForce = FJUMPFORCE;
	m_fJumpTimer = FJUMPTIME;
	m_fDodgeSpeed = FDODGESPEED;
	m_fDodgeTime = FDODGETIME;
	m_fDodgeForce = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_fBlockTimer = FBLOCKTIME;
	m_fCatchTimer = FCATCHTIME;
	m_fRespawnTimer = FRESPAWNTIME;
	m_fInvincibilityTimer = FINVCINCIBLETIME;
	m_fCoolDownTimer = 0.0f;
	m_fStunTimer = FSTUNTIME;
	m_fFrozenTimer = FFROZENTIMER;
	m_fStartDist = 0.0f;
	m_fCurDist = 0.0f;
	m_fDistFromPlanetoid = 0.0f;
	m_fAnimationTimer = 0.0f;
	m_eHandStatus = eEMPTY;
	m_eLegStatus = eIDLE;
	m_ePlayerType = ePlayerType;
	m_eCurrentGameType = eCurrentGameType;
	m_nPlayerNumber = (unsigned int)nPlayerNumber;
	m_nAirDodges = MAXAIRDODGE;
	m_nBlocks = 0;
	m_nDeaths = 0;
	m_nKills = 0;
	m_nLives = 0;
	m_nScore = 0;
	m_handPosition = XMVectorSet(pWorldTransform->_41 + 0.5f, pWorldTransform->_42 + 2.0f, pWorldTransform->_43, 1.0f);
	m_pcHeldBall = nullptr;
	m_pcActivatedPowerUp = nullptr;

	//Make a Character Physics Component
	m_pcCharacterComponent = new CPhysicsManager::cCharacterPhysComponent(XMLoadFloat4x4(pWorldTransform));
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update(float fT, vector<CPlayer*> Lobby, vector<CBall*> m_vSpawnedBalls, vector<CBarrier*> theBarriers, vector<CPowerUp*> thePowerUps, bool bControlsOn)
{
	m_cUISystem->SendPlayerInfo(m_nPlayerNumber, m_nScore);
	XMFLOAT3 fPosition = this->m_pcCharacterComponent->GetPosition();
	m_handPosition = XMVectorSet(fPosition.x + 0.5f, fPosition.y + 2.0f, fPosition.z, 1.0f);

	//Set Radius
	float radius = m_pcPhysics->m_tPlanetoidCollider.radius;
	//Fix Wall
	if (m_pcCharacterComponent->GetOnWallStatus())
	{
		radius = m_fDistFromPlanetoid;
	}

	//Depending on Player Type
	switch (m_ePlayerType)
	{
	case eUSER:
	{
		if (bControlsOn)
		{
			if ((m_pcInput->IsButtonPressed(m_nPlayerNumber - 1, G_XBOX_LEFT_SHOULDER_BTN)
				&& m_pcInput->IsButtonPressed(m_nPlayerNumber - 1, G_XBOX_LEFT_TRIGGER_AXIS)
				&& m_pcInput->IsButtonPressed(m_nPlayerNumber - 1, G_XBOX_START_BTN)
				&& m_pcInput->IsButtonPressed(m_nPlayerNumber - 1, G_XBOX_A_BTN))
				|| m_pcInput->IsKeyPressed(0x12) && m_pcInput->IsKeyPressed(0x39))
			{
				if (!m_bDebugInvinc)
				{
					m_bDebugInvinc = true;
					cout << "*Mario Invincibility song intensifies*" << "\n";
				}
			}
			else if ((m_pcInput->IsButtonPressed(m_nPlayerNumber - 1, G_XBOX_LEFT_SHOULDER_BTN)
				&& m_pcInput->IsButtonPressed(m_nPlayerNumber - 1, G_XBOX_LEFT_TRIGGER_AXIS)
				&& m_pcInput->IsButtonPressed(m_nPlayerNumber - 1, G_XBOX_START_BTN)
				&& m_pcInput->IsButtonPressed(m_nPlayerNumber - 1, G_XBOX_B_BTN))
				|| m_pcInput->IsKeyPressed(0x12) && m_pcInput->IsKeyPressed(0x3d))
			{
				if (m_bDebugInvinc)
				{
					m_bDebugInvinc = false;
					cout << "*Mario Invincibility song fades out*" << "\n";
				}
			}

			//If the Throw/Catch Button is Pressed
			if (m_pcInput->IsKeyPressed(0x64) || m_pcInput->IsControllerConnected(m_nPlayerNumber - 1) && m_pcInput->IsButtonPressed(m_nPlayerNumber - 1, G_XBOX_RIGHT_TRIGGER_AXIS)) {
				//If the Player has a ball in their hand and they aren't already catching
				if (m_pcHeldBall != nullptr && m_eHandStatus != CPlayer::eCATCHING)
				{
					//If the ball isn't a WIND ball, Throw it!
					if (m_pcHeldBall->GetBallType() != CBall::eWIND)
					{
						if (m_fCoolDownTimer == 0.0f && m_eHandStatus != eBLOCKING && m_eHandStatus != eCATCHING)
						{
							//Speed is dependent on the Ball type
							switch (m_pcHeldBall->GetBallType())
							{
							case CBall::eELECTRIC:
							{
								m_pcHeldBall->SetLevelOfForceVector(XMVectorSet(0.0f, 0.0f, (70.0f + m_fPowerUpThrowSpeed), 1.0f));
								break;
							}
							case CBall::eFIRE:
							{
								m_pcHeldBall->SetLevelOfForceVector(XMVectorSet(0.0f, 0.0f, (50.0f + m_fPowerUpThrowSpeed), 1.0f));
								break;
							}
							default:
							{
								m_pcHeldBall->SetLevelOfForceVector(XMVectorSet(0.0f, 0.0f, (45.0f + m_fPowerUpThrowSpeed), 1.0f));
								break;
							}
							}

							ThrowBall(fT);
							ResetCoolDownTimer();
							m_pcSound->PlaySoundFile(m_pcAsset->getSound(2));
						}
					}
					//If the ball is a WIND ball, Activate it
					else
					{
						m_pcHeldBall->SetBallStatus(CBall::eACTIVATED);
						m_pcHeldBall = nullptr;
						m_pcSound->PlaySoundFile(m_pcAsset->getSound(26));
					}
				}
				//If the Player doesn't have a ball
				else
				{
					//And they aren't catching or blocking, Set status to Catching, and the cooldown is done
					if (m_eHandStatus != CPlayer::eCATCHING && m_eHandStatus != CPlayer::eBLOCKING)
					{
						//If the cooldown timer is done
						if (m_fCoolDownTimer <= 0.0f)
						{
							m_eHandStatus = eCATCHING;
							ResetCatchTimer();
						}
					}
				}
			}
			//If the Player doesn't have a ball
			else
			{
				//If the Player doesn't have a ball in their hands
				if (m_pcHeldBall == nullptr)
				{
					//If the Player's Hand Status isn't EMPTY, set it
					if (m_eHandStatus != CPlayer::eEMPTY)
					{
						m_eHandStatus = eEMPTY;
					}
				}
			}

			//If the Block Button is Pressed
			if (m_pcInput->IsKeyPressed(0x65) || m_pcInput->IsControllerConnected(m_nPlayerNumber - 1) && m_pcInput->IsButtonPressed(m_nPlayerNumber - 1, G_XBOX_LEFT_TRIGGER_AXIS))
			{
				//If the Player has a ball in it's hands and they aren't already blocking
				if (m_pcHeldBall && m_eHandStatus != CPlayer::eBLOCKING)
				{
					//If the incoming ball isn't an ICE ball
					if (m_pcHeldBall->GetBallType() != CBall::eICE)
					{
						//If the cooldown timer is done
						if (m_fCoolDownTimer <= 0.0f)
						{
							//Set/Reset Player's and Ball Status
							m_eHandStatus = eBLOCKING;

							ResetBlockTimer();
						}
					}

					//If the ball in hand is an ICE ball
					else
					{
						XMMATRIX playerTransform = *m_pcHeldBall->GetBallComponent()->GetWorldTransform();
						XMMATRIX newWorldZTransform = XMMatrixMultiply(XMMatrixTranslation(0.0f, -2.0f, 0.0f), playerTransform);
						XMMATRIX newWorldXTransform = XMMatrixMultiply(XMMatrixTranslation(-2.0f, 0.0f, 0.0f), newWorldZTransform);
						m_pcHeldBall->GetBallComponent()->SetWorldTransform(newWorldXTransform);

						m_pcHeldBall->SetLevelOfForceVector(XMVectorSet(0.0f, 0.0f, 30.0f, 1.0f));
						RollBall(fT);
						ResetCoolDownTimer();
					}
				}
			}
			else
			{
				//If the player has a ball
				if (m_pcHeldBall)
				{
					//If the player Status isn't set to holding
					if (m_eHandStatus != CPlayer::eHOLDING)
					{
						//If the ball hasn't been activated
						if (m_pcHeldBall->GetBallStatus() != CBall::eACTIVATED)
						{
							//Set/Reset Player's and their ball's statuses
							m_pcHeldBall->SetBallStatus(CBall::eHELD);
							m_eHandStatus = eHOLDING;
						}
						//If the ball has been activated
						if (m_pcHeldBall->GetBallStatus() == CBall::eACTIVATED)
						{
							//Set Status so it is following you
							m_pcHeldBall->SetBallStatus(CBall::eHELD);
						}
					}
				}
			}

			//If the Player is Frozen
			if (m_eLegStatus == CPlayer::eFROZEN)
			{
				//If the A key is pressed || Move the Left Analog Stick to the Left
				if (m_pcInput->IsKeyPressed(0x27) || m_pcInput->IsControllerConnected(m_nPlayerNumber - 1) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNumber - 1, G_XBOX_LX_AXIS) < -0.85f)
				{
					IncreaseUnFreezeCount();
				}

				//If the D key is pressed || Move the Left Analog Stick to the Right
				if (m_pcInput->IsKeyPressed(0x2a) || m_pcInput->IsControllerConnected(m_nPlayerNumber - 1) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNumber - 1, G_XBOX_LX_AXIS) > 0.85f) //This is the D key
				{
					IncreaseUnFreezeCount();
				}
			}

			if (m_eLegStatus != CPlayer::eLEGRESPAWNING && m_eLegStatus != CPlayer::eFROZEN && m_eLegStatus != CPlayer::eSTUNNED)
			{
				//If you are not dodging
				if (m_eLegStatus != CPlayer::eDODGING)
				{
					//Doding Checks and Applicants
					//Up Arrow
					if (m_fDodgeCoolDownTimer == 0.0f && (m_pcInput->IsKeyPressed(0x1b) || (m_pcInput->IsKeyPressed(0x3d) && (m_pcInput->IsKeyPressed(0xd) || m_pcInput->IsKeyPressed(0x12)))
						|| m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_DPAD_UP_BTN)
						|| m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNum, G_XBOX_LY_AXIS) > 0.0f
						&& (m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_X_BTN) || m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_LEFT_SHOULDER_BTN) || m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_RIGHT_SHOULDER_BTN)))) //up arrow
					{
						XMVECTOR vdiagonal = XMVectorSet(0.0f, 1.0f, 1.0f, 1.0f);
						SetLegStatus(CPlayer::eDODGING);
						SetDodgeForce(vdiagonal);
						SetStartDistance(end::Distance(end::turnToFloat3(m_pcCharacterComponent->GetWorldTransform()->r[3]), m_pcPhysics->m_tPlanetoidCollider.center));
						m_pcSound->PlaySoundFile(m_pcAsset->getSound(2));
						if (!m_bPowerUpDodge)
							m_fDodgeCoolDownTimer = FDODGECOOLDOWNTIMER;
					}
					//DownArrow
					else if (m_fDodgeCoolDownTimer == 0.0f && (m_pcInput->IsKeyPressed(0x23) || (m_pcInput->IsKeyPressed(0x39) && (m_pcInput->IsKeyPressed(0xd) || m_pcInput->IsKeyPressed(0x12)))
						|| m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_DPAD_DOWN_BTN)
						|| m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNum, G_XBOX_LY_AXIS) < 0.0f
						&& (m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_X_BTN) || m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_LEFT_SHOULDER_BTN) || m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_RIGHT_SHOULDER_BTN))))//down arrow
					{
						XMVECTOR vdiagonal = XMVectorSet(0.0f, 1.0f, -1.0f, 1.0f);
						SetLegStatus(CPlayer::eDODGING);
						SetDodgeForce(vdiagonal);
						SetStartDistance(end::Distance(end::turnToFloat3(m_pcCharacterComponent->GetWorldTransform()->r[3]), m_pcPhysics->m_tPlanetoidCollider.center));
						m_pcSound->PlaySoundFile(m_pcAsset->getSound(2));
						if (!m_bPowerUpDodge)
							m_fDodgeCoolDownTimer = FDODGECOOLDOWNTIMER;
					}
					//Left Arrow
					else if (m_fDodgeCoolDownTimer == 0.0f && (m_pcInput->IsKeyPressed(0x1e) || (m_pcInput->IsKeyPressed(0x27) && (m_pcInput->IsKeyPressed(0xd) || m_pcInput->IsKeyPressed(0x12))) || m_pcInput->IsKeyPressed(0x37)
						|| m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_DPAD_LEFT_BTN)
						|| m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNum, G_XBOX_LX_AXIS) < 0.0f
						&& (m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_X_BTN) || m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_LEFT_SHOULDER_BTN) || m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_RIGHT_SHOULDER_BTN)))) //left arrow
					{
						XMVECTOR vdiagonal = XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f);
						SetLegStatus(CPlayer::eDODGING);
						SetDodgeForce(vdiagonal);
						SetStartDistance(end::Distance(end::turnToFloat3(m_pcCharacterComponent->GetWorldTransform()->r[3]), m_pcPhysics->m_tPlanetoidCollider.center));
						m_pcSound->PlaySoundFile(m_pcAsset->getSound(2));
						if (!m_bPowerUpDodge)
							m_fDodgeCoolDownTimer = FDODGECOOLDOWNTIMER;
					}
					//Right Arrow
					else if (m_fDodgeCoolDownTimer == 0.0f && (m_pcInput->IsKeyPressed(0x20) || (m_pcInput->IsKeyPressed(0x2a) && (m_pcInput->IsKeyPressed(0xd) || m_pcInput->IsKeyPressed(0x12))) || m_pcInput->IsKeyPressed(0x2b)
						|| m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_DPAD_RIGHT_BTN)
						|| m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNum, G_XBOX_LX_AXIS) > 0.0f
						&& (m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_X_BTN) || m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_LEFT_SHOULDER_BTN) || m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_RIGHT_SHOULDER_BTN)))) //right arrow
					{
						XMVECTOR vdiagonal = XMVectorSet(-1.0f, 1.0f, 0.0f, 1.0f);
						SetLegStatus(CPlayer::eDODGING);
						SetDodgeForce(vdiagonal);
						SetStartDistance(end::Distance(end::turnToFloat3(m_pcCharacterComponent->GetWorldTransform()->r[3]), m_pcPhysics->m_tPlanetoidCollider.center));
						m_pcSound->PlaySoundFile(m_pcAsset->getSound(2));
						if (!m_bPowerUpDodge)
							m_fDodgeCoolDownTimer = FDODGECOOLDOWNTIMER;
					}

					//Camera Movement
					if (m_pcInput->GetMouseXDelta() < 0.0f || m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNum, G_XBOX_RX_AXIS) < 0.0f)
					{
						XMMATRIX newWorldTransform = XMMatrixRotationAxis(XMVECTOR{ m_pcCharacterComponent->GetUp().x, m_pcCharacterComponent->GetUp().y, m_pcCharacterComponent->GetUp().z, m_pcCharacterComponent->GetUp().w }, fT * 2.0f);
						XMVECTOR vPrevW = XMVectorSet(m_pcCharacterComponent->GetWorldTransform()->r[3].m128_f32[0], m_pcCharacterComponent->GetWorldTransform()->r[3].m128_f32[1], m_pcCharacterComponent->GetWorldTransform()->r[3].m128_f32[2], m_pcCharacterComponent->GetWorldTransform()->r[3].m128_f32[3]);

						m_pcCharacterComponent->GetWorldTransform()->r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
						m_pcCharacterComponent->SetWorldTransform(XMMatrixMultiply(*m_pcCharacterComponent->GetWorldTransform(), newWorldTransform));
						m_pcCharacterComponent->GetWorldTransform()->r[3] = vPrevW;
					}

					if (m_pcInput->GetMouseXDelta() > 0.0f || m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNum, G_XBOX_RX_AXIS) > 0.0f)
					{
						XMMATRIX newWorldTransform = XMMatrixRotationAxis(XMVECTOR{ m_pcCharacterComponent->GetUp().x, m_pcCharacterComponent->GetUp().y, m_pcCharacterComponent->GetUp().z, m_pcCharacterComponent->GetUp().w }, fT * -2.0f);
						XMVECTOR vPrevW = XMVectorSet(m_pcCharacterComponent->GetWorldTransform()->r[3].m128_f32[0], m_pcCharacterComponent->GetWorldTransform()->r[3].m128_f32[1], m_pcCharacterComponent->GetWorldTransform()->r[3].m128_f32[2], m_pcCharacterComponent->GetWorldTransform()->r[3].m128_f32[3]);

						m_pcCharacterComponent->GetWorldTransform()->r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
						m_pcCharacterComponent->SetWorldTransform(XMMatrixMultiply(*m_pcCharacterComponent->GetWorldTransform(), newWorldTransform));
						m_pcCharacterComponent->GetWorldTransform()->r[3] = vPrevW;
					}

					//Player Movement
					//Another Insanity Check 
					if (m_eLegStatus != CPlayer::eDODGING)
					{
						//If the W key is pressed 
						if (m_pcInput->IsKeyPressed(0x3d) || m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNum, G_XBOX_LY_AXIS) > 0.0f)
						{
							XMMATRIX newWorldTransform = XMMatrixMultiply(XMMatrixTranslation(0.0f, 0.0f, (fT * (10.0f + m_fPowerUpRunSpeed))), *m_pcCharacterComponent->GetWorldTransform());
							m_pcCharacterComponent->SetWorldTransform(newWorldTransform);
							if (m_eLegStatus != CPlayer::eJUMPING)
							{
								m_eLegStatus = eRUNNING;
							}
						}
						//If the S key is pressed
						if (m_pcInput->IsKeyPressed(0x39) || m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNum, G_XBOX_LY_AXIS) < 0.0f)
						{
							XMMATRIX newWorldTransform = XMMatrixMultiply(XMMatrixTranslation(0.0f, 0.0f, (fT * (-10.0f - m_fPowerUpRunSpeed))), *m_pcCharacterComponent->GetWorldTransform());
							m_pcCharacterComponent->SetWorldTransform(newWorldTransform);
							if (m_eLegStatus != CPlayer::eJUMPING)
							{
								m_eLegStatus = CPlayer::eRUNNING;
							}
						}
						//If the A key is pressed
						if (m_pcInput->IsKeyPressed(0x27) || m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNum, G_XBOX_LX_AXIS) < 0.0f)
						{
							XMMATRIX newWorldTransform = XMMatrixMultiply(XMMatrixTranslation((fT * (10.0f + m_fPowerUpRunSpeed)), 0.0f, 0.0f), *m_pcCharacterComponent->GetWorldTransform());
							m_pcCharacterComponent->SetWorldTransform(newWorldTransform);
							if (m_eLegStatus != CPlayer::eJUMPING)
							{
								m_eLegStatus = CPlayer::eRUNNING;
							}
						}
						//If the D key is pressed
						if (m_pcInput->IsKeyPressed(0x2a) || m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNum, G_XBOX_LX_AXIS) > 0.0f)
						{
							XMMATRIX newWorldTransform = XMMatrixMultiply(XMMatrixTranslation((fT * (-10.0f - m_fPowerUpRunSpeed)), 0.0f, 0.0f), *m_pcCharacterComponent->GetWorldTransform());
							m_pcCharacterComponent->SetWorldTransform(newWorldTransform);
							if (m_eLegStatus != CPlayer::eJUMPING)
							{
								m_eLegStatus = CPlayer::eRUNNING;
							}
						}

						//If Not Moving 
						if (!m_pcInput->IsKeyPressed(G_KEY_W) && !m_pcInput->IsKeyPressed(G_KEY_S) && !m_pcInput->IsKeyPressed(G_KEY_A) && !m_pcInput->IsKeyPressed(G_KEY_D) && m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNum, G_XBOX_LY_AXIS == 0.0f) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNum, G_XBOX_LX_AXIS) == 0.0f && m_eLegStatus != CPlayer::eJUMPING && m_eLegStatus != CPlayer::eDODGING)
						{
							m_eLegStatus = CPlayer::eIDLE;
						}

						//If the Space Bar is pressed || If A Button is Pressed
						//Jump
						if (m_pcInput->IsKeyPressed(0x16) || m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_A_BTN)) //this is the space bar
						{
							if (m_eLegStatus != CPlayer::eJUMPING)
							{
								m_eLegStatus = CPlayer::eJUMPING;
								m_pcSound->PlaySoundFile(m_pcAsset->getSound(25));
							}
						}
					}
				}
			}
		}

#pragma region Animations
		if (m_eLegStatus == eDODGING)
		{
			if (m_cAnimPlayer->GetAnimationID() != CHARACTERDODGEIDNUM)
			{
				PlayAnimation(CHARACTERDODGEIDNUM);
			}
		}
		else if ((m_eLegStatus == eRUNNING || m_eLegStatus == eIDLE) && m_bPickUp)
		{
			if (m_cAnimPlayer->GetAnimationID() != CHARACTERPICKUPIDNUM)
			{
				PlayAnimation(CHARACTERPICKUPIDNUM);
			}

			if (m_fAnimationTimer <= 0.0f)
				m_bPickUp = false;
		}
		else if ((m_eLegStatus == eRUNNING || m_eLegStatus == eIDLE) && m_bRolling)
		{
			if (m_cAnimPlayer->GetAnimationID() != CHARACTERROLLIDNUM)
			{
				PlayAnimation(CHARACTERROLLIDNUM);
			}

			if (m_fAnimationTimer <= 0.0f)
				m_bRolling = false;
		}
		else if ((m_eLegStatus == eRUNNING || m_eLegStatus == eIDLE || m_eLegStatus == eJUMPING) && m_bThrowing)
		{
			if (m_cAnimPlayer->GetAnimationID() != CHARACTERTHROWIDNUM)
			{
				PlayAnimation(CHARACTERTHROWIDNUM);
			}

			if (m_fAnimationTimer <= 0.0) {
				m_bThrowing = false;
			}
			else if (m_fAnimationTimer <= 0.4f || m_eLegStatus == eJUMPING)
			{
				
				if (this->m_pcHeldBall != nullptr)
				{
					for (int i = 0; i < (int)theBarriers.size(); i++)
					{
						if (end::classify_sphere_to_wall(*theBarriers[i]->GetWallCollider(), this->m_pcHeldBall->GetBallComponent()->m_tBallCollider) != 1)
						{
							this->m_pcHeldBall->SetBallStatus(CBall::eTHROWN);
							
							//Save Ball current position and time for a later check
							XMMATRIX ballMatrix = *this->m_pcHeldBall->GetBallComponent()->GetWorldTransform();
							this->m_pcHeldBall->ResetThrownTime();

							//Set Physics IsThrown variable
							this->m_pcHeldBall->GetBallComponent()->SetThrownStatus(true);

							//Detach Ball from Player
							this->m_pcHeldBall = nullptr;
							this->m_eHandStatus = eEMPTY;

							if (m_eLegStatus == eJUMPING)
								m_bThrowing = false;

							break;
						}
					}
				}
			}
		}
		else if (m_eLegStatus == eRUNNING)
		{
			if (m_cAnimPlayer->GetAnimationID() != CHARACTERRUNIDNUM)
			{
				PlayAnimation(CHARACTERRUNIDNUM);
			}
		}
		else if ((m_eLegStatus == eRUNNING || m_eLegStatus == eIDLE) && m_eHandStatus == eCATCHING)
		{
			if (m_cAnimPlayer->GetAnimationID() != CHARACTERCATCHIDNUM)
			{
				PlayAnimation(CHARACTERCATCHIDNUM);
			}
		}
		else if ((m_eLegStatus == eRUNNING || m_eLegStatus == eIDLE) && m_eHandStatus == eBLOCKING)
		{
			if (m_cAnimPlayer->GetAnimationID() != CHARACTERBLOCKIDNUM)
			{
				PlayAnimation(CHARACTERBLOCKIDNUM);
			}
		}
		else if (m_eLegStatus == eIDLE)
		{
			if (m_cAnimPlayer->GetAnimationID() != CHARACTERIDLEIDNUM)
			{
				PlayAnimation(CHARACTERIDLEIDNUM);
			}
		}
		else if (m_eLegStatus == eJUMPING)
		{
			if (m_cAnimPlayer->GetAnimationID() != CHARACTERJUMPIDNUM)
			{
				PlayAnimation(CHARACTERJUMPIDNUM);
			}
		}
		else if (m_eLegStatus == eLEGRESPAWNING)
		{
			if (m_cAnimPlayer->GetAnimationID() != CHARACTERDEATHIDNUM)
			{
				PlayAnimation(CHARACTERDEATHIDNUM);
			}
		}
		else if (m_eLegStatus == eSTUNNED || m_eLegStatus == eFROZEN)
		{
			if (m_cAnimPlayer->GetAnimationID() != CHARACTERLOSEIDNUM)
			{
				PlayAnimation(CHARACTERLOSEIDNUM);
			}
		}
#pragma endregion

		break;
	}
	}

	//Depending on Hand Status
	switch (m_eHandStatus)
	{
	case eBLOCKING:
	{
		if (m_pcHeldBall)
		{
			m_pcHeldBall->SetBallStatus(CBall::eBLOCKING);
			m_pcCharacterComponent->m_bBC = true;

			if (m_fBlockTimer > 0.0f)
			{
				m_fBlockTimer -= fT;

				if (m_fBlockTimer < 0.0f)
					m_fBlockTimer = 0.0f;
			}

			if (m_fBlockTimer <= 0.0f)
			{
				m_pcHeldBall->SetBallStatus(CBall::eHELD);
				m_eHandStatus = eHOLDING;
				m_pcCharacterComponent->m_bBC = false;
				m_fCoolDownTimer = FCOOLDOWNTIMER;
			}
			break;
		}
	}
	case eCATCHING:
	{
		m_pcCharacterComponent->m_bBC = true;
		if (m_fCatchTimer > 0.0f)
		{
			m_fCatchTimer -= fT;

			if (m_fCatchTimer < 0.0f)
				m_fCatchTimer = 0.0f;
		}

		if (m_fCatchTimer <= 0.0f)
		{
			m_eHandStatus = eEMPTY;
			m_pcCharacterComponent->m_bBC = false;
			m_fCoolDownTimer = FCOOLDOWNTIMER;
		}
		break;
	}
	case eHOLDING:
	{
		if (m_pcHeldBall)
			m_pcHeldBall->SetBallStatus(CBall::eHELD);
		break;
	}
	}
	//Turn on Block Pill
	if (m_eHandStatus != eBLOCKING && m_eHandStatus != eCATCHING)
		m_pcCharacterComponent->m_bBC = false;

	//Depending on Leg Status
	switch (m_eLegStatus)
	{
	case eJUMPING:
	{
		XMMATRIX newWorldTransform = XMMatrixMultiply(XMMatrixTranslation(0.0f, fT * (m_fJumpForce + m_fPowerUpJumpSpeed) * 1.5f, 0.0f), *m_pcCharacterComponent->GetWorldTransform());
		m_pcCharacterComponent->SetWorldTransform(newWorldTransform);

		m_fJumpForce -= GRAVITYBASE * fT;

		if (m_fJumpTimer > 0.0f)
		{
			m_fJumpTimer -= fT;

			if (m_fJumpTimer < 0.0f)
				m_fJumpTimer = 0.0f;
		}

		//If enough time has passed since you jumped
		if (m_fJumpTimer <= 0.0f)
		{
			//Check Player's distance from the Planetoid, if on the ground, reset stats and set Player to IDLE
			if (m_fDistFromPlanetoid <= radius)
			{
				SetLegStatus(eIDLE);
				ResetJumpForce();
				ResetJumpTimer();
			}
		}

		break;
	}
	case eDODGING:
	{
		XMMATRIX newWorldTransform = XMMatrixMultiply(
			XMMatrixTranslation(fT * m_fDodgeForce.m128_f32[0] * m_fDodgeSpeed, fT * m_fDodgeForce.m128_f32[1] * m_fDodgeSpeed, fT * m_fDodgeForce.m128_f32[2] * m_fDodgeSpeed),
			*m_pcCharacterComponent->GetWorldTransform());

		if (m_fStartDist > m_fCurDist)
		{
			newWorldTransform = XMMatrixMultiply(
				XMMatrixTranslation(fT * m_fDodgeForce.m128_f32[0], fT * m_fDodgeForce.m128_f32[1], fT * m_fDodgeForce.m128_f32[2]),
				*m_pcCharacterComponent->GetWorldTransform());
		}
		m_pcCharacterComponent->SetWorldTransform(newWorldTransform);

		m_fDodgeForce.m128_f32[1] -= GRAVITYBASE * fT;

		if (m_fDodgeTime > 0.0f)
		{
			m_fDodgeTime -= fT;
			if (m_fDodgeTime < 0.0f)
				m_fDodgeTime = 0.0f;
		}

		//If enough time has passed since you dodged
		if (m_fDodgeTime <= 0.0f)
		{
			//Check Player's distance from the Planetoid, if on the ground, reset stats and set Player to IDLE
			if (m_fDistFromPlanetoid <= radius)
			{
				SetLegStatus(CPlayer::eIDLE);
				ResetDodgeTimer();
				SetDodgeForce(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
			}
		}
		break;
	}
	case eLEGRESPAWNING:
	{
		m_eHandStatus = eHANDRESPAWNING;

		if (m_fRespawnTimer > 0.0f)
		{
			m_fRespawnTimer -= fT;

			if (m_fRespawnTimer < 0.0f)
				m_fRespawnTimer = 0.0f;
		}
		break;
	}
	case eSTUNNED:
	{
		if (m_fStunTimer > 0.0f)
		{
			m_fStunTimer -= fT;

			if (m_fStunTimer < 0.0f)
				m_fStunTimer = 0.0f;
		}

		if (m_fStunTimer <= 0.0f)
		{
			m_eLegStatus = eIDLE;
			ResetStunTimer();

			//If you have an electricity
			if (m_pcHeldBall != nullptr && m_pcHeldBall->GetBallType() == CBall::eELECTRIC)
			{
				m_pcHeldBall->SetBallType(CBall::eNORM);
			}
		}
		break;
	}
	case eFROZEN:
	{
		if (m_fFrozenTimer > 0.0f)
		{
			m_fFrozenTimer -= fT;

			if (m_fFrozenTimer < 0.0f)
				m_fFrozenTimer = 0.0f;
		}

		if (m_fFrozenTimer <= 0.0f || m_nUnFreezeCounter > NNUMTOUNFREEZE)
		{
			m_eLegStatus = eIDLE;
			ResetUnFreezeCount();
			ResetFrozenTimer();
		}
		break;
	}
	}

	if (m_bInvincibility)
	{
		if (m_fInvincibilityTimer > 0.0f && m_eLegStatus != eLEGRESPAWNING)
		{
			m_fInvincibilityTimer -= fT;
			if (m_fInvincibilityTimer < 0.0f)
				m_fInvincibilityTimer = 0.0f;
		}

		if (m_fInvincibilityTimer == 0.0f)
		{
			m_bInvincibility = false;
			m_eLegStatus = eIDLE;
			ResetInvincibilityTimer();
		}
	}

	if (m_eLegStatus != eLEGRESPAWNING && m_eLegStatus != eSTUNNED && m_eLegStatus != eFROZEN && !m_bInvincibility)
	{
		m_pcCharacterComponent->SetPlayersFrustum();

		//Check against each spawned Ball for collision
		for (int i = 0; i < m_vSpawnedBalls.size(); i++)
		{
			if (classify_sphere_to_pill(m_vSpawnedBalls[i]->GetBallComponent()->m_tBallCollider, m_pcCharacterComponent->m_tCharacterCollider) == 1 && !m_bInvincibility)
			{
				m_pcCharacterComponent->m_tCharacterCollider.collided = true;
				m_vSpawnedBalls[i]->GetBallComponent()->m_tBallCollider.collided = true;

				switch (m_eCurrentGameType)
				{
				case eSOLO:
				{
					//If the incoming ball is not yours, and it has been activated
					if (m_vSpawnedBalls[i]->GetOwner() != nullptr && m_vSpawnedBalls[i]->GetOwner() != this && m_vSpawnedBalls[i]->GetBallStatus() == CBall::eACTIVATED)
					{
						//If you have a ball, drop it.
						if (m_pcHeldBall != nullptr)
						{
							m_pcHeldBall->SetOwner(nullptr);
							m_pcHeldBall->SetBallStatus(CBall::eIDLE);
						}

						m_pcHeldBall = nullptr;
						m_eHandStatus = eEMPTY;
						continue;
					}

					//If the incoming ball is not yours, and it has been thrown
					if (m_vSpawnedBalls[i]->GetOwner() != this && m_vSpawnedBalls[i]->GetOwner() != nullptr && m_vSpawnedBalls[i]->GetBallStatus() == CBall::eTHROWN)
					{
						//If the Ball is Fire, Explode!
						if (m_vSpawnedBalls[i]->GetBallType() == CBall::eFIRE && m_vSpawnedBalls[i]->GetBallStatus() != CBall::eEXPLODING)
						{
							if (m_vSpawnedBalls[i]->GetBallStatus() != CBall::eEXPLODING)
							{
								m_vSpawnedBalls[i]->SetBallStatus(CBall::eEXPLODING);
								m_pcSound->PlaySoundFile(m_pcAsset->getSound(20));
							}
						}

						//If the Ball is Ice, Freeze Them!
						if (m_vSpawnedBalls[i]->GetBallType() == CBall::eICE)
						{
							m_eLegStatus = CPlayer::eFROZEN;
							m_vSpawnedBalls[i]->SetActiveStatus(false);
							m_pcSound->PlaySoundFile(m_pcAsset->getSound(16));
							m_pcSound->PlaySoundFile(m_pcAsset->getSound(15));
						}

						if (m_vSpawnedBalls[i]->GetBallType() == CBall::eELECTRIC)
						{
							if (m_vSpawnedBalls[i]->GetTarget() == this)
							{
								m_vSpawnedBalls[i]->SetActiveStatus(false);
							}
							m_pcSound->PlaySoundFile(m_pcAsset->getSound(22));
						}

						if (m_vSpawnedBalls[i]->GetBallType() == CBall::eNORM)
						{
							m_pcSound->PlaySoundFile(m_pcAsset->getSound(18));
						}

						//Respawn the player and apply gravity
						m_vSpawnedBalls[i]->GetBallComponent()->SetThrownStatus(false);
						XMVECTOR negateForce = XMVectorSet(-m_vSpawnedBalls[i]->GetLevelOfForceVector().m128_f32[0], -m_vSpawnedBalls[i]->GetLevelOfForceVector().m128_f32[1], -m_vSpawnedBalls[i]->GetLevelOfForceVector().m128_f32[2], 1.0f);

						if (negateForce.m128_f32[2] > 0.0f)
							negateForce.m128_f32[2] -= 15.0f;
						if (negateForce.m128_f32[2] < 0.0f)
							negateForce.m128_f32[2] += 15.0f;

						m_vSpawnedBalls[i]->SetLevelOfForceVector(negateForce);
						m_eLegStatus = eLEGRESPAWNING;
						m_bRespawnStatus = true;
						m_pcSound->PlaySoundFile(m_pcAsset->getSound(3));

						//Increase kills for the ball's owner
						m_vSpawnedBalls[i]->GetOwner()->IncreaseKillCount();
						m_vSpawnedBalls[i]->GetOwner()->IncreaseScoreCount(4);

						//Increase the deaths for the player
						IncreaseDeathCount();

						continue;
					}

					//If the Ball is an ice ball and it isn't yours
					if (m_vSpawnedBalls[i]->GetBallType() == CBall::eICE && m_vSpawnedBalls[i]->GetOwner() != this)
					{
						//If the Ball has ice traps
						if (m_vSpawnedBalls[i]->GetBallComponent()->m_tIceTraps.size() > 0)
						{
							for (int k = 0; k < m_vSpawnedBalls[i]->GetBallComponent()->m_tIceTraps.size(); k++)
							{
								if (end::classify_sphere_to_pill(m_vSpawnedBalls[i]->GetBallComponent()->m_tIceTraps[k], m_pcCharacterComponent->m_tCharacterCollider) == 1)
								{
									m_eLegStatus = CPlayer::eFROZEN;
									m_pcScene->removeObjectFromScene(m_vSpawnedBalls[i]->GetBallComponent()->m_ptIceTrapModels[k]);
									m_vSpawnedBalls[i]->GetBallComponent()->m_ptIceTrapModels.erase(m_vSpawnedBalls[i]->GetBallComponent()->m_ptIceTrapModels.begin() + k);
									m_vSpawnedBalls[i]->GetBallComponent()->m_tIceTraps.erase(m_vSpawnedBalls[i]->GetBallComponent()->m_tIceTraps.begin() + k);
									m_vSpawnedBalls[i]->GetBallComponent()->m_fIceTrapsTimes.erase(m_vSpawnedBalls[i]->GetBallComponent()->m_fIceTrapsTimes.begin() + k);
									m_pcSound->PlaySoundFile(m_pcAsset->getSound(15));
									continue;
								}
							}
						}
					}
				}
				break;
				case eTEAM:
				{
					//If the incoming ball is not on your team, and it has been activated
					if (m_vSpawnedBalls[i]->GetOwner() != nullptr && m_vSpawnedBalls[i]->GetOwner()->GetPlayerSide() != m_eTeamSide && m_vSpawnedBalls[i]->GetBallStatus() == CBall::eACTIVATED)
					{
						//If you have a ball, drop it.
						if (m_pcHeldBall != nullptr)
						{
							m_pcHeldBall->SetOwner(nullptr);
							m_pcHeldBall->SetBallStatus(CBall::eIDLE);
						}

						m_pcHeldBall = nullptr;
						m_eHandStatus = eEMPTY;
						continue;
					}

					//If the incoming ball is not on your team, and it has been thrown
					if (m_vSpawnedBalls[i]->GetOwner() != nullptr && m_vSpawnedBalls[i]->GetOwner()->GetPlayerSide() != m_eTeamSide && m_vSpawnedBalls[i]->GetBallStatus() == CBall::eTHROWN)
					{
						//If the Ball is Fire, Explode!
						if (m_vSpawnedBalls[i]->GetBallType() == CBall::eFIRE && m_vSpawnedBalls[i]->GetBallStatus() != CBall::eEXPLODING)
						{
							if (m_vSpawnedBalls[i]->GetBallStatus() != CBall::eEXPLODING)
							{
								m_vSpawnedBalls[i]->SetBallStatus(CBall::eEXPLODING);
								m_pcSound->PlaySoundFile(m_pcAsset->getSound(20));
							}
						}

						//If the Ball is Ice, Freeze Them!
						if (m_vSpawnedBalls[i]->GetBallType() == CBall::eICE)
						{
							m_eLegStatus = CPlayer::eFROZEN;
							m_vSpawnedBalls[i]->SetActiveStatus(false);
							m_pcSound->PlaySoundFile(m_pcAsset->getSound(16));
							m_pcSound->PlaySoundFile(m_pcAsset->getSound(15));
						}

						//Respawn the player and apply gravity
						m_vSpawnedBalls[i]->GetBallComponent()->SetThrownStatus(false);
						XMVECTOR negateForce = XMVectorSet(-m_vSpawnedBalls[i]->GetLevelOfForceVector().m128_f32[0], -m_vSpawnedBalls[i]->GetLevelOfForceVector().m128_f32[1], -m_vSpawnedBalls[i]->GetLevelOfForceVector().m128_f32[2], 1.0f);

						if (negateForce.m128_f32[2] > 0.0f)
							negateForce.m128_f32[2] -= 15.0f;
						if (negateForce.m128_f32[2] < 0.0f)
							negateForce.m128_f32[2] += 15.0f;

						m_vSpawnedBalls[i]->SetLevelOfForceVector(negateForce);
						m_vSpawnedBalls[i]->IncreaseNumBounced();
						m_eLegStatus = eLEGRESPAWNING;
						m_bRespawnStatus = true;
						m_pcSound->PlaySoundFile(m_pcAsset->getSound(3));

						//Increase kills for the ball's owner
						m_vSpawnedBalls[i]->GetOwner()->IncreaseKillCount();
						m_vSpawnedBalls[i]->GetOwner()->IncreaseScoreCount(4);

						//Increase the deaths for the player
						IncreaseDeathCount();

						continue;
					}

					//If the incoming ball is on your team, and it has been thrown
					if (m_vSpawnedBalls[i]->GetBallStatus() == CBall::eTHROWN && m_vSpawnedBalls[i]->GetOwner() != nullptr && m_vSpawnedBalls[i]->GetOwner()->GetPlayerSide() == m_eTeamSide)
					{
						if (m_pcHeldBall == nullptr)
						{
							PickUpBall(m_vSpawnedBalls[i]);
							m_pcSound->PlaySoundFile(m_pcAsset->getSound(1));
						}

						else
						{
							//Bounce that ball off of you
							m_vSpawnedBalls[i]->GetBallComponent()->SetThrownStatus(false);

							//Set New Level Of Force
							XMVECTOR negateForce = XMVectorSet(-m_vSpawnedBalls[i]->GetLevelOfForceVector().m128_f32[0], -m_vSpawnedBalls[i]->GetLevelOfForceVector().m128_f32[1], -m_vSpawnedBalls[i]->GetLevelOfForceVector().m128_f32[2], 1.0f);

							if (negateForce.m128_f32[2] > 0.0f)
								negateForce.m128_f32[2] -= 15.0f;
							if (negateForce.m128_f32[2] < 0.0f)
								negateForce.m128_f32[2] += 15.0f;

							m_vSpawnedBalls[i]->SetLevelOfForceVector(negateForce);
							m_vSpawnedBalls[i]->IncreaseNumBounced();
						}
						continue;
					}

					//If the Ball is an ice ball and it isn't on your team
					if (m_vSpawnedBalls[i]->GetBallType() == CBall::eICE && m_vSpawnedBalls[i]->GetBallStatus() != CBall::eIDLE && m_vSpawnedBalls[i]->GetOwner() != nullptr && m_vSpawnedBalls[i]->GetOwner()->GetPlayerSide() != m_eTeamSide)
					{
						//If the Ball has ice traps
						if (m_vSpawnedBalls[i]->GetBallComponent()->m_tIceTraps.size() > 0)
						{
							for (int k = 0; k < m_vSpawnedBalls[i]->GetBallComponent()->m_tIceTraps.size(); k++)
							{
								if (end::classify_sphere_to_pill(m_vSpawnedBalls[i]->GetBallComponent()->m_tIceTraps[k], m_pcCharacterComponent->m_tCharacterCollider) == 1)
								{
									m_eLegStatus = CPlayer::eFROZEN;
									m_pcScene->removeObjectFromScene(m_vSpawnedBalls[i]->GetBallComponent()->m_ptIceTrapModels[k]);
									m_vSpawnedBalls[i]->GetBallComponent()->m_ptIceTrapModels.erase(m_vSpawnedBalls[i]->GetBallComponent()->m_ptIceTrapModels.begin() + k);
									m_vSpawnedBalls[i]->GetBallComponent()->m_tIceTraps.erase(m_vSpawnedBalls[i]->GetBallComponent()->m_tIceTraps.begin() + k);
									m_vSpawnedBalls[i]->GetBallComponent()->m_fIceTrapsTimes.erase(m_vSpawnedBalls[i]->GetBallComponent()->m_fIceTrapsTimes.begin() + k);
									m_pcSound->PlaySoundFile(m_pcAsset->getSound(17));
									continue;
								}
							}
						}
					}
				}
				break;
				}

				//If you both don't have ownership, pick up the ball
				if (m_eHandStatus == eEMPTY && !m_vSpawnedBalls[i]->GetOwner() && m_vSpawnedBalls[i]->GetBallStatus() == CBall::eIDLE)
				{
					PickUpBall(m_vSpawnedBalls[i]);
					m_pcSound->PlaySoundFile(m_pcAsset->getSound(0));
					continue;
				}

				//If the incoming ball is yours, and it has been thrown
				if (m_vSpawnedBalls[i]->GetOwner() == this && m_vSpawnedBalls[i]->GetBallStatus() == CBall::eTHROWN)
				{
					//You are now stunned
					m_eLegStatus = CPlayer::eSTUNNED;
					m_pcSound->PlaySoundFile(m_pcAsset->getSound(21));

					//Stop the Balls orbit, make it bounce, and slow it down a bit
					m_vSpawnedBalls[i]->GetBallComponent()->SetThrownStatus(false);
					XMVECTOR negateForce = XMVectorSet(-m_vSpawnedBalls[i]->GetLevelOfForceVector().m128_f32[0], -m_vSpawnedBalls[i]->GetLevelOfForceVector().m128_f32[1], -m_vSpawnedBalls[i]->GetLevelOfForceVector().m128_f32[2], 1.0f);

					if (negateForce.m128_f32[2] > 0.0f)
						negateForce.m128_f32[2] -= 15.0f;
					if (negateForce.m128_f32[2] < 0.0f)
						negateForce.m128_f32[2] += 15.0f;

					m_vSpawnedBalls[i]->SetLevelOfForceVector(negateForce);
					m_vSpawnedBalls[i]->IncreaseNumBounced();
					m_pcSound->PlaySoundFile(m_pcAsset->getSound(18));
					continue;
				}

				//If the incoming ball is not yours, and it has been thrown
				if (m_vSpawnedBalls[i]->GetOwner() != this && m_vSpawnedBalls[i]->GetBallStatus() == CBall::eTHROWN)
				{
					//Stop the Balls orbit, make it bounce, and slow it down a bit
					m_vSpawnedBalls[i]->GetBallComponent()->SetThrownStatus(false);
					XMVECTOR negateForce = XMVectorSet(-m_vSpawnedBalls[i]->GetLevelOfForceVector().m128_f32[0], -m_vSpawnedBalls[i]->GetLevelOfForceVector().m128_f32[1], -m_vSpawnedBalls[i]->GetLevelOfForceVector().m128_f32[2], 1.0f);

					if (negateForce.m128_f32[2] > 0.0f)
						negateForce.m128_f32[2] -= 15.0f;
					if (negateForce.m128_f32[2] < 0.0f)
						negateForce.m128_f32[2] += 15.0f;

					m_vSpawnedBalls[i]->SetLevelOfForceVector(negateForce);
					m_vSpawnedBalls[i]->IncreaseNumBounced();
					continue;
				}
			}
			else
			{
				m_pcCharacterComponent->m_tCharacterCollider.collided = false;
				m_vSpawnedBalls[i]->GetBallComponent()->m_tBallCollider.collided = false;
			}

			//If the Ball Hits the Block Collider
			if (classify_sphere_to_pill(m_vSpawnedBalls[i]->GetBallComponent()->m_tBallCollider, m_pcCharacterComponent->m_tBCCollider) == 1)
			{
				m_pcCharacterComponent->m_tCharacterCollider.collided = true;
				m_vSpawnedBalls[i]->GetBallComponent()->m_tBallCollider.collided = true;

				//If the incoming ball is not yours, and it has been thrown
				if ((m_vSpawnedBalls[i]->GetOwner() != nullptr && m_vSpawnedBalls[i]->GetOwner() != this) && m_vSpawnedBalls[i]->GetBallStatus() == CBall::eTHROWN)
				{
					//If the player is blocking and the current ball is blockable
					if (m_eHandStatus == CPlayer::eBLOCKING && m_vSpawnedBalls[i]->GetBlockableStat())
					{
						//Turn on Gravity 
						m_vSpawnedBalls[i]->GetBallComponent()->SetThrownStatus(false);

						//Set New Force
						XMVECTOR negateForce = XMVectorSet(-m_vSpawnedBalls[i]->GetLevelOfForceVector().m128_f32[0], -m_vSpawnedBalls[i]->GetLevelOfForceVector().m128_f32[1], -m_vSpawnedBalls[i]->GetLevelOfForceVector().m128_f32[2], 1.0f);

						if (negateForce.m128_f32[2] > 0.0f)
							negateForce.m128_f32[2] -= 15.0f;
						if (negateForce.m128_f32[2] < 0.0f)
							negateForce.m128_f32[2] += 15.0f;

						m_vSpawnedBalls[i]->SetLevelOfForceVector(negateForce);
						m_vSpawnedBalls[i]->IncreaseNumBounced();

						m_pcSound->PlaySoundFile(m_pcAsset->getSound(1));
					}

					//If the player is catching and the current ball is catchable
					if (m_eHandStatus == eCATCHING && m_vSpawnedBalls[i]->GetCatchableStat())
					{
						PickUpBall(m_vSpawnedBalls[i]);
						IncreaseScoreCount(2);
						m_pcSound->PlaySoundFile(m_pcAsset->getSound(1));
					}
				}

				if (m_vSpawnedBalls[i]->GetOwner() == nullptr && m_vSpawnedBalls[i]->GetBallStatus() == CBall::eTHROWN)
				{
					//If the player is blocking and the current ball is blockable
					if (m_eHandStatus == CPlayer::eBLOCKING && m_vSpawnedBalls[i]->GetBlockableStat())
					{
						//Turn on Gravity 
						m_vSpawnedBalls[i]->GetBallComponent()->SetThrownStatus(false);

						//Set New Force
						XMVECTOR negateForce = XMVectorSet(-m_vSpawnedBalls[i]->GetLevelOfForceVector().m128_f32[0], -m_vSpawnedBalls[i]->GetLevelOfForceVector().m128_f32[1], -m_vSpawnedBalls[i]->GetLevelOfForceVector().m128_f32[2], 1.0f);

						if (negateForce.m128_f32[2] > 0.0f)
							negateForce.m128_f32[2] -= 15.0f;
						if (negateForce.m128_f32[2] < 0.0f)
							negateForce.m128_f32[2] += 15.0f;

						m_vSpawnedBalls[i]->SetLevelOfForceVector(negateForce);
						m_vSpawnedBalls[i]->IncreaseNumBounced();

						m_pcSound->PlaySoundFile(m_pcAsset->getSound(1));
					}

					//If the player is catching and the current ball is catchable
					if (m_eHandStatus == eCATCHING && m_vSpawnedBalls[i]->GetCatchableStat())
					{
						PickUpBall(m_vSpawnedBalls[i]);
						IncreaseScoreCount(2);
						m_pcSound->PlaySoundFile(m_pcAsset->getSound(1));
					}
				}
			}
		}

		//Calculate current Position
		XMFLOAT3 curPos1 = m_pcCharacterComponent->GetPosition();

		//Check against each spawned Barrier for collision
		for (int i = 0; i < (int)theBarriers.size(); i++)
		{
			int m_nClosetPlane = end::classify_pill_to_wall(*theBarriers[i]->GetWallCollider(), m_pcCharacterComponent->m_tCharacterCollider);
			if (m_nClosetPlane == -1)
			{
				m_pcCharacterComponent->m_tCharacterCollider.collided = false;
				theBarriers[i]->GetWallCollider()->collided = false;
				m_pcCharacterComponent->SetOnWallStatus(false);
			}
			else
			{
				m_pcCharacterComponent->m_tCharacterCollider.collided = true;
				theBarriers[i]->GetWallCollider()->collided = true;

				XMVECTOR lerp = end::wallerp(*theBarriers[i]->GetWallCollider(), m_pcCharacterComponent->m_tCharacterCollider, m_nClosetPlane);
				XMMATRIX mLerp = XMMatrixTranslationFromVector(lerp);
				m_pcCharacterComponent->SetWorldTransform(XMMatrixMultiply(*m_pcCharacterComponent->GetWorldTransform(), mLerp));
				if (m_nClosetPlane == 5)
				{
					m_fDistFromPlanetoid = end::Distance(curPos1, m_pcPhysics->m_tPlanetoidCollider.center);
					m_pcCharacterComponent->SetOnWallStatus(true);
					break;
				}
			}
		}

		//Check against each spawned Power Up for collision
		for (int i = 0; i < (int)thePowerUps.size(); i++)
		{
			if (end::classify_sphere_to_pill(thePowerUps[i]->GetComponent()->m_tMeshCollider, m_pcCharacterComponent->m_tCharacterCollider) == 1 && !m_bInvincibility)
			{
				m_pcCharacterComponent->m_tCharacterCollider.collided = true;
				thePowerUps[i]->GetComponent()->m_tMeshCollider.collided = true;

				if (m_pcActivatedPowerUp == nullptr)
				{
					PickUpPowerUp(thePowerUps[i]);
				}
			}
		}
	}

	if (m_pcActivatedPowerUp)
	{
		if (m_pcActivatedPowerUp->GetActivationTime() == 0.0f || m_eLegStatus == eLEGRESPAWNING)
		{
			switch (m_pcActivatedPowerUp->GetPowerUpType())
			{
			case CPowerUp::eJUMPBOOST:
			{
				m_fPowerUpJumpSpeed = 0.0f;
				break;
			}
			case CPowerUp::eREFLECTOR:
			{
				m_bPowerUpReflector = false;
				break;
			}
			case CPowerUp::eSPEEDBOOST:
			{
				m_fPowerUpRunSpeed = 0.0f;
				break;
			}
			case CPowerUp::eTHROWBOOST:
			{
				m_fPowerUpThrowSpeed = 0.0f;
				break;
			}
			case CPowerUp::eDODGEBOOST:
			{
				m_bPowerUpDodge = false;
				break;
			}
			}

			m_pcSound->PlaySoundFile(m_pcAsset->getSound(28));
			m_pcActivatedPowerUp->SetActiveStatus(false);
			m_pcActivatedPowerUp = nullptr;
		}
	}

	if (m_fCoolDownTimer > 0.0f)
	{
		m_fCoolDownTimer -= fT;

		if (m_fCoolDownTimer < 0.0f)
			m_fCoolDownTimer = 0.0f;
	}

	if (m_fDodgeCoolDownTimer > 0.0f)
	{
		m_fDodgeCoolDownTimer -= fT;

		if (m_fDodgeCoolDownTimer < 0.0f)
			m_fDodgeCoolDownTimer = 0.0f;
	}

	if (m_fAnimationTimer > 0.0f)
	{
		m_fAnimationTimer -= fT;

		if (m_fAnimationTimer < 0.0f)
			m_fAnimationTimer = 0.0f;
	}

	//Calculate current Position
	XMMATRIX worldTransform = *m_pcCharacterComponent->GetWorldTransform();
	XMFLOAT3 curPos;
	curPos.x = worldTransform.r[3].m128_f32[0];
	curPos.y = worldTransform.r[3].m128_f32[1];
	curPos.z = worldTransform.r[3].m128_f32[2];

	//Set Distance
	SetCurrentDistance(end::Distance(curPos, m_pcPhysics->m_tPlanetoidCollider.center));
	SetPlanetoidDistance(end::Distance(curPos, m_pcPhysics->m_tPlanetoidCollider.center));

	XMMATRIX modelMatrix = *m_pcCharacterComponent->GetWorldTransform();

	XMStoreFloat4x4(&worldMatrix, modelMatrix);
}

void CPlayer::Update(float fT, vector<CPlayer*> pcLobby, vector<CBall*> pcBalls, vector<CBarrier*> theBarriers, bool bControlsOn)
{

	m_cUISystem->SendPlayerInfo(m_nPlayerNumber, m_nScore);
	XMFLOAT3 fPosition = this->m_pcCharacterComponent->GetPosition();
	m_handPosition = XMVectorSet(fPosition.x + 0.5f, fPosition.y + 2.0f, fPosition.z, 1.0f);

	//Set Radius
	float radius = m_pcPhysics->m_tPlanetoidCollider.radius;
	//Fix Wall
	if (m_pcCharacterComponent->GetOnWallStatus())
	{
		radius = m_fDistFromPlanetoid;
	}

	if (bControlsOn && m_ePlayerType == eUSER)
	{
		//If the Throw/Catch Button is Pressed
		if (m_pcInput->IsKeyPressed(0x64) || m_pcInput->IsControllerConnected(m_nPlayerNumber - 1) && m_pcInput->IsButtonPressed(m_nPlayerNumber - 1, G_XBOX_RIGHT_TRIGGER_AXIS)) {
			//If the Player has a ball in their hand and they aren't already catching
			if (m_pcHeldBall != nullptr && m_eHandStatus != CPlayer::eCATCHING)
			{
				//If the ball isn't a WIND ball, Throw it!
				if (m_pcHeldBall->GetBallType() != CBall::eWIND)
				{
					if (m_fCoolDownTimer == 0.0f && m_eHandStatus != eBLOCKING && m_eHandStatus != eCATCHING)
					{
						//Speed is dependent on the Ball type
						switch (m_pcHeldBall->GetBallType())
						{
						case CBall::eELECTRIC:
						{
							m_pcHeldBall->SetLevelOfForceVector(XMVectorSet(0.0f, 0.0f, (70.0f + m_fPowerUpThrowSpeed), 1.0f));
							break;
						}
						case CBall::eFIRE:
						{
							m_pcHeldBall->SetLevelOfForceVector(XMVectorSet(0.0f, 0.0f, (50.0f + m_fPowerUpThrowSpeed), 1.0f));
							break;
						}
						default:
						{
							m_pcHeldBall->SetLevelOfForceVector(XMVectorSet(0.0f, 0.0f, (45.0f + m_fPowerUpThrowSpeed), 1.0f));
							break;
						}
						}

						ThrowBall(fT);
						ResetCoolDownTimer();
						m_pcSound->PlaySoundFile(m_pcAsset->getSound(2));
					}
				}
				//If the ball is a WIND ball, Activate it
				else
				{
					m_pcHeldBall->SetBallStatus(CBall::eACTIVATED);
					m_pcHeldBall = nullptr;
					m_pcSound->PlaySoundFile(m_pcAsset->getSound(26));
				}
			}
			//If the Player doesn't have a ball
			else
			{
				//And they aren't catching or blocking, Set status to Catching, and the cooldown is done
				if (m_eHandStatus != CPlayer::eCATCHING && m_eHandStatus != CPlayer::eBLOCKING)
				{
					//If the cooldown timer is done
					if (m_fCoolDownTimer <= 0.0f)
					{
						m_eHandStatus = eCATCHING;
						ResetCatchTimer();
					}
				}

			}
		}
		//If the Player doesn't have a ball
		else
		{
			//If the Player doesn't have a ball in their hands
			if (m_pcHeldBall == nullptr)
			{
				//If the Player's Hand Status isn't EMPTY, set it
				if (m_eHandStatus != CPlayer::eEMPTY)
				{
					m_eHandStatus = eEMPTY;
				}
			}
		}

		//If the Block Button is Pressed
		if (m_pcInput->IsKeyPressed(0x65) || m_pcInput->IsControllerConnected(m_nPlayerNumber - 1) && m_pcInput->IsButtonPressed(m_nPlayerNumber - 1, G_XBOX_LEFT_TRIGGER_AXIS))
		{
			//If the Player has a ball in it's hands and they aren't already blocking
			if (m_pcHeldBall && m_eHandStatus != CPlayer::eBLOCKING)
			{
				//If the incoming ball isn't an ICE ball
				if (m_pcHeldBall->GetBallType() != CBall::eICE)
				{
					//If the cooldown timer is done
					if (m_fCoolDownTimer <= 0.0f)
					{
						//Set/Reset Player's and Ball Status
						m_eHandStatus = eBLOCKING;
						ResetBlockTimer();
					}
				}

				//If the ball in hand is an ICE ball
				else
				{
					XMMATRIX playerTransform = *m_pcHeldBall->GetBallComponent()->GetWorldTransform();
					XMMATRIX newWorldZTransform = XMMatrixMultiply(XMMatrixTranslation(0.0f, -2.0f, 0.0f), playerTransform);
					XMMATRIX newWorldXTransform = XMMatrixMultiply(XMMatrixTranslation(-2.0f, 0.0f, 0.0f), newWorldZTransform);
					m_pcHeldBall->GetBallComponent()->SetWorldTransform(newWorldXTransform);

					m_pcHeldBall->SetLevelOfForceVector(XMVectorSet(0.0f, 0.0f, 30.0f, 1.0f));
					RollBall(fT);
					ResetCoolDownTimer();
				}
			}

		}
		else
		{
			//If the player has a ball
			if (m_pcHeldBall)
			{
				//If the player Status isn't set to holding
				if (m_eHandStatus != CPlayer::eHOLDING)
				{
					//If the ball hasn't been activated
					if (m_pcHeldBall->GetBallStatus() != CBall::eACTIVATED)
					{
						//Set/Reset Player's and their ball's statuses
						m_pcHeldBall->SetBallStatus(CBall::eHELD);
						m_eHandStatus = eHOLDING;
					}
					//If the ball has been activated
					if (m_pcHeldBall->GetBallStatus() == CBall::eACTIVATED)
					{
						//Set Status so it is following you
						m_pcHeldBall->SetBallStatus(CBall::eHELD);
					}
				}
			}
		}

		//If the Player is Frozen
		if (m_eLegStatus == CPlayer::eFROZEN)
		{
			//If the A key is pressed || Move the Left Analog Stick to the Left
			if (m_pcInput->IsKeyPressed(0x27) || m_pcInput->IsControllerConnected(m_nPlayerNumber - 1) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNumber - 1, G_XBOX_LX_AXIS) < -0.85f)
			{
				IncreaseUnFreezeCount();
			}

			//If the D key is pressed || Move the Left Analog Stick to the Right
			if (m_pcInput->IsKeyPressed(0x2a) || m_pcInput->IsControllerConnected(m_nPlayerNumber - 1) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNumber - 1, G_XBOX_LX_AXIS) > 0.85f) //This is the D key
			{
				IncreaseUnFreezeCount();
			}
		}

		if (m_eLegStatus != CPlayer::eLEGRESPAWNING && m_eLegStatus != CPlayer::eFROZEN && m_eLegStatus != CPlayer::eSTUNNED)
		{
			//If you are not dodging
			if (m_eLegStatus != CPlayer::eDODGING)
			{
				//Doding Checks and Applicants
				//Up Arrow
				if (m_fDodgeCoolDownTimer == 0.0f && (m_pcInput->IsKeyPressed(0x1b) || (m_pcInput->IsKeyPressed(0x3d) && (m_pcInput->IsKeyPressed(0xd) || m_pcInput->IsKeyPressed(0x12)))
					|| m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_DPAD_UP_BTN)
					|| m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNum, G_XBOX_LY_AXIS) > 0.0f
					&& (m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_X_BTN) || m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_LEFT_SHOULDER_BTN) || m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_RIGHT_SHOULDER_BTN)))) //up arrow
				{
					XMVECTOR vdiagonal = XMVectorSet(0.0f, 1.0f, 1.0f, 1.0f);
					SetLegStatus(CPlayer::eDODGING);
					SetDodgeForce(vdiagonal);
					SetStartDistance(end::Distance(end::turnToFloat3(m_pcCharacterComponent->GetWorldTransform()->r[3]), m_pcPhysics->m_tPlanetoidCollider.center));
					m_pcSound->PlaySoundFile(m_pcAsset->getSound(2));
					if (!m_bPowerUpDodge)
						m_fDodgeCoolDownTimer = FDODGECOOLDOWNTIMER;
				}
				//DownArrow
				else if (m_fDodgeCoolDownTimer == 0.0f && (m_pcInput->IsKeyPressed(0x23) || (m_pcInput->IsKeyPressed(0x39) && (m_pcInput->IsKeyPressed(0xd) || m_pcInput->IsKeyPressed(0x12)))
					|| m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_DPAD_DOWN_BTN)
					|| m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNum, G_XBOX_LY_AXIS) < 0.0f
					&& (m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_X_BTN) || m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_LEFT_SHOULDER_BTN) || m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_RIGHT_SHOULDER_BTN))))//down arrow
				{
					XMVECTOR vdiagonal = XMVectorSet(0.0f, 1.0f, -1.0f, 1.0f);
					SetLegStatus(CPlayer::eDODGING);
					SetDodgeForce(vdiagonal);
					SetStartDistance(end::Distance(end::turnToFloat3(m_pcCharacterComponent->GetWorldTransform()->r[3]), m_pcPhysics->m_tPlanetoidCollider.center));
					m_pcSound->PlaySoundFile(m_pcAsset->getSound(2));
					if (!m_bPowerUpDodge)
						m_fDodgeCoolDownTimer = FDODGECOOLDOWNTIMER;
				}
				//Left Arrow
				else if (m_fDodgeCoolDownTimer == 0.0f && (m_pcInput->IsKeyPressed(0x1e) || (m_pcInput->IsKeyPressed(0x27) && (m_pcInput->IsKeyPressed(0xd) || m_pcInput->IsKeyPressed(0x12))) || m_pcInput->IsKeyPressed(0x37)
					|| m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_DPAD_LEFT_BTN)
					|| m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNum, G_XBOX_LX_AXIS) < 0.0f
					&& (m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_X_BTN) || m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_LEFT_SHOULDER_BTN) || m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_RIGHT_SHOULDER_BTN)))) //left arrow
				{
					XMVECTOR vdiagonal = XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f);
					SetLegStatus(CPlayer::eDODGING);
					SetDodgeForce(vdiagonal);
					SetStartDistance(end::Distance(end::turnToFloat3(m_pcCharacterComponent->GetWorldTransform()->r[3]), m_pcPhysics->m_tPlanetoidCollider.center));
					m_pcSound->PlaySoundFile(m_pcAsset->getSound(2));
					if (!m_bPowerUpDodge)
						m_fDodgeCoolDownTimer = FDODGECOOLDOWNTIMER;
				}
				//Right Arrow
				else if (m_fDodgeCoolDownTimer == 0.0f && (m_pcInput->IsKeyPressed(0x20) || (m_pcInput->IsKeyPressed(0x2a) && (m_pcInput->IsKeyPressed(0xd) || m_pcInput->IsKeyPressed(0x12))) || m_pcInput->IsKeyPressed(0x2b)
					|| m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_DPAD_RIGHT_BTN)
					|| m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNum, G_XBOX_LX_AXIS) > 0.0f
					&& (m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_X_BTN) || m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_LEFT_SHOULDER_BTN) || m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_RIGHT_SHOULDER_BTN)))) //right arrow
				{
					XMVECTOR vdiagonal = XMVectorSet(-1.0f, 1.0f, 0.0f, 1.0f);
					SetLegStatus(CPlayer::eDODGING);
					SetDodgeForce(vdiagonal);
					SetStartDistance(end::Distance(end::turnToFloat3(m_pcCharacterComponent->GetWorldTransform()->r[3]), m_pcPhysics->m_tPlanetoidCollider.center));
					m_pcSound->PlaySoundFile(m_pcAsset->getSound(2));
					if (!m_bPowerUpDodge)
						m_fDodgeCoolDownTimer = FDODGECOOLDOWNTIMER;
				}

				//Camera Movement
				if (m_pcInput->GetMouseXDelta() < 0.0f || m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNum, G_XBOX_RX_AXIS) < 0.0f)
				{
					XMMATRIX newWorldTransform = XMMatrixRotationAxis(XMVECTOR{ m_pcCharacterComponent->GetUp().x, m_pcCharacterComponent->GetUp().y, m_pcCharacterComponent->GetUp().z, m_pcCharacterComponent->GetUp().w }, fT * 2.0f);
					XMVECTOR vPrevW = XMVectorSet(m_pcCharacterComponent->GetWorldTransform()->r[3].m128_f32[0], m_pcCharacterComponent->GetWorldTransform()->r[3].m128_f32[1], m_pcCharacterComponent->GetWorldTransform()->r[3].m128_f32[2], m_pcCharacterComponent->GetWorldTransform()->r[3].m128_f32[3]);

					m_pcCharacterComponent->GetWorldTransform()->r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
					m_pcCharacterComponent->SetWorldTransform(XMMatrixMultiply(*m_pcCharacterComponent->GetWorldTransform(), newWorldTransform));
					m_pcCharacterComponent->GetWorldTransform()->r[3] = vPrevW;
				}

				if (m_pcInput->GetMouseXDelta() > 0.0f || m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNum, G_XBOX_RX_AXIS) > 0.0f)
				{
					XMMATRIX newWorldTransform = XMMatrixRotationAxis(XMVECTOR{ m_pcCharacterComponent->GetUp().x, m_pcCharacterComponent->GetUp().y, m_pcCharacterComponent->GetUp().z, m_pcCharacterComponent->GetUp().w }, fT * -2.0f);
					XMVECTOR vPrevW = XMVectorSet(m_pcCharacterComponent->GetWorldTransform()->r[3].m128_f32[0], m_pcCharacterComponent->GetWorldTransform()->r[3].m128_f32[1], m_pcCharacterComponent->GetWorldTransform()->r[3].m128_f32[2], m_pcCharacterComponent->GetWorldTransform()->r[3].m128_f32[3]);

					m_pcCharacterComponent->GetWorldTransform()->r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
					m_pcCharacterComponent->SetWorldTransform(XMMatrixMultiply(*m_pcCharacterComponent->GetWorldTransform(), newWorldTransform));
					m_pcCharacterComponent->GetWorldTransform()->r[3] = vPrevW;
				}

				//If Not Moving 
				if (!m_pcInput->IsKeyPressed(G_KEY_W) && !m_pcInput->IsKeyPressed(G_KEY_S) && !m_pcInput->IsKeyPressed(G_KEY_A) && !m_pcInput->IsKeyPressed(G_KEY_D) && m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNum, G_XBOX_LY_AXIS == 0.0f) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNum, G_XBOX_LX_AXIS) == 0.0f && m_eLegStatus != CPlayer::eJUMPING && m_eLegStatus != CPlayer::eDODGING)
				{
					m_eLegStatus = CPlayer::eIDLE;
				}

				//Player Movement
				//Another Insanity Check 
				if (m_eLegStatus != CPlayer::eDODGING)
				{
					//If the W key is pressed 
					if (m_pcInput->IsKeyPressed(0x3d) || m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNum, G_XBOX_LY_AXIS) > 0.0f)
					{
						XMMATRIX newWorldTransform = XMMatrixMultiply(XMMatrixTranslation(0.0f, 0.0f, (fT * (10.0f + m_fPowerUpRunSpeed))), *m_pcCharacterComponent->GetWorldTransform());
						m_pcCharacterComponent->SetWorldTransform(newWorldTransform);
						if (m_eLegStatus != CPlayer::eJUMPING)
						{
							m_eLegStatus = eRUNNING;
						}
					}
					//If the S key is pressed
					if (m_pcInput->IsKeyPressed(0x39) || m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNum, G_XBOX_LY_AXIS) < 0.0f)
					{
						XMMATRIX newWorldTransform = XMMatrixMultiply(XMMatrixTranslation(0.0f, 0.0f, (fT * (-10.0f - m_fPowerUpRunSpeed))), *m_pcCharacterComponent->GetWorldTransform());
						m_pcCharacterComponent->SetWorldTransform(newWorldTransform);
						if (m_eLegStatus != CPlayer::eJUMPING)
						{
							m_eLegStatus = CPlayer::eRUNNING;
						}
					}
					//If the A key is pressed
					if (m_pcInput->IsKeyPressed(0x27) || m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNum, G_XBOX_LX_AXIS) < 0.0f)
					{
						XMMATRIX newWorldTransform = XMMatrixMultiply(XMMatrixTranslation((fT * (10.0f + m_fPowerUpRunSpeed)), 0.0f, 0.0f), *m_pcCharacterComponent->GetWorldTransform());
						m_pcCharacterComponent->SetWorldTransform(newWorldTransform);
						if (m_eLegStatus != CPlayer::eJUMPING)
						{
							m_eLegStatus = CPlayer::eRUNNING;
						}
					}
					//If the D key is pressed
					if (m_pcInput->IsKeyPressed(0x2a) || m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->GetAnalogAxisPosition(m_nPlayerNum, G_XBOX_LX_AXIS) > 0.0f)
					{
						XMMATRIX newWorldTransform = XMMatrixMultiply(XMMatrixTranslation((fT * (-10.0f - m_fPowerUpRunSpeed)), 0.0f, 0.0f), *m_pcCharacterComponent->GetWorldTransform());
						m_pcCharacterComponent->SetWorldTransform(newWorldTransform);
						if (m_eLegStatus != CPlayer::eJUMPING)
						{
							m_eLegStatus = CPlayer::eRUNNING;
						}
					}

					//If the Space Bar is pressed || If A Button is Pressed
					//Jump
					if (m_pcInput->IsKeyPressed(0x16) || m_pcInput->IsControllerConnected(m_nPlayerNum) && m_pcInput->IsButtonPressed(m_nPlayerNum, G_XBOX_A_BTN)) //this is the space bar
					{
						if (m_eLegStatus != CPlayer::eJUMPING)
						{
							m_eLegStatus = CPlayer::eJUMPING;
							m_pcSound->PlaySoundFile(m_pcAsset->getSound(25));
						}
					}
				}
			}
		}

#pragma region Animations
		if (m_eLegStatus == eDODGING)
		{
			if (m_cAnimPlayer->GetAnimationID() != CHARACTERDODGEIDNUM)
			{
				PlayAnimation(CHARACTERDODGEIDNUM);
			}
		}
		else if ((m_eLegStatus == eRUNNING || m_eLegStatus == eIDLE) && m_bPickUp)
		{
			if (m_cAnimPlayer->GetAnimationID() != CHARACTERPICKUPIDNUM)
			{
				PlayAnimation(CHARACTERPICKUPIDNUM);
			}

			if (m_fAnimationTimer <= 0.0f)
				m_bPickUp = false;
		}
		else if ((m_eLegStatus == eRUNNING || m_eLegStatus == eIDLE) && m_bRolling)
		{
			if (m_cAnimPlayer->GetAnimationID() != CHARACTERROLLIDNUM)
			{
				PlayAnimation(CHARACTERROLLIDNUM);
			}

			if (m_fAnimationTimer <= 0.0f)
				m_bRolling = false;
		}
		else if ((m_eLegStatus == eRUNNING || m_eLegStatus == eIDLE || m_eLegStatus == eJUMPING) && m_bThrowing)
		{
			if (m_cAnimPlayer->GetAnimationID() != CHARACTERTHROWIDNUM)
			{
				PlayAnimation(CHARACTERTHROWIDNUM);
			}
			if (m_fAnimationTimer <= 0.0) {
				m_bThrowing = false;
			}
			else if (m_fAnimationTimer <= 0.4f || m_eLegStatus == eJUMPING)
			{
				if (this->m_pcHeldBall != nullptr )
				{
					for (int i = 0; i < (int)theBarriers.size(); i++)
					{
						if (end::classify_sphere_to_wall(*theBarriers[i]->GetWallCollider(), m_pcHeldBall->GetBallComponent()->m_tBallCollider) != 1)
						{
							this->m_pcHeldBall->SetBallStatus(CBall::eTHROWN);

							//Save Ball current position and time for a later check
							XMMATRIX ballMatrix = *this->m_pcHeldBall->GetBallComponent()->GetWorldTransform();
							this->m_pcHeldBall->ResetThrownTime();

							//Set Physics IsThrown variable
							this->m_pcHeldBall->GetBallComponent()->SetThrownStatus(true);

							//Detach Ball from Player
							this->m_pcHeldBall = nullptr;
							this->m_eHandStatus = eEMPTY;

							if (m_eLegStatus == eJUMPING)
								m_bThrowing = false;

							break;
						}
					}
				}
			}
		}
		else if (m_eLegStatus == eRUNNING)
		{
			if (m_cAnimPlayer->GetAnimationID() != CHARACTERRUNIDNUM)
			{
				PlayAnimation(CHARACTERRUNIDNUM);
			}
		}
		else if ((m_eLegStatus == eRUNNING || m_eLegStatus == eIDLE) && m_eHandStatus == eCATCHING)
		{
			if (m_cAnimPlayer->GetAnimationID() != CHARACTERCATCHIDNUM)
			{
				PlayAnimation(CHARACTERCATCHIDNUM);
			}
		}
		else if ((m_eLegStatus == eRUNNING || m_eLegStatus == eIDLE) && m_eHandStatus == eBLOCKING)
		{
			if (m_cAnimPlayer->GetAnimationID() != CHARACTERBLOCKIDNUM)
			{
				PlayAnimation(CHARACTERBLOCKIDNUM);
			}
		}
		else if (m_eLegStatus == eIDLE)
		{
			if (m_cAnimPlayer->GetAnimationID() != CHARACTERIDLEIDNUM)
			{
				PlayAnimation(CHARACTERIDLEIDNUM);
			}
		}
		else if (m_eLegStatus == eJUMPING)
		{
			if (m_cAnimPlayer->GetAnimationID() != CHARACTERJUMPIDNUM)
			{
				PlayAnimation(CHARACTERJUMPIDNUM);
			}
		}
		else if (m_eLegStatus == eLEGRESPAWNING)
		{
			if (m_cAnimPlayer->GetAnimationID() != CHARACTERDEATHIDNUM)
			{
				PlayAnimation(CHARACTERDEATHIDNUM);
			}
		}
		else if (m_eLegStatus == eSTUNNED || m_eLegStatus == eFROZEN)
		{
			if (m_cAnimPlayer->GetAnimationID() != CHARACTERLOSEIDNUM)
			{
				PlayAnimation(CHARACTERLOSEIDNUM);
			}
		}
#pragma endregion
	}

	//Depending on Hand Status
	switch (m_eHandStatus)
	{
	case eBLOCKING:
	{
		m_pcHeldBall->SetBallStatus(CBall::eBLOCKING);
		m_pcCharacterComponent->m_bBC = true;

		if (m_fBlockTimer > 0.0f)
		{
			m_fBlockTimer -= fT;

			if (m_fBlockTimer < 0.0f)
				m_fBlockTimer = 0.0f;
		}

		if (m_fBlockTimer <= 0.0f)
		{
			m_pcHeldBall->SetBallStatus(CBall::eHELD);
			m_eHandStatus = eHOLDING;
			m_pcCharacterComponent->m_bBC = false;
			m_fCoolDownTimer = FCOOLDOWNTIMER;
		}
		break;
	}
	case eCATCHING:
	{
		m_pcCharacterComponent->m_bBC = true;
		if (m_fCatchTimer > 0.0f)
		{
			m_fCatchTimer -= fT;

			if (m_fCatchTimer < 0.0f)
				m_fCatchTimer = 0.0f;
		}

		if (m_fCatchTimer <= 0.0f)
		{
			m_eHandStatus = eEMPTY;
			m_pcCharacterComponent->m_bBC = false;
			m_fCoolDownTimer = FCOOLDOWNTIMER;
		}
		break;
	}
	case eHOLDING:
	{
		if (m_pcHeldBall)
			m_pcHeldBall->SetBallStatus(CBall::eHELD);
		break;
	}
	}

	//Turn on Block Pill
	if (m_eHandStatus != eBLOCKING && m_eHandStatus != eCATCHING)
		m_pcCharacterComponent->m_bBC = false;

	//Depending on Leg Status
	switch (m_eLegStatus)
	{
	case eJUMPING:
	{
		XMMATRIX newWorldTransform = XMMatrixMultiply(XMMatrixTranslation(0.0f, fT * (m_fJumpForce + m_fPowerUpJumpSpeed) * 1.5f, 0.0f), *m_pcCharacterComponent->GetWorldTransform());
		m_pcCharacterComponent->SetWorldTransform(newWorldTransform);

		m_fJumpForce -= GRAVITYBASE * fT;

		if (m_fJumpTimer > 0.0f)
		{
			m_fJumpTimer -= fT;

			if (m_fJumpTimer < 0.0f)
				m_fJumpTimer = 0.0f;
		}

		//If enough time has passed since you jumped
		if (m_fJumpTimer <= 0.0f)
		{
			//Check Player's distance from the Planetoid, if on the ground, reset stats and set Player to IDLE
			if (m_fDistFromPlanetoid <= radius)
			{
				SetLegStatus(eIDLE);
				ResetJumpForce();
				ResetJumpTimer();
			}
		}

		break;
	}
	case eDODGING:
	{
		XMMATRIX newWorldTransform = XMMatrixMultiply(
			XMMatrixTranslation(fT * m_fDodgeForce.m128_f32[0] * m_fDodgeSpeed, fT * m_fDodgeForce.m128_f32[1] * m_fDodgeSpeed, fT * m_fDodgeForce.m128_f32[2] * m_fDodgeSpeed),
			*m_pcCharacterComponent->GetWorldTransform());

		if (m_fStartDist > m_fCurDist)
		{
			newWorldTransform = XMMatrixMultiply(
				XMMatrixTranslation(fT * m_fDodgeForce.m128_f32[0], fT * m_fDodgeForce.m128_f32[1], fT * m_fDodgeForce.m128_f32[2]),
				*m_pcCharacterComponent->GetWorldTransform());
		}
		m_pcCharacterComponent->SetWorldTransform(newWorldTransform);

		m_fDodgeForce.m128_f32[1] -= GRAVITYBASE * fT;

		if (m_fDodgeTime > 0.0f)
		{
			m_fDodgeTime -= fT;
			if (m_fDodgeTime < 0.0f)
				m_fDodgeTime = 0.0f;
		}

		//If enough time has passed since you dodged
		if (m_fDodgeTime <= 0.0f)
		{
			//Check Player's distance from the Planetoid, if on the ground, reset stats and set Player to IDLE
			if (m_fDistFromPlanetoid <= radius)
			{
				SetLegStatus(CPlayer::eIDLE);
				ResetDodgeTimer();
				SetDodgeForce(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
			}
		}
		break;
	}
	case eSTUNNED:
	{
		if (m_fStunTimer > 0.0f)
		{
			m_fStunTimer -= fT;

			if (m_fStunTimer < 0.0f)
				m_fStunTimer = 0.0f;
		}

		if (m_fStunTimer <= 0.0f)
		{
			m_eLegStatus = eIDLE;
			ResetStunTimer();

			//If you have an electricity
			if (m_pcHeldBall != nullptr && m_pcHeldBall->GetBallType() == CBall::eELECTRIC)
			{
				m_pcHeldBall->SetBallType(CBall::eNORM);
			}
		}
		break;
	}
	case eFROZEN:
	{
		if (m_fFrozenTimer > 0.0f)
		{
			m_fFrozenTimer -= fT;

			if (m_fFrozenTimer < 0.0f)
				m_fFrozenTimer = 0.0f;
		}

		if (m_fFrozenTimer <= 0.0f || m_nUnFreezeCounter > NNUMTOUNFREEZE)
		{
			m_eLegStatus = eIDLE;
			ResetUnFreezeCount();
			ResetFrozenTimer();
		}
		break;
	}
	}

	if (m_eLegStatus != eLEGRESPAWNING && m_eLegStatus != eSTUNNED && m_eLegStatus != eFROZEN && !m_bInvincibility)
	{
		m_pcCharacterComponent->SetPlayersFrustum();

		//Check against each spawned Ball for collision
		for (int i = 0; i < pcBalls.size(); i++)
		{
			if (classify_sphere_to_pill(pcBalls[i]->GetBallComponent()->m_tBallCollider, m_pcCharacterComponent->m_tCharacterCollider) == 1 && !m_bInvincibility)
			{
				m_pcCharacterComponent->m_tCharacterCollider.collided = true;
				pcBalls[i]->GetBallComponent()->m_tBallCollider.collided = true;

				switch (m_eCurrentGameType)
				{
				case eSOLO:
				{
					//If the incoming ball is not yours, and it has been activated
					if (pcBalls[i]->GetOwner() != nullptr && pcBalls[i]->GetOwner() != this && pcBalls[i]->GetBallStatus() == CBall::eACTIVATED)
					{
						//If you have a ball, drop it.
						if (m_pcHeldBall != nullptr)
						{
							m_pcHeldBall->SetOwner(nullptr);
							m_pcHeldBall->SetBallStatus(CBall::eIDLE);
						}

						m_pcHeldBall = nullptr;
						m_eHandStatus = eEMPTY;
						continue;
					}

					//If the incoming ball is not yours, and it has been thrown
					if (pcBalls[i]->GetOwner() != this && pcBalls[i]->GetOwner() != nullptr && pcBalls[i]->GetBallStatus() == CBall::eTHROWN)
					{
						//If the Ball is Fire, Explode!
						if (pcBalls[i]->GetBallType() == CBall::eFIRE)
						{
							if (pcBalls[i]->GetBallStatus() != CBall::eEXPLODING)
							{
								pcBalls[i]->SetBallStatus(CBall::eEXPLODING);
								m_pcSound->PlaySoundFile(m_pcAsset->getSound(20));
							}
						}

						//If the Ball is Ice, Freeze Them!
						if (pcBalls[i]->GetBallType() == CBall::eICE)
						{
							m_eLegStatus = CPlayer::eFROZEN;
							pcBalls[i]->SetActiveStatus(false);
							m_pcSound->PlaySoundFile(m_pcAsset->getSound(16));
							m_pcSound->PlaySoundFile(m_pcAsset->getSound(15));
						}

						if (pcBalls[i]->GetBallType() == CBall::eELECTRIC)
						{
							if (pcBalls[i]->GetTarget() == this)
							{
								pcBalls[i]->SetActiveStatus(false);
							}
							m_pcSound->PlaySoundFile(m_pcAsset->getSound(22));
						}

						if (pcBalls[i]->GetBallType() == CBall::eNORM)
						{
							m_pcSound->PlaySoundFile(m_pcAsset->getSound(18));
						}

						//Respawn the player and apply gravity
						pcBalls[i]->GetBallComponent()->SetThrownStatus(false);
						XMVECTOR negateForce = XMVectorSet(-pcBalls[i]->GetLevelOfForceVector().m128_f32[0], -pcBalls[i]->GetLevelOfForceVector().m128_f32[1], -pcBalls[i]->GetLevelOfForceVector().m128_f32[2], 1.0f);

						if (negateForce.m128_f32[2] > 0)
							negateForce.m128_f32[2] -= 15.0f;
						if (negateForce.m128_f32[2] < 0)
							negateForce.m128_f32[2] += 15.0f;

						pcBalls[i]->SetLevelOfForceVector(negateForce);
						m_pcSound->PlaySoundFile(m_pcAsset->getSound(3));

						continue;
					}

					//If the Ball is an ice ball and it isn't yours
					if (pcBalls[i]->GetBallType() == CBall::eICE && pcBalls[i]->GetOwner() != this)
					{
						//If the Ball has ice traps
						if (pcBalls[i]->GetBallComponent()->m_tIceTraps.size() > 0)
						{
							for (int k = 0; k < pcBalls[i]->GetBallComponent()->m_tIceTraps.size(); k++)
							{
								if (end::classify_sphere_to_pill(pcBalls[i]->GetBallComponent()->m_tIceTraps[k], m_pcCharacterComponent->m_tCharacterCollider) == 1)
								{
									m_eLegStatus = CPlayer::eFROZEN;
									m_pcScene->removeObjectFromScene(pcBalls[i]->GetBallComponent()->m_ptIceTrapModels[k]);
									pcBalls[i]->GetBallComponent()->m_ptIceTrapModels.erase(pcBalls[i]->GetBallComponent()->m_ptIceTrapModels.begin() + k);
									pcBalls[i]->GetBallComponent()->m_tIceTraps.erase(pcBalls[i]->GetBallComponent()->m_tIceTraps.begin() + k);
									pcBalls[i]->GetBallComponent()->m_fIceTrapsTimes.erase(pcBalls[i]->GetBallComponent()->m_fIceTrapsTimes.begin() + k);
									m_pcSound->PlaySoundFile(m_pcAsset->getSound(15));
									continue;
								}
							}
						}
					}
				}
				break;
				}

				//If you both don't have ownership, pick up the ball
				if (m_eHandStatus == eEMPTY && !pcBalls[i]->GetOwner() && pcBalls[i]->GetBallStatus() == CBall::eIDLE)
				{
					PickUpBall(pcBalls[i]);
					m_pcSound->PlaySoundFile(m_pcAsset->getSound(0));
					continue;
				}

				//If the incoming ball is yours, and it has been thrown
				if (pcBalls[i]->GetOwner() == this && pcBalls[i]->GetBallStatus() == CBall::eTHROWN)
				{
					//You are now stunned
					m_eLegStatus = CPlayer::eSTUNNED;
					m_pcSound->PlaySoundFile(m_pcAsset->getSound(21));

					//Stop the Balls orbit, make it bounce, and slow it down a bit
					pcBalls[i]->GetBallComponent()->SetThrownStatus(false);
					XMVECTOR negateForce = XMVectorSet(-pcBalls[i]->GetLevelOfForceVector().m128_f32[0], -pcBalls[i]->GetLevelOfForceVector().m128_f32[1], -pcBalls[i]->GetLevelOfForceVector().m128_f32[2], 1.0f);

					if (negateForce.m128_f32[2] > 0)
						negateForce.m128_f32[2] -= 15.0f;
					if (negateForce.m128_f32[2] < 0)
						negateForce.m128_f32[2] += 15.0f;

					pcBalls[i]->SetLevelOfForceVector(negateForce);
					pcBalls[i]->IncreaseNumBounced();
					m_pcSound->PlaySoundFile(m_pcAsset->getSound(18));
					continue;
				}

				//If the incoming ball is not yours, and it has been thrown
				if (pcBalls[i]->GetOwner() != this && pcBalls[i]->GetBallStatus() == CBall::eTHROWN)
				{
					//Stop the Balls orbit, make it bounce, and slow it down a bit
					pcBalls[i]->GetBallComponent()->SetThrownStatus(false);
					XMVECTOR negateForce = XMVectorSet(-pcBalls[i]->GetLevelOfForceVector().m128_f32[0], -pcBalls[i]->GetLevelOfForceVector().m128_f32[1], -pcBalls[i]->GetLevelOfForceVector().m128_f32[2], 1.0f);

					if (negateForce.m128_f32[2] > 0)
						negateForce.m128_f32[2] -= 15.0f;
					if (negateForce.m128_f32[2] < 0)
						negateForce.m128_f32[2] += 15.0f;

					pcBalls[i]->SetLevelOfForceVector(negateForce);
					pcBalls[i]->IncreaseNumBounced();
					continue;
				}
			}
			else
			{
				m_pcCharacterComponent->m_tCharacterCollider.collided = false;
				pcBalls[i]->GetBallComponent()->m_tBallCollider.collided = false;
			}

			//If the Ball Hits the Block Collider
			if (classify_sphere_to_pill(pcBalls[i]->GetBallComponent()->m_tBallCollider, m_pcCharacterComponent->m_tBCCollider) == 1)
			{
				m_pcCharacterComponent->m_tCharacterCollider.collided = true;
				pcBalls[i]->GetBallComponent()->m_tBallCollider.collided = true;

				//If the incoming ball is not yours, and it has been thrown
				if ((pcBalls[i]->GetOwner() != nullptr && pcBalls[i]->GetOwner() != this) && pcBalls[i]->GetBallStatus() == CBall::eTHROWN)
				{
					//If the player is blocking and the current ball is blockable
					if (m_eHandStatus == CPlayer::eBLOCKING && pcBalls[i]->GetBlockableStat())
					{
						//Turn on Gravity 
						pcBalls[i]->GetBallComponent()->SetThrownStatus(false);

						//Set New Force
						XMVECTOR negateForce = XMVectorSet(-pcBalls[i]->GetLevelOfForceVector().m128_f32[0], -pcBalls[i]->GetLevelOfForceVector().m128_f32[1], -pcBalls[i]->GetLevelOfForceVector().m128_f32[2], 1.0f);

						if (negateForce.m128_f32[2] > 0)
							negateForce.m128_f32[2] -= 15.0f;
						if (negateForce.m128_f32[2] < 0)
							negateForce.m128_f32[2] += 15.0f;

						pcBalls[i]->SetLevelOfForceVector(negateForce);
						pcBalls[i]->IncreaseNumBounced();

						m_pcSound->PlaySoundFile(m_pcAsset->getSound(1));
					}

					//If the player is catching and the current ball is catchable
					if (m_eHandStatus == eCATCHING && pcBalls[i]->GetCatchableStat())
					{
						PickUpBall(pcBalls[i]);
						m_pcSound->PlaySoundFile(m_pcAsset->getSound(1));
					}
				}

				if (pcBalls[i]->GetOwner() == nullptr && pcBalls[i]->GetBallStatus() == CBall::eTHROWN)
				{
					//If the player is blocking and the current ball is blockable
					if (m_eHandStatus == CPlayer::eBLOCKING && pcBalls[i]->GetBlockableStat())
					{
						//Turn on Gravity 
						pcBalls[i]->GetBallComponent()->SetThrownStatus(false);

						//Set New Force
						XMVECTOR negateForce = XMVectorSet(-pcBalls[i]->GetLevelOfForceVector().m128_f32[0], -pcBalls[i]->GetLevelOfForceVector().m128_f32[1], -pcBalls[i]->GetLevelOfForceVector().m128_f32[2], 1.0f);

						if (negateForce.m128_f32[2] > 0)
							negateForce.m128_f32[2] -= 15.0f;
						if (negateForce.m128_f32[2] < 0)
							negateForce.m128_f32[2] += 15.0f;

						pcBalls[i]->SetLevelOfForceVector(negateForce);
						pcBalls[i]->IncreaseNumBounced();

						m_pcSound->PlaySoundFile(m_pcAsset->getSound(1));
					}

					//If the player is catching and the current ball is catchable
					if (m_eHandStatus == eCATCHING && pcBalls[i]->GetCatchableStat())
					{
						PickUpBall(pcBalls[i]);
						m_pcSound->PlaySoundFile(m_pcAsset->getSound(1));
					}
				}
			}
		}

		//Calculate current Position
		XMFLOAT3 curPos1 = m_pcCharacterComponent->GetPosition();

		//Check against each spawned Barrier for collision
		for (int i = 0; i < (int)theBarriers.size(); i++)
		{
			int m_nClosetPlane = end::classify_pill_to_wall(*theBarriers[i]->GetWallCollider(), m_pcCharacterComponent->m_tCharacterCollider);
			if (m_nClosetPlane == -1)
			{
				m_pcCharacterComponent->m_tCharacterCollider.collided = false;
				theBarriers[i]->GetWallCollider()->collided = false;
				m_pcCharacterComponent->SetOnWallStatus(false);
			}
			else
			{
				m_pcCharacterComponent->m_tCharacterCollider.collided = true;
				theBarriers[i]->GetWallCollider()->collided = true;

				XMVECTOR lerp = end::wallerp(*theBarriers[i]->GetWallCollider(), m_pcCharacterComponent->m_tCharacterCollider, m_nClosetPlane);
				XMMATRIX mLerp = XMMatrixTranslationFromVector(lerp);
				m_pcCharacterComponent->SetWorldTransform(XMMatrixMultiply(*m_pcCharacterComponent->GetWorldTransform(), mLerp));
				if (m_nClosetPlane == 5)
				{
					m_fDistFromPlanetoid = end::Distance(curPos1, m_pcPhysics->m_tPlanetoidCollider.center);
					m_pcCharacterComponent->SetOnWallStatus(true);
					break;
				}
			}
		}
	}

	if (m_fCoolDownTimer > 0.0f)
	{
		m_fCoolDownTimer -= fT;

		if (m_fCoolDownTimer < 0.0f)
			m_fCoolDownTimer = 0.0f;
	}

	if (m_fDodgeCoolDownTimer > 0.0f)
	{
		m_fDodgeCoolDownTimer -= fT;

		if (m_fDodgeCoolDownTimer < 0.0f)
			m_fDodgeCoolDownTimer = 0.0f;
	}

	if (m_fAnimationTimer > 0.0f)
	{
		m_fAnimationTimer -= fT;
		if (m_fAnimationTimer < 0.0f)
			m_fAnimationTimer = 0.0f;
	}

	//Calculate current Position
	XMMATRIX worldTransform = *m_pcCharacterComponent->GetWorldTransform();
	XMFLOAT3 curPos;
	curPos.x = worldTransform.r[3].m128_f32[0];
	curPos.y = worldTransform.r[3].m128_f32[1];
	curPos.z = worldTransform.r[3].m128_f32[2];

	//Set Distance
	SetCurrentDistance(end::Distance(curPos, m_pcPhysics->m_tPlanetoidCollider.center));
	SetPlanetoidDistance(end::Distance(curPos, m_pcPhysics->m_tPlanetoidCollider.center));

	XMMATRIX modelMatrix = *m_pcCharacterComponent->GetWorldTransform();

	XMStoreFloat4x4(&worldMatrix, modelMatrix);
}

void CPlayer::PickUpBall(CBall * tBall)
{
	//Reset Stats
	tBall->ResetNumBounced();

	//Set Ownership of Ball and Player
	this->m_eHandStatus = eHOLDING;
	this->m_pcHeldBall = tBall;
	tBall->SetBallStatus(CBall::eHELD);
	tBall->SetOwner(this);

	//Play Animation
	m_bPickUp = true;
	SetAnimationTimer(0.25f);

	//Calculate In Hand Positon and Set Position of Ball
	XMFLOAT4 heldInHand = XMFLOAT4(this->m_handPosition.m128_f32[0] + tBall->GetBallComponent()->m_tBallCollider.radius,
		this->m_handPosition.m128_f32[1],
		this->m_handPosition.m128_f32[2],
		this->m_handPosition.m128_f32[3]);

	tBall->GetBallComponent()->SetPosition(heldInHand);
}

void CPlayer::PickUpPowerUp(CPowerUp * cPowerUp)
{
	//Set Ownership of Player
	m_pcActivatedPowerUp = cPowerUp;
	m_pcPhysics->RemovePhysComponent(cPowerUp->GetComponent());
	m_pcScene->removeObjectFromScene(cPowerUp);
	m_pcActivatedPowerUp->SetActivationStatus(true);

	switch (m_pcActivatedPowerUp->GetPowerUpType())
	{
	case CPowerUp::eJUMPBOOST:
	{
		m_fPowerUpJumpSpeed = 5.0f;
		break;
	}
	case CPowerUp::eREFLECTOR:
	{
		m_bPowerUpReflector = true;
		break;
	}
	case CPowerUp::eSPEEDBOOST:
	{
		m_fPowerUpRunSpeed = 7.5f;
		break;
	}
	case CPowerUp::eTHROWBOOST:
	{
		m_fPowerUpThrowSpeed = 5.0f;
		break;
	}
	case CPowerUp::eDODGEBOOST:
	{
		m_bPowerUpDodge = true;
		break;
	}
	}

	SetAnimationTimer(2.0f);
	m_bPickUp = true;

	m_pcSound->PlaySoundFile(m_pcAsset->getSound(27));
}

void CPlayer::ThrowBall(float fT)
{
	//Play Animation
	m_bThrowing = true;
	
	SetAnimationTimer(0.55f);
}

void CPlayer::RollBall(float fT)
{
	m_pcHeldBall->SetBallStatus(CBall::eROLLING);

	m_pcHeldBall = nullptr;
	m_eHandStatus = eEMPTY;

	m_bRolling = true;
	SetAnimationTimer(1.0f);
}

//void CPlayer::SetPlayerController(CPlayer tPlayer, int nControllerNum)
//{
//	if (m_pcInput->IsControllerConnected(nControllerNum))
//	{
//		//tPlayer.m_nPlayerNum = nControllerNum;
//	}
//}

void CPlayer::SetPlayerPosition(XMFLOAT4 fPosition)
{
	m_pcCharacterComponent->SetPosition(fPosition);
}

CPlayer::ePlayerHandStatus CPlayer::GetHandStatus()
{
	return m_eHandStatus;
}

void CPlayer::SetHandStatus(CPlayer::ePlayerHandStatus eHandStatus)
{
	m_eHandStatus = eHandStatus;
}

CPlayer::ePlayerLegStatus CPlayer::GetLegStatus()
{
	return m_eLegStatus;
}

void CPlayer::SetLegStatus(CPlayer::ePlayerLegStatus eLegStatus)
{
	m_eLegStatus = eLegStatus;
}

CPlayer::ePlayerType CPlayer::GetPlayerType()
{
	return m_ePlayerType;
}

void CPlayer::SetPlayerType(CPlayer::ePlayerType eType)
{
	m_ePlayerType = eType;
}

CPlayer::eTeamSide CPlayer::GetPlayerSide()
{
	return m_eTeamSide;
}

void CPlayer::SetPlayerSide(CPlayer::eTeamSide eSide)
{
	m_eTeamSide = eSide;
}

CPhysicsManager::cCharacterPhysComponent * CPlayer::GetCharacterComponent()
{
	return m_pcCharacterComponent;
}

CBall * CPlayer::GetHeldBall()
{
	return m_pcHeldBall;
}

void CPlayer::SetHeldBall(CBall* cHeldBall)
{
	m_pcHeldBall = cHeldBall;
}

unsigned int CPlayer::GetKillCount()
{
	return m_nKills;
}

void CPlayer::IncreaseKillCount()
{
	m_nKills++; // Momma, I just killed a man!
}

void CPlayer::ResetKillCount()
{
	m_nKills = 0;
}

void CPlayer::SetLivesCount(unsigned int nLives)
{
	m_nLives = nLives;
}

unsigned int CPlayer::GetBlockCount()
{
	return m_nBlocks;
}

void CPlayer::IncreaseBlockCount()
{
	m_nBlocks++;
}

void CPlayer::ResetBlockCount()
{
	m_nBlocks = 0;
}

unsigned int CPlayer::GetDeathCount()
{
	return m_nDeaths;
}

void CPlayer::IncreaseDeathCount()
{
	m_nDeaths++;
}

void CPlayer::ResetDeathCount()
{
	m_nDeaths = 0;
}

unsigned int CPlayer::GetLivesCount()
{
	return m_nLives;
}

void CPlayer::IncreaseLivesCount()
{
	m_nLives++;
}

void CPlayer::DecreaseLivesCount()
{
	m_nLives--;
}

unsigned int CPlayer::GetScoreCount()
{
	return m_nScore;
}

void CPlayer::IncreaseScoreCount(unsigned int nScore)
{
	m_nScore += nScore;
}

void CPlayer::DecreaseScoreCount(unsigned int nScore)
{
	m_nScore -= nScore;
}

void CPlayer::SetScoreCount(unsigned int nScore)
{
	m_nScore = nScore;
}

void CPlayer::ResetScoreCount()
{
	m_nScore = 0;
}

XMVECTOR CPlayer::GetDodgeForce()
{
	return m_fDodgeForce;
}

void CPlayer::SetDodgeForce(XMVECTOR dodgeForce)
{
	m_fDodgeForce = dodgeForce;
}

void CPlayer::ResetJumpForce()
{
	m_fJumpForce = FJUMPFORCE;
}

float CPlayer::GetStartDistance()
{
	return m_fStartDist;
}

void CPlayer::SetStartDistance(float fDistance)
{
	m_fStartDist = fDistance;
}

float CPlayer::GetCurrentDistance()
{
	return m_fCurDist;
}

void CPlayer::SetCurrentDistance(float fDistance)
{
	m_fCurDist = fDistance;
}

float CPlayer::GetPlanetoidDistance()
{
	return m_fDistFromPlanetoid;
}

void CPlayer::SetPlanetoidDistance(float fDistance)
{
	m_fDistFromPlanetoid = fDistance;
}

float CPlayer::GetBlockTimer()
{
	return m_fBlockTimer;
}

void CPlayer::ResetBlockTimer()
{
	m_fBlockTimer = FBLOCKTIME;
}

float CPlayer::GetCatchTimer()
{
	return m_fCatchTimer;
}

void CPlayer::ResetCatchTimer()
{
	m_fCatchTimer = FCATCHTIME;
}

float CPlayer::GetDodgeTimer()
{
	return m_fDodgeTime;
}

void CPlayer::ResetDodgeTimer()
{
	m_fDodgeTime = FDODGETIME;
}

float CPlayer::GetJumpTimer()
{
	return m_fJumpTimer;
}

void CPlayer::ResetJumpTimer()
{
	m_fJumpTimer = FJUMPTIME;
}

float CPlayer::GetInvincibilityTimer()
{
	return m_fInvincibilityTimer;
}

void CPlayer::ResetInvincibilityTimer()
{
	m_fJumpTimer = FINVCINCIBLETIME;
}

float CPlayer::GetRespawnTimer()
{
	return m_fRespawnTimer;
}

void CPlayer::ResetRespawnTimer()
{
	m_fRespawnTimer = FRESPAWNTIME;
}

unsigned int CPlayer::GetPlayerNumber()
{
	return m_nPlayerNumber;
}

void CPlayer::SetPlayerNumber(unsigned int nNumber)
{
	m_nPlayerNumber = nNumber;
}

float CPlayer::GetCoolDownTimer()
{
	return m_fCoolDownTimer;
}

void CPlayer::ResetCoolDownTimer()
{
	m_fCoolDownTimer = FCOOLDOWNTIMER;
}

float CPlayer::GetDodgeCoolDownTimer()
{
	return m_fDodgeCoolDownTimer;
}

void CPlayer::ResetDodgeCoolDownTimer()
{
	m_fDodgeCoolDownTimer = FDODGECOOLDOWNTIMER;
}

float CPlayer::GetStunTimer() {
	return m_fStunTimer;
}

void CPlayer::ResetStunTimer()
{
	m_fStunTimer = FSTUNTIME;
}

float CPlayer::GetFrozenTimer()
{
	return m_fFrozenTimer;
}

void CPlayer::ResetFrozenTimer()
{
	m_fFrozenTimer = FFROZENTIMER;
}

unsigned int CPlayer::GetUnFreezeCount()
{
	return m_nUnFreezeCounter;
}

void CPlayer::IncreaseUnFreezeCount()
{
	m_nUnFreezeCounter++;
}

void CPlayer::ResetUnFreezeCount()
{
	m_nUnFreezeCounter = 0;
}

bool CPlayer::GetRespawnStatus()
{
	return m_bRespawnStatus;
}

void CPlayer::SetRespawnStatus(bool bRespawnStatus)
{
	m_bRespawnStatus = bRespawnStatus;
}

bool CPlayer::GetInvincibilityStatus()
{
	if (m_bDebugInvinc)
		return true;

	return m_bInvincibility; // *insert Mario invincibility music here*

}

void CPlayer::SetInvincibilityStatus(bool bInvincibilityStatus)
{
	m_bInvincibility = bInvincibilityStatus;

	if (bInvincibilityStatus == true)
		ResetInvincibilityTimer();
}

float CPlayer::GetAnimationTimer()
{
	return m_fAnimationTimer;
}

void CPlayer::SetAnimationTimer(float fTime)
{
	m_fAnimationTimer = fTime;
}
