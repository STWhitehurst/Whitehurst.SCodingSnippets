/*****************************************************************************************
* Filename:										CPowerUp.h
* Date:											05/07/2019
* Mod. Date:									05/07/2019
* Mod. Initials:								STW
* Author:										Sydney Whitehurst
* Purpose:										The Power Up class to hold all
*												properties and virtual Update function.
*												Derived from TSceneObj and connected to
*												CStaticMeshPhysComponent to apply Physics
******************************************************************************************/
#pragma once
#include "CSceneManager.h"
#include "CPhysicsManager.h"
#include "CSound.h"
#include "CAssetManager.h"

//DEFINES
#define MODELIDNUM								3
#define	SPEEDBOOSTTEXTUREIDNUM					29
#define	THROWBOOSTTEXTUREIDNUM					30
#define	JUMPBOOSTTEXTUREIDNUM					31
#define	DODGEBOOSTTEXTUREIDNUM					32
#define	REFLECTORTEXTUREIDNUM					33
#define	FACTIVATIONTIME							10.0f

class CPowerUp : public CSceneManager::TSceneObj
{
public:
	enum ePowerUpType {eNONE = 0, eSPEEDBOOST, eJUMPBOOST, eTHROWBOOST, eREFLECTOR, eDODGEBOOST};

private:
	bool												m_bIsActive;
	bool												m_bIsActivated;
	float												m_fActivationTimer;
	ePowerUpType										m_eType;
	CPhysicsManager::cStaticMeshPhysComponent*			m_pcComponent;
	CSceneManager*										m_pcScene;
	CPhysicsManager::CPhysicsManager*					m_pcPhysics;
	CSound*												m_pcSound;
	CAssetManager*										m_pcAsset;

public:

	/***************************************************************************************
	* CPowerUp()									Constructor
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										N/A
	*
	* Mod. Date:									05/07/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	CPowerUp();

	/***************************************************************************************
	* CPower()										Constructor - Sets up Stats, Type and 
	*												component
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										N/A
	*
	* Mod. Date:									05/07/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	CPowerUp(XMFLOAT4X4* pWorldTransform, enum ePowerUpType ePowerUpType, CSceneManager* pcScene, CPhysicsManager::CPhysicsManager* pcPhysics, CSound* pcSound, CAssetManager* pcAsset);

	/***************************************************************************************
	* ~CBall()										Destructor - Deletes pointers and cleans up memory
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										N/A
	*
	* Mod. Date:									05/07/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	~CPowerUp();

	/***************************************************************************************
	* Update()										Individual Update 
	*
	* Ins:											Float
	*
	* Outs:											N/A
	*
	* Returns:										N/A
	*
	* Mod. Date:									05/07/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											Update(float fT);

	/***************************************************************************************
	* GetActiveStatus()								Returns the Power Up Active Status
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										bool
	*
	* Mod. Date:									05/07/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	bool											GetActiveStatus();

	/***************************************************************************************
	* SetActiveStatus()								Sets the Power Up Active Status
	*
	* Ins:											bool
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									05/07/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											SetActiveStatus(bool bStatus);

	/***************************************************************************************
	* GetActivationStatus()							Returns the Power Up Activation Status
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										bool
	*
	* Mod. Date:									05/07/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	bool											GetActivationStatus();

	/***************************************************************************************
	* SetActivationStatus()							Sets the Power Up Activation Status
	*
	* Ins:											bool
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									05/07/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											SetActivationStatus(bool bStatus);

	/***************************************************************************************
	* GetActivationTime()							Returns the Power Up Activation Time
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										float
	*
	* Mod. Date:									05/07/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	float											GetActivationTime();

	/***************************************************************************************
	* GetPowerUpType()								Returns the Power Up Type
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										ePowerUpType
	*
	* Mod. Date:									05/07/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	CPowerUp::ePowerUpType							GetPowerUpType();

	/***************************************************************************************
	* GetComponent()								Returns the Power Up StaticMeshPhysComponent*
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										cStaticMeshPhysComponent*
	*
	* Mod. Date:									05/07/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	CPhysicsManager::cStaticMeshPhysComponent*		GetComponent();
};

