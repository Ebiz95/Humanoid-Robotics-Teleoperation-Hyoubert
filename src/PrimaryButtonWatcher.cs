using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;
using UnityEngine.XR;
// For serial
using System;
using System.IO.Ports;

[System.Serializable]
public class PrimaryButtonEvent : UnityEvent<bool> { }

public class PrimaryButtonWatcher : MonoBehaviour
{
    // For serial, change this to the port the Arduino is currently connected to
    public SerialPort serial = new SerialPort("COM4", 115200);
    // A reference to the gameobject that is the controller
    public GameObject controller;
    public bool testing;
    private string angle;
    private int elbow_angle;
    private int neck_angle;
    private int sholder_angle;
    // variables for calibrating the scale of the human compared to Hyoubert
    public double humanL1 = 1;
    public double humanArmLength = 0.6;

    private string hand_angle = "050";
    private double[] prev_thetas = { 0, 0, 0 };

    public PrimaryButtonEvent primaryButtonPress;

    private bool lastButtonState = false;
    private List<InputDevice> devicesWithPrimaryButton;

    private void Awake()
    {
        if (primaryButtonPress == null)
        {
            primaryButtonPress = new PrimaryButtonEvent();
        }

        devicesWithPrimaryButton = new List<InputDevice>();

        // Runs inverse_kinematics() every 100ms
        InvokeRepeating("inverse_kinematics", 0, 0.1f);
    }

    // Function copied from Unity documentation, only needed for reading buttonpresses
    void OnEnable()
    {
        List<InputDevice> allDevices = new List<InputDevice>();
        InputDevices.GetDevices(allDevices);
        foreach (InputDevice device in allDevices)
            InputDevices_deviceConnected(device);

        InputDevices.deviceConnected += InputDevices_deviceConnected;
        InputDevices.deviceDisconnected += InputDevices_deviceDisconnected;
    }

    // Function copied from Unity documentation, only needed for reading buttonpresses
    private void OnDisable()
    {
        InputDevices.deviceConnected -= InputDevices_deviceConnected;
        InputDevices.deviceDisconnected -= InputDevices_deviceDisconnected;
        devicesWithPrimaryButton.Clear();
    }

    // Function copied from Unity documentation, only needed for reading buttonpresses
    private void InputDevices_deviceConnected(InputDevice device)
    {
        bool discardedValue;
        if (device.TryGetFeatureValue(CommonUsages.primaryButton, out discardedValue))
        {
            devicesWithPrimaryButton.Add(device); // Add any devices that have a primary button.
        }
    }

    // Function copied from Unity documentation, only needed for reading buttonpresses
    private void InputDevices_deviceDisconnected(InputDevice device)
    {
        if (devicesWithPrimaryButton.Contains(device))
            devicesWithPrimaryButton.Remove(device);
    }

