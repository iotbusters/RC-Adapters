#include <StateMachine.h>
#include <memory.h>

StateMachine stateMachine;

void setup()
{
  stateMachine.begin();

    printMemory();
    Serial.println("==== SETUP STARTED ====");
}

auto time = millis();

void loop()
{
  if (millis() - time > 10000) {
    time = millis();
    printMemory();
  }

  stateMachine.next();
}
