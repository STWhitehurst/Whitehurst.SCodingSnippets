using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ThrowableItem : MasterInventoryItemBase {

    Rigidbody item;



    public float force = 1000.0f;

    public float angle;

    float timer = 0;
    public bool timerstart = false;

    public bool InHand = false;

    public bool IsThrown = false;

    public override void Use()
    {
        base.Use();
        InHand = true;
    }


    private void Update()
    {
        if (IsThrown)
        {
            if (timerstart)
            {
                timer += Time.deltaTime;
                if (timer >= 3.0f)
                    gameObject.SetActive(false);
                //Destroy(gameObject);
                GameManager.Instance.Player.playerInventory.Inventory.RemoveItem(this);
            }
        }
    }

    private void FixedUpdate()
    {
        if (InHand)
        {
            InteractText = "Pickup (E) or Throw (F)";
            GameManager.Instance.Player.playerInventory.HUD.OpenMessagePanel(this);
            Rotate();
            if (Input.GetKeyDown(KeyCode.F))
            {
                GameManager.Instance.Player.gameObject.GetComponent<BoxCollider>().isTrigger = true;
                AimAndThrow();
            }

            if (Input.GetKeyDown(KeyCode.E))
            {
                GameManager.Instance.Player.gameObject.GetComponent<BoxCollider>().isTrigger = true;
                GameManager.Instance.Player.playerInventory.Inventory.RemoveItem(this);
                GameManager.Instance.Player.playerInventory.Inventory.AddItem(this);
                GameManager.Instance.Player.playerInventory.HUD.CloseMessagePanel();
                Pickup();
                InHand = false;
            }
        }
    }

    float AngleBetweenTwoPoints(Vector3 a, Vector3 b)
    {
        return Mathf.Atan2(a.y - b.y, a.x - b.x) * Mathf.Rad2Deg;
    }

    public void AimAndThrow()
    {
        gameObject.AddComponent<Rigidbody>();
        item = GetComponent<Rigidbody>();
        GetComponent<SphereCollider>().isTrigger = false;
        GetComponent<SphereCollider>().radius = 1.0f;
        item.transform.parent = null;
        item.AddForce(item.transform.forward.x * force, 75, item.transform.forward.z * force);
        item.useGravity = true;
        InHand = false;
        IsThrown = true;
        GameManager.Instance.Player.playerInventory.Inventory.RemoveItem(this);
    }

    void OnCollisionEnter(Collision collision)
    {
        if (collision.collider.tag == "Player")
        {
            Physics.IgnoreCollision(GetComponent<Collider>(), GameManager.Instance.Player.GetComponent<Collider>());
        }
        if (IsThrown && collision.collider.tag == "Ground")
        {
            item.constraints = RigidbodyConstraints.FreezeAll;
            timerstart = true;
        }
    }

    void Rotate()
    {
        //Get the Screen positions of the object
        Vector2 positionOnScreen = Camera.main.WorldToViewportPoint(GameManager.Instance.Player.transform.position);

        //Get the Screen position of the mouse
        Vector2 mouseOnScreen = (Vector2)Camera.main.ScreenToViewportPoint(Input.mousePosition);

        //Get the angle between the points
        float angle = AngleBetweenTwoPoints(positionOnScreen, mouseOnScreen);

        //Ta Daaa
        GameManager.Instance.Player.transform.rotation = Quaternion.Euler(new Vector3(0f, -angle - 90, 0f));

        transform.rotation = Quaternion.Euler(new Vector3(0f, -angle - 90, 0f));
    }
}
