using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class ControlPlayer : MonoBehaviour {
    public float inputDelay = 0.1f;
    public float forwardVelocity = 12;
    public float rotateVelocity = 100;
    private float timer = 0.0f;
    private int life;
    public Text scoreText;
    public Text lifeText;
    public Text timeText;
    private int score = 0;
    float op;
    private GameObject prefab;

    private Vector3 currentScale;
    private float scaleParam;
    private int directionScale;

    Quaternion targetRotation;
    public new Rigidbody rigidbody;
    float forwardInput, turnInput;
    private Color col;
	// Use this for initialization
	private void Start () {
        
        directionScale = 1;
        scaleParam = 0.5f;
        currentScale = new Vector3(transform.localScale.x, transform.localScale.y, transform.localScale.z);
        
        prefab = Resources.Load("purple_ghost") as GameObject;
        life = 4;
        op = 0;
        targetRotation = transform.rotation;
        if (GetComponent<Rigidbody>())
        {
            rigidbody = GetComponent<Rigidbody>();
        }
        else
        {
            Debug.LogError("Add rigidbody component!");
        }
        forwardInput = turnInput = 0;
    }

    public void declife()
    {
        if (op < 0)
            life--;
    }

    public Quaternion TargetRotation
    {
        get { return targetRotation; }
    }

    private void GetInput()
    {
        forwardInput = Input.GetAxis("Vertical");
        turnInput = Input.GetAxis("Horizontal");
    }

	void FixedUpdate () {
        op -= Time.deltaTime;
        if(op<0)
        {
            /*Renderer rend = GetComponent<Renderer>();
            rend.material.shader = Shader.Find("Specular");
            rend.material.SetColor("_SpecColor", col);*/
        }

        if (life <= 0)
        {
            //gameObject.SetActive(false);
            SceneManager.LoadScene(2);
            Time.timeScale = 0;
        }

        GetInput();
        Turn();
        Run();
        timer += Time.deltaTime;
        if (timer >= 2.5)
        {
            GameObject enemy = Instantiate(prefab) as GameObject;
            enemy.transform.position = new Vector3(Random.Range(-40.0f, 40.0f), 0.0f, Random.Range(-40.0f, 40.0f));
            timer = 0.0f;
        }

        scoreText.text = "Score: " + score;
        lifeText.text = "Life: " + life;
        if (op >= 0)
        {
            timeText.text = "Remaining time: " + (int) op + " sec";
        }
        else
        {
            timeText.text = " ";
        }
}

    private void Run()
    {
        // check for dead zone
        if (Mathf.Abs(forwardInput)  > inputDelay)
        {
            // move
            rigidbody.velocity = transform.forward * forwardInput * forwardVelocity;
        }
        else
        {
            // zero velocity
            rigidbody.velocity = Vector3.zero;
        }

        if (op > 0)
        {
            if (scaleParam <= 1.5f && directionScale == 1)
            {
                scaleParam += Time.deltaTime;
            }
            else if (scaleParam > 1.5f && directionScale == 1)
            {
                directionScale = -1;
            }

            if (scaleParam > 0.6f && directionScale == -1)
            {
                scaleParam -= Time.deltaTime;
            }
            else if (scaleParam <= 0.6f && directionScale == -1)
            {
                directionScale = 1;
            }
            transform.localScale = scaleParam * currentScale;
        }
        
    }
    private void Turn()
    {
        // check for dead zone
        if (Mathf.Abs(turnInput) > inputDelay)
        {
            targetRotation *= Quaternion.AngleAxis(rotateVelocity * turnInput * Time.deltaTime, Vector3.up);
        }
        transform.rotation = targetRotation;
    }

    void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.CompareTag("PickUP"))
        {
            score++;
            if (score % 20 == 0)
            {
                /*Renderer rend = GetComponent<Renderer>();
                rend.material.shader = Shader.Find("Specular");
                col = rend.material.color;
                rend.material.SetColor("_SpecColor", Color.green);*/
                op = 20.0f;
            }
            other.gameObject.SetActive(false);
            Destroy(other.gameObject);
            
        }

        if (other.gameObject.CompareTag("ghost"))
        {
            Destroy (other.gameObject);
            if (op < 0)
                life--;
            Debug.Log ("Life Damage");
        }
    }

}