    void inverse_kinematics()
    {
        // Constants
        double[] L = { 0.0, 0.498, 0.1185, 0.075, 0.160, 0.149, 0.060, 0.004 };
        L[1] = 0.0;
        double U3 = Math.Sqrt(Math.Pow(L[5] + L[6], 2) + Math.Pow(L[7], 2));
        double[] minAngles = { -3 * Math.PI / 4, -3 * Math.PI / 4, -3 * Math.PI / 4 };
        double[] maxAngles = { +3 * Math.PI / 4, +3 * Math.PI / 4, +3 * Math.PI / 4 };
        double thetaA,thetaB,thetaC,thetaD;
        double U1,U2,U3,U4,U5,U6;

        // scale position to Hyoubert scale
        double[] pos = new double[]{ 0.0, 0.0, 0.0 };
        Vector3 position = controller.transform.position;
        double scale = ((L[3] + L[4] + L[5] + L[6]) / humanArmLength);
        pos[0] = position.z * scale;
        pos[1] = (position.x - 0.2) * scale;
        double tmp = position.y - humanL1;
        pos[2] = tmp * scale;

        // Debug.Log("Poitions:" + pos[0] + ", " + pos[1] + ", " + pos[2]);

        double[] thetas = { 0, 0, 0 };

        // Calculate first theta
        if (pos[0] < 0)
        {
            if (pos[1] < 0)
            {   
                thetaA = Math.Abs(Math.Atan(pos[0] / pos[1]));
                U1 = Math.Sqrt(Math.Pow(pos[0], 2) + Math.Pow(pos[1], 2));
                thetaB = Math.Acos(L[2] / U1);
                thetas[0] = -thetaA - thetaB;
            }
            else
            {
                thetaA = Math.Abs(Math.Atan(pos[0] / pos[1]));
                U1 = Math.Sqrt(Math.Pow(pos[0], 2) + Math.Pow(pos[1], 2));
                thetaB = Math.Acos(L[2] / U1);
                thetas[0] = Math.PI + thetaA - thetaB;
            }
        }
        else
        {   
            if (pos[1] < 0)
            {   
                thetaA = Math.Abs(Math.Atan(pos[1] / pos[0]));
                U1 = Math.Sqrt(Math.Pow(pos[0], 2) + Math.Pow(pos[1], 2));
                thetaB = Math.Acos(L[2] / U1);
                thetas[0] = Math.PI/2 - (thetaA + thetaB);
            }
            else
            {
                thetaA = Math.Abs(Math.Atan(pos[1] / pos[0]));
                U1 = Math.Sqrt(Math.Pow(pos[0], 2) + Math.Pow(pos[1], 2));
                thetaB = Math.Acos(L[2] / U1);
                thetas[0] = Math.PI/2 + thetaA - thetaB;
            }
        }
        
        // Arm length
        U2 = Math.Sqrt(Math.Pow(U1, 2) - Math.Pow(L[2], 2));
        
        // Calculate third theta
        double[] posFrame1 = { U2 - L[3], 0, pos[2]};
        
        U4 = Math.Sqrt(Math.Pow(posFrame1[0], 2) + Math.Pow(posFrame1[2], 2));
        double numerator = Math.Pow(U4, 2) - Math.Pow(L[4], 2) - Math.Pow(U3, 2);
        double denominator = 2 * L[4] * U3;
        thetas[2] = Math.Acos(numerator / denominator);

        // Calculate second theta
        thetaC = Math.Atan(posFrame1[2] / posFrame1[0])
        U5 = U3 * Math.Sin(thetas[2]);
        U6 = U3 * Math.Cos(thetas[2]);
        thetaD = Math.Atan(U5 / (L[4] + U6));
        thetas[1] = thetaC - thetaD;
        
        // Give thetas the previous value if 
        // they are out of range or
        // if they have NaN values
        for (int i = 0; i < 3; i++)
        {
            if (double.IsNaN(thetas[i]))
            {
                thetas[0] = prev_thetas[0];
                thetas[1] = prev_thetas[1];
                thetas[2] = prev_thetas[2];
                Debug.Log("NaN values");
                break;
            }
            if (thetas[i] < minAngles[i] || thetas[i] > maxAngles[i])
            {
                thetas[0] = prev_thetas[0];
                thetas[1] = prev_thetas[1];
                thetas[2] = prev_thetas[2];
                Debug.Log("Range limited");
                break;
            }
        }
        prev_thetas[0] = thetas[0];
        prev_thetas[1] = thetas[1];
        prev_thetas[2] = thetas[2];

        Debug.Log("Thetas: " + thetas[0]*(180/Math.PI) + ", " + thetas[1] * (180 / Math.PI) + ", " + thetas[2] * (180 / Math.PI));

        // Convert thetas to hubert motor angles
        double[] alphas = thetas;
        alphas[0] = getArduinoAngle(thetas[0], -Math.PI / 2, Math.PI / 2, 0, 180); // Torso
        alphas[1] = getArduinoAngle(thetas[1], -Math.PI / 2, Math.PI / 2, 80, 260); // Sholder
        alphas[2] = getArduinoAngle(thetas[2], 0, Math.PI / 2, 140, 20); // Elbow

        alphas[0] = restrict(alphas[0], 0, 180);
        alphas[1] = restrict(alphas[1], 80, 260);
        alphas[2] = restrict(alphas[2], 20, 140);


        // Debug.Log("Alphas: " + alphas[0] + ", " + alphas[1] + ", " + alphas[2]);

        if (!testing)
        {
            send_angle(alphas);
        }
        
    }

