#include <stdio.h>
#include <stdlib.h>

#include "mpe.h"


int main(int argc, char **argv) {
  
  MPE_Init_log();

  int event1 = MPE_Log_get_event_number();
  int event2 = MPE_Log_get_event_number();
  int startEvent = MPE_Log_get_event_number();
  int endEvent = MPE_Log_get_event_number();

  MPE_Describe_event(event1, "Foo", "Green");
  MPE_Describe_event(event2, "Bar", "Red");
  MPE_Describe_state(startEvent, endEvent, "Thinking", "Blue");

  printf("Started.\n");

  sleep(1);
  printf("Foo event\n");
  MPE_Log_event(event1, 0, 0);

  sleep(1);
  printf("Start thinking\n");
  MPE_Log_event(startEvent, 0, 0);

  sleep(1);
  printf("End thinking\n");
  MPE_Log_event(endEvent, 0, 0);

  sleep(1);
  printf("Bar event\n");
  MPE_Log_event(event2, 0, 0);

  sleep(1);
  MPE_Finish_log(argv[0]);

  printf("Ended.\n");

  return 0;
}