using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class MasterItemDragHandler : MonoBehaviour, IDragHandler, IEndDragHandler
{
    public MasterInventoryItemBase Item { get; set; }
    
    //Point of reference to drag the item's image
    public void OnDrag(PointerEventData eventData)
    {
        transform.position = Input.mousePosition;
    }

    //When done dragging the item take the sprite back to where its original position was
    public void OnEndDrag(PointerEventData eventData)
    {
        transform.localPosition = Vector3.zero;
    }
}
