# Humanoid Robotics Teleoperation

This project was conducted as a part of the course TIF160 Humanoid Robotics at Chalmers university of technology, Gothenburg, Sweden.

##           Hyoubert - we put the you in Hubert
This repository includes the files used in the project Hyoubert, virtual reality teleoperation of the hummanoid robot called Hubert.

In the folder "[src](https://github.com/Ebiz95/Humanoid-Robotics-Teleoperation-Hyoubert/tree/main/src)" you will find the final code.  "Hyoubert_Serial_Reader.ino" is the code used in the Arduino on the Hubert robot and "PrimaryButtonWatcher.cs" includes the IK-model and part of the interaction between VR-Unity-Arduino. 

The C# script primaryButtonWatcher.cs is used within the Unity game engine project which could not be included into this git.
The Unity Software project can be recreated by following the tutorial at https://youtu.be/wnn-dzHz-tA

For further instructions, read the Method section in the report.

##         IK model
The used inverse kinematics model is described in the project report and implemented in "PrimaryButtonWatcher.cs". It was also tested using a Python-script found in the folder "[test IK](https://github.com/Ebiz95/Humanoid-Robotics-Teleoperation-Hyoubert/tree/main/test_IK)"

##        Assessment
Several tests for assessment of the resulting robot are demonstrated in the folder "[assessment](https://github.com/Ebiz95/Humanoid-Robotics-Teleoperation-Hyoubert/tree/main/assessment)" and further described in the project report. 

###### Project carried out by Jonathan Nordström, Sara Larsson, Ebaa Asaad and Ria Raj.
