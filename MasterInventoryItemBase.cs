using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//Interactable Item.
//Derive from this class if you just want to interact with it
//and not add it to your inventory
//EX. Doors, Spotlights, Civilians
public class MasterInteractableItemBase : MonoBehaviour
{
    public string Name;
    
    //Not needed for some items
    public Sprite Image;

    //What the message box will say when inside of the item's collider
    public string InteractText = "";

    public bool IsConsumable;

    MasterInventorySlot Slot { get; set; }

    //override if you want something to happen when you interact with it
    public virtual void OnInteract()
    {

    }
}

//Derived from MasterInteractableItem Base
//Use this if you want to add the item to the player's inventory
//EX. Rocks, Pills, KeyCards
public class MasterInventoryItemBase : MasterInteractableItemBase
{
    public Vector3 InHandPosition; //When clicked on, when it will be placed in the player's hand

    public Vector3 InHandRotation; //Rotation of the item when places in player's hand

    public Vector3 DroppedRotation; //How the item will be placed when dropped by the player

    public MasterInventorySlot Slot { get; set; }

    //When using, put in players hand
    //Override if you want something else or additional to happen with clicked on
    public virtual void Use()
    {
        transform.localPosition = InHandPosition;
        transform.localEulerAngles = InHandRotation;
    }

    //What happens to the item when you pick it up
    //Override if you want something else or additional to happen with clicked on
    public virtual void Pickup()
    {
        Destroy(gameObject.GetComponent<Rigidbody>());
        gameObject.SetActive(false);
    }

    //What happens to the item when you drop it off of the player
    //Override if you want something else or additional to happen with clicked on
    public virtual void Drop()
    {
        RaycastHit hit = new RaycastHit();
        Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
        if (Physics.Raycast(ray, out hit, 1000))
        {
            gameObject.SetActive(true);
            gameObject.transform.position = hit.point;
            gameObject.transform.eulerAngles = DroppedRotation;
        }
    }
}
