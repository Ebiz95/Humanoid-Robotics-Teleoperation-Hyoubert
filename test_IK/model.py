import matplotlib.pyplot as plt
import math
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
from matplotlib import pyplot as plt
from matplotlib.widgets import Slider, Button, RadioButtons
from numpy.lib.function_base import angle


def plot_base(ax, L):
    alpha = 0.2
    # Torso
    from_pos = [0, 0, -L[1]]
    to_pos = [0, 0, 0]
    ax.plot([from_pos[0], to_pos[0]], [from_pos[1], to_pos[1]],
            [from_pos[2], to_pos[2]], c="k", alpha=alpha)

    # Sholders
    from_pos = [0, -L[2], 0]
    to_pos = [0, L[2], 0]
    ax.plot([from_pos[0], to_pos[0]], [from_pos[1], to_pos[1]],
            [from_pos[2], to_pos[2]], c="k", alpha=alpha)

    # dist1
    from_pos = [0, -L[2], 0]
    to_pos = [L[3], -L[2], 0]
    ax.plot([from_pos[0], to_pos[0]], [from_pos[1], to_pos[1]],
            [from_pos[2], to_pos[2]], c="k", alpha=alpha)

    # Upper arm
    from_pos = [L[3], -L[2], 0]
    to_pos = [L[3]+L[4], -L[2], 0]
    ax.plot([from_pos[0], to_pos[0]], [from_pos[1], to_pos[1]],
            [from_pos[2], to_pos[2]], c="k", alpha=alpha)

    # Lower arm
    from_pos = [L[3]+L[4], -L[2], 0]
    to_pos = [L[3]+L[4] + L[5], -L[2], 0]
    ax.plot([from_pos[0], to_pos[0]], [from_pos[1], to_pos[1]],
            [from_pos[2], to_pos[2]], c="k", alpha=alpha)

    # dist2
    from_pos = [L[3]+L[4] + L[5], -L[2], 0]
    to_pos = [L[3]+L[4] + L[5], -L[2], L[7]]
    ax.plot([from_pos[0], to_pos[0]], [from_pos[1], to_pos[1]],
            [from_pos[2], to_pos[2]], c="k", alpha=alpha)

    # dist2
    from_pos = [L[3]+L[4] + L[5], -L[2], L[7]]
    to_pos = [L[3]+L[4] + L[5] + L[6], -L[2], L[7]]
    ax.plot([from_pos[0], to_pos[0]], [from_pos[1], to_pos[1]],
            [from_pos[2], to_pos[2]], c="k", alpha=alpha)


def plot_result(ax, L, P1, P2, P3, P4, P5):
    # Torso
    from_pos = [0, 0, -L[1]]
    to_pos = [0, 0, 0]
    ax.plot([from_pos[0], to_pos[0]], [from_pos[1], to_pos[1]],
            [from_pos[2], to_pos[2]], c="b")

    # Sholders
    from_pos = P1
    to_pos = P2
    ax.plot([from_pos[0], to_pos[0]], [from_pos[1], to_pos[1]],
            [from_pos[2], to_pos[2]], c="orange")

    # dist1
    from_pos = P2
    to_pos = P3
    ax.plot([from_pos[0], to_pos[0]], [from_pos[1], to_pos[1]],
            [from_pos[2], to_pos[2]], c="orange")

    # Upper arm
    from_pos = P3
    to_pos = P4
    ax.plot([from_pos[0], to_pos[0]], [from_pos[1], to_pos[1]],
            [from_pos[2], to_pos[2]], c="g")

    # Lower arm
    from_pos = P4
    to_pos = P5
    ax.plot([from_pos[0], to_pos[0]], [from_pos[1], to_pos[1]],
            [from_pos[2], to_pos[2]], c="purple")


