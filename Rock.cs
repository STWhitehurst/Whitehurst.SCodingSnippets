using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Rock : MasterInventoryItemBase
{
    Rigidbody rock;
    public float force;

    public float angle;

    public Vector3 direction;
    Vector2 positionOnScreen;
    Vector2 mouseOnScreen;
    private int count = 0;

    float timer = 0;
    public bool timerstart = false;


    public bool IsClicked;
    public bool IsPickedUp = false;


    public override void Use()
    {
        base.Use();
        //TODO: Do something with the object.
        gameObject.AddComponent<Rigidbody>();
        rock = GetComponent<Rigidbody>();
        rock.useGravity = false;
        force = 800.0f;
        print("The Rock is ready!");
        IsClicked = false;
        IsPickedUp = true;
        GetComponent<BoxCollider>().enabled = false;

        //GameManager.Instance.Player.playerInventory.Inventory.RemoveItem(this);

    }

    void Update()
    {
        if (IsPickedUp)
        {
            if (Input.GetKeyDown(KeyCode.F) && count < 1)
            {
                AimAndThrow();
                count++;
                print("Rock thrown");
            }
        }

        if (timerstart)
        {
            timer += Time.deltaTime;
            if (timer >= 3.0f)
                gameObject.SetActive(false);
            //Destroy(gameObject);
        }
    }

    public void AimAndThrow()
    {
        rock.GetComponent<SphereCollider>().isTrigger = false;
        rock.GetComponent<SphereCollider>().radius = 1.0f;
        rock.transform.parent = null;
        rock.AddForce(rock.transform.forward.x * force, 75, rock.transform.forward.z * force);
        rock.useGravity = true;
    }

    void OnCollisionEnter(Collision collision)
    {
        //if(collision.collider.tag == "Obstacles")
        //{
        //    //print("Hit Wall");
        //}

        if (IsPickedUp == true && collision.collider.tag == "Ground")
        {
            //print("Hit Floor");
            rock.constraints = RigidbodyConstraints.FreezeAll;
            timerstart = true;
        }
    }
}
