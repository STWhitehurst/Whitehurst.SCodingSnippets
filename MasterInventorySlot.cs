using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MasterInventorySlot
{
    //Stack to hold more than one item in each slot
    private Stack<MasterInventoryItemBase> theItemStack = new Stack<MasterInventoryItemBase>();

    //ID to keep track what is what to add to the right slot
    private int theID = 0;

    public MasterInventorySlot(int id)
    {
        theID = id;
    }

    public int ID { get { return theID; } }

    //Add item to the Slot
    public void AddItem(MasterInventoryItemBase item)
    {
        item.Slot = this;
        theItemStack.Push(item);
    }

    //Check to see if it is the first item in the slot
    public MasterInventoryItemBase FirstItem
    {
        get
        {
            if (IsEmpty)
                return null;

            return theItemStack.Peek();
        }
    }

    //Check to see if the slot is empty, if not check ID of first item in that slot
    public bool IsStackable(MasterInventoryItemBase item)
    {
        if (IsEmpty)
            return false;

        MasterInventoryItemBase theFirst = theItemStack.Peek();

        if (theFirst.Name == item.Name)
            return true;

        return false;
    }

    //Is the slot empty
    public bool IsEmpty { get { return Count == 0; } }

    //Number of items in that slot
    public int Count { get { return theItemStack.Count; } }

    //Remove item from the slot
    public bool Remove(MasterInventoryItemBase item)
    {
        if (IsEmpty)
            return false;

        MasterInventoryItemBase theFirst = theItemStack.Peek();

        if (theFirst.Name == item.Name)
        {
            theItemStack.Pop();
            return true;
        }

        return false;
    }
}
