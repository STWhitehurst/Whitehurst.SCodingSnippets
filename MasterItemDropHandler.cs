using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class MasterItemDropHandler : MonoBehaviour, IDropHandler {
    public MasterInventory theInventory;

    //What happens when dropping the item
    public void OnDrop(PointerEventData eventData)
    {
        RectTransform invPanel = transform as RectTransform;

        if (!RectTransformUtility.RectangleContainsScreenPoint(invPanel, Input.mousePosition))
        {
            MasterInventoryItemBase item = eventData.pointerDrag.gameObject.GetComponent<MasterItemDragHandler>().Item;

            //Call the Inventory's RemoveItem and the Item's Drop function
            if (item != null)
            {
                theInventory.RemoveItem(item);
                item.Drop();
            }
        }
    }
}
