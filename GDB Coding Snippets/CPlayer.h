/*****************************************************************************************
* Filename:										CPlayer.h
* Date:											03/14/2019
* Mod. Date:									03/14/2019
* Mod. Initials:								NH, STW
* Author:										Nicholas Hatcher and Sydney Whitehurst
* Purpose:										The Player's individul class to hold all
*												properties and virtual Update function.
*												Derived from TSceneObj and connected to
*												CCharacterPhysComponent to apply Physics
******************************************************************************************/

#pragma once
#include "CSceneManager.h"
#include "CPhysicsManager.h"
#include "CInputManager.h"
#include "CAssetManager.h"
#include "CSound.h"
#include "CBarrier.h"
#include "CPowerUp.h"
#include "CFXs.h"

//DEFINES
#define	CHARACTERMODELIDNUM								5
#define CHARACTERTEXTUREIDNUM							7
#define	CHARACTERIDLEIDNUM								0
#define	CHARACTERRUNIDNUM								1
#define	CHARACTERTHROWIDNUM								2
#define	CHARACTERROLLIDNUM								3
#define	CHARACTERBLOCKIDNUM								4
#define	CHARACTERCATCHIDNUM								5
#define	CHARACTERPICKUPIDNUM							6
#define	CHARACTERJUMPIDNUM								7
#define	CHARACTERDODGEIDNUM								8
#define	CHARACTERLOSEIDNUM								9
#define	CHARACTERVICTORYIDNUM							10
#define	CHARACTERDEATHIDNUM								11
#define FJUMPTIME										0.5f
#define FJUMPFORCE										13.0f
#define FDODGETIME										0.1f
#define FDODGESPEED										25.0f
#define FBLOCKTIME										0.5f
#define	FCATCHTIME										0.5f
#define FSTUNTIME										2.0f
#define MAXAIRDODGE										3
#define FRESPAWNTIME									2.0f
#define FINVCINCIBLETIME								3.0f
#define FCOOLDOWNTIMER									1.0f
#define FDODGECOOLDOWNTIMER								0.75f
#define FFROZENTIMER									3.0f
#define NNUMTOUNFREEZE									10

class CBall;