    double restrict(double target, double minValue, double maxValue)
    {
        double outValue;
        if (target > maxValue) outValue = maxValue;
        else if (target < minValue) outValue = minValue;
        else outValue = target;
        return outValue;
    }

    double getArduinoAngle(double theta, double thetaMin, double thetaMax, double alphaMin, double alphaMax)
    {
        double outAngle = (theta - thetaMin) * (alphaMax - alphaMin) / (thetaMax - thetaMin) + alphaMin;
        return outAngle;
    }

    // Currently grabs the angles of the right handcontroller and sends them as a 12 long string over serial  
    // "010120030040" means angles 10, 120, 30 and 40 to the hand, elbow, neck and shoulder in that order
    void send_angle(double[] alphas)
    {
        // For orientation
        if (serial.IsOpen == false)
        {
            serial.Open();
        }

        // alphas are Parsed to strings of correct length and added to the angle variable in order
        // get elbow angle
        elbow_angle = (int)alphas[2];
        //elbow_angle = (int)70;
        if (elbow_angle < 100 && elbow_angle > 9)
        {
            angle = "0" + elbow_angle.ToString();
        }
        else if (elbow_angle < 10)
        {
            angle = "00" + elbow_angle.ToString();
        }
        else
        {
            angle = elbow_angle.ToString();
        }

        // get neck angle 
        neck_angle = (int)alphas[0];
        //neck_angle = (int)90;
        if (neck_angle < 100 && neck_angle > 9)
        {
            angle = angle + "0" + neck_angle.ToString();
        }
        else if (neck_angle < 10)
        {
            angle = angle + "00" + neck_angle.ToString();
        }
        else
        {
            angle = angle + neck_angle.ToString();
        }

        // get sholder angle
        sholder_angle = (int)alphas[1];
        //sholder_angle = (int)100;

        // do transforms to get a normal range (won't be needed in the future)
        if (sholder_angle < 100 && sholder_angle > 9)
        {
            angle = angle + "0" + sholder_angle.ToString();
        }
        else if (sholder_angle < 10)
        {
            angle = angle + "00" + sholder_angle.ToString();
        }
        else
        {
            angle = angle + sholder_angle.ToString();
        }

        // final two angles are set to arbitrary values as they are not needed currently
        angle = hand_angle + angle;
        serial.Write(angle);
        // Debug.Log("string: " + angle);
    }

    // functions that runs every time the game renders a frame
    void Update()
    {   
        // Code that handles buttonpresses
        bool tempState = false;
        foreach (var device in devicesWithPrimaryButton)
        {
            bool primaryButtonState = false;
            tempState = device.TryGetFeatureValue(CommonUsages.primaryButton, out primaryButtonState) // did get a value
                        && primaryButtonState // the value we got
                        || tempState; // cumulative result from other 
        }

        // Runs once when button is pressed and sets the hand angle to 180deg (closed)
        if ((tempState != lastButtonState) && tempState) // Button state changed since last frame
        {    
            primaryButtonPress.Invoke(tempState);
            lastButtonState = tempState;
            Debug.Log("Button is pressed!");
            hand_angle = "180";
        }
        // Runs once when button is unpressed and sets the hand angle to 50deg (open)
        else if (tempState != lastButtonState)
        {
            primaryButtonPress.Invoke(tempState);
            lastButtonState = tempState;
            Debug.Log("Button is unpressed!");
            hand_angle = "050";
        }
    }
}
