﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Minimap : MonoBehaviour {

    public Transform target;
	
	// Update is called once per frame
	void LateUpdate () {
        transform.position = new Vector3(target.position.x, target.position.y + 5, target.position.z);
	}
}
