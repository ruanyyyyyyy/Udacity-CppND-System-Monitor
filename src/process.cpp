#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    long activejiffies = LinuxParser::ActiveJiffies(pid);
    //std::cout << LinuxParser::UpTime(pid)  << std::endl;
    float uptime = (float)LinuxParser::UpTime() - (float)LinuxParser::UpTime(pid);
    // std::cout << "pid:" << LinuxParser::UpTime(pid)  << std::endl;
    // std::cout << "system:" << LinuxParser::UpTime()  << std::endl;
    float utilization = 0.0;
    try{
      utilization = float(activejiffies)/sysconf(_SC_CLK_TCK) / uptime;
    } catch(...) {
      utilization = 0.0;
    }
   return utilization;
}
// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime() - LinuxParser::UpTime(pid); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    //if(this->pid < a->Pid())
      return true; 
}