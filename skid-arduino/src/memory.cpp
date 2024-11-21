#include <memory.h>

#if defined(ARDUINO_AVR_NANO)
#define TOTAL_MEMORY 2048l
#else
#error "Update directives to support your board correctly."
#endif

long freeMemory() {
    extern int __heap_start, *__brkval;
    int top;
    return (int)&top - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

long usedMemory() { return TOTAL_MEMORY - freeMemory(); }

void printMemory(const char label[]) {
    println(label);
    delay(50); // time for serial
    printMemory();
}

void printMemory() {
    const auto used = usedMemory();
    const auto free = freeMemory();
    println("---- Memory used: %ld (%ld%%) free %ld (%ld%%) ----", used, used * 100l / TOTAL_MEMORY, free, free * 100l / TOTAL_MEMORY);
}