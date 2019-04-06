using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MasterInventory : MonoBehaviour {
    private const int SLOTS = 8;

    public IList<MasterInventorySlot> theSlots = new List<MasterInventorySlot>();

    public event EventHandler<MasterInventoryEventArgs> ItemAdded;

    public event EventHandler<MasterInventoryEventArgs> ItemRemoved;

    public event EventHandler<MasterInventoryEventArgs> ItemUsed;

    public MasterInventory()
    {
        for (int i = 0; i < SLOTS; i++)
        {
            theSlots.Add(new MasterInventorySlot(i));
        }
    }

    private MasterInventorySlot FindStackableSlot(MasterInventoryItemBase item)
    {
        foreach (MasterInventorySlot slot in theSlots)
        {
            //Edit max items in slots here
            if (slot.IsStackable(item) && slot.Count < 5)
                return slot;
        }

        return null;
    }

    private MasterInventorySlot FindNextEmptySlot()
    {
        foreach (MasterInventorySlot slot in theSlots)
        {
            if (slot.IsEmpty)
                return slot;
        }

        return null;
    }

    public bool FindItem(MasterInventoryItemBase item)
    {
        foreach (MasterInventorySlot slot in theSlots)
        {
            if (slot.IsStackable(item))
                return true;
        }

        return false;
    }

    public void AddItem(MasterInventoryItemBase item)
    {
        MasterInventorySlot theFreeSlot = FindStackableSlot(item);

        if (theFreeSlot == null)
        {
            theFreeSlot = FindNextEmptySlot();
        }

        if (theFreeSlot != null)
        {
            theFreeSlot.AddItem(item);

            if (ItemAdded != null)
            {
                ItemAdded(this, new MasterInventoryEventArgs(item));
            }
        }
    }

    internal void UseItem(MasterInventoryItemBase item)
    {
        if (ItemUsed != null)
        {
            ItemUsed(this, new MasterInventoryEventArgs(item));
        }
    }

    public void RemoveItem(MasterInventoryItemBase item)
    {
        foreach(MasterInventorySlot slot in theSlots)
        {
            if (slot.Remove(item))
            {
                if (ItemRemoved != null)
                {
                    ItemRemoved(this, new MasterInventoryEventArgs(item));
                }
                break;
            }
        }
    }
}
