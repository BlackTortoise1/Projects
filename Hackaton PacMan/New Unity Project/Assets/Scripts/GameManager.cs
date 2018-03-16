using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManager : MonoBehaviour {

    private void Start()
    {
        Begin();
    }
    private void Update()
    {
        if (Input.GetKeyDown(KeyCode.Space))
        {
            RestartGame();
        }
    }
   
    private void Begin()
    {
    }

    private void RestartGame()
    {
    }
}
