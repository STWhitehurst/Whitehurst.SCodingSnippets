using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//Creating the Event for the item
public class MasterInventoryEventArgs : EventArgs
{
    public MasterInventoryItemBase Item;

    public MasterInventoryEventArgs(MasterInventoryItemBase item)
    {
        Item = item;
    }
}
