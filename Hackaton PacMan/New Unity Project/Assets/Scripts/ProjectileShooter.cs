using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ProjectileShooter : MonoBehaviour {

    public GameObject prefab;
    float acceleration;
    void Start () {
        acceleration = 26.59f;
    }
	
	// Update is called once per frame
	void Update () {
        if (Time.timeScale == 0.0f)
            return;

        if (Input.GetKeyDown(KeyCode.Space))
        {
            GameObject projectile = Instantiate(prefab) as GameObject;
            projectile.transform.position = (new Vector3(0.0f, 1.0f, 0.0f)) + transform.position + Camera.main.transform.forward;
            Rigidbody rb = projectile.GetComponent<Rigidbody>();
            rb.velocity = Camera.main.transform.forward * acceleration;
        }
	}
}
