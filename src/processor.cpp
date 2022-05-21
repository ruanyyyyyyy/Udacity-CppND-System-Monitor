#include "processor.h"
#include <iostream>
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  long Idle = LinuxParser::IdleJiffies();
  long Total = LinuxParser::Jiffies();

  float totald = Total - PrevTotal;
  float idled = Idle - PrevIdle;

  PrevTotal = Total;
  PrevIdle = Idle;

  float utilization;
  try {
    utilization = (totald - idled) / totald;
  } catch (...) {
    utilization = 0.0;
  }

  return utilization;
}