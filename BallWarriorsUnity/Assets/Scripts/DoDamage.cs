using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DoDamage : MonoBehaviour {

    // Use this for initialization
    public int amount;
	void Start () {
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    private void OnTriggerEnter(Collider other)
    {
        //Debug.Log(other.tag);
        if(other.CompareTag("Player") && transform.GetComponentInParent<MovementScript>().isInHit())
        {
            other.gameObject.GetComponentInParent<HealthControler>().takeDamage(amount, transform);
        }
    }
}
