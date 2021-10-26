# Test IK-model
The python file called "model.py" plots the body of Hubert when he tries to reach the position acquired by the user. Following steps are carried out every time a new position is given:
* A combination of angles are calculated using the IK-model
* The forward kinematics is then used to get the position of the different body parts
* The body parts and all values are updated in the plot
* If any of the angles are out of range or has invalid values, it will be written ''Out of range!'' on the plot

[Sliders](https://matplotlib.org/stable/gallery/widgets/slider_demo.html) are used to give the user the possibility to change the target position and see the result directly. However it was not possible to add the function to change the angle values, and update all values on the sliders at the same time. This was because the functions for updating the plot are triggered when a slider value changes, and since both position sliders and angle sliders are depended on each other, they create an infinite loop of changing values in the end of a function and calling functions. 

The lengths of the body parts are the real values of resulting Hyoubert. The range for all angles are restricted with a arbitrarily min- and max-value that is bigger than the real range of the motors for illustration. The black shadow is the position of Hubert when all angles are 0. 

The video below shows a demo of the user interface. Note that the mouse is not recorded at the correct position, it should be dragging the x,y,z-sliders...

https://user-images.githubusercontent.com/38947347/138922859-8f70cef4-2ae6-4db4-ab6a-b4548b30d7ba.mp4
