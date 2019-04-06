using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Pills : MasterInventoryItemBase
{
    public override void Use()
    {
        this.gameObject.SetActive(false);
        if (GameManager.Instance.Player.playerStun.IsStunned == true)
        {
            GameManager.Instance.Player.playerStun.UnStunPLayer();
            GameManager.Instance.Player.playerInventory.Inventory.RemoveItem(this);
            GameManager.Instance.Player.playerStun.hits = 0;
            GameManager.Instance.pillsUsed++;
            Destroy(gameObject);
            InteractText = "";
        }
    }
}
