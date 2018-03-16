using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MovementScript : MonoBehaviour {

    //Movement axis
    float h;
    float v;

    //Hit variable
    float hit;
    float slash;
    bool inHit;
    //Camera camera;

    //Settings
    public float speed = 1f;
    public float rotationSpeed = 0.5f;
    public string horizontal;
    public string vertical;
    public string punchAttack;
    public string slashAttack;
    //Animator
    Animator animator;

    void StartInHit()
    {
        inHit = true;
    }

    void EndInHit()
    {
        inHit = false;
    }

    public bool isInHit()
    {
        return inHit;
    }

	void Start () {
    //   camera = FindObjectOfType<Camera>();
        animator = GetComponent<Animator>();
        inHit = false;
    }

    // Update is called once per frame
    void FixedUpdate() {
        h = Input.GetAxis(horizontal);
        v = Input.GetAxis(vertical);
        hit = Input.GetAxis(punchAttack);
        slash = Input.GetAxis(slashAttack);

        Vector3 movement = new Vector3(-v, 0f, h);

        if (movement != Vector3.zero && !inHit)
        {
            transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.LookRotation(movement), rotationSpeed);
            transform.position += transform.right * speed * Time.deltaTime;
            animator.SetBool("Walk", true);
        }
        else
        {
            animator.SetBool("Walk", false);
        }



        if(hit == 1)
        {
            animator.SetBool("Hit", true);
        }
        else
        {
            animator.SetBool("Hit", false);
        }

        if(slash == 1)
        {
            animator.SetBool("Slash", true);
        }
        else
        {
            animator.SetBool("Slash", false);
        }
        
        //Debug.Log(movement.normalized);
    }
}
