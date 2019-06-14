/*****************************************************************************************
* Filename:										CBall.h
* Date:											03/14/2019
* Mod. Date:									03/14/2019
* Mod. Initials:								STW
* Author:										Sydney Whitehurst
* Purpose:										The Balls individul's class to hold all 
*												properties and virtual Update function. 
*												Derived from TSceneObj and connected to 
*												CBallPhysComponent to apply Physics 
******************************************************************************************/
#pragma once
#include "CSceneManager.h"
#include "CPhysicsManager.h"
#include "CPlayer.h"
#include "CBarrier.h"
#include "CSound.h"
#include "CAssetManager.h"

//DEFINES
#define MODELIDNUM								3
#define NORMTEXTUREIDNUM						1
#define FIRETEXTUREIDNUM						2
#define ICETEXTUREIDNUM							4
#define ELECTRICTEXTUREIDNUM					5
#define RABIDTEXTUREIDNUM
#define WINDTEXTUREIDNUM						3
#define MAXBOUNCES								3
#define ELECTRICMAXBOUNCES						6
#define FLVLOFFORCE								45.0f
#define FTHROWNTIME								2.7f
#define FEXPLOSIONTIME							2.0f
#define FBALLACTIVATIONTIME						5.0f
#define FROLLTIME								15.0f


