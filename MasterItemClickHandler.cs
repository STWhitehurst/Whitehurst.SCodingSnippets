using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

//What happens when you click on the item
public class MasterItemClickHandler : MonoBehaviour
{

    public MasterInventory theInventory;

    public KeyCode theKey;

    private Button theButton;

    private void Awake()
    {
        theButton = GetComponent<Button>();
    }

    private void Update()
    {
        if (Input.GetKeyDown(theKey))
        {
            FadeToColor(theButton.colors.pressedColor);

            //Click the button
            theButton.onClick.Invoke();
        }

        else if (Input.GetKeyUp(theKey))
        {
            FadeToColor(theButton.colors.normalColor);
        }
    }

    void FadeToColor(Color color)
    {
        Graphic graphic = GetComponent<Graphic>();
        graphic.CrossFadeColor(color, theButton.colors.fadeDuration, true, true);
    }

    //Find the item under the cursor
    private MasterInventoryItemBase AttachedItem
    {
        get
        {
            MasterItemDragHandler dragHandler = gameObject.transform.Find("ItemImage").GetComponent<MasterItemDragHandler>();

            return dragHandler.Item;
        }
    }

    //When clicked all the item's Use function and the Inventory's UseItem function
    public void OnItemClicked()
    {
        MasterInventoryItemBase item = AttachedItem;

        if (item != null)
        {
            theInventory.UseItem(item);
            item.Use();
        }
    }
}
