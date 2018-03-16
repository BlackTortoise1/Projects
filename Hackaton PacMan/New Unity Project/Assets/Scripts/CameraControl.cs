using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraControl : MonoBehaviour {
    public Transform target;
    public float lookSmooth = 0.09f;
    private ControlPlayer playerController;
    // camera Ox inclination 
    public float xTilt = 10;
    private float rotateVelocity = 0.0f;

    // offset between camera position and player position
    public Vector3 offset;
    private Vector3 destination;

    private void Start()
    {
        SetCameraTarget(target);
        offset = transform.position - target.position;
    }

    void SetCameraTarget (Transform tr)
    {
        target = tr;
         
        if (target != null)
        {
            if (target.GetComponent<ControlPlayer>())
            {
                playerController = target.GetComponent<ControlPlayer>();
            }
            else
            {
                Debug.LogError("The camera's target needs a character controller");
            }
        }
        else
        {
            Debug.LogError("Camera doesn't have target");
        }
    }

    private void LateUpdate()
    {
        // move
        MoveToTarget();
        // rotate
        LookToTarget();
    }

    void MoveToTarget()
    {
        destination = playerController.TargetRotation * offset;
        destination += target.position;
        transform.position = destination;

    }

    void LookToTarget()
    {
        float angle = Mathf.SmoothDampAngle(transform.eulerAngles.y, transform.eulerAngles.y, ref rotateVelocity, lookSmooth);
        transform.rotation = Quaternion.Euler(transform.eulerAngles.x, angle, 0);
    }
}
