#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  long Idle = LinuxParser::IdleJiffies();
  long Total = LinuxParser::Jiffies();

  long totald = Total - PrevTotal;
  long idled = Idle - PrevIdle;  
  
  PrevTotal = Total;
  PrevIdle = Idle;
  
  return (totald - idled)/totald;
  
}