class CPlayer :
	public CSceneManager::TSceneObj
{
public:

	enum ePlayerType { eAI = 0, eUSER };
	enum eTeamSide { eBLUETEAM = 0, eREDTEAM };
	enum eGameType { eSOLO = 0, eTEAM };
	enum ePlayerHandStatus { eEMPTY = 0, eHOLDING, eBLOCKING, eCATCHING, eHANDRESPAWNING };
	enum ePlayerLegStatus { eIDLE = 0, eRUNNING, eJUMPING, eDODGING, eAIRDODGING, eLEGRESPAWNING, eSTUNNED, eFROZEN };


protected:
	bool										m_bPickUp = false;
	bool										m_bThrowing = false;
	bool										m_bRolling = false;
	bool										m_bInvincibility;
	bool										m_bRespawnStatus;
	bool										m_bPowerUpReflector;
	bool										m_bDebugInvinc;
	bool										m_bPowerUpDodge = false;
	float										m_fPowerUpThrowSpeed;
	float										m_fPowerUpJumpSpeed;
	float										m_fPowerUpRunSpeed;
	float										m_fJumpForce;
	float										m_fJumpTimer;
	float										m_fDodgeSpeed;
	float										m_fDodgeTime;
	float										m_fBlockTimer;
	float										m_fCatchTimer;
	float										m_fRespawnTimer;
	float										m_fInvincibilityTimer;
	float										m_fCoolDownTimer;
	float										m_fDodgeCoolDownTimer;
	float										m_fStunTimer;
	float										m_fFrozenTimer;
	float										m_fAnimationTimer;
	float										m_fStartDist;
	float										m_fCurDist;
	float										m_fDistFromPlanetoid;
	ePlayerHandStatus							m_eHandStatus;
	ePlayerLegStatus							m_eLegStatus;
	ePlayerType									m_ePlayerType;
	eTeamSide									m_eTeamSide;
	eGameType									m_eCurrentGameType;
	unsigned int								m_nUnFreezeCounter;
	unsigned int								m_nAirDodges;
	unsigned int								m_nPlayerNumber;
	unsigned int								m_nLives;
	unsigned int								m_nKills;
	unsigned int								m_nDeaths;
	unsigned int								m_nBlocks;
	unsigned int								m_nScore;
	XMVECTOR									m_fDodgeForce;
	XMVECTOR									m_handPosition;
	CBall*										m_pcHeldBall;
	CPowerUp*									m_pcActivatedPowerUp;
	CPhysicsManager::cCharacterPhysComponent*	m_pcCharacterComponent;
	CUIManager*									m_cUISystem; //Don't delete
	CPhysicsManager::CPhysicsManager*			m_pcPhysics;
	CSceneManager*								m_pcScene;
	CInputManager*								m_pcInput;
	CSound*										m_pcSound;
	CAssetManager*								m_pcAsset;
	CAnimationManager*							m_pcAnim;
	
public:

	/***************************************************************************************
	* CPlayer()										Constructor
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										N/A
	*
	* Mod. Date:									03/14/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	CPlayer();

	/***************************************************************************************
	* CPlayer()										Constructor -Sets Specific Stats based
	*												on the passed in enums.
	*
	* Ins:											XMFLOAT4X4*, enum ePlayerType
	*
	* Outs:											N/A
	*
	* Returns:										N/A
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	CPlayer(XMFLOAT4X4* pWorldTransform, enum ePlayerType ePlayerType, enum eGameType eCurrentGameType, enum eTeamSide eCurrentTeamSide, int nPlayerNumber, CUIManager * cUISystem, CPhysicsManager::CPhysicsManager* pcPhysics, CSceneManager* pcScene, CInputManager* pcInput, CSound* pcSound, CAssetManager* pcAsset, CAnimationManager* pcAnim);

	/***************************************************************************************
	* CPlayer()										Overloaded Constructor -Sets Specific Stats 
	*												based on the passed in enums and the Game
	*												Type being SOLO.
	*
	* Ins:											XMFLOAT4X4*, enum ePlayerType
	*
	* Outs:											N/A
	*
	* Returns:										N/A
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	CPlayer(XMFLOAT4X4* pWorldTransform, enum ePlayerType ePlayerType, enum eGameType eCurrentGameType, int nPlayerNumber, CUIManager * cUISystem, CPhysicsManager::CPhysicsManager* pcPhysics, CSceneManager* pcScene, CInputManager* pcInput, CSound* pcSound, CAssetManager* pcAsset, CAnimationManager* pcAnim);

	/***************************************************************************************
	* ~CPlayer()									Deletes pointers and cleans up memory
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										N/A
	*
	* Mod. Date:									03/14/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	~CPlayer();

	/****************************************************************************************
	* Update()										Will update the player and ball info
	*												in the match.
	*
	* Ins:											float
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									02/19/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	virtual void									Update(float fT, vector<CPlayer*> Lobby, vector<CBall*> m_vSpawnedBalls, vector<CBarrier*> theBarriers, vector<CPowerUp*> thePowerUps, bool bControlsOn);

	/****************************************************************************************
	* Update()										Will update the player and ball info
	*												in the Kiln(HUB).
	*
	* Ins:											float
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									02/19/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	virtual void									Update(float fT, vector<CPlayer*> pcLobby, vector<CBall*> pcBalls, vector<CBarrier*> theBarriers, bool bControlsOn);

	/****************************************************************************************
	* PickupBall()									Picks up the ball and places it within
	*												that player's posession.
	*
	* Ins:											CPlayer*, TBall*
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									02/19/2019
	* Mod. Initals:									STW
	****************************************************************************************/
	void											PickUpBall(CBall * tBall);

	/****************************************************************************************
	* PickupPowerUp()								Picks up the Power Up and places it within
	*												that player's posession.
	*
	* Ins:											CPowerUp*
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									05/08/2019
	* Mod. Initals:									STW
	****************************************************************************************/
	void											PickUpPowerUp(CPowerUp * cPowerUp);

	/***************************************************************************************
	* ThrowBall()									Throws the Ball within the Player's posession.
	*
	* Ins:											float
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									02/19/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											ThrowBall(float fT);

	/***************************************************************************************
	* RollBall()									Rolls the Ball within the Player's posession.
	*
	* Ins:											float
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									02/19/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											RollBall(float fT);

	/******************************************************************************************
	* SetPlayerController()							Takes a tPlayer and a controller number (1-4) and attempts
	*												to set that controller to the player. It will not work if
	*												the controller passed in isn't connected.
	*
	* Ins:											tPlayer, nControllerNum
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									02/28/2019
	* Mod. Initals:									JRD
	******************************************************************************************/
	//void SetPlayerController(CPlayer tPlayer, int nControllerNum);


	/*****************************************************************************************
	* SetPlayerPosition()							Sets the player's position.
	*
	* Ins:											XMFLOAT4
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									02/28/2019
	* Mod. Initals:									STW
	****************************************************************************************/
	void											SetPlayerPosition(XMFLOAT4 fPosition);

	/***************************************************************************************
	* GetHandStatus()								Returns the Player's current Hand Status
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										CPlayer::ePlayerHandStatus
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	CPlayer::ePlayerHandStatus						GetHandStatus();

	/***************************************************************************************
	* SetHandStatus()								Sets the Player's Hand Status
	*
	* Ins:											CPlayer::ePlayerHandStatus
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											SetHandStatus(CPlayer::ePlayerHandStatus eHandStatus);

	/***************************************************************************************
	* GetLegStatus()								Returns the Player's current Leg Status
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										CPlayer::ePlayerLegStatus
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	CPlayer::ePlayerLegStatus						GetLegStatus();

	/***************************************************************************************
	* SetLegStatus()								Sets the Player's Leg Status
	*
	* Ins:											CPlayer::ePlayerLegStatus
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											SetLegStatus(CPlayer::ePlayerLegStatus eLegStatus);

	/***************************************************************************************
	* GetPlayerType()								Returns the Player's Type
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										CPlayer::ePlayerType
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	CPlayer::ePlayerType							GetPlayerType();

	/***************************************************************************************
	* SetPlayerType()								Sets the Player's Type
	*
	* Ins:											CPlayer::ePlayerType
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											SetPlayerType(CPlayer::ePlayerType eType);

	/***************************************************************************************
	* GetPlayerSide()								Returns the Player's Team Side
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										CPlayer::eTeamSide
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	CPlayer::eTeamSide								GetPlayerSide();

	/***************************************************************************************
	* SetPlayerSide()								Sets the Player's Team Side
	*
	* Ins:											CPlayer::eTeamSide
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											SetPlayerSide(CPlayer::eTeamSide eSide);

	/***************************************************************************************
	* GetCharacterComponent()						Returns the Player's Character Component
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										CPhysicsManager::cCharacterPhysComponent*
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	CPhysicsManager::cCharacterPhysComponent*		GetCharacterComponent();

	/***************************************************************************************
	* GetHeldBall()									Returns the Ball in the Player's Hand
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										CBall*
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	CBall*											GetHeldBall();

	/***************************************************************************************
	* SetHeldBall()									Sets a Ball for the Player to hold
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										CBall*
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											SetHeldBall(CBall* cHeldBall);

	/***************************************************************************************
	* GetKillCount()								Returns the Player's Kill Count
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										unsigned int
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	unsigned int									GetKillCount();

	/***************************************************************************************
	* IncreaseKillCount()							Increases the Player's Kill Count
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											IncreaseKillCount();

	/***************************************************************************************
	* ResetKillCount()								Resets the Player's Kill Count to zero
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											ResetKillCount();

	/***************************************************************************************
	* GetDeathCount()								Returns the Player's Death Count
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										unsigned int
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	unsigned int									GetDeathCount();

	/***************************************************************************************
	* IncreaseDeathCount()							Increases the Player's Death Count
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											IncreaseDeathCount();

	/***************************************************************************************
	* ResetDeathCount()								Resets the Player's Score Death to zero
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											ResetDeathCount();

	/***************************************************************************************
	* GetLivesCount()								Returns the Player's Lives Count
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										unsigned int
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	unsigned int									GetLivesCount();

	/***************************************************************************************
	* IncreaseLivesCount()							Increases the Player's Lives Count
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											IncreaseLivesCount();

	/***************************************************************************************
	* DecreaseLivesCount()							Decreases the Player's Lives Count
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											DecreaseLivesCount();

	/***************************************************************************************
	* SetLivesCount()								Sets the Player's Score Count to
	*												the amount passed int
	*
	* Ins:											unsigned int
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											SetLivesCount(unsigned int nLives);

	/***************************************************************************************
	* GetBlockCount()								Returns the Player's Block Count
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										unsigned int
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	unsigned int									GetBlockCount();

	/***************************************************************************************
	* IncreaseBlockCount()							Increases the Player's Block Count
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											IncreaseBlockCount();

	/***************************************************************************************
	* ResetBlockCount()								Resets the Player's Block Count to zero
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											ResetBlockCount();

	/***************************************************************************************
	* GetScoreCount()								Returns the Player's Score Count
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										unsigned int
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	unsigned int									GetScoreCount();

	/***************************************************************************************
	* IncreaseScoreCount()							Increases the Player's Score Count by
	*												the amount passed int
	*
	* Ins:											unsigned int
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											IncreaseScoreCount(unsigned int nScore);

	/***************************************************************************************
	* DecreaseScoreCount()							Decreases the Player's Score Count by
	*												the amount passed in
	*
	* Ins:											unsigned int
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											DecreaseScoreCount(unsigned int nScore);

	/***************************************************************************************
	* SetScoreCount()								Sets the Player's Score Count
	*
	* Ins:											unsigned int
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											SetScoreCount(unsigned int nScore);

	/***************************************************************************************
	* ResetScoreCount()								Resets the Player's Score Count to zero
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											ResetScoreCount();

	/***************************************************************************************
	* GetDodgeForce()								Returns the Player's DodgeForce
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										XMVECTOR
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	XMVECTOR										GetDodgeForce();

	/***************************************************************************************
	* SetDodgeForce()								Sets the Player's Dodge Force
	*
	* Ins:											XMVECTOR
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											SetDodgeForce(XMVECTOR dodgeForce);

	/***************************************************************************************
	* ResetJumpForce()								Resets the Player's Jump Force
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											ResetJumpForce();

	/***************************************************************************************
	* GetStartDistance()							Returns the Player's Start Distance
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										float
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	float											GetStartDistance();

	/***************************************************************************************
	* SetStartDistance()							Sets the Player's Start Distance
	*
	* Ins:											float
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											SetStartDistance(float fDistance);

	/***************************************************************************************
	* GetCurrentDistance()							Returns the Player's Current Distance
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										float
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	float											GetCurrentDistance();

	/***************************************************************************************
	* SetCurrentDistance()							Sets the Player's Current Distance
	*
	* Ins:											float
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											SetCurrentDistance(float fDistance);

	/***************************************************************************************
	* GetPlanetoidDistance()						Returns the Player's Distance From the
	*												Planetoid
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										float
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	float											GetPlanetoidDistance();

	/***************************************************************************************
	* SetPlanetoidDistance()						Sets the Player's Distance from the
	*												Planetoid
	*
	* Ins:											float
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											SetPlanetoidDistance(float fDistance);

	/***************************************************************************************
	* GetBlockTimer()								Returns the Player's Block Timer
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										float
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	float											GetBlockTimer();

	/***************************************************************************************
	* ResetBlockTimer()								Resets the Player's Block Timer
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											ResetBlockTimer();

	/***************************************************************************************
	* GetCatchTimer()								Returns the Player's Catch Timer
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										float
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	float											GetCatchTimer();

	/***************************************************************************************
	* ResetCatchTimer()								Resets the Player's Catch Timer
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											ResetCatchTimer();

	/***************************************************************************************
	* GetDodgeTimer()								Returns the Player's Dodge Timer
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										float
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	float											GetDodgeTimer();

	/***************************************************************************************
	* ResetDodgeTimer()								Resets the Player's Dodge Timer
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											ResetDodgeTimer();

	/***************************************************************************************
	* GetJumpTimer()								Returns the Player's Jump Timer
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										float
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	float											GetJumpTimer();

	/***************************************************************************************
	* ResetJumpTimer()								Resets the Player's Jump Timer
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											ResetJumpTimer();

	/***************************************************************************************
	* GetInvincibilityTimer()						Returns the Player's Invincibility Timer
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										float
	*
	* Mod. Date:									03/18/2019
	* Mod. Initals:									JRD
	*****************************************************************************************/
	float											GetInvincibilityTimer();

	/***************************************************************************************
	* ResetInvincibilityTimer()						Resets the Player's Invincibility Timer
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/18/2019
	* Mod. Initals:									JRD
	*****************************************************************************************/
	void											ResetInvincibilityTimer();

	/***************************************************************************************
	* GetRespawnTimer()								Returns the Player's Respawn Timer
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										float
	*
	* Mod. Date:									03/17/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	float											GetRespawnTimer();

	/***************************************************************************************
	* ResetRespawnTimer()							Resets the Player's Respawn Timer
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/17/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											ResetRespawnTimer();

	/***************************************************************************************
	* GetPlayerNumber()								Returns the Player's Number
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										unsigned int
	*
	* Mod. Date:									03/18/2019
	* Mod. Initals:									JRD
	*****************************************************************************************/
	unsigned int									GetPlayerNumber();

	/***************************************************************************************
	* SetPlayerNumber()								Sets the Player's Number
	*
	* Ins:											unsigned int
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/20/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											SetPlayerNumber(unsigned int nNumber);

	/***************************************************************************************
	* GetCoolDownTimer()							Returns the Player's Cool Down Timer
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										float
	*
	* Mod. Date:									03/20/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	float											GetCoolDownTimer();

	/***************************************************************************************
	* ResetCoolDownTimer()							Resets the Player's Cool Down Timer
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/20/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											ResetCoolDownTimer();

	/***************************************************************************************
	* GetDodgeCoolDownTimer()						Returns the Dodge's Cool Down Timer
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										float
	*
	* Mod. Date:									05/01/2019
	* Mod. Initals:									JRD
	*****************************************************************************************/
	float											GetDodgeCoolDownTimer();

	/***************************************************************************************
	* ResetDodgeCoolDownTimer()						Resets the Dodge's Cool Down Timer
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									05/01/2019
	* Mod. Initals:									JRD
	*****************************************************************************************/
	void											ResetDodgeCoolDownTimer();

	/***************************************************************************************
	* GetStunTimer()								Returns the Player's Stun Timer
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										float
	*
	* Mod. Date:									03/20/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	float											GetStunTimer();

	/***************************************************************************************
	* ResetStunTimer()								Resets the Player's Stun Timer
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/20/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											ResetStunTimer();

	/***************************************************************************************
	* GetFrozenTimer()								Returns the Player's Frozen Timer
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										float
	*
	* Mod. Date:									03/25/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	float											GetFrozenTimer();

	/***************************************************************************************
	* ResetFrozenTimer()							Resets the Player's Frozen Timer
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/25/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											ResetFrozenTimer();

	/***************************************************************************************
	* GetUnFreezeCount()							Returns the Player's UnFreeze Count
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										unsigned int
	*
	* Mod. Date:									03/26/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	unsigned int									GetUnFreezeCount();

	/***************************************************************************************
	* IncreaseUnFreezeCount()						Increases the Player's UnFreeze Count
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/26/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											IncreaseUnFreezeCount();

	/***************************************************************************************
	* ResetUnFreezeCount()							Resets the Player's UnFreeze Count to zero
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/26/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											ResetUnFreezeCount();

	/***************************************************************************************
	* GetRespawnStatus()							Returns the Player's Respawn Status
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										bool
	*
	* Mod. Date:									03/30/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	bool											GetRespawnStatus();

	/***************************************************************************************
	* SetRespawnStatus()							Sets the Player's Respawn Status
	*
	* Ins:											bool
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/30/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											SetRespawnStatus(bool bRespawnStatus);

	/***************************************************************************************
	* GetInvincibiltyStatus()						Returns the Player's Invincibility Status
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										bool
	*
	* Mod. Date:									04/17/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	bool											GetInvincibilityStatus();

	/***************************************************************************************
	* SetInvincibilityStatus()						Sets the Player's Invincibility Status
	*
	* Ins:											bool
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									04/17/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											SetInvincibilityStatus(bool bInvincibilityStatus);

	/***************************************************************************************
	* GetAnimationTimer()							Returns the Player's Animatioon Timer
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										float
	*
	* Mod. Date:									05/28/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	float											GetAnimationTimer();

	/***************************************************************************************
	* SetAnimationTimer()							Resets the Player's Animation Timer
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									05/28/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											SetAnimationTimer(float fTime);
};

