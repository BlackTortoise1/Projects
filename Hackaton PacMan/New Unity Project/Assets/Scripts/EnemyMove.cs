using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyMove : MonoBehaviour {

    private GameObject Player;
    float MoveSpeed = 4;
    float MaxDist = 10;
    float MinDist = 1;

    private void Start()
    {
        Player = GameObject.Find("Player");
    }

    void Update()
    {
        transform.LookAt(Player.transform);

        if (Vector3.Distance(transform.position, Player.transform.position) >= MinDist)
        {
            transform.position += new Vector3 (transform.forward.x, 0, transform.forward.z) * MoveSpeed * Time.deltaTime;

            if (Vector3.Distance(transform.position, Player.transform.position) <= MaxDist)
            {
                //Here Call any function U want Like Shoot at here or something
            }

        }
        transform.Rotate(new Vector3(0, 90, 0));

    }

    void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.CompareTag("projectile"))
        {
            Debug.Log("[Enemies] Collide with projectile");
            Destroy(gameObject);
        }
        if (collision.gameObject.CompareTag("Player"))
        {
            ControlPlayer ctplayer = collision.gameObject.GetComponent<ControlPlayer>();
            ctplayer.declife();
            Destroy(gameObject);
            Debug.Log("[Enemies] Collide with player");
        }

    }
}
