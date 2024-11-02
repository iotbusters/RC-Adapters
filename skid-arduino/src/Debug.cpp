#include <Debug.h>

void Debug::begin()
{
    Serial.begin(9600); // debugging serial
}

void Debug::logLn(const ControllerOutput &left, const ControllerOutput &right)
{
    Debug::log("Left", left);
    Debug::logLn("  Right", right);
}

void Debug::log(const char label[], const ControllerOutput &output)
{
    Serial.print(label);
    Serial.print(": (");
    Serial.print(output.highSpeed ? 2 : 1);
    Serial.print(!output.reversed ? "F) " : "R) ");
    if (!output.breaks())
    {
        Serial.print(output.speedThrottle, 3);
        Serial.print("(");
        Serial.print(output.throttle, 3);
        Serial.print(")");
    }
    else
        Serial.print("[break]");
}

void Debug::logLn(const char label[], const ControllerOutput &output)
{
    Debug::log(label, output);
    Serial.println();
}

void Debug::log(const char label[])
{
    Serial.print(label);
}

void Debug::logLn(const char label[])
{
    Serial.println(label);
}
