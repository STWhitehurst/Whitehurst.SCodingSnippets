using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class PlayerInventory : MonoBehaviour
{

    public MasterInventory Inventory;
    public MasterHUD HUD;

    private MasterInventoryItemBase mCurrentItem = null;
    private MasterInteractableItemBase mInteractItem = null;

    public bool IsHUDActive = true;

    //HUD Elements
    GameObject DetectionBar;
    GameObject StaminaBar;
    GameObject BulletTimeBar;
    GameObject InventoryPanel;
    GameObject SleepDartHUD;
    GameObject BuffDeBuffPanel;

    // Use this for initialization
    void Start()
    {
        Inventory.ItemUsed += Inventory_ItemUsed;
        Inventory.ItemRemoved += Inventory_ItemRemoved;
        Inventory = GameObject.FindWithTag("Inventory").GetComponent<MasterInventory>();
        HUD = GameObject.FindWithTag("HUD").GetComponent<MasterHUD>();
        DetectionBar = GameObject.FindWithTag("DetectionBar");
        StaminaBar = GameObject.FindWithTag("StaminaBar");
        BulletTimeBar = GameObject.FindWithTag("BulletTimeBar");
        InventoryPanel = GameObject.FindWithTag("InventoryPanel");
        SleepDartHUD = GameObject.FindWithTag("SleepDartHUD");
        BuffDeBuffPanel = GameObject.FindWithTag("BuffDebuff");
    }

    //ItemRemoved Event
    private void Inventory_ItemRemoved(object sender, MasterInventoryEventArgs e)
    {
        MasterInventoryItemBase item = e.Item;

        //Set the set item back to active when its dropped
        GameObject goItem = (item as MonoBehaviour).gameObject;
        goItem.SetActive(true);

        goItem.transform.parent = null;
    }


    private void SetItemActive(MasterInventoryItemBase item, bool active)
    {
        GameObject currentItem = (item as MonoBehaviour).gameObject;
        currentItem.SetActive(active);
    }


    private void Inventory_ItemUsed(object sender, MasterInventoryEventArgs e)
    {
        if (!e.Item.IsConsumable)
        {
            //If the player carries an item, un-use it (remove from player's hand)
            if (mCurrentItem != null)
            {
                SetItemActive(mCurrentItem, false);
            }

            MasterInventoryItemBase item = e.Item;

            //Use item (put it to the hand of the player)
            SetItemActive(item, true);

            mCurrentItem = e.Item;
        }
    }

    //Drop the current item
    private void DropCurrentItem()
    {
        //animator.SetTrigger("tr_drop");

        GameObject goItem = (mCurrentItem as MonoBehaviour).gameObject;

        Inventory.RemoveItem(mCurrentItem);

        //Throw animation
        Rigidbody rbItem = goItem.AddComponent<Rigidbody>();
        if (rbItem != null)
        {
            rbItem.AddForce(transform.forward * 2.0f, ForceMode.Impulse);

            Invoke("DoDropItem", 0.25f);
        }
    }

    public void DoDropItem()
    {
        //Remove Rigibody
        Destroy((mCurrentItem as MonoBehaviour).GetComponent<Rigidbody>());

        mCurrentItem = null;
    }

    private void FixedUpdate()
    {
        if (mCurrentItem != null && Input.GetKeyDown(KeyCode.M))
        {
            DropCurrentItem();
        }
    }

    // Update is called once per frame
    void Update()
    {
        //Toggle the Inventory's Visiblity
        if (Input.GetKeyDown(KeyCode.Space))
        {
            IsHUDActive = !IsHUDActive;
        }

        if (IsHUDActive == true)
        {
            DetectionBar.SetActive(true);
            StaminaBar.SetActive(true);
            BulletTimeBar.SetActive(true);
            InventoryPanel.SetActive(true);
            SleepDartHUD.SetActive(true);
            BuffDeBuffPanel.SetActive(true);


        }

        else
        {
            DetectionBar.SetActive(false);
            StaminaBar.SetActive(false);
            BulletTimeBar.SetActive(false);
            InventoryPanel.SetActive(false);
            SleepDartHUD.SetActive(false);
            BuffDeBuffPanel.SetActive(false);
        }


        //Interact with the item
        if (mInteractItem != null && Input.GetKeyDown(KeyCode.Comma))
        {
            //Common interact method
            mInteractItem.OnInteract();

            if (mInteractItem is MasterInventoryItemBase)
            {
                Inventory.AddItem(mInteractItem as MasterInventoryItemBase);
                (mInteractItem as MasterInventoryItemBase).Pickup();
                HUD.CloseMessagePanel();
            }

            mInteractItem = null;
        }
    }

    //For picking up the item you collide with
    private void OnTriggerEnter(Collider other)
    {

        MasterInteractableItemBase item = other.GetComponent<MasterInteractableItemBase>();
        if (item != null)
        {
            mInteractItem = item;
            HUD.OpenMessagePanel(mInteractItem);
        }
    }

    private void OnTriggerExit(Collider other)
    {
        MasterInteractableItemBase item = other.GetComponent<MasterInteractableItemBase>();
        if (item != null)
        {
            HUD.CloseMessagePanel();
            mInteractItem = null;
        }
    }
}
