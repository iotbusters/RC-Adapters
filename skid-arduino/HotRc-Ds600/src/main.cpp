#include <StateMachine.h>

StateMachine stateMachine;

void setup()
{
  stateMachine.begin();
}

void loop()
{
  stateMachine.next();
}