def forward_kinematics(theta, link):
    # forw kinematics
    frame_a_to_0 = np.array([[math.cos(theta[0]), -math.sin(theta[0]), 0, 0],
                            [math.sin(theta[0]), math.cos(theta[0]), 0, 0],
                            [0, 0, 1, 0],
                            [0, 0, 0, 1]])
    frame_b_to_a = np.array([[1, 0, 0, 0],
                            [0, 1, 0, -L[2]],
                            [0, 0, 1, 0],
                            [0, 0, 0, 1]])
    frame_1_to_b = np.array([[1, 0, 0, L[3]],
                            [0, 1, 0, 0],
                            [0, 0, 1, 0],
                            [0, 0, 0, 1]])
    frame_c_to_1 = np.array([[math.cos(theta[1]), -math.sin(theta[1]), 0, L[4]*math.cos(theta[1])],
                            [0, 0, -1, 0],
                            [math.sin(theta[1]), math.cos(theta[1]), 0, L[4]*math.sin(theta[1])],
                            [0, 0, 0, 1]])
    frame_d_to_c = np.array([[math.cos(theta[2]), -math.sin(theta[2]), 0, U3*math.cos(theta[2])],
                            [math.sin(theta[2]), math.cos(theta[2]), 0, U3*math.sin(theta[2])],
                            [0, 0, 1, 0],
                            [0, 0, 0, 1]])

    target_frame = [0, 0, 0, 1]

    P2 = np.matmul(frame_a_to_0, 
         np.matmul(frame_b_to_a, target_frame))
    P1 = [-P2[0], -P2[1], P2[2]]
    print(f"{P1 = }")
    print(f"{P2 = }")
    P3 = np.matmul(frame_a_to_0, 
         np.matmul(frame_b_to_a, 
         np.matmul(frame_1_to_b, target_frame)))
    print(f"{P3 = }")
    P4 = np.matmul(frame_a_to_0, 
         np.matmul(frame_b_to_a, 
         np.matmul(frame_1_to_b, 
         np.matmul(frame_c_to_1, target_frame))))
    print(f"{P4 = }")
    P5 = np.matmul(frame_a_to_0, 
         np.matmul(frame_b_to_a, 
         np.matmul(frame_1_to_b, 
         np.matmul(frame_c_to_1, 
         np.matmul(frame_d_to_c, target_frame)))))
    print(f"{P5 = }")

    return P1, P2, P3, P4, P5

def restrict(target, max, min):
    if target > max:
        target = max
    elif target < min:
        target = min
    return target

def calculate_angles(pos, U3, previous_theta, ax):
    theta = np.zeros(3)
    out_of_reach = False

    # Calculate first theta
    if pos[0] < 0:
        if pos[1] < 0:
            theta_a = np.abs(np.arctan(pos[0]/pos[1]))
            U1 = np.sqrt(pos[0]**2+pos[1]**2)
            theta_b = np.arccos(L[2]/U1)
            theta[0] = -theta_a - theta_b
        else:
            theta_a = np.abs(np.arctan(pos[0]/pos[1]))
            U1 = np.sqrt(pos[0]**2+pos[1]**2)
            theta_b = np.arccos(L[2]/U1)
            theta[0] = np.pi + theta_a - theta_b
    else:
        if pos[1] < 0:
            theta_a = np.abs(np.arctan(pos[1]/pos[0]))
            U1 = np.sqrt(pos[0]**2+pos[1]**2)
            theta_b = np.arccos(L[2]/U1)
            theta[0] = np.pi/2 - (theta_a + theta_b)
        else:
            theta_a = np.abs(np.arctan(pos[1]/pos[0]))
            U1 = np.sqrt(pos[0]**2+pos[1]**2)
            theta_b = np.arccos(L[2]/U1)
            theta[0] = np.pi/2 + theta_a - theta_b

    # Arm Length
    U2 = np.sqrt(U1**2-L[2]**2)


    # Calculate third theta
    pos_frame1 = [U2-L[3], 0, pos[2]]
    print(f"{pos_frame1 = }")

    U4 = np.sqrt(pos_frame1[0]**2 + pos_frame1[2]**2)
    numerator = U4**2 - L[4]**2 - U3**2 
    denominator = 2*L[4]*U3
    theta[2] = np.arccos(numerator/denominator)

    # Calculate second theta
    theta_c = np.arctan(pos_frame1[2]/pos_frame1[0])
    U5 = U3*np.sin(theta[2])
    U6 = U3*np.cos(theta[2])
    theta_d = np.arctan(U5/(L[4]+U6))
    theta[1] = theta_c-theta_d

    # Check if thetas are out of range
    restricted = restrict(theta[0], max_angles[0], min_angles[0])
    if not (restricted == theta[0]):
        out_of_reach = True

    restricted = restrict(theta[1], max_angles[1], min_angles[1])
    if not (restricted == theta[1]):
        out_of_reach = True

    restricted = restrict(theta[2], max_angles[2], min_angles[2])
    if not (restricted == theta[2]):
        out_of_reach = True

    # Check if thetas has nan values
    for i,t in enumerate(theta):
        if np.isnan(t):
            out_of_reach = True

    print(f"{theta = }")

    # Notify that the position is invalid
    if out_of_reach:
        ax.text(0.1, 0.1, 0.5, "Out of reach!")
        theta[0] = previous_theta[0]
        theta[1] = previous_theta[1]
        theta[2] = previous_theta[2]
    else:
        ax.text(0.1, 0.1, 0.5, "")

    return theta


