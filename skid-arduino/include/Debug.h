#pragma once

#include <ControllerOutput.h>

class Debug
{
public:
    static void begin();

    static void logLn(const ControllerOutput &left, const ControllerOutput &right);
    static void log(const char label[], const ControllerOutput &output);
    static void logLn(const char label[], const ControllerOutput &output);
    static void log(const char label[]);
    static void logLn(const char label[]);
};
