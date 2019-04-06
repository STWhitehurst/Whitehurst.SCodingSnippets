using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Door : MasterInteractableItemBase
{

    private bool mIsOpen = false;
    private bool mIsUnlocked = false;

    private float moveTime;
    public Vector3 OpenOffset;
    private Vector3 startingPosition;
    public Key theKey;
    //SoundSource noise;


    public override void OnInteract()
    {
        if (mIsUnlocked == false)
        {
            if (GameManager.Instance.Player.playerInventory.Inventory.FindItem(theKey))
            {
                //noise.isMakingSound = true;

                mIsUnlocked = true;

                GameManager.Instance.Player.playerInventory.Inventory.RemoveItem(theKey);

                Destroy(theKey);

                theKey.gameObject.SetActive(false);

                InteractText = "";
            }
            else
            {
                InteractText = "Find the correct key";
                GameManager.Instance.Player.playerInventory.HUD.OpenMessagePanel(this);
                mIsUnlocked = false;
            }
        }

        if (mIsUnlocked == true)
        {
            mIsOpen = !mIsOpen;
            moveTime = 0;
        }
    }

    private void Awake()
    {
        startingPosition = transform.position;
        moveTime = 0;
        //noise = GetComponent<SoundSource>();

    }

    private void FixedUpdate()
    {
        if (mIsOpen == true)
        {
            OpenDoor();
        }

        if (mIsOpen == false)
        {
            CloseDoor();
        }

        if (moveTime >= 1)
        {
            //noise.isMakingSound = false;
        }
    }

    public void OpenDoor()
    {
        mIsOpen = true;
        if (moveTime <= 1)
        {
            moveTime += Time.deltaTime;
            transform.position = Vector3.Lerp(transform.position, OpenOffset, moveTime);

        }
    }

    public void CloseDoor()
    {
        mIsOpen = false;
        if (moveTime >= 1)
        {
            moveTime -= Time.deltaTime;
            transform.position = Vector3.Lerp(transform.position, startingPosition, moveTime);
        }

    }

    private void OnTriggerEnter(Collider other)
    {
        if (GameManager.Instance.Player.playerInventory.Inventory.FindItem(theKey))
        {
            InteractText = "Open (</,)";
        }
    }

    public bool GetDoorOpen()
    {
        return mIsOpen;
    }
}

