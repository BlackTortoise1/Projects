using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InitializePoints : MonoBehaviour
{
    int i, j;
    GameObject prefab;
    // Use this for initialization
    void Start()
    { 
        prefab = Resources.Load("PickUp") as GameObject;
        if (prefab == null)
            Debug.LogError("NULL prefab");

        for (i = -32; i < 32; i += 4)
            for (j = -32; j < 32; j += 4)
            {
                GameObject point = Instantiate(prefab) as GameObject;
                point.transform.position = (new Vector3(i, 1.0f, j));
            }
    }
}