L = np.array([0, 0.498, 0.1185, 0.075, 0.160, 0.149, 0.060, 0.004])

min_angles = np.array([-np.pi/2-np.pi/4, -np.pi/2-np.pi/4, -np.pi/2-np.pi/4])
max_angles = np.array([np.pi/2+np.pi/4, np.pi/2+np.pi/4, np.pi/2+np.pi/4])

pos = np.array([0.25, -0.11, -0.1])
U3 = np.sqrt((L[5] + L[6])**2 + L[7]**2)

### Create plot

fig = plt.figure(figsize=[10,6])
ax = Axes3D(fig, auto_add_to_figure=False)
fig.add_axes(ax)
ax_min = [-0.5, -0.5, -L[1]]
ax_max = [0.5, 0.5, 0.5]
ax.set_xlim3d(ax_min[0], ax_max[0])
ax.set_ylim3d(ax_min[1], ax_max[1])
ax.set_zlim3d(ax_min[2], ax_max[2])
ax.set_ylabel("Y")
ax.set_xlabel("X")
ax.set_zlabel("Z")
ax.text(0.1, 0.1, 0.5, s="let's begin!")

### Calculate thetas
theta = np.zeros(3)
theta = calculate_angles(pos, U3, theta, ax)

### Calculate resulting positions
P1, P2, P3, P4, P5 = forward_kinematics(theta, U3)

### Plot target
ax.scatter3D(pos[0], pos[1], pos[2])

### Plot base
plot_base(ax,L)

### Plot result
plot_result(ax, L, P1, P2, P3, P4, P5)

def update_wave(val):
    print(" ")
    theta = np.zeros(3)
    pos = np.array([sliderx.val, slidery.val, sliderz.val])
    print(f"{pos = }")

    ax.cla()
    ax.set_xlim3d(ax_min[0], ax_max[0])
    ax.set_ylim3d(ax_min[1], ax_max[1])
    ax.set_zlim3d(ax_min[2], ax_max[2])
    ax.set_ylabel("Y")
    ax.set_xlabel("X")
    ax.set_zlabel("Z")

    ### Calculate thetas
    theta = calculate_angles(pos, U3, theta, ax)

    ### Calculate resulting positions
    P1, P2, P3, P4, P5 = forward_kinematics(theta, U3)

    ### Plot target
    ax.scatter3D(pos[0], pos[1], pos[2])

    ### Plot base
    plot_base(ax,L)

    ### Plot result
    plot_result(ax, L, P1, P2, P3, P4, P5)

    slider1.set_val(theta[0])
    slider2.set_val(theta[1])
    slider3.set_val(theta[2])

    fig.canvas.draw_idle()

# Sliders

axwavex = plt.axes([0.05, 0.25, 0.0225, 0.63])
sliderx = Slider(axwavex, 'Px', ax_min[0], ax_max[0], valinit=pos[0], orientation="vertical")
sliderx.on_changed(update_wave)

axwavey = plt.axes([0.1, 0.25, 0.0225, 0.63])
slidery = Slider(axwavey, 'Py', ax_min[1],
                 ax_max[1], valinit=pos[1], orientation="vertical")
slidery.on_changed(update_wave)

axwavez = plt.axes([0.15, 0.25, 0.0225, 0.63])
sliderz = Slider(axwavez, 'Pz', ax_min[2],
                 ax_max[2], valinit=pos[2], orientation="vertical")
sliderz.on_changed(update_wave)

axwave1 = plt.axes([0.85, 0.25, 0.0225, 0.63])
slider1 = Slider(axwave1, 'Theta1', min_angles[0],
                 max_angles[0], valinit=theta[0], orientation="vertical")

axwave2 = plt.axes([0.9, 0.25, 0.0225, 0.63])
slider2 = Slider(axwave2, 'Theta2', min_angles[1],
                 max_angles[1], valinit=theta[1], orientation="vertical")

axwave3 = plt.axes([0.95, 0.25, 0.0225, 0.63])
slider3 = Slider(axwave3, 'Theta3',  min_angles[2],
                 max_angles[2], valinit=theta[2], orientation="vertical")

plt.show()




