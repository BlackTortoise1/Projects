using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PauseGame : MonoBehaviour {
    public bool paused;
    public GameObject screen;

    void Start () {
        paused = false;	
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetKeyDown(KeyCode.Escape))
        {
            paused = !paused;
        }

        if (paused)
        {
            EnableScreen();
        }
        else
        {
            DisableScreen();
        }
	}

    // enable PauseScreen 
    public void EnableScreen()
    {
        screen.SetActive(true);
        Time.timeScale = 0.0f;
    }

    public void DisableScreen()
    {
        Time.timeScale = 1.0f;
        screen.SetActive(false);
    }

    public void QuitGame()
    {
        Application.Quit();
    }
}
