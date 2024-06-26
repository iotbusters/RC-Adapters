struct InputData
{
    static InputData idle;

    float currentThrottle; // current average throttle value [0..1]
    float desiredThrottle; // desired throttle value [0..1]
    float steering;        // steering value [0..1]

    InputData();
    InputData(float currentThrottle, float desiredThrottle, float steering);
};
