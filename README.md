# Humanoid Robotics Teleoperation
##           Hyoubert
These are all the files used in the project Hyoubert, virtual reality teleoperation of hummanoid robot.

In the folder "src" you will find the final code.  "Hyoubert_Serial_Reader.ino" is the code used in the Arduino on the Hyoubert robot and "PrimaryButtonWatcher.cs" includes the IK-model and part of the interaction between VR-Unity-Arduino. 

The C# script primaryButtonWatcher.cs is used within the Unity game engine project which could not be included into this git.
The Unity Software project can be recreated by following the tutorial at https://youtu.be/wnn-dzHz-tA

For further instructions, read the Method section in the report.

##         IK model
The used inverse kinematics model is described in the project report and implemented in "PrimaryButtonWatcher.cs". It was also tested using a Python-script found in the folder [test IK](https://github.com/Ebiz95/Humanoid-Robotics-Teleoperation-Hyoubert/tree/main/test_IK)
