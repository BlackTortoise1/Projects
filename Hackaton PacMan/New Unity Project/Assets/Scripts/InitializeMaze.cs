using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InitializeMaze : MonoBehaviour
{

    public GameObject prefab;
    // Use this for initialization
    void Start()
    {

        if (prefab == null)
            Debug.LogError("NULL prefab");

        int i, j;
        int random = 0;
        for (i = -50; i < 50; i += 10)
        {
            random++;
            for (j = -40; j < 50; j += 10 * random)
            {
                GameObject point = Instantiate(prefab) as GameObject;
                point.transform.position = (new Vector3(i, 1.0f, j));
            }
            if (random > 2)
                random = 0;
        }
        random = 0;
        for (i = -40; i < 50; i += 10)
        {
            random++;
            for (j = -50; j < 50; j += 10 * random)
            {
                GameObject point = Instantiate(prefab) as GameObject;
                point.transform.Rotate(Vector3.up, 90);
                point.transform.position = (new Vector3(i, 1.0f, j));
            }
            if (random > 2)
                random = 0;
        }
    }
}