class CBall :
	public CSceneManager::TSceneObj
{
public:
	enum eBallType { eNORM = 0, eFIRE, eICE, eELECTRIC, eRABID, eWIND };
	enum eBallStatus { eIDLE = 0, eHELD, eTHROWN, eBLOCKING, eBOUNCING, eEXPLODING, eACTIVATED, eROLLING, eTRAP };

private:
	bool										m_bBlockable;
	bool										m_bCatchable;
	bool										m_bIsActive;
	float										m_fThrownTimer;
	float										m_fExplosionTimer;
	float										m_fActivationTimer;
	float										m_fRollTimer;
	float										m_fPrevRollTime;
	float										m_fClosestEnemyDistance = 50;
	float										m_fScaleAmount;
	unsigned int								m_nNumBounced;
	eBallStatus									m_eStatus;
	eBallType									m_eBallType;
	CPlayer::eGameType							m_eCurrentGameType;
	XMVECTOR									m_fLevelOfForce;
	CPlayer*									m_pcOwner;
	CPlayer*									m_pcTarget;
	CPhysicsManager::cBallPhysComponent*		m_pcBallComponent;
	CSceneManager*								m_pcSceneManager;
	CPhysicsManager::CPhysicsManager*			m_pcPhysicsManager;
	CSound*										m_pcSoundManager;
	CAssetManager*								m_pcAssetManager;
	end::TSphere*								m_ptPlanetoid;

	CFXs										m_cEffects;
	float										m_fEffectCooldown = 0.2f;
public:
	
	/***************************************************************************************
	* CBall()										Constructor 
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
												CBall();

	/***************************************************************************************
	* CBall()										Constructor -Sets Specific Stats based 
	*												on the passed in enum. 
	*
	* Ins:											XMFLOAT4X4* pWorldTransform, enum eBallType eBallType, enum CPlayer::eGameType, bool bActiveStatus, CSceneManager*, CPhysicsManager::CPhysicsManager*, end::TSphere*
	*
	* Outs:											N/A
	*
	* Returns:										N/A
	*
	* Mod. Date:									03/14/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
												CBall(XMFLOAT4X4* pWorldTransform, enum eBallType eBallType, enum CPlayer::eGameType eCurrentGameType, bool bActiveStatus, CSceneManager* pcSceneManager, CPhysicsManager::CPhysicsManager* pcPhysicsManager, CSound* pcSound, CAssetManager* pcAsset, end::TSphere* tPlanetoid);
	
	/***************************************************************************************
	* ~CBall()										Destructor - Deletes pointers and cleans up memory 
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
												~CBall();

	/***************************************************************************************
	* Update()										Individual Update Function for Specific
	*												Types of Balls
	*
	* Ins:											float fT, vector<CBall*>, vector<CBarrier*>, vector<CPlayer*>
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									02/19/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void										Update(float fT, vector<CBall*> m_vSpawnedBalls, vector<CBarrier*> theBarriers, vector<CPlayer*> pcLobby);

	/***************************************************************************************
	* GetBlockableStat()							Returns Blockable Stat
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										bool
	*
	* Mod. Date:									03/14/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	bool										GetBlockableStat();

	/***************************************************************************************
	* GetCatchableStat()							Returns Catchable Stat
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										bool
	*
	* Mod. Date:									03/14/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	bool										GetCatchableStat();

	/***************************************************************************************
	* GetActiveStatus()								Returns Active Status
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										bool
	*
	* Mod. Date:									03/14/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	bool										GetActiveStatus();

	/***************************************************************************************
	* SetActiveStatus()								Sets Active Status
	*
	* Ins:											bool
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/14/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void										SetActiveStatus(bool bActiveStatus);

	/***************************************************************************************
	* GetThrownTime()								Returns Thrown Time
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										float
	*
	* Mod. Date:									03/14/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	float										GetThrownTime();

	/***************************************************************************************
	* ResetThrownTime()								Resets Thrown Time
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/14/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void										ResetThrownTime();

	/***************************************************************************************
	* GetExplosionTime()							Returns Explosion Time
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										float
	*
	* Mod. Date:									03/14/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	float										GetExplosionTime();

	/***************************************************************************************
	* GetRollTime()									Returns Roll Time
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
	float										GetRollTime();

	/***************************************************************************************
	* GetPrevRollTime()								Returns Prev Roll Time
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
	float										GetPrevRollTime();

	/***************************************************************************************
	* SetPrevRollTime()								Sets Prev Roll Time
	*
	* Ins:											float
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/25/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void										SetPrevRollTime(float fNewTime);

	/***************************************************************************************
	* GetNumBounced()								Returns the Number of Times Bounced
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										bool
	*
	* Mod. Date:									03/14/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	int											GetNumBounced();

	/***************************************************************************************
	* IncreaseNumBounced()							Increases the Number of Times Bounced
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/15/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void										IncreaseNumBounced();


	/***************************************************************************************
	* ResetNumBounced()								Resets the Number of Times Bounced to zero
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
	void										ResetNumBounced();

	/***************************************************************************************
	* GetBallType()									Returns the Ball Type
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										eBallType
	*
	* Mod. Date:									03/14/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	CBall::eBallType							GetBallType();

	/***************************************************************************************
	* SetBallType()									Sets the Ball to a Different Type
	*
	* Ins:											eBallType
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									04/04/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void										SetBallType(CBall::eBallType eType);

	/***************************************************************************************
	* GetBallStatus()								Returns the Ball Status
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										eBallType
	*
	* Mod. Date:									03/14/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	CBall::eBallStatus							GetBallStatus();

	/***************************************************************************************
	* SetBallStatus()								Sets the Ball Status
	*
	* Ins:											eBallType
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/14/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void										SetBallStatus(CBall::eBallStatus eStatus);

	/***************************************************************************************
	* GetOwner()									Returns the Ball'sOwner
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										CPlayer*
	*
	* Mod. Date:									03/14/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	CPlayer*									GetOwner();
	
	/***************************************************************************************
	* SetOwner()									Sets the Ball's Owner
	*
	* Ins:											CGameLogic::TPlayer*
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/14/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void										SetOwner(CPlayer* pcOwner);

	/***************************************************************************************
	* GetTarget()									Returns the Ball's Target
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										CPlayer*
	*
	* Mod. Date:									05/03/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	CPlayer*									GetTarget();

	/***************************************************************************************
	* GetBallComponent()							Returns Ball Phys Component
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										CPhysicsManager::CBallComponent*
	*
	* Mod. Date:									03/14/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	CPhysicsManager::cBallPhysComponent*		GetBallComponent();

	/***************************************************************************************
	* GetLevelOfForceVector()						Returns the Ball's Force Vector
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										XMVECTOR
	*
	* Mod. Date:									03/14/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	XMVECTOR									GetLevelOfForceVector();

	/***************************************************************************************
	* SetLevelOfForceVector()						Sets the Ball's Force Vector
	*
	* Ins:											XMVECTOR
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									03/14/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void										SetLevelOfForceVector(XMVECTOR fLevelOfForce);
};

