def calculate_angles(pos, link, ax):
    theta = np.zeros(3)

    U1 = pos[0]
    U2 = pos[1]

    if pos[0] < 0:
        theta_a = -np.arctan(U1/U2)
        U3 = np.sqrt(U1**2+U2**2)
        U4 = np.sqrt(U3**2-L[2]**2)
        theta_b = np.arccos(L[2]/U3)

        if pos[1] > 0:
            theta[0] = np.pi + theta_a-theta_b
        else:
            theta[0] = theta_a-theta_b
    else:
        theta_a = np.arctan(U2/U1)
        U3 = np.sqrt(U1**2+U2**2)
        U4 = np.sqrt(U3**2-L[2]**2)
        theta_b = np.arccos(L[2]/U3)

        theta[0] = np.pi/2-(-theta_a+theta_b)
    
    

    print(f"{link = }")
    target_frame3 = [U4-L[3], 0, pos[2]-L[1]]
    print(f"{target_frame3 = }")

    theta[2] = np.arccos((target_frame3[0]**2+target_frame3[2]**2 - link[0]**2 - link[1]**2)
                        / (2*link[0]*link[1]))

    theta[1] = np.arctan(target_frame3[2]/target_frame3[0])-np.arctan((link[1]*np.sin(theta[2]))
                                                                    / (link[0]+link[1]*np.cos(theta[2])))

    print(f"{theta = }")
    restricted_theta = np.minimum(np.maximum(theta, min_angles), max_angles)
    print(f"{restricted_theta = }")
    if not np.equal(theta, restricted_theta).all():
        ax.text(0.1, 0.1, 0.5, "Out of reach!")
    else:
        ax.text(0.1, 0.1, 0.5, "")

    return restricted_theta
