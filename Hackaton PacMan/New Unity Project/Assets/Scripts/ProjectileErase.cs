using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ProjectileErase : MonoBehaviour {
    
	// Update is called once per frame
	void Update () {
        Destroy(gameObject, 2.0f);
	}

    void OnCollisionEnter(Collision collision)
    {
        Destroy(gameObject);
    }

}
