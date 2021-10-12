using System;

public float[] AddNumbers(float[] pos)
{
    // Constants
    float[] L = {0, 0.498, 0.1185, 0.075, 0.160, 0.149, 0.060, 0.004};
    float[] links = {L[4], Math.Sqrt(Math.Pow(L[5] + L[6], 2) + Math.Pow(L[7], 2))};
    float[] minAngles = {-3*Math.PI/4, -3*Math.PI/4, -3*Math.PI/4};
    float[] maxAngles = {+3*Math.PI/4, +3*Math.PI/4, +3*Math.PI/4};

    float[] thetas = {0,0,0};

    // Calculate first theta
    float U1 = pos[0];
    float U2 = pos[1];

    if (U1 < 0) {
        float thetasA = -Math.Atan(U1/U2);
        float U3 = Math.Sqrt(Math.Pow(U1,2) + Math.Pow(U2,2));
        float U4 = Math.Sqrt(Math.Pow(U3,2) - Math.Pow(L[2],2));
        float thetasB = Math.Acos(L[2]/U3);

        if (pos[1] > 0){
            thetas[0] = Math.PI + thetasA - thetasB
        }
        else {
            thetas[0] = thetasA - thetasB
        }
    }
    else {
        float thetasA = Math.Atan(U1/U2);
        float U3 = Math.Sqrt(Math.Pow(U1,2) + Math.Pow(U2,2));
        float U4 = Math.Sqrt(Math.Pow(U3,2) - Math.Pow(L[2],2));
        float thetasB = Math.Acos(L[2]/U3);

        thetas[0] = Math.PI/2-(-thetasA + thetasB);
    }    
    
    // Calculate third theta
    float[] targetFrame3 = {U4 - L[3], 0, pos[2] - L[1]};

    float nominator = Math.Pow(targetFrame3[0],2) + 
                        Math.Pow(targetFrame3[2],2) - 
                        Math.Pow(links[0],2) - 
                        Math.Pow(links[1],2);
    float denominator = 2*links[0]*links[1];
    thetas[2] = Math.Acos(nominator/denominator);

    // Calculate second theta
    float term = Math.Atan(targetFrame3[2]/targetFrame3[0]);
    float nominator = links[1]*Math.Sin(thetas[2]);
    float denominator = links[0] + links[1]*Math.Cos(thetas[2]);
    thetas[1] = term - Math.Atan(nominator/denominator);

    // Restrict thetas
    for (int i = 0; i < thetas.Length; i++) {
        if (thetas[i] < minAngles[i])
            thetas[i] = minAngles[i];
        else if (thetas[i] > maxAngles[i])
            thetas[i] = maxAngles[i];
    }

    return thetas;
}