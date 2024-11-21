#include <StateMachine/StateMachineBuilder.h>
#include <main.h>

StateMachine *stateMachine;

void setup() {
    begin();

    stateMachine = StateMachineBuilder()
                       .add(&updateRxState)
                       .add(&writeRelayState, &writeRelayReadiness)
                       .add(&writeThrottleState, &writeThrottleReadiness)
                       .add(&updateRxState, &updateReadiness)
                       .build();
}

void loop() {
    if (!tryRead())
        return; // no RC links.

    if (!stateMachine->run()) {
        Serial.println("Application pipeline is over. Quit.");
        abort();
    }
    printMemory("  ---- Run ----");
}
