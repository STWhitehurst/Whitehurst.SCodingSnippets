using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MasterHUD : MonoBehaviour {

    public MasterInventory Inventory;

    public GameObject MessagePanel;

    private void Start()
    {
        Inventory.ItemAdded += MasterInventoryScript_ItemAdded;
        Inventory.ItemRemoved += MasterInventory_ItemRemoved;
    }

    private void MasterInventoryScript_ItemAdded(object sender, MasterInventoryEventArgs e)
    {
        Transform inventoryPanel = transform.Find("InventoryPanel");
        int index = -1;
        foreach (Transform slot in inventoryPanel)
        {
            index++;
            //Border... Image
            Transform imageTransform = slot.GetChild(0).GetChild(0);
            Transform textTransform = slot.GetChild(0).GetChild(1);

            Image image = imageTransform.GetComponent<Image>();
            Text txtCount = textTransform.GetComponent<Text>();

            MasterItemDragHandler itemDragHandler = imageTransform.GetComponent<MasterItemDragHandler>();

            //We found the empty slot
            if (index == e.Item.Slot.ID)
            {
                image.enabled = true;
                image.sprite = e.Item.Image;

                int itemCount = e.Item.Slot.Count;

                if (itemCount > 1)
                    txtCount.text = itemCount.ToString();

                //Store a reference to the item
                itemDragHandler.Item = e.Item;

                break;
            }
        }
    }

    private void MasterInventory_ItemRemoved(object sender, MasterInventoryEventArgs e)
    {
        Transform inventoryPanel = transform.Find("InventoryPanel");

        int index = -1;

        foreach (Transform slot in inventoryPanel)
        {
            index++;

            Transform imageTransform = slot.GetChild(0).GetChild(0);
            Transform textTransform = slot.GetChild(0).GetChild(1);

            Image image = imageTransform.GetComponent<Image>();
            Text txtCount = textTransform.GetComponent<Text>();

            MasterItemDragHandler itemDragHandler = imageTransform.GetComponent<MasterItemDragHandler>();

            //We found the item in the UI
            if (itemDragHandler.Item == null)
                continue;

            //Found the slot to remoce from
            if (e.Item.Slot.ID == index)
            {
                int itemCount = e.Item.Slot.Count;

                itemDragHandler.Item = e.Item.Slot.FirstItem;

                if (itemCount < 2)
                {
                    txtCount.text = "";
                }
                else
                {
                    txtCount.text = itemCount.ToString();
                }

                if (itemCount == 0)
                {
                    image.enabled = false;
                    image.sprite = null;
                }
                break;
            }
        }
    }

    private bool mIsMessagePanelOpened = false;

    public bool IsMessagePanelOpened
    {
        get { return mIsMessagePanelOpened; }
    }

    public void OpenMessagePanel(MasterInteractableItemBase item)
    {
        MessagePanel.SetActive(true);

        Text mpText = MessagePanel.transform.Find("Text").GetComponent<Text>();
        mpText.text = item.InteractText;

        mIsMessagePanelOpened = true;
    }

    public void CloseMessagePanel()
    {
        MessagePanel.SetActive(false);

        mIsMessagePanelOpened = false;
    }
}
