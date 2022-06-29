#include <stdio.h>
#include "arm_math.h"
#include "pmu-and-marker.h"
#include "serial.h"      // for record_boot
extern int testmain(const char*);

extern "C" const char* patternData;

// main() is the entry point to cmsis dsp test application.
// Similar to submain() to other common applications
int main() {
  SOFTWARE_MARK = 2;  // write to indicate startup code is complete
  (void)start_marker();
  return (testmain(patternData));
  (void)stop_marker();
}
