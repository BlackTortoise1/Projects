using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class QuitButton : MonoBehaviour {

	// Update is called once per frame
	public void QuitGame () {
        Application.Quit();
        Debug.Log("Game is exiting");
    }
}
