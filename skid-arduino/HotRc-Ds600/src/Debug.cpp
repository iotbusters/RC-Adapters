#include <Debug.h>

void debug(const char label[], ControllerOutput output)
{
    Serial.print(label);
    Serial.print(": (");
    Serial.print(output.speed);
    Serial.print(!output.reversed ? "F) " : "R) ");
    if (!output.Breaks())
    {
        Serial.print(output.speedThrottle, 3);
        Serial.print("(");
        Serial.print(output.throttle, 3);
        Serial.print(")");
    }
    else
        Serial.print("[break]");
}

void Debug::begin()
{
    Serial.begin(9600); // debugging serial
}
void Debug::log(ControllerOutput left, ControllerOutput right)
{
    debug("Left", left);

    Serial.print("  ");

    debug("Right", right);

    Serial.println();
}
