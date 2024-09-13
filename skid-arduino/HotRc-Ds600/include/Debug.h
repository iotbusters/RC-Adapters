#pragma once

#include <ControllerOutput.h>

class Debug
{
public:
    static void begin();

    static void log(ControllerOutput left, ControllerOutput right);
};
