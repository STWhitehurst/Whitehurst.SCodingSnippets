/*****************************************************************************************
* Filename:											CBarrier.h
* Date:												04/08/2019
* Mod. Date:										04/08/2019
* Mod. Initials:									STW
* Author:											Sydney Whitehurst
* Purpose:											The Barrier's individul class to hold all
*													properties and Update function.
*													Derived from TSceneObj and connected to
*													CStaticMeshPhysComponent to apply Physics
******************************************************************************************/

#pragma once
#include "CSceneManager.h"
#include "CPhysicsManager.h"
#define MODELID					4

class CBarrier : public CSceneManager::TSceneObj
{
	end::TWall										m_tWallCollider;
	XMMATRIX										m_WorldTransform;
	bool											m_bDrawColliders;

public:

	/***************************************************************************************
	* CBarrier()									Constructor
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										N/A
	*
	* Mod. Date:									04/09/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	CBarrier();

	/***************************************************************************************
	* CBarrier()									Constructor - Set up StaticMeshComponent
	*
	* Ins:											XMFLOAT4X4*, float, float, float
	*
	* Outs:											N/A
	*
	* Returns:										N/A
	*
	* Mod. Date:									04/08/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	CBarrier(float fHeight, float fWidth, float fLength, XMVECTOR wallPosition, end::TSphere tPlanetoidCollider, int nTextureId);

	/***************************************************************************************
	* ~CBarrier()									Deletes pointers and cleans up memory
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										N/A
	*
	* Mod. Date:									04/08/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	~CBarrier();

	/***************************************************************************************
	* GetWorldTransform()							Returns the World transform of the barriers
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										XMMATRIX*
	*
	* Mod. Date:									04/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	XMMATRIX*										GetWorldTransform();

	/***************************************************************************************
	* SetWorldTransform()							Sets the barrier's world transform
	*
	* Ins:											XMMATRIX
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									04/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											SetWorldTransform(XMMATRIX worldTransform);

	/***************************************************************************************
	* GetWallCollider()								Returns the wall's collider
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										end::TWall*	
	*
	* Mod. Date:									04/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	end::TWall*										GetWallCollider();

	/***************************************************************************************
	*SetWallCollider()								Set the wall's collider
	*
	* Ins:											end::TWall
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									04/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											SetWallCollider(end::TWall tWallCollider);

	/***************************************************************************************
	* GetColliderDraw()								Returns the draw barrier collider bool
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										bool
	*
	* Mod. Date:									04/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	bool											GetColliderDraw();

	/***************************************************************************************
	*SetColliderDraw()								Set the barrier's collider draw
	*
	* Ins:											end::TWall
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									04/16/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											SetColliderDraw(bool bColliderDraw);

	/***************************************************************************************
	* Update()										The barrier's update function
	*
	* Ins:											N/A
	*
	* Outs:											N/A
	*
	* Returns:										void
	*
	* Mod. Date:									04/08/2019
	* Mod. Initals:									STW
	*****************************************************************************************/
	void											Update();
};

