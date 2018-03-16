using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class HealthControler : MonoBehaviour {

    // Use this for initialization
    public Image healthBar;
    public int health;
    int maxHealth;
    Animator animator;
    GameObject particleSystem;

    void Start() {
        maxHealth = health;
        animator = GetComponent<Animator>();
        particleSystem = transform.FindChild("Particle System").gameObject;
    }

    // Update is called once per frame
    void Update() {
        healthBar.fillAmount = (float)health / maxHealth;
        if (health < 0)
        {
            MovementScript comp = GetComponent<MovementScript>();
            if (comp.enabled)
            {

                comp.enabled = false;
                animator.SetBool("Dead", true);
                for (int i = 0; i < transform.childCount; i++)
                {
                    transform.GetChild(i).gameObject.AddComponent<Rigidbody>();
                    StartCoroutine(Reload(5.0f));

                }
            }
        }

    }

    IEnumerator Reload(float time)
    {
        yield return new WaitForSeconds(time);
        SceneManager.LoadScene(0);
    }

    public void takeDamage(int amount, Transform from)
    {
        health -= amount;
        particleSystem.GetComponent<ParticleSystem>().Play();
        particleSystem.transform.LookAt(from.position);
    }
}
