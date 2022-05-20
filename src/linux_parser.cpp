#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value_s;
  string ignore;
  float memtotal;
  float memfree;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value_s >> ignore) {
        if (key == "MemTotal") {
          memtotal = stof(value_s);
        }
        if (key == "MemFree") {
          memfree = stof(value_s);
        }
      }
    }
  }
  return (memtotal-memfree)/memtotal; // return total used memory
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string value1;
  string value2;
  string line;
  long uptime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> value1 >> value2;
    uptime = stoi(value1);
  }

  return uptime;  // in seconds
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  vector<string> cpu_utils = CpuUtilization();
  long total = 0;
  for (string s: cpu_utils) {
    total += stol(s);
  }
  
  return total;

}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line;
  string key;
  int utime, stime, cutime, cstime;
  int i=0;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      while (linestream >> key) {
        i += 1;
        if (i==14) { utime = stoi(key); }
        if (i==15) { stime = stoi(key); }
        if (i==16) { cutime = stoi(key); }
        if (i==17) { cstime = stoi(key); }
      }
    return utime + stime + cutime + cstime;
  }
  return 0;
}
// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  return Jiffies() - IdleJiffies(); 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> cpu_utils = CpuUtilization();
  long idle = stol(cpu_utils[3]) + stol(cpu_utils[4]);
  return idle; 
}


// TODO: Read and return CPU utilization 
vector<string> LinuxParser::CpuUtilization() { 
  std::ifstream proc_stat(kProcDirectory + kStatFilename);
  if (proc_stat.is_open()) {
    string line;
    std::getline(proc_stat, line);
    std::istringstream linestream(line);
    linestream.ignore(5, ' ');
    std::vector<string> cpu_utils;
    for (string cpu_util; linestream >> cpu_util; cpu_utils.push_back(cpu_util));
  return cpu_utils;
  }
  return {};
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key=="processes") {
          linestream >> value;
        }
      }
    }
  }

  return stoi(value); 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key=="procs_running") {
          linestream >> value;
        }
      }
    }
  }

  return stoi(value);  
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    return line;
  }
  return string(); 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      while(linestream >> key) {
        if (key=="VmSize:") {
          linestream >> value;
          int res = stoi(value) / 1024;
          return to_string(res);
        }
      }

    };

  }
  return string(); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line;
  string key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if(key=="Uid:") {
        linestream >> value;
        return value;
      }
      
    }
  }
  return string(); 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line;
  std::ifstream stream(kPasswordPath);
  string uid = Uid(pid);
  string key, x, value;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> x >> value;
        if (value==uid) {
          return key;
        }
    }
  }
  return string(); 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  string key;
  int i=0;
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> key) {
      i += 1;
      if (i==22) {
        int start_time = stoi(key) / sysconf(_SC_CLK_TCK); // values in jiffies, convert it to seconds.
        long uptime = UpTime() - start_time;
        return uptime;
      }
    }
  }
  return 0; 
